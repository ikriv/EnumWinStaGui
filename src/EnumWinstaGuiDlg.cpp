// EnumWinstaGuiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EnumWinstaGui.h"
#include "EnumWinstaGuiDlg.h"
#include "WinstaDacl.h"
#include "WinstaSwitcher.h"
#include "fksec/fksec.h"
#include "NewDesktopDlg.h"
#include "NewWinstaDlg.h"
#include "XError.h"
#include "AutoPtr.h"
#include "KHandle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum
{
    DESKTOP_ITEM = 1,
    WINSTA_ITEM = 2
};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnumWinstaGuiDlg dialog

CEnumWinstaGuiDlg::CEnumWinstaGuiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEnumWinstaGuiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnumWinstaGuiDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEnumWinstaGuiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnumWinstaGuiDlg)
	DDX_Control(pDX, IDC_WINSTA_TREE, m_WinstaTree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEnumWinstaGuiDlg, CDialog)
	//{{AFX_MSG_MAP(CEnumWinstaGuiDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RCLICK, IDC_WINSTA_TREE, OnRclickWinstaTree)
    ON_COMMAND(IDC_REFRESH_TREE, OnRefreshTree)
    ON_COMMAND(IDC_NEW_WINSTA, OnNewWinsta)
    ON_COMMAND(IDC_EDIT_SECURITY, OnEditSecurity)
    ON_COMMAND(IDC_CREATE_DESKTOP, OnCreateDesktop)
    ON_COMMAND(IDC_TAKE_OWNERSHIP, OnTakeOwnership)
    ON_COMMAND(IDC_SWITCH_TO, OnSwitchTo)
    ON_COMMAND(IDC_RUN_CMD_EXE, OnRunCmdExe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnumWinstaGuiDlg message handlers

BOOL CEnumWinstaGuiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    m_ImageList.Create( IDC_IMAGE_LIST, 16, 3, RGB(255,255,255) );
    m_WinstaTree.SetImageList( &m_ImageList, TVSIL_NORMAL );
	PopulateTree();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEnumWinstaGuiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEnumWinstaGuiDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEnumWinstaGuiDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//------------------------------------------------------------
struct KInsertContext
{
    CEnumWinstaGuiDlg* pThis;
    HTREEITEM hParent;
};

//------------------------------------------------------------
BOOL CALLBACK DesktopEnumProc(
                                LPTSTR lpszDesktop,	// name of a desktop
                                LPARAM lParam	// value specified in EnumDesktops call
                             )
{
    KInsertContext* pContext = (KInsertContext*)lParam;
    pContext->pThis->m_WinstaTree.InsertItem( TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM, 
                                              lpszDesktop, 2, 2, 0, 0, DESKTOP_ITEM, pContext->hParent, NULL );
    pContext->pThis->m_WinstaTree.Expand( pContext->hParent, TVE_EXPAND );
    return TRUE;
};

//------------------------------------------------------------
BOOL CALLBACK EnumWindowStationProc(
                                    LPTSTR lpszWindowStation,	// name of a window station
                                    LPARAM lParam	            // value specified in EnumWindowStations call
                                   )
{
    KInsertContext* pContext = (KInsertContext*)lParam;

    HTREEITEM hWinstaItem = 
        pContext->pThis->m_WinstaTree.InsertItem( TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM, 
                                                  lpszWindowStation, 0, 1, 0, 0, WINSTA_ITEM, pContext->hParent, NULL );

    pContext->pThis->m_WinstaTree.Expand( pContext->hParent, TVE_EXPAND );

    HWINSTA hWinsta = OpenWindowStation( lpszWindowStation, FALSE, WINSTA_ENUMDESKTOPS );

    if (hWinsta)
    {
        KInsertContext Context = { pContext->pThis, hWinstaItem };
        EnumDesktops( hWinsta, DesktopEnumProc, (LPARAM)&Context );
    };

    return TRUE;
};

void CEnumWinstaGuiDlg::PopulateTree()
{
    // Get the handle to the system image list, for our icons
    m_WinstaTree.DeleteAllItems();
    HTREEITEM hRoot = 
        m_WinstaTree.InsertItem( TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE,
                                 "Window Stations", 0,1,0,0,0, NULL, NULL );

    KInsertContext Context = {this, hRoot};
    EnumWindowStations( EnumWindowStationProc, (LPARAM)&Context ); 
};

//---------------------------------------------------------------------------
HANDLE CEnumWinstaGuiDlg::OpenUserObject( HTREEITEM hItem, ACCESS_MASK Mask )
{
    return OpenUserObject( hItem, 1, &Mask );
};

//---------------------------------------------------------------------------
HANDLE CEnumWinstaGuiDlg::OpenUserObject( HTREEITEM hItem, int nAttempts, ACCESS_MASK const* pMasks )
{
    LPARAM ItemType = m_WinstaTree.GetItemData(hItem);
    switch (ItemType)
    {
        case WINSTA_ITEM:
            return (HANDLE)OpenWinsta( hItem, nAttempts, pMasks );

        case DESKTOP_ITEM:
            return (HANDLE)OpenDesktop( hItem, nAttempts, pMasks );
    };

    throw XError( ERROR_INVALID_PARAMETER, "OpenUserObject() failed. Internal error" );
    return NULL; // just to make compiler happy
};

//---------------------------------------------------------------------------
HDESK CEnumWinstaGuiDlg::OpenDesktop( HTREEITEM hItem, ACCESS_MASK Mask )
{
    return OpenDesktop( hItem, 1, &Mask );
};

//---------------------------------------------------------------------------
HDESK CEnumWinstaGuiDlg::OpenDesktop( HTREEITEM hItem, int nAttempts, ACCESS_MASK const* pMasks )
{
    ASSERT(hItem);

    HTREEITEM hParent = m_WinstaTree.GetParentItem(hItem);
    if (!hParent) 
        throw XError( ERROR_INVALID_PARAMETER, "OpenDesktop failed. Internal error." );

    CString WinstaName = m_WinstaTree.GetItemText(hParent);
    KWinsta hWinsta = OpenWindowStation( WinstaName.GetBuffer(0), FALSE, WINSTA_ENUMDESKTOPS );
    DWORD Error = GetLastError();
    WinstaName.ReleaseBuffer();

    if (!hWinsta) 
    {
        throw XError(Error, _T("Cannot open window station %s"), WinstaName );
    };

    HDESK hDesktop = NULL;
    CString Name = m_WinstaTree.GetItemText(hItem);

    {
        
        // switcher will switch back to normal winsta in its destructor
        KWinstaSwitcher Switcher; 

        if (!Switcher.SwitchTo(hWinsta))
        {
            Error = GetLastError();
            throw XError(Error, _T("Cannot switch to window station %s"), WinstaName );
        };

        TCHAR* NameBuf = Name.GetBuffer(0);

        for (int nAttempt = 0; nAttempt < nAttempts; ++nAttempt)
        {
            hDesktop = ::OpenDesktop( NameBuf, 0, FALSE, pMasks[nAttempt] );
            if (hDesktop) break;

            Error = GetLastError();
            if (Error != ERROR_ACCESS_DENIED) break;
        };

        Name.ReleaseBuffer();
    }

    if (!hDesktop)
        throw XError( Error, "Cannot open desktop %s", Name );

    return hDesktop;
};

//---------------------------------------------------------------------------
HWINSTA CEnumWinstaGuiDlg::OpenWinsta( HTREEITEM hItem, ACCESS_MASK Mask )
{
    return OpenWinsta( hItem, 1, &Mask );
};

//---------------------------------------------------------------------------
HWINSTA CEnumWinstaGuiDlg::OpenWinsta( HTREEITEM hItem, int nAttempts, ACCESS_MASK const* pMasks )
{
    ASSERT(hItem);

    CString Name = m_WinstaTree.GetItemText(hItem);
    LPTSTR NameBuf = Name.GetBuffer(0);

    HWINSTA hWinsta = NULL;
    DWORD Error = 0;

    for (int nAttempt = 0; nAttempt < nAttempts; ++nAttempt)
    {
        hWinsta = OpenWindowStation( NameBuf, FALSE, pMasks[nAttempt] );
        if (hWinsta) break;

        Error = GetLastError();
        if (Error != ERROR_ACCESS_DENIED) break;
    };

    if (!hWinsta)
    {
        throw XError( Error, "Cannot open window station %s", Name );
    };

    return hWinsta;
};

//-------------------------------------------------------------------------
void CEnumWinstaGuiDlg::OnRclickWinstaTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
    *pResult = 0;

    CPoint Pos = GetMessagePos();
    m_WinstaTree.ScreenToClient(&Pos);
    HTREEITEM hItem = m_WinstaTree.HitTest(Pos, NULL);

    UINT MenuId = IDR_ROOT_MENU;

    if (hItem)
    {
        m_WinstaTree.Select(hItem, TVGN_CARET);

        // check item type
        LPARAM ItemType = m_WinstaTree.GetItemData(hItem);
        switch (ItemType)
        {
            case WINSTA_ITEM:
                MenuId = IDR_WINSTA_MENU;
                break;

            case DESKTOP_ITEM:
                MenuId = IDR_DESKTOP_MENU;
                break;
        }
    }

    ShowContextMenu( MenuId, GetMessagePos() );
};

//-------------------------------------------------------------------------
void CEnumWinstaGuiDlg::ShowContextMenu( UINT MenuId, CPoint ScreenPos )
{
    CMenu Menu;
    if (!Menu.LoadMenu(MenuId)) return;

    CMenu* pPopup = Menu.GetSubMenu(0);
    if (!pPopup) return;

    if (MenuId == IDR_DESKTOP_MENU)
    {
        BOOL bEnableSwitchTo = FALSE;

        // only desktops of Winsta0 can be switched to
        HTREEITEM hItem = m_WinstaTree.GetSelectedItem();
        if (hItem)
        {
            HTREEITEM hParent = m_WinstaTree.GetParentItem(hItem);
            if (hParent)
            {
                CString ParentName = m_WinstaTree.GetItemText(hParent);
                if (ParentName == _T("WinSta0"))
                    bEnableSwitchTo = TRUE;
            }
        };

        UINT State = bEnableSwitchTo ? MF_ENABLED : MF_GRAYED;
        pPopup->EnableMenuItem( IDC_SWITCH_TO, MF_BYCOMMAND | State );
    }

    pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, ScreenPos.x, ScreenPos.y, this, NULL );
};

//-------------------------------------------------------------------------
void CEnumWinstaGuiDlg::OnEditWinstaSecurity( HTREEITEM hItem )
{
    ASSERT(hItem);

    static ACCESS_MASK const AccessMask[] = { 
                                                 READ_CONTROL | WRITE_DAC | WRITE_OWNER,
                                                 READ_CONTROL | WRITE_DAC,
                                                 READ_CONTROL | WRITE_OWNER,
                                                 READ_CONTROL
                                            };

    static int const nMaxAttempts = sizeof(AccessMask)/sizeof(ACCESS_MASK);

    try
    {
        KWinsta hWinsta = OpenWinsta(hItem, nMaxAttempts, AccessMask);
        ASSERT(hWinsta);

        CString Name = m_WinstaTree.GetItemText(hItem);
        EditWinstaSecurity(hWinsta, Name );
    }
    catch (XError& X)
    {
        X.Show(this);
        return;
    };
};

//-------------------------------------------------------------------------
void CEnumWinstaGuiDlg::OnEditDesktopSecurity( HTREEITEM hItem )
{
    ASSERT(hItem);

    static ACCESS_MASK const AccessMask[] = { 
                                                 READ_CONTROL | WRITE_DAC | WRITE_OWNER,
                                                 READ_CONTROL | WRITE_DAC,
                                                 READ_CONTROL | WRITE_OWNER,
                                                 READ_CONTROL
                                            };

    static int const nMaxAttempts = sizeof(AccessMask)/sizeof(ACCESS_MASK);

    try
    {
        KDesktop hDesktop = OpenDesktop( hItem, nMaxAttempts, AccessMask );
        ASSERT(hDesktop);
        CString Name = m_WinstaTree.GetItemText(hItem);
        EditDesktopSecurity(hDesktop, Name);
    }
    catch (XError& X)
    {
        X.Show(this);
        return;
    }
};

//-------------------------------------------------------------------------
void CEnumWinstaGuiDlg::OnRefreshTree()
{
    PopulateTree();
};

void CEnumWinstaGuiDlg::OnEditSecurity()
{
    HTREEITEM hItem = m_WinstaTree.GetSelectedItem();
    if (!hItem) return;
    LPARAM ItemType = m_WinstaTree.GetItemData(hItem);

    switch (ItemType)
    {
        case WINSTA_ITEM:
            OnEditWinstaSecurity(hItem);
            break;

        case DESKTOP_ITEM:
            OnEditDesktopSecurity(hItem);
            break;
    };
};

void CEnumWinstaGuiDlg::OnNewWinsta()
{
    CNewWinstaDlg Dlg(this);
    if (Dlg.DoModal() != IDOK) return;

    HWINSTA hWinsta = 
        CreateWindowStation( 
                                Dlg.m_Name.GetBuffer(0),
                                0,
                                0,
                                NULL
                            );

    DWORD Error = GetLastError();
    Dlg.m_Name.ReleaseBuffer();

    if (hWinsta)
    {
        PopulateTree();
    }
    else
    {
        XError X( Error, _T("Cannot create window station %s"), Dlg.m_Name );
        X.Show(this);
    };

    // don't close the Winsta handle!

};

void CEnumWinstaGuiDlg::OnCreateDesktop()
{
    HTREEITEM hItem = m_WinstaTree.GetSelectedItem();
    if (!hItem) return;

    LPARAM ItemType = m_WinstaTree.GetItemData(hItem);
    if (ItemType != WINSTA_ITEM) return;

    KWinsta hWinsta = OpenWinsta( hItem, WINSTA_CREATEDESKTOP );

    CNewDesktopDlg Dlg;
    if (Dlg.DoModal() != IDOK)
    {
        return;
    };

    DWORD Error = 0;
    HDESK hDesktop = NULL;

    {
        KWinstaSwitcher Switcher;
        if (!Switcher.SwitchTo(hWinsta))
        {
            Error = GetLastError();
            XError X(Error, _T("Cannot switch to window station %s"), m_WinstaTree.GetItemText(hItem) );
            X.Show(this);
            return;
        };

        hDesktop = 
            CreateDesktop( 
                            Dlg.m_Name.GetBuffer(0), 
                            NULL, 
                            NULL, 
                            DF_ALLOWOTHERACCOUNTHOOK,
                            DESKTOP_CREATEWINDOW,
                            NULL 
                         );

        Error = GetLastError();
        Dlg.m_Name.ReleaseBuffer();
    }

    if (!hDesktop)
    {
        XError X( Error, "Cannot create desktop %s", Dlg.m_Name );
        X.Show(this);
        return;
    };

    // don't close handle of new desktop!
    PopulateTree();
};

void CEnumWinstaGuiDlg::OnRunCmdExe()
{
    HTREEITEM hItem = m_WinstaTree.GetSelectedItem();
    if (!hItem) return;

    LPARAM ItemType = m_WinstaTree.GetItemData(hItem);
    if (ItemType != DESKTOP_ITEM) return;

    HTREEITEM hParent = m_WinstaTree.GetParentItem(hItem);
    LPARAM ParentType = m_WinstaTree.GetItemData(hParent);
    if (ParentType != WINSTA_ITEM) return;

    CString DesktopName = 
        m_WinstaTree.GetItemText(hParent) +
        _T("\\") + 
        m_WinstaTree.GetItemText(hItem);

    STARTUPINFO si = { sizeof(si), NULL, DesktopName.GetBuffer(0) };
    PROCESS_INFORMATION pi;

    TCHAR Buffer[] = _T("cmd.exe");
    
    BOOL bResult = CreateProcess( 
                                    NULL, 
                                    "cmd.exe", 
                                    NULL, 
                                    NULL, 
                                    FALSE, 
                                    CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
                                    NULL,
                                    NULL,
                                    &si,
                                    &pi
                                );
    DWORD Error = GetLastError();
    DesktopName.ReleaseBuffer();

    if (bResult)
    {
        CloseHandle( pi.hThread );

        // check to see if newborn cmd.exe dies prematurely
        DWORD WaitResult = WaitForSingleObject( pi.hProcess, 100 );
        CloseHandle( pi.hProcess );

        CString Msg;
        UINT Icon = MB_ICONINFORMATION;

        if (WaitResult == WAIT_OBJECT_0)
        {
            Msg.Format("Started cmd.exe on desktop %s, but it ended prematurely.\n"
                       "Probably, you don't have enough rights to create windows on that desktop.\n"
                       "Process ID was %d",
                       DesktopName,
                       pi.dwProcessId
                      );

            Icon = MB_ICONWARNING;
        }
        else
        {
            Msg.Format("Started cmd.exe on desktop %s. Process ID %d", DesktopName, pi.dwProcessId );
        };

        MessageBox( Msg, NULL, Icon | MB_OK );
    }
    else
    {
        XError X(Error, "Cannot run cmd.exe on desktop %s", DesktopName );
        X.Show(this);
        return;
    };
};

static HANDLE GetProcessToken()
{
    HANDLE hProcessToken;
    if (!OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hProcessToken ))
    {
        DWORD Error = GetLastError();
        throw XError(Error, "OpenProcessToken() failed");
    };

    return hProcessToken;
};

static fksec::sid GetUserSid( HANDLE hProcessToken )
{
    DWORD Size = 0;
    BOOL bResult = GetTokenInformation( hProcessToken, TokenUser, NULL, 0, &Size );
    DWORD Error = GetLastError();

    if (bResult) 
        // something very strange happened
        throw XError(ERROR_GEN_FAILURE, "GetTokenInformation() return TRUE when data size is 0"); 

    if (Error != ERROR_INSUFFICIENT_BUFFER) 
        throw XError(Error, "GetTokenInformation() failed"); 

    KAutoArrayPtr<BYTE> Buffer = new BYTE[Size];

    bResult = GetTokenInformation( hProcessToken, TokenUser, Buffer, Size, &Size );

    if (!bResult)
    {
        Error = GetLastError();
        throw XError(Error, "GetTokenInformation() failed"); 
    };

    SID_AND_ATTRIBUTES* pSidAndAttr = (SID_AND_ATTRIBUTES*)(BYTE*)Buffer;

    try
    {
        fksec::sid TheSid( pSidAndAttr->Sid );
        return TheSid;
    }
    catch( fksec::ex* pX)
    {
        DWORD Error = pX->GetErrWin32();
        pX->shoo();
        throw XError( Error, "GetCurrentUserSid: fksec::sid constructor failed" );
    }
};

void CEnumWinstaGuiDlg::OnTakeOwnership()
{
    HTREEITEM hItem = m_WinstaTree.GetSelectedItem();
    if (!hItem) return;

    try
    {
        KHandle hProcessToken = GetProcessToken();

        // Get current user SID
        fksec::sid UserSid = GetUserSid( hProcessToken );
        BOOL bNeedPrivilege = FALSE;
                       
        KHandle hObject;

        // try to open the object with WRITE_OWNER access
        try
        {
            hObject = OpenUserObject( hItem, WRITE_OWNER );
        }
        catch (XError X)
        {
            // we don't have WRITE_OWNER rights on the object
            // try open the object without rights, but use "take ownership" privilege            
            // if this fails, exception will be caught by the outmost 'try/catch' block
            bNeedPrivilege = TRUE;
            hObject = OpenUserObject( hItem, 0 );
        };

        // if need the privilege, enable it
        if (bNeedPrivilege)
        {
            fksec::priv Privilege( SE_TAKE_OWNERSHIP_NAME );
            Privilege.SetHandle( hProcessToken );
            Privilege.Enable();
        };

        // try to update owner information on the object
        fksec::sd NewSd;
        NewSd.SetOwnerSid(UserSid);
        fksec::SetUserObjectSecurity( hObject, OWNER_SECURITY_INFORMATION, NewSd );
    }
    catch (fksec::ex* pX)
    {
        DWORD Error = pX->GetErrWin32();
        pX->shoo();
        XError X(Error, "Cannot update owner information on the object");
        X.Show(this);
        return;
    }
    catch (XError X)
    {
        X.Show(this);
        return;
    };
};

struct KDesktopThreadParam
{
    CEnumWinstaGuiDlg* pDialog;
    HDESK hDesktop;
    LPCTSTR MessageText;
    DWORD Result;
};

static DWORD WINAPI DesktopThread( LPVOID RawParam )
{
    KDesktopThreadParam* Param = (KDesktopThreadParam*)RawParam;
    HDESK hDesktop = Param->hDesktop;
    CEnumWinstaGuiDlg* pDlg = Param->pDialog;
    Param->Result = 0;

    KDesktop hCurrentDesktop = GetThreadDesktop( GetCurrentThreadId() );

    if (!SetThreadDesktop(hDesktop))
    {
        Param->Result = GetLastError();
        return 1;
    };

    if (!SwitchDesktop(hDesktop))
    {
        Param->Result = GetLastError();
        return 1;
    };

    ::MessageBox( 
                  NULL, Param->MessageText, 
                  "EnumWinstaGui", 
                  MB_OK | MB_ICONINFORMATION | MB_TOPMOST  | MB_SERVICE_NOTIFICATION 
                );

    SetThreadDesktop(hCurrentDesktop);
    SwitchDesktop(hCurrentDesktop);
    return 0;
};

void CEnumWinstaGuiDlg::OnSwitchTo()
{
    // Can't switch to other desktop from current thread,
    // because it already has a window on current desktop.
    // Must use another thread for that

    HTREEITEM hItem = m_WinstaTree.GetSelectedItem();
    if (!hItem) return;

    KDesktop hDesktop;

    try
    {
        hDesktop = OpenDesktop( hItem, DESKTOP_SWITCHDESKTOP | DESKTOP_CREATEWINDOW );
    }
    catch (XError &X)
    {
        X.Show(this);
        return;
    };

    CString Name = m_WinstaTree.GetItemText(hItem);
    CString MessageText;
    MessageText.Format("You are now on desktop \"%s\".\nClose this window to return to default desktop", Name );

    KDesktopThreadParam Param;
    Param.pDialog = this;
    Param.hDesktop = hDesktop;
    Param.MessageText = MessageText;
    Param.Result = 0;

    DWORD Id;

    // Can't use CWinThread, because it somehow ties the thread
    // being created to the current desktop.
    HANDLE hThread = CreateThread( 
                                    NULL,
                                    0,
                                    DesktopThread, 
                                    &Param,
                                    0,
                                    &Id
                                 );

    WaitForSingleObject( hThread, INFINITE );
    CloseHandle(hThread);

    if (Param.Result != 0)
    {
        XError X( Param.Result, "Cannot switch to desktop %s", Name );
        X.Show(this);
        return;
    }
};
