#include "stdafx.h"
#include "aclui.h"  // platform SDK
#include "aclapi.h" // platform SDK
#include <afxpriv.h>
#pragma comment( lib, "aclui.lib" )
#pragma comment( lib, "comctl32.lib" )
#pragma comment( lib, "advapi32.lib" )
#pragma comment( lib, "user32.lib" )

// here's the access permission masks for window stations
// and desktops, excerpted from winuser.h
/*
#define WINSTA_ENUMDESKTOPS         0x0001L
#define WINSTA_READATTRIBUTES       0x0002L
#define WINSTA_ACCESSCLIPBOARD      0x0004L
#define WINSTA_CREATEDESKTOP        0x0008L
#define WINSTA_WRITEATTRIBUTES      0x0010L
#define WINSTA_ACCESSGLOBALATOMS    0x0020L
#define WINSTA_EXITWINDOWS          0x0040L
#define WINSTA_ENUMERATE            0x0100L
#define WINSTA_READSCREEN           0x0200L

#define DESKTOP_READOBJECTS         0x0001L
#define DESKTOP_CREATEWINDOW        0x0002L
#define DESKTOP_CREATEMENU          0x0004L
#define DESKTOP_HOOKCONTROL         0x0008L
#define DESKTOP_JOURNALRECORD       0x0010L
#define DESKTOP_JOURNALPLAYBACK     0x0020L
#define DESKTOP_ENUMERATE           0x0040L
#define DESKTOP_WRITEOBJECTS        0x0080L
#define DESKTOP_SWITCHDESKTOP       0x0100L
*/

// This table maps window station permissions onto strings.
// Notice the first entry, which includes all permissions; it maps to a string "Full Control".
// I love this feature of being able to provide multiple permissions in one entry,
// if you use it carefully, it's pretty powerful - the access control editor will
// automatically keep the permission checkboxes synchronized, which makes it clear
// to the user what is going on (if they pay close enough attention).
//
// Notice that I included standard permissions - don't forget them...
// Notice that I left some of the more esoteric permissions for the Advanced dialog,
// and that I only show "Full Control" in the basic permissions editor.
// This is consistent with the way the file acl editor works.
SI_ACCESS g_winstaAccess[] = {	

	// these are the full-blown permissions, listing both winsta and desktop
	// permissions (since inheritable desktop ACEs are often found in winsta DACLs)
	{ &GUID_NULL, 0x00000001,	MAKEINTRESOURCEW( 1), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000002,	MAKEINTRESOURCEW( 2), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000004,	MAKEINTRESOURCEW( 3), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000008,	MAKEINTRESOURCEW( 4), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000010,	MAKEINTRESOURCEW( 5), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000020,	MAKEINTRESOURCEW( 6), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000040,	MAKEINTRESOURCEW( 7), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000080,	MAKEINTRESOURCEW( 8), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000100,	MAKEINTRESOURCEW( 9), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000200,	MAKEINTRESOURCEW(10), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, DELETE,		MAKEINTRESOURCEW(30), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, READ_CONTROL,	MAKEINTRESOURCEW(31), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, WRITE_DAC,	MAKEINTRESOURCEW(32), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, WRITE_OWNER,	MAKEINTRESOURCEW(33), SI_ACCESS_SPECIFIC },

	// these are a much easier-to-swallow listing of basic rights for window stations
	{ &GUID_NULL, 0x000F037F,	MAKEINTRESOURCEW(11), SI_ACCESS_GENERAL  },	// Full Control
	{ &GUID_NULL, 0x00020302,	MAKEINTRESOURCEW(12), SI_ACCESS_GENERAL  },	// Read
	{ &GUID_NULL, 0x000F007C,	MAKEINTRESOURCEW(13), SI_ACCESS_GENERAL  },	// Write
};

// Here's the corresponding table for desktop permissions
SI_ACCESS g_desktopAccess[] = {	
	{ &GUID_NULL, 0x00000001,	MAKEINTRESOURCEW(21), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000002,	MAKEINTRESOURCEW(22), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000004,	MAKEINTRESOURCEW(23), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000008,	MAKEINTRESOURCEW(24), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000010,	MAKEINTRESOURCEW(25), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000020,	MAKEINTRESOURCEW(26), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000040,	MAKEINTRESOURCEW(27), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000080,	MAKEINTRESOURCEW(28), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, 0x00000100,	MAKEINTRESOURCEW(29), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, DELETE,		MAKEINTRESOURCEW(30), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, READ_CONTROL,	MAKEINTRESOURCEW(31), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, WRITE_DAC,	MAKEINTRESOURCEW(32), SI_ACCESS_SPECIFIC },
	{ &GUID_NULL, WRITE_OWNER,	MAKEINTRESOURCEW(33), SI_ACCESS_SPECIFIC },

	// these are a much easier-to-swallow listing of basic rights for desktops
	{ &GUID_NULL, 0x000F01FF,	MAKEINTRESOURCEW(11), SI_ACCESS_GENERAL },	// Full Control
	{ &GUID_NULL, 0x000F0051,	MAKEINTRESOURCEW(12), SI_ACCESS_GENERAL },	// Read
	{ &GUID_NULL, 0x000F01AE,	MAKEINTRESOURCEW(13), SI_ACCESS_GENERAL },	// Write
};

// Here's my crufted-up mapping for desktop generic rights
GENERIC_MAPPING g_desktopGenericMapping = {
	STANDARD_RIGHTS_READ | DESKTOP_READOBJECTS | DESKTOP_JOURNALRECORD | DESKTOP_ENUMERATE,
	STANDARD_RIGHTS_WRITE | DESKTOP_CREATEWINDOW | DESKTOP_CREATEMENU | DESKTOP_HOOKCONTROL | DESKTOP_JOURNALPLAYBACK | DESKTOP_WRITEOBJECTS | DESKTOP_SWITCHDESKTOP,
	STANDARD_RIGHTS_EXECUTE,
	STANDARD_RIGHTS_REQUIRED | 0x000001FF
};

// Here's my crufted-up mapping for winstation generic rights
GENERIC_MAPPING g_winstaGenericMapping = {
	STANDARD_RIGHTS_READ | WINSTA_ENUMDESKTOPS | WINSTA_READATTRIBUTES | WINSTA_ENUMERATE | WINSTA_READSCREEN,
	STANDARD_RIGHTS_WRITE | WINSTA_ACCESSCLIPBOARD | WINSTA_CREATEDESKTOP | WINSTA_WRITEATTRIBUTES | WINSTA_ACCESSGLOBALATOMS | WINSTA_EXITWINDOWS,
	STANDARD_RIGHTS_EXECUTE,
	STANDARD_RIGHTS_REQUIRED | 0x0000037F
};

// This table maps the various inheritance options supported by window stations
// onto human readable strings ("This window station only", "Desktops only", etc.)
// These strings show up in the Advanced DACL editor for window stations, in a dropdown
SI_INHERIT_TYPE g_winstaInheritTypes[] = {
	{ &GUID_NULL,															  0, MAKEINTRESOURCEW( 40 ) },
	{ &GUID_NULL,                    CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE, MAKEINTRESOURCEW( 41 ) },
	{ &GUID_NULL, INHERIT_ONLY_ACE | CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE, MAKEINTRESOURCEW( 42 ) }
};

HINSTANCE g_hInst;

// This base class factors out common implementation between
// the window station and desktop implementations of ISecurityInformation.
// For instance, the Set/GetSecurity calls can be implemented once,
// since they both just call Set/GetSecurityInfo passing a handle
// (of either a winsta or a desktop).
struct CWindowObjectSecurityInfoBase : ISecurityInformation
{
	long	m_cRefs;
	HANDLE	m_handle;
	DWORD	m_grfExtraFlags;
	const wchar_t* const m_pszObjectName;
	const wchar_t* const m_pszPageTitle;

	CWindowObjectSecurityInfoBase(	HANDLE h,
									DWORD grfExtraFlags,
									const wchar_t* pszObjectName,
									const wchar_t* pszPageTitle = 0 )
	  : m_cRefs(0),
		m_handle(h),
		m_grfExtraFlags( grfExtraFlags ),
		m_pszObjectName( pszObjectName ),
		m_pszPageTitle( pszPageTitle )
	{}
	virtual ~CWindowObjectSecurityInfoBase() {}

	STDMETHODIMP QueryInterface( REFIID iid, void** ppv )
	{
		if ( IID_IUnknown == iid || IID_ISecurityInformation == iid )
			 *ppv = static_cast<ISecurityInformation*>(this);
		else return (*ppv = 0), E_NOINTERFACE;
		reinterpret_cast<IUnknown*>( *ppv )->AddRef();
		return S_OK;
	}
	STDMETHODIMP_(ULONG) AddRef()
	{
		return ++m_cRefs;
	}
	STDMETHODIMP_(ULONG) Release()
	{
		ULONG n = --m_cRefs;
		if ( 0 == n )
			delete this;
		return n;
	}

	STDMETHODIMP GetObjectInformation( SI_OBJECT_INFO* poi )
	{
		// We want to edit the DACL (PERMS), the OWNER,
		// and we want the Advanced button
		poi->dwFlags		= SI_EDIT_OWNER | SI_EDIT_PERMS |
							  SI_ADVANCED | m_grfExtraFlags;

		// this determines the module used to discover stringtable entries
		poi->hInstance		= g_hInst;
		poi->pszServerName	= L"";
		poi->pszObjectName	= const_cast<wchar_t*>( m_pszObjectName );
		poi->pszPageTitle	= const_cast<wchar_t*>( m_pszPageTitle );

		if ( m_pszPageTitle )
			poi->dwFlags |= SI_PAGE_TITLE;

		return S_OK;
	}

	STDMETHODIMP GetSecurity( SECURITY_INFORMATION ri, void** ppsd, BOOL bDefault )
	{
		// map directly onto the winsta/desktop security descriptor
		const DWORD err = GetSecurityInfo( m_handle, SE_WINDOW_OBJECT, ri, 0, 0, 0, 0, ppsd );
		return err ? HRESULT_FROM_WIN32(err) : S_OK;
	}

	STDMETHODIMP SetSecurity( SECURITY_INFORMATION ri, void* psd )
	{
		// map directly onto the winsta/desktop security descriptor
		void* pOwner = 0;
		void* pGroup = 0;
		ACL* pdacl = 0;
		ACL* psacl = 0;
		BOOL bDefaulted;
		BOOL bPresent;
		if ( OWNER_SECURITY_INFORMATION & ri ) 
			GetSecurityDescriptorOwner( psd, &pOwner, &bDefaulted );
		if ( GROUP_SECURITY_INFORMATION & ri )
			GetSecurityDescriptorGroup( psd, &pGroup, &bDefaulted );
		if ( DACL_SECURITY_INFORMATION & ri )
			GetSecurityDescriptorDacl( psd, &bPresent, &pdacl, &bDefaulted );
		if ( SACL_SECURITY_INFORMATION & ri )
			GetSecurityDescriptorSacl( psd, &bPresent, &psacl, &bDefaulted );

		const DWORD err = SetSecurityInfo( m_handle, SE_WINDOW_OBJECT, ri, pOwner, pGroup, pdacl, psacl );
		return err ? HRESULT_FROM_WIN32(err) : S_OK;
	}

	STDMETHODIMP PropertySheetPageCallback( HWND hwnd, UINT msg, SI_PAGE_TYPE pt )
	{
		// this is effectively a pass-through from the PropertySheet callback,
		// which we don't care about for this sample
		return S_OK;
	}
};

struct CWinstaSecurityInfo : CWindowObjectSecurityInfoBase
{
	CWinstaSecurityInfo( HWINSTA h, const wchar_t* pszObjectTitle, const wchar_t* pszPageTitle = 0 )
	  : CWindowObjectSecurityInfoBase( h, SI_CONTAINER | SI_NO_ACL_PROTECT,
										pszObjectTitle, pszPageTitle )
	{}
	
	~CWinstaSecurityInfo()
	{
#ifdef _DEBUG
		OutputDebugString( "CWinstaSecurityInfo was destroyed\n" );
#endif // _DEBUG
	}

	STDMETHODIMP GetAccessRights(	const GUID*,
									DWORD dwFlags,
									SI_ACCESS** ppAccess,
									ULONG* pcAccesses,
									ULONG* piDefaultAccess )
	{
		// here's where we hand back the winsta permissions->strings mapping
		*ppAccess = const_cast<SI_ACCESS*>( g_winstaAccess );
		*pcAccesses = sizeof g_winstaAccess / sizeof *g_winstaAccess;
		*piDefaultAccess = 0;
		return S_OK;
	}

	STDMETHODIMP MapGeneric( const GUID*, UCHAR* pAceFlags, ACCESS_MASK* pMask )
	{
		// here's where we hand back the winsta generic permissions mapping
		MapGenericMask( pMask, const_cast<GENERIC_MAPPING*>( &g_winstaGenericMapping ) );
		return S_OK;
	}

	STDMETHODIMP GetInheritTypes( SI_INHERIT_TYPE** ppInheritTypes, ULONG* pcInheritTypes )
	{
		// here's where we hand back the winsta inheritance combinations and string mappings
		*ppInheritTypes = g_winstaInheritTypes;
		*pcInheritTypes = sizeof g_winstaInheritTypes / sizeof *g_winstaInheritTypes;
		return S_OK;
	}

};

struct CDesktopSecurityInfo : CWindowObjectSecurityInfoBase
{
	CDesktopSecurityInfo( HDESK h, const wchar_t* pszObjectTitle, const wchar_t* pszPageTitle = 0 )
	  : CWindowObjectSecurityInfoBase( h, 0, pszObjectTitle, pszPageTitle )
	{}

	~CDesktopSecurityInfo()
	{
#ifdef _DEBUG
		OutputDebugString( "CDesktopSecurityInfo was destroyed\n" );
#endif // _DEBUG
	}

	STDMETHODIMP GetAccessRights(	const GUID*,
									DWORD dwFlags,
									SI_ACCESS** ppAccess,
									ULONG* pcAccesses,
									ULONG* piDefaultAccess )
	{
		// here's where we hand back the desktop permissions->strings mapping
		*ppAccess = const_cast<SI_ACCESS*>( g_desktopAccess );
		*pcAccesses = sizeof g_desktopAccess / sizeof *g_desktopAccess;
		*piDefaultAccess = 0;
		return S_OK;
	}

	STDMETHODIMP MapGeneric( const GUID*, UCHAR* pAceFlags, ACCESS_MASK* pMask )
	{
		// here's where we hand back the desktop generic permissions mapping
		MapGenericMask( pMask, const_cast<GENERIC_MAPPING*>( &g_desktopGenericMapping ) );
		return S_OK;
	}

	STDMETHODIMP GetInheritTypes( SI_INHERIT_TYPE** ppInheritTypes, ULONG* pcInheritTypes )
	{
		// Desktops are not containers, and thus have no options for inheritable
		// entries in the DACL or SACL. Since we didn't specify SI_CONTAINER in our
		// GetObjectInformation call, this function will never be called.
		return E_NOTIMPL;
	}
};

void EditWinstaSecurity( HWINSTA hWinsta, LPCTSTR Name )
{
	USES_CONVERSION;

    // Convert our ISecurityInformation implementations into property pages
	HPROPSHEETPAGE rghps;

	{
		CWinstaSecurityInfo* pwsi = new CWinstaSecurityInfo( hWinsta, T2W(Name), L"Window Station" );
		pwsi->AddRef();
		rghps = CreateSecurityPage( pwsi );
		pwsi->Release();
	}

	// Wrap our two property pages in a modal dialog by calling PropertySheet
	PROPSHEETHEADER psh; ZeroMemory( &psh, sizeof psh );
	psh.dwSize		= sizeof psh;
	psh.hInstance	= AfxGetInstanceHandle();
	psh.pszCaption	= Name;
	psh.nPages		= 1;
	psh.phpage		= &rghps;

	PropertySheet( &psh );

	// By the time PropertySheet returns, the property pages have been torn down
	// and have released our ISecurityInformation implementations. Watch in the
	// VC output window for a debug message that shows they have been destroyed
	// correctly.
}

void EditDesktopSecurity( HDESK hDesktop, LPCTSTR Name )
{
	USES_CONVERSION;

	// Convert our ISecurityInformation implementations into property pages
	HPROPSHEETPAGE rghps;
	{
		CDesktopSecurityInfo* pdsi = new CDesktopSecurityInfo( hDesktop, T2W(Name), L"Desktop" );
		pdsi->AddRef();
		rghps = CreateSecurityPage( pdsi );
		pdsi->Release();
	}

	// Wrap our two property pages in a modal dialog by calling PropertySheet
	PROPSHEETHEADER psh; ZeroMemory( &psh, sizeof psh );
	psh.dwSize		= sizeof psh;
	psh.hInstance	= AfxGetInstanceHandle();
	psh.pszCaption	= Name;
	psh.nPages		= 1;
	psh.phpage		= &rghps;

	PropertySheet( &psh );

	// By the time PropertySheet returns, the property pages have been torn down
	// and have released our ISecurityInformation implementations. Watch in the
	// VC output window for a debug message that shows they have been destroyed
	// correctly.
}
