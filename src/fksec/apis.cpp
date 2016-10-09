// recommended includes in stdafx.h (or in the precompiled header, to be precise):
// windows.h, tchar.h, string, vector, algorithm, exception, sstream, iomanip

#include "stdafx.h"
#define FKSEC_NO_AUTO_INCLUDES 1
#include "fksec.h"
#include "ex.h"
#include "priv.h"
#include "sid.h"
#include "ace.h"
#include "acl.h"
#include "sd.h"
#include "apis.h"

/**@#-*/ // ccdoc parsing disabled -- gives dups of global functions

namespace fksec {
	static const int initialBufferSize = 2048;
} // namespace fksec

using fksec::errUnreadableSD;
using fksec::errUnwritableSD;
using fksec::ex;
using fksec::priv;
using fksec::sd;
using fksec::GetFileSecurity;
using fksec::SetFileSecurity;
using fksec::GetKernelObjectSecurity;
using fksec::SetKernelObjectSecurity;
using fksec::GetUserObjectSecurity;
using fksec::SetUserObjectSecurity;
using fksec::QueryServiceObjectSecurity;
using fksec::SetServiceObjectSecurity;
using fksec::RegGetKeySecurity;
using fksec::RegSetKeySecurity;
using fksec::initialBufferSize;



void GetFileSecurity( const TCHAR *filename, SECURITY_INFORMATION whichParts, sd &sd )
{
	SECURITY_DESCRIPTOR *psd;
	DWORD needed, rc;
	bool haveRetriedPrivilege = false, haveRetriedSize = false;
	bool oldSecPrivState = false;
	priv secPriv;

	try { secPriv = _T( "SeSecurityPrivilege" ); }
	RETHROWEX( "fksec::GetFileSecurity(): cannot identify SeSecurityPrivilege" )

	needed = initialBufferSize;
	psd = (SECURITY_DESCRIPTOR *) new byte[initialBufferSize];

	for ( ; ; )
	{
		if ( ::GetFileSecurity( filename, whichParts, psd, needed, &needed ) )
			rc = 0;
		else
			rc = GetLastError();

		if ( rc == ERROR_INSUFFICIENT_BUFFER && ! haveRetriedSize )
		{
			delete [] (byte *) psd;
			psd = (SECURITY_DESCRIPTOR *) new byte[needed];
			haveRetriedSize = true;
		}
		else if ( rc == ERROR_PRIVILEGE_NOT_HELD && ! haveRetriedPrivilege )
		{
			try { oldSecPrivState = secPriv.Enable(); }
			catch ( ex *e ) { delete e; }
			haveRetriedPrivilege = true;
		}
		else
			break; // unrecoverable or all OK
	}

	if ( rc == 0 )
		sd = psd;

	delete [] (byte *) psd;

	if ( haveRetriedPrivilege ) // did we fuck with the privilege?
	{
		try { secPriv.SetState( oldSecPrivState ); }
		catch ( ex *e ) { delete e; } // just ignore errors here
	}

	if ( rc )
		throw NEWEX32( errUnreadableSD, "fksec::GetFileSecurity(): cannot read SD, see ex::GetErrWin32()", rc );
}



void SetFileSecurity( const TCHAR *filename, SECURITY_INFORMATION whichParts, const sd &sd )
{
	DWORD rc;
	bool haveRetriedPrivilege = false;
	bool oldSecPrivState = false;
	priv secPriv;

	try { secPriv = _T( "SeSecurityPrivilege" ); }
	RETHROWEX( "fksec::SetFileSecurity(): cannot identify SeSecurityPrivilege" )

	for ( ; ; )
	{
		if ( ::SetFileSecurity( filename, whichParts, (SECURITY_DESCRIPTOR *) sd ) )
			rc = 0;
		else
			rc = GetLastError();

		if ( rc == ERROR_PRIVILEGE_NOT_HELD && ! haveRetriedPrivilege )
		{
			try { oldSecPrivState = secPriv.Enable(); }
			catch ( ex *e ) { delete e; }
			haveRetriedPrivilege = true;
		}
		else
			break; // unrecoverable or all OK
	}

	if ( haveRetriedPrivilege ) // did we fuck with the privilege?
	{
		try { secPriv.SetState( oldSecPrivState ); }
		catch ( ex *e ) { delete e; } // just ignore errors here
	}

	if ( rc )
		throw NEWEX32( errUnwritableSD, "fksec::SetFileSecurity(): cannot write SD, see ex::GetErrWin32()", rc );
}



void GetKernelObjectSecurity( HANDLE h, SECURITY_INFORMATION whichParts, sd &sd )
{
	SECURITY_DESCRIPTOR *psd;
	DWORD needed, rc;
	bool haveRetriedPrivilege = false, haveRetriedSize = false;
	bool oldSecPrivState = false;
	priv secPriv;

	try { secPriv = _T( "SeSecurityPrivilege" ); }
	RETHROWEX( "fksec::GetKernelObjectSecurity(): cannot identify SeSecurityPrivilege" )

	needed = initialBufferSize;
	psd = (SECURITY_DESCRIPTOR *) new byte[initialBufferSize];

	for ( ; ; )
	{
		if ( ::GetKernelObjectSecurity( h, whichParts, psd, needed, &needed ) )
			rc = 0;
		else
			rc = GetLastError();

		if ( rc == ERROR_INSUFFICIENT_BUFFER && ! haveRetriedSize )
		{
			delete [] (byte *) psd;
			psd = (SECURITY_DESCRIPTOR *) new byte[needed];
			haveRetriedSize = true;
		}
		else if ( rc == ERROR_PRIVILEGE_NOT_HELD && ! haveRetriedPrivilege )
		{
			try { oldSecPrivState = secPriv.Enable(); }
			catch ( ex *e ) { delete e; }
			haveRetriedPrivilege = true;
		}
		else
			break; // unrecoverable or just fine
	}

	if ( rc == 0 )
		sd = psd;

	delete [] (byte *) psd;

	if ( haveRetriedPrivilege ) // did we fuck with the privilege?
	{
		try { secPriv.SetState( oldSecPrivState ); }
		catch ( ex *e ) { delete e; } // just ignore errors here
	}

	if ( rc )
		throw NEWEX32( errUnreadableSD, "fksec::GetKernelObjectSecurity(): cannot read SD, see ex::GetErrWin32()", rc );
}



void SetKernelObjectSecurity( HANDLE h, SECURITY_INFORMATION whichParts, const sd &sd )
{
	DWORD rc;
	bool haveRetriedPrivilege = false;
	bool oldSecPrivState = false;
	priv secPriv;

	try { secPriv = _T( "SeSecurityPrivilege" ); }
	RETHROWEX( "fksec::SetKernelObjectSecurity(): cannot identify SeSecurityPrivilege" )

	for ( ; ; )
	{
		if ( ::SetKernelObjectSecurity( h, whichParts, (SECURITY_DESCRIPTOR *) sd ) )
			rc = 0;
		else
			rc = GetLastError();

		if ( rc == ERROR_PRIVILEGE_NOT_HELD && ! haveRetriedPrivilege )
		{
			try { oldSecPrivState = secPriv.Enable(); }
			catch ( ex *e ) { delete e; }
			haveRetriedPrivilege = true;
		}
		else
			break; // unrecoverable
	}

	if ( haveRetriedPrivilege ) // did we fuck with the privilege?
	{
		try { secPriv.SetState( oldSecPrivState ); }
		catch ( ex *e ) { delete e; } // just ignore errors here
	}

	if ( rc )
		throw NEWEX32( errUnwritableSD, "fksec::SetKernelObjectSecurity(): cannot write SD, see ex::GetErrWin32()", rc );
}



void GetUserObjectSecurity( HANDLE h, SECURITY_INFORMATION whichParts, sd &sd )
{
	SECURITY_DESCRIPTOR *psd;
	DWORD needed, rc;
	bool haveRetriedPrivilege = false, haveRetriedSize = false;
	bool oldSecPrivState = false;
	priv secPriv;

	try { secPriv = _T( "SeSecurityPrivilege" ); }
	RETHROWEX( "fksec::GetUserObjectSecurity(): cannot identify SeSecurityPrivilege" )

	needed = initialBufferSize;
	psd = (SECURITY_DESCRIPTOR *) new byte[initialBufferSize];

	for ( ; ; )
	{
		if ( ::GetUserObjectSecurity( h, &whichParts, psd, needed, &needed ) )
			rc = 0;
		else
			rc = GetLastError();

		if ( rc == ERROR_INSUFFICIENT_BUFFER && ! haveRetriedSize )
		{
			delete [] (byte *) psd;
			psd = (SECURITY_DESCRIPTOR *) new byte[needed];
			haveRetriedSize = true;
		}
		else if ( rc == ERROR_PRIVILEGE_NOT_HELD && ! haveRetriedPrivilege )
		{
			try { oldSecPrivState = secPriv.Enable(); }
			catch ( ex *e ) { delete e; }
			haveRetriedPrivilege = true;
		}
		else
			break; // unrecoverable
	}

	if ( rc == 0 )
		sd = psd;

	delete [] (byte *) psd;

	if ( haveRetriedPrivilege ) // did we fuck with the privilege?
	{
		try { secPriv.SetState( oldSecPrivState ); }
		catch ( ex *e ) { delete e; } // just ignore errors here
	}

	if ( rc )
		throw NEWEX32( errUnreadableSD, "fksec::GetUserObjectSecurity(): cannot read SD, see ex::GetErrWin32()", rc );
}



void SetUserObjectSecurity( HANDLE h, SECURITY_INFORMATION whichParts, const sd &sd )
{
	DWORD rc;
	bool haveRetriedPrivilege = false;
	bool oldSecPrivState = false;
	priv secPriv;

	try { secPriv = _T( "SeSecurityPrivilege" ); }
	RETHROWEX( "fksec::SetUserObjectSecurity(): cannot identify SeSecurityPrivilege" )

	for ( ; ; )
	{
		if ( ::SetUserObjectSecurity( h, &whichParts, (SECURITY_DESCRIPTOR *) sd ) )
			rc = 0;
		else
			rc = GetLastError();

		if ( rc == ERROR_PRIVILEGE_NOT_HELD && ! haveRetriedPrivilege )
		{
			try { oldSecPrivState = secPriv.Enable(); }
			catch ( ex *e ) { delete e; }
			haveRetriedPrivilege = true;
		}
		else
			break; // unrecoverable
	}

	if ( haveRetriedPrivilege ) // did we fuck with the privilege?
	{
		try { secPriv.SetState( oldSecPrivState ); }
		catch ( ex *e ) { delete e; } // just ignore errors here
	}

	if ( rc )
		throw NEWEX32( errUnwritableSD, "fksec::SetUserObjectSecurity(): cannot write SD, see ex::GetErrWin32()", rc );
}



void QueryServiceObjectSecurity( SC_HANDLE hSvc, SECURITY_INFORMATION whichParts, sd &sd )
{
	SECURITY_DESCRIPTOR *psd;
	DWORD needed, rc;
	bool haveRetriedPrivilege = false, haveRetriedSize = false;
	bool oldSecPrivState = false;
	priv secPriv;

	try { secPriv = _T( "SeSecurityPrivilege" ); }
	RETHROWEX( "fksec::QueryServiceObjectSecurity(): cannot identify SeSecurityPrivilege" )

	needed = initialBufferSize;
	psd = (SECURITY_DESCRIPTOR *) new byte[initialBufferSize];

	for ( ; ; )
	{
		if ( ::QueryServiceObjectSecurity( hSvc, whichParts, psd, needed, &needed ) )
			rc = 0;
		else
			rc = GetLastError();

		if ( rc == ERROR_INSUFFICIENT_BUFFER && ! haveRetriedSize )
		{
			delete [] (byte *) psd;
			psd = (SECURITY_DESCRIPTOR *) new byte[needed];
			haveRetriedSize = true;
		}
		else if ( rc == ERROR_PRIVILEGE_NOT_HELD && ! haveRetriedPrivilege )
		{
			try { oldSecPrivState = secPriv.Enable(); }
			catch ( ex *e ) { delete e; }
			haveRetriedPrivilege = true;
		}
		else
			break; // unrecoverable
	}

	if ( rc == 0 )
		sd = psd;

	delete [] (byte *) psd;

	if ( haveRetriedPrivilege ) // did we fuck with the privilege?
	{
		try { secPriv.SetState( oldSecPrivState ); }
		catch ( ex *e ) { delete e; } // just ignore errors here
	}

	if ( rc )
		throw NEWEX32( errUnreadableSD, "fksec::QueryServiceObjectSecurity(): cannot read SD, see ex::GetErrWin32()", rc );
}



void SetServiceObjectSecurity( SC_HANDLE hSvc, SECURITY_INFORMATION whichParts, const sd &sd )
{
	DWORD rc;
	bool haveRetriedPrivilege = false;
	bool oldSecPrivState = false;
	priv secPriv;

	try { secPriv = _T( "SeSecurityPrivilege" ); }
	RETHROWEX( "fksec::SetServiceObjectSecurity(): cannot identify SeSecurityPrivilege" )

	for ( ; ; )
	{
		if ( ::SetServiceObjectSecurity( hSvc, whichParts, (SECURITY_DESCRIPTOR *) sd ) )
			rc = 0;
		else
			rc = GetLastError();

		if ( rc == ERROR_PRIVILEGE_NOT_HELD && ! haveRetriedPrivilege )
		{
			try { oldSecPrivState = secPriv.Enable(); }
			catch ( ex *e ) { delete e; }
			haveRetriedPrivilege = true;
		}
		else
			break; // unrecoverable
	}

	if ( haveRetriedPrivilege ) // did we fuck with the privilege?
	{
		try { secPriv.SetState( oldSecPrivState ); }
		catch ( ex *e ) { delete e; } // just ignore errors here
	}

	if ( rc )
		throw NEWEX32( errUnwritableSD, "fksec::SetServiceObjectSecurity(): cannot write SD, see ex::GetErrWin32()", rc );
}



void RegGetKeySecurity( HKEY hk, SECURITY_INFORMATION whichParts, sd &sd )
{
	SECURITY_DESCRIPTOR *psd;
	DWORD needed, rc;
	bool haveRetriedPrivilege = false, haveRetriedSize = false;
	bool oldSecPrivState = false;
	priv secPriv;

	try { secPriv = _T( "SeSecurityPrivilege" ); }
	RETHROWEX( "fksec::RegGetKeySecurity(): cannot identify SeSecurityPrivilege" )

	needed = initialBufferSize;
	psd = (SECURITY_DESCRIPTOR *) new byte[initialBufferSize];

	for ( ; ; )
	{
		rc = ::RegGetKeySecurity( hk, whichParts, psd, &needed );

		if ( rc == ERROR_INSUFFICIENT_BUFFER && ! haveRetriedSize )
		{
			delete [] (byte *) psd;
			psd = (SECURITY_DESCRIPTOR *) new byte[needed];
			haveRetriedSize = true;
		}
		else if ( rc == ERROR_PRIVILEGE_NOT_HELD && ! haveRetriedPrivilege )
		{
			try { oldSecPrivState = secPriv.Enable(); }
			catch ( ex *e ) { delete e; }
			haveRetriedPrivilege = true;
		}
		else
			break; // unrecoverable
	}

	if ( rc == 0 )
		sd = psd;

	delete [] (byte *) psd;

	if ( haveRetriedPrivilege ) // did we fuck with the privilege?
	{
		try { secPriv.SetState( oldSecPrivState ); }
		catch ( ex *e ) { delete e; } // just ignore errors here
	}

	if ( rc )
		throw NEWEX32( errUnreadableSD, "fksec::RegGetKeySecurity(): cannot read SD, see ex::GetErrWin32()", rc );
}



void RegSetKeySecurity( HKEY hk, SECURITY_INFORMATION whichParts, const sd &sd )
{
	DWORD rc;
	bool haveRetriedPrivilege = false;
	bool oldSecPrivState = false;
	priv secPriv;

	try { secPriv = _T( "SeSecurityPrivilege" ); }
	RETHROWEX( "fksec::RegSetKeySecurity(): cannot identify SeSecurityPrivilege" )

	for ( ; ; )
	{
		rc = ::RegSetKeySecurity( hk, whichParts, (SECURITY_DESCRIPTOR *) sd );

		if ( rc == ERROR_PRIVILEGE_NOT_HELD && ! haveRetriedPrivilege )
		{
			try { oldSecPrivState = secPriv.Enable(); }
			catch ( ex *e ) { delete e; }
			haveRetriedPrivilege = true;
		}
		else
			break; // unrecoverable or all OK
	}

	if ( haveRetriedPrivilege ) // did we fuck with the privilege?
	{
		try { secPriv.SetState( oldSecPrivState ); }
		catch ( ex *e ) { delete e; } // just ignore errors here
	}

	if ( rc )
		throw NEWEX32( errUnwritableSD, "fksec::RegSetKeySecurity(): cannot write SD, see ex::GetErrWin32()", rc );
}

/**@#+*/ // ccdoc parsing disabled -- gives dups of global functions
