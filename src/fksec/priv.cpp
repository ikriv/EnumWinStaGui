// recommended includes in stdafx.h (or in the precompiled header, to be precise):
// windows.h, tchar.h, string, vector, algorithm, exception, sstream, iomanip

#include "stdafx.h"
#define FKSEC_NO_AUTO_INCLUDES 1
#include "fksec.h"
#include "ex.h"
#include "priv.h"

/**@#-*/
using namespace fksec;
/**@#+*/



// --- ctors/dtor ---

priv::priv()
{
	Init();
}


priv::priv( const priv &s )
{
	privNum = s.privNum;
	privName = s.privName;
	dispName = s.dispName;
	ht = 0;
	try { SetHandle( s.ht ); }
	RETHROWEX( "priv::priv(const priv&): invalid token handle" )
}


priv::priv( const LUID s )
{
	privNum = s;
	try { LookupNames(); }
	RETHROWEX( "priv::priv(const LUID): LookupNames() failed; invalid privilege LUID?" )

	ht = 0;
}


priv::priv( unsigned __int64 s )
{
	privNum.LowPart = (DWORD) ( s & 0xffffffffUI64 );
	privNum.HighPart = (DWORD) ( s >> 32 );

	try { LookupNames(); }
	RETHROWEX( "priv::priv(unsigned __int64): LookupNames() failed; invalid privilege LUID?" )

	ht = 0;
}


priv::priv( const TCHAR *name )
{
	try { LookupValue( name ); }
	RETHROWEX( "priv::priv(const TCHAR *): LookupValue() failed; invalid privilege name?" )

	ht = 0;
}


priv::~priv()
{
	try { closeToken(); }
	catch ( ex & ) { } // eat exceptions
}



// --- assignment ---

const priv &priv::operator=( const priv &s )
{
	if ( this != &s )
	{
		privNum = s.privNum;
		privName = s.privName;
		dispName = s.dispName;
		try { closeToken(); }
		RETHROWEX( "priv::operator=(): closeToken() failed" )
		try { SetHandle( s.ht ); }
		RETHROWEX( "priv::operator=(const priv&): invalid token handle" )
	}

	return *this;
}


const priv &priv::operator=( const LUID s )
{
	privNum = s;
	try { LookupNames(); }
	RETHROWEX( "priv::operator=(const LUID): LookupNames() failed; invalid privilege LUID?" )
	return *this;
}


const priv &priv::operator=( unsigned __int64 s )
{
	privNum.LowPart = (DWORD) ( s & 0xffffffffUI64 );
	privNum.HighPart = (DWORD) ( s >> 32 );

	try { LookupNames(); }
	RETHROWEX( "priv::operator=(unsigned __int64): LookupNames() failed; invalid privilege LUID?" )
	return *this;
}


const priv &priv::operator=( const TCHAR *name )
{
	try { LookupValue( name ); }
	RETHROWEX( "priv::operator=(const TCHAR *): LookupValue() failed; invalid privilege name?" )

	return *this;
}



// --- comparisons ---

bool priv::operator==( const priv &r ) const
{
	return privNum.LowPart == r.privNum.LowPart && privNum.HighPart == r.privNum.HighPart;
}


bool priv::operator!=( const priv &r ) const
{
	return privNum.LowPart != r.privNum.LowPart || privNum.HighPart != r.privNum.HighPart;
}



// --- inserters ---

fkostream &fksec::operator<<( fkostream &o, const priv& p )
{
	bool state, haveState;

	try { state = p.GetState(); haveState = true; }
	catch ( ex & ) { haveState = false; }

	o << _T( "Privilege: " ) << p.privNum.LowPart << _T( ", " ) <<
		p.privName << _T( " (" ) << p.dispName << _T( ")" );
	if ( haveState )
		o << _T( ", state: " ) << ( state? _T( "enabled" ): _T( "disabled" ) );
	return o;
}



// --- utilities ---

const TCHAR *priv::GetPrivilegeName() const
{
	return privName.c_str();
}


const TCHAR *priv::GetDisplayName() const
{
	return dispName.c_str();
}


bool priv::Enable()
{
	try { return SetState( true ); }
	RETHROWEX( "priv::Enable(): SetState() failed" )
}


bool priv::Disable()
{
	try { return SetState( false ); }
	RETHROWEX( "priv::Disable(): SetState() failed" )
}


bool priv::Toggle()
{
	bool oldState;

	try { oldState = GetState(); }
	RETHROWEX( "priv::Toggle(): GetState() failed" )

	try { return SetState( ! oldState ); }
	RETHROWEX( "priv::Toggle(): SetState() failed" )
}


bool priv::SetState( bool newState )
{
	TOKEN_PRIVILEGES tp, otp;
	DWORD returnLength = sizeof otp;

	try { openToken(); }
	RETHROWEX( "priv::SetState(): openToken() failed" )

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = privNum;
	tp.Privileges[0].Attributes = newState? SE_PRIVILEGE_ENABLED: 0;

	if ( ::AdjustTokenPrivileges( ht, FALSE, &tp, sizeof tp, &otp, &returnLength ) )
	{
		if ( GetLastError() == ERROR_SUCCESS )
			// all is well, return previous state
			return !! ( ( SE_PRIVILEGE_ENABLED | SE_PRIVILEGE_ENABLED_BY_DEFAULT ) &
				otp.Privileges[0].Attributes );
		throw NEWEX32( errStubbornPriv, "priv::SetState(): AdjustTokenPrivileges() succeeded but did not set the privilege state, see ex::GetErrWin32()", GetLastError() );
	}

	throw NEWEX32( errStubbornPriv, "priv::SetState(): AdjustTokenPrivileges() failed, see ex::GetErrWin32()", GetLastError() );
}


bool priv::GetState() const
{
	// this limits us to 50 privs; currently defined (NT5 RC3) are 27, plus 4 logon rights.
	byte buf[sizeof TOKEN_PRIVILEGES + 50 * sizeof LUID_AND_ATTRIBUTES];
	TOKEN_PRIVILEGES &tp = *( (TOKEN_PRIVILEGES *) buf );
	DWORD returnLength, i;

	try { openToken(); }
	RETHROWEX( "priv::GetState(): openToken() failed" )

	if ( ::GetTokenInformation( ht, TokenPrivileges, &buf[0], sizeof buf, &returnLength ) )
	{
		for ( i = 0; i < tp.PrivilegeCount; ++ i )
		{
			if ( privNum.LowPart == tp.Privileges[i].Luid.LowPart &&
				privNum.HighPart == tp.Privileges[i].Luid.HighPart )
				return !! ( ( SE_PRIVILEGE_ENABLED | SE_PRIVILEGE_ENABLED_BY_DEFAULT ) & tp.Privileges[i].Attributes );
		}
		return false; // a missing priv is considered disabled
	}

	throw NEWEX32( errQueryToken, "priv::GetState(): GetTokenInformation() failed, see ex::GetErrWin32()", GetLastError() );
}


void priv::SetHandle( HANDLE h )
{
	try { closeToken(); }
	RETHROWEX( "priv::SetHandle(): closeToken() failed" )

	if ( h != 0 )
	{
		if ( ! ::DuplicateHandle( GetCurrentProcess(), h, GetCurrentProcess(),
			&ht, TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, FALSE, 0 ) )
			throw NEWEX32( errDupTokenHandle, "priv::SetHandle(): DuplicateHandle() failed, see ex::GetErrWin32()", GetLastError() );
	}
	else
		ht = h; // == 0
}


void priv::Init()
{
	ht = 0;
	privNum.LowPart = privNum.HighPart = 0;
	privName.erase();
	dispName.erase();
}


void priv::openToken() const
{
	DWORD rc;
	const TCHAR *func = _T( "what?" );

	if ( ht != 0 )
		return;

	rc = 0;
	func = _T( "OpenThreadToken()" );
	if ( ! ::OpenThreadToken( GetCurrentThread(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, FALSE, &ht ) )
		rc = GetLastError();

	// OTT() failed with something recoverable?
	if ( rc == ERROR_ACCESS_DENIED || rc == ERROR_NO_TOKEN || rc == ERROR_NO_IMPERSONATION_TOKEN )
	{
		rc = 0;
		func = _T( "OpenThreadToken() as self" );
		if ( ! ::OpenThreadToken( GetCurrentThread(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, TRUE, &ht ) )
			rc = GetLastError();

		// OTT() failed again? try OPT()
		if ( rc == ERROR_ACCESS_DENIED || rc == ERROR_NO_TOKEN || rc == ERROR_NO_IMPERSONATION_TOKEN )
		{
			rc = 0;
			func = _T( "OpenProcessToken()" );
			if ( ! ::OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &ht ) )
				rc = GetLastError();
		}
	}

	if ( rc != 0 )
	{
		ht = 0;
		fkstr errstr = _T( "priv::openToken(): " ) + fkstr( func ) + _T( " failed, see ex::GetErrWin32()" );
		throw new ex( _T( __FILE__ ), __LINE__, errOpenToken, errstr.c_str(), rc );
	}
}


void priv::closeToken() const
{
	if ( ht != 0 )
	{
		if ( ! CloseHandle( ht ) )
		{
			ht = 0;
			throw NEWEX32( errCloseToken, "priv::closeToken(): CloseHandle() failed, see ex::GetErrWin32()", GetLastError() );
		}
		ht = 0;
	}
}


void priv::LookupNames() const
{
	const static DWORD initialBufLen = 128;
	DWORD namelen, buflen, langID, rc;
	BOOL result;
	TCHAR *name;

	buflen = initialBufLen;
	name = 0;
	do
	{
		buflen *= 2;
		delete [] name;
		name = new TCHAR[buflen];
		if ( name == 0 )
			throw NEWEX( errNoMemory, "priv::LookupNames(): no memory for priv name buffer" );
		namelen = buflen;
		result = ::LookupPrivilegeName( 0,
			const_cast<LUID *>( &privNum ), name, &namelen );
		rc = result? 0: GetLastError();
	} while ( ! result && rc == ERROR_INSUFFICIENT_BUFFER );

	if ( rc == 0 )
		privName = name;

	delete [] name;

	if ( rc != 0 )
		throw NEWEX32( errInvalidPriv, "priv::LookupNames(): privilege name not found", rc );

	buflen = initialBufLen;
	name = 0;
	do
	{
		buflen *= 2;
		delete [] name;
		name = new TCHAR[buflen];
		if ( name == 0 )
			throw NEWEX( errNoMemory, "priv::LookupNames(): no memory for priv display-name buffer" );
		namelen = buflen;
		result = ::LookupPrivilegeDisplayName( 0,
			privName.c_str(), name, &namelen, &langID );
		rc = result? 0: GetLastError();
	} while ( ! result && rc == ERROR_INSUFFICIENT_BUFFER );

	if ( rc == 0 )
		dispName = name;

	delete [] name;

	if ( rc != 0 )
		throw NEWEX32( errInvalidPriv, "priv::LookupNames(): privilege display-name not found", rc );
}


void priv::LookupValue( const TCHAR *name )
{
	if ( ! ::LookupPrivilegeValue( 0, name, &privNum ) )
		throw NEWEX32( errInvalidPriv, "priv::LookupValue(): privilege not found", GetLastError() );

	try { LookupNames(); }
	RETHROWEX( "priv::LookupValue(): cannot get names for privilege" )
}
