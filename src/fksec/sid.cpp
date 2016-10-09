// recommended includes in stdafx.h (or in the precompiled header, to be precise):
// windows.h, tchar.h, string, vector, algorithm, exception, sstream, iomanip

#include "stdafx.h"
#define FKSEC_NO_AUTO_INCLUDES 1
#include "fksec.h"
#include "ex.h"
#include "sid.h"
#include <lm.h>

/**@#-*/
using namespace fksec;
/**@#+*/



// --- ctors/dtor ---

// construct an empty sid object
sid::sid()
{
	Init();
	ClearPSID();
}


// construct from another sid object
sid::sid( const sid &s )
{
	Init();
	ClearPSID();
	isValid = s.isValid;
	if ( isValid )
	{
		sia = s.sia;
		subAuthCount = s.subAuthCount;
		memcpy( subAuth, s.subAuth, sizeof subAuth );
	}
}


// construct from a PSID
sid::sid( const PSID s )
{
	DWORD i;

	Init();
	ClearPSID();
	if ( IsValidSid( s ) )
	{
		isValid = true;
		sia = *::GetSidIdentifierAuthority( s );
		subAuthCount = (DWORD) *GetSidSubAuthorityCount( s );
		if ( subAuthCount < lenof( subAuth ) )
		{
			for ( i = 0; i < subAuthCount; ++ i )
				subAuth[i] = *GetSidSubAuthority( s, i );
		}
		else
			throw NEWEX( errTooManySubAuths, "sid::sid( PSID ): more than SID_MAX_SUB_AUTHORITIES subauthorities in the SID" );
	}
	else
		throw NEWEX( errInvalidSid, "sid::sid( PSID ): invalid SID passed in" );
}


// fill the SID by looking up a name (server will be NULL)
// or by interpreting a strig SID
sid::sid( const TCHAR *name )
{
	Init();
	ClearPSID();

	try { ConvertFromStringSid( name ); }
	catch ( ex *e )
	{
		if ( e->GetErr() != errInvalidSid )
		{
			e->FKSECADDHOP( "sid::sid( name )" );
			throw;
		}

		// not a string SID, so let's try something else
		delete e;
		try { LookupName( /* server */ 0, name ); }
		RETHROWEX( "sid::sid( name )" )
	}
}


// construct from parts
sid::sid( SID_IDENTIFIER_AUTHORITY newsia, DWORD nSubAuths,
	DWORD subAuth0 /* = 0 */, DWORD subAuth1 /* = 0 */, DWORD subAuth2 /* = 0 */,
	DWORD subAuth3 /* = 0 */, DWORD subAuth4 /* = 0 */, DWORD subAuth5 /* = 0 */,
	DWORD subAuth6 /* = 0 */, DWORD subAuth7 /* = 0 */ )
{
	Init();
	ClearPSID();
	sia = newsia;
	subAuthCount = nSubAuths;
	subAuth[0] = subAuth0;
	subAuth[1] = subAuth1;
	subAuth[2] = subAuth2;
	subAuth[3] = subAuth3;
	subAuth[4] = subAuth4;
	subAuth[5] = subAuth5;
	subAuth[6] = subAuth6;
	subAuth[7] = subAuth7;
	isValid = true;
}


// clean up
sid::~sid()
{
	ReleasePSID();
}



// --- assignment ---

// from another sid
const sid &sid::operator=( const sid &s )
{
	if ( this != &s )
	{
		ReleasePSID();
		isValid = s.isValid;
		if ( isValid )
		{
			sia = s.sia;
			subAuthCount = s.subAuthCount;
			memcpy( subAuth, s.subAuth, sizeof subAuth );
		}
	}

	return *this;
}


// from PSID
const sid &sid::operator=( const PSID s )
{
	DWORD i;

	ReleasePSID();
	Init();
	if ( ::IsValidSid( s ) )
	{
		isValid = true;
		sia = *::GetSidIdentifierAuthority( s );
		subAuthCount = (DWORD) *::GetSidSubAuthorityCount( s );
		if ( subAuthCount < lenof( subAuth ) )
		{
			for ( i = 0; i < subAuthCount; ++ i )
				subAuth[i] = *::GetSidSubAuthority( s, i );
		}
		else
			throw NEWEX( errTooManySubAuths,
				"sid::operator=( PSID ): more than SID_MAX_SUB_AUTHORITIES subauthorities in the SID" );
	}
	else
		throw NEWEX( errInvalidSid, "sid::operator=( PSID ): invalid SID passed in" );

	return *this;
}


// fill the SID by looking up a name (server will be NULL)
// or by interpreting a strig SID
const sid &sid::operator=( const TCHAR *name )
{
	ReleasePSID();
	Init();

	try { ConvertFromStringSid( name ); }
	catch ( ex *e )
	{
		if ( e->GetErr() != errInvalidSid )
		{
			e->FKSECADDHOP( "sid::operator=(TCHAR*)" );
			throw;
		}

		// invalid SID, let's try something different
		delete e;

		try { LookupName( /* server */ 0, name ); }
		RETHROWEX( "sid::operator=(TCHAR*)" )
	}


	return *this;
}



// --- conversions ---

// return a pointer to an internally-maintained SID
sid::operator const PSID() const
{
	if ( ! isValid )
		throw NEWEX( errInvalidSid, "sid::operator const PSID(): invalid SID" );

	try { MakePSID(); }
	RETHROWEX( "sid::operator const PSID(): MakePSID() failed" )

	return psid;
}



// --- comparisons ---

bool sid::operator<( const sid &r ) const
{
	DWORD i;
	__int64 tsia, rsia;

	if ( ! isValid || ! r.isValid )
		throw NEWEX( errInvalidSid, "sid::operator<( sid ): invalid SID" );

	tsia = GetSidIdentifierAuthority();
	rsia = r.GetSidIdentifierAuthority();
	if ( tsia < rsia )
		return true;
	if ( tsia > rsia )
		return false;

	if ( subAuthCount < r.subAuthCount )
		return true;
	if ( subAuthCount > r.subAuthCount )
		return false;

	for ( i = 0; i < subAuthCount; ++ i )
	{
		if ( subAuth[i] < r.subAuth[i] )
			return true;
		if ( subAuth[i] > r.subAuth[i] )
			return false;
	}

	return false;
}


bool sid::operator==( const sid &r ) const
{
	DWORD i;

	if ( ! isValid || ! r.isValid )
		throw NEWEX( errInvalidSid, "sid::operator==( sid ): invalid SID" );

	if ( subAuthCount != r.subAuthCount || 0 != memcmp( &sia, &r.sia, sizeof( sia ) ) )
		return false;

	for ( i = 0; i < subAuthCount; ++ i )
		if ( subAuth[i] != r.subAuth[i] )
			return false;

	return true;
}


bool sid::operator!=( const sid &r ) const
{
	DWORD i;

	if ( ! isValid || ! r.isValid )
		throw NEWEX( errInvalidSid, "sid::operator!=( sid ): invalid SID" );

	if ( subAuthCount != r.subAuthCount || 0 != memcmp( &sia, &r.sia, sizeof( sia ) ) )
		return true;

	for ( i = 0; i < subAuthCount; ++ i )
		if ( subAuth[i] != r.subAuth[i] )
			return true;

	return false;
}


bool sid::EqualPrefix( const sid &r ) const
{
	DWORD i;

	if ( ! isValid || ! r.isValid )
		throw NEWEX( errInvalidSid, "sid::EqualPrefix(): invalid SID" );

	if ( subAuthCount != r.subAuthCount || 0 != memcmp( &sia, &r.sia, sizeof( sia ) ) )
		return false;

	for ( i = 0; i < subAuthCount - 1; ++ i )
		if ( subAuth[i] != r.subAuth[i] )
			return false;

	return true;
}



// --- utilities ---

// how many bytes for an NT-formatted SID?
DWORD sid::GetLength() const
{
	return ::GetSidLengthRequired( (byte) subAuthCount );
}


// return SIA
__int64 sid::GetSidIdentifierAuthority() const
{
	unsigned __int64 r;

	// SIA is stored high byte first; we avoid platform
	// dependency by processing it byte for byte
	r = ( ( (unsigned __int64) sia.Value[0] ) << 40 ) |
		( ( (unsigned __int64) sia.Value[1] ) << 32 ) |
		( ( (unsigned __int64) sia.Value[2] ) << 24 ) |
		( ( (unsigned __int64) sia.Value[3] ) << 16 ) |
		( ( (unsigned __int64) sia.Value[4] ) <<  8 ) |
		( ( (unsigned __int64) sia.Value[5] )       );

	return (__int64) r;
}


// remove the last RID from this sid
void sid::RemoveLastRid()
{
	if ( ! isValid || subAuthCount == 0 )
		throw NEWEX( errInvalidSid, "sid::RemoveLastRid(): invalid SID" );

	ReleasePSID();
	-- subAuthCount;
	subAuth[subAuthCount] = 0;
}


// append a RID to the end of this SID
void sid::AppendSubAuthority( DWORD rid )
{
	if ( ! isValid )
		throw NEWEX( errInvalidSid, "sid::AppendSubAuthority(): invalid SID" );

	if ( subAuthCount >= SID_MAX_SUB_AUTHORITIES )
		throw NEWEX( errTooManySubAuths, "sid::AppendSubAuthority(): already SID_MAX_SUB_AUTHORITIES in the SID" );

	ReleasePSID();
	subAuth[subAuthCount] = rid;
	++ subAuthCount;
}


// set an existing RID
void sid::SetSubAuthority( DWORD index, DWORD rid )
{
	if ( ! isValid )
		throw NEWEX( errInvalidSid, "sid::SetSubAuthority(): invalid SID" );

	if ( index >= subAuthCount )
		throw NEWEX( errInvalidSubAuthIndex,
			"sid::SetSubAuthority(): invalid subauthority index" );

	ReleasePSID();
	subAuth[index] = rid;
}


// return a specific RID
DWORD sid::GetSubAuthority( DWORD index ) const
{
	if ( ! isValid )
		throw NEWEX( errInvalidSid, "sid::GetSubAuthority(): invalid SID" );

	if ( index >= subAuthCount )
		throw NEWEX( errInvalidSubAuthIndex,
			"sid::GetSubAuthority(): invalid subauthority index" );

	return subAuth[index];
}


// return the current subAuthCount
DWORD sid::GetSubAuthorityCount() const
{
	if ( ! isValid )
		throw NEWEX( errInvalidSid, "sid::GetSubAuthorityCount(): invalid SID" );

	return subAuthCount;
}


// test for validity
bool sid::IsValid() const
{
	PSID p;

	if ( ! isValid )
		return false;

	try { p = MakePSID(); }
	catch ( ex *e )
	{
		delete e;
		return false;
	}

	if ( p == 0 )
		return false;

	return !! IsValidSid( p );
}


// translate SID_NAME_USE to text description
const TCHAR *sid::SnuToText( SID_NAME_USE snu )
{
	static TCHAR *snuText[16] = { 0 };
	static const TCHAR *badSnuText = _T( "*SNU?*" );
	static bool snuTextInitialized = false;

	if ( ! snuTextInitialized )
	{
		snuTextInitialized = true;
		snuText[SidTypeUser]			= _T( "user" );
		snuText[SidTypeGroup]			= _T( "group" );
		snuText[SidTypeDomain]			= _T( "domain" );
		snuText[SidTypeAlias]			= _T( "alias" );
		snuText[SidTypeWellKnownGroup]	= _T( "well-known group" );
		snuText[SidTypeDeletedAccount]	= _T( "deleted" );
		snuText[SidTypeInvalid]			= _T( "invalid" );
		snuText[SidTypeUnknown]			= _T( "unknown" );
		snuText[SidTypeComputer]		= _T( "computer" );
	}

	if ( snu < (SID_NAME_USE) 1 || snu > lenof( snuText ) || snuText[snu] == 0 )
		return badSnuText;

	return snuText[snu];
}


// return a stringified SID
fkstr sid::ConvertToStringSid() const
{
	// S-rev- + SIA + subauthlen*maxsubauth + terminator
	TCHAR buf[15 + 12 + 12*SID_MAX_SUB_AUTHORITIES + 1];
	TCHAR *p = &buf[0];
	DWORD i;

	// Validate the binary SID.

	if ( ! IsValid() )
		return fkstr( _T( "*invalid*" ) );

	p += _sntprintf( p, &buf[lenof( buf )] - p, _T( "S-%lu-" ), SID_REVISION );

	if ( ( sia.Value[0] != 0 ) || ( sia.Value[1] != 0 ) )
		p += _sntprintf( p, &buf[lenof( buf )] - p, _T( "0x%02hx%02hx%02hx%02hx%02hx%02hx" ),
			(USHORT) sia.Value[0], (USHORT) sia.Value[1],
			(USHORT) sia.Value[2], (USHORT) sia.Value[3],
			(USHORT) sia.Value[4], (USHORT) sia.Value[5] );
	else
		p += _sntprintf( p, &buf[lenof( buf )] - p, _T( "%lu" ),
			(ULONG) ( sia.Value[5]       ) + (ULONG) ( sia.Value[4] << 8  ) +
			(ULONG) ( sia.Value[3] << 16 ) + (ULONG) ( sia.Value[2] << 24 ) );

	// Add SID subauthorities to the string.

	for ( i = 0; i < subAuthCount; ++ i )
		p += _sntprintf( p, &buf[lenof( buf )] - p, _T( "-%lu" ), subAuth[i] );

	return fkstr( buf );
}


// fill a client-provided buffer
void sid::StoreSid( PSID ps, DWORD size ) const
{
	DWORD i;

	if ( ! isValid )
		throw NEWEX( errInvalidSid, "sid::StoreSid(): invalid SID" );

	i = GetLength();
	if ( size < i )
		throw new ex( _T( __FILE__ ), __LINE__, errBufferTooSmall,
			_T( "sid::StoreSid(): provided buffer is too small, ex::GetData() gives required size" ), 0, i );

	if ( InitializeSid( ps, &sia, (byte) subAuthCount ) )
		for ( i = 0; i < subAuthCount; ++ i )
			*GetSidSubAuthority( ps, i ) = subAuth[i];
	else
		throw NEWEX32( errInvalidSid,
			"sid::StoreSid(): InitializeSid() failed, see GetErrWin32()", GetLastError() );
}


// fill the SID by interpreting a string SID
void sid::ConvertFromStringSid( const TCHAR *stringsid )
{
	// a string SID has the form
	// S-rev-sia-rid-rid...
	DWORD i;
	unsigned __int64 r;
	SID_IDENTIFIER_AUTHORITY tempsia = { 0 };
	DWORD rids[SID_MAX_SUB_AUTHORITIES];

	if ( stringsid == 0 )
		throw NEWEX( errInvalidSid, "sid::ConvertFromStringSid(): invalid SID (NULL)" );

	if ( *stringsid != _T( 's' ) && *stringsid != _T( 'S' ) )
		throw NEWEX( errInvalidSid, "sid::ConvertFromStringSid(): invalid SID (first char not 'S')" );

	++ stringsid; // skip 'S'

	if ( ! CvsGetUInt64( stringsid, r ) )
		throw NEWEX( errInvalidSid, "sid::ConvertFromStringSid(): invalid SID (cannot parse revision)" );
	if ( r != SID_REVISION )
		throw NEWEX( errInvalidSid, "sid::ConvertFromStringSid(): invalid SID (revision != SID_REVISION)" );

	if ( ! CvsGetUInt64( stringsid, r ) )
		throw NEWEX( errInvalidSid, "sid::ConvertFromStringSid(): invalid SID (cannot parse SIA)" );
	if ( r == 0 || r > 0xffffffffffffUI64 )
		throw NEWEX( errInvalidSid, "sid::ConvertFromStringSid(): invalid SID (SIA out of range)" );
	// SIA is stored high-byte first; to avoid machine dependency,
	// we fill it byte for byte
	for ( i = 0; i < lenof( tempsia.Value ); ++ i )
	{
		tempsia.Value[lenof( tempsia.Value ) - i - 1] = (byte) ( r & 0xff );
		r >>= 8;
	}

	for ( i = 0; i < SID_MAX_SUB_AUTHORITIES; ++ i )
	{
		if ( ! CvsGetUInt64( stringsid, r ) )
			break;
		if ( r > 0xffffffffUI64 )
			throw NEWEX( errInvalidSid, "sid::ConvertFromStringSid(): invalid SID (RID out of range)" );
		rids[i] = (DWORD) r;
	}
	// we ran out of array space, or the SID was finished.
	// in either case, stringsid should point to a '\0'.

	if ( *stringsid != _T( '\0' ) )
		throw NEWEX( errInvalidSid, "sid::ConvertFromStringSid(): invalid SID (cannot parse RID, or trailing garbage in string)" );

	// we have a complete SID. Stash it.
	ReleasePSID();
	Init();
	sia = tempsia;
	subAuthCount = i;
	memcpy( subAuth, rids, subAuthCount * sizeof rids[0] );
	isValid = true;
}


// fill the SID by looking the user up on a server
void sid::LookupName( const TCHAR *server, const TCHAR *name )
{
	PSID ps;
	DWORD pslen, domlen;
	SID_NAME_USE snu;
	TCHAR domain[256];

	domlen = lenof( domain );
	pslen = GetSidLengthRequired( SID_MAX_SUB_AUTHORITIES );
	ps = new byte[pslen];
	if ( ps == 0 )
		throw NEWEX( errNoMemory, "sid::LookupName(): no memory for SID buffer" );

	ReleasePSID();
	if ( ::LookupAccountName( server, name, ps, &pslen, domain, &domlen, &snu ) )
		*this = ps;
	else
	{
		delete [] ps;
		throw NEWEX32( errInvalidSid, "sid::LookupName(): failed, see GetErrWin32()", GetLastError() );
	}

	delete [] ps;
}


// look the SID up on a server
void sid::LookupSid( const TCHAR *server, fkstr &name, fkstr &domain, SID_NAME_USE &snu ) const
{
	PSID ps;
	DWORD rc, tempnamelen, tempdomainlen;
	TCHAR *tempname = 0, *tempdomain = 0;

	try { ps = MakePSID(); }
	RETHROWEX( "sid::lookupSid(): cannot generate NT-formatted SID" );

	if ( ps == 0 )
		throw NEWEX( errNoMemory, "sid::LookupSid(): no memory for SID buffer" );

	rc = 0; // assume all is well
	tempnamelen = tempdomainlen = 256;
	tempname = new TCHAR[tempnamelen];
	tempdomain = new TCHAR[tempdomainlen];
	if ( ! ::LookupAccountSid( server, ps, tempname, &tempnamelen,
		tempdomain, &tempdomainlen, &snu ) )
	{
		// it failed == buffers too small?
		rc = GetLastError();
		delete [] tempname;
		delete [] tempdomain;
		if ( rc == ERROR_INSUFFICIENT_BUFFER )
		{
			tempname = new TCHAR[tempnamelen];
			tempdomain = new TCHAR[tempdomainlen];
			if ( ! ::LookupAccountSid( server, ps, tempname, &tempnamelen,
				tempdomain, &tempdomainlen, &snu ) )
			{
				delete [] tempname;
				delete [] tempdomain;
				throw NEWEX32( errInvalidSid, "sid::LookupSid(): LookupAccountSid() failed, see GetErrWin32()", GetLastError() );
			}
		}
		else
			throw NEWEX32( errInvalidSid, "sid::LookupSid(): LookupAccountSid() failed, see GetErrWin32()", rc );
	}

	name = tempname;
	domain = tempdomain;

	delete [] tempname;
	delete [] tempdomain;
}



// --- inserters ---

fkostream &fksec::operator<<( fkostream &o, const sid &s )
{
	fkstr name, domain;
	SID_NAME_USE snu;

	o << _T( "[" ) + s.ConvertToStringSid() + _T( "]" );

	try { s.LookupSid( 0, name, domain, snu ); }
	catch ( ex *e )
	{
		// this didn't work too well. Don't display names, then.
		domain.erase();
		name.erase();
		delete e;
	}

	if ( ! domain.empty() || ! name.empty() )
	{
		o << _T( " (" );
		if ( ! domain.empty() )
			o << domain << _T( "\\" );
		o << name << _T( ", " ) << s.SnuToText( snu ) << _T( ")" );
	}

	return o;
}



// --- helpers ---

// initialize all members of a sid object
void sid::Init()
{
	isValid = false;
	memset( &sia, '\0', sizeof sia );
	subAuthCount = 0;
	memset( subAuth, '\0', sizeof subAuth );
}


// initialize the PSID part
void sid::ClearPSID() const
{
	havePSID = false;
	psid = 0;
}


// release the PSID part
void sid::ReleasePSID() const
{
	if ( havePSID )
	{
		delete [] (byte *) psid;
		psid = 0;
		havePSID = false;
	}
}


// read and convert a "-<uint64>" string
bool sid::CvsGetUInt64( const TCHAR *&str, unsigned __int64 &r )
{
	DWORD base = 10, d;
	const TCHAR *startOfNumber;
	static byte value[256] = { 0 };

	if ( str == 0 || *str != _T( '-' ) )
		return false;

	if ( value[0] == 0 ) // array not yet initialized
	{
		memset( value, '\xff', sizeof value );
		for ( d = '0'; d <= '9'; ++ d )
			value[d] = (byte) d - '0';
		for ( d = 'A'; d <= 'F'; ++ d )
			value[d | 0x20] = value[d] = (byte) d - '7';
	}

	r = 0;
	++ str;
	if ( *str == _T( '0' ) )
	{
		base = 8;
		++ str;
		if ( *str == _T( 'x' ) || *str == _T( 'X' ) )
		{
			base = 16;
			++ str;
		}
	}

	startOfNumber = str;
	for ( ; ; )
	{
		d = value[(DWORD) (byte) *str]; // would hate to have an o-o-b index
		if ( d >= base )
			break;
		r = r * base + (unsigned __int64) d;
		++ str;
	}

	// no digits converted?
	// if base != 10, we have seen a leading zero ... which, in this case,
	// was not a base prefix but the entire number. Not an error!
	if ( str == startOfNumber && base == 10 )
		return false;

	return true;
}



PSID sid::MakePSID() const
{
	if ( ! isValid )
		throw NEWEX( errInvalidSid, "sid::MakePSID(): invalid SID" );

	if ( ! havePSID )
	{
		DWORD i;

		psid = (PSID) new byte[GetLength()];
		if ( psid == 0 )
			throw NEWEX( errNoMemory, "sid::MakePSID(): no memory for SID buffer" );

		if ( InitializeSid( psid, &sia, (byte) subAuthCount ) )
			for ( i = 0; i < subAuthCount; ++ i )
				*GetSidSubAuthority( psid, i ) = subAuth[i];
		else
		{
			delete [] (byte *) psid;
			psid = 0;
			throw NEWEX( errInvalidSid, "sid::MakePSID(): invalid SID" );
		}

		havePSID = true;
	}

	return psid;
}



sid::SidType sid::LookupSidType()
{
	const SID_IDENTIFIER_AUTHORITY sec_nt_auth = SECURITY_NT_AUTHORITY;
	DWORD rc;
	byte *buf = 0;

	byte buf2[SID_MAX_SUB_AUTHORITIES * sizeof DWORD + sizeof SID_IDENTIFIER_AUTHORITY + sizeof DWORD];
	TCHAR refdom[256], dom[256];
	SID_NAME_USE snu;
	wchar_t *ws;
	sid temp;
	DWORD sidsize = sizeof buf2, refdomsize = lenof( refdom );

	if ( ! IsValid() )
		throw NEWEX( errInvalidSid, "sid::LookupSidType(): invalid SID" );

	if ( memcmp( &sia, &sec_nt_auth, sizeof sia ) != 0 )
		return stWellKnown;

	// for SECURITY_NT_AUTHORITY, we need to look at the first RID, too.
	if ( GetSubAuthorityCount() < 1 )
		throw NEWEX( errInvalidSid, "sid::LookupSidType(): invalid SID" );

	// local account?
	if ( GetSubAuthority( 0 ) == SECURITY_BUILTIN_DOMAIN_RID )
		return stWellKnown;

	// domain account?
	if ( GetSubAuthority( 0 ) == SECURITY_NT_NON_UNIQUE )
	{
		rc = NetUserModalsGet( 0, 2, &buf );
		if ( rc != ERROR_SUCCESS )
			throw NEWEX32( errNoPrefixSid, "sid::LookupSidType(): can't get local domain prefix", rc );

		try {
			if ( ( (USER_MODALS_INFO_2 *) buf )->usrmod2_domain_id == 0 )
				throw NEWEX( errNoPrefixSid, "sid::LookupSidType(): NUMG() returned a NULL PSID. Feh!" );

			temp = ( (USER_MODALS_INFO_2 *) buf )->usrmod2_domain_id;
			NetApiBufferFree( buf );
			buf = 0;
			temp.AppendSubAuthority( 0 );
			if ( EqualPrefix( temp ) )
				return stLocal;
			else
			{
				// it's not local; now check if the SID belongs to the domain
				// that we are a member of (if we are, that is).

				// first, find the domain name
				rc = NetWkstaGetInfo( 0, 100, &buf );
				if ( rc != ERROR_SUCCESS )
					throw NEWEX32( errNoPrefixSid, "sid::LookupSidType(): cannot get member domain name", rc );

				// so, are we a domain member in the first place?
				ws = (wchar_t *) ( (WKSTA_INFO_100 *) buf )->wki100_langroup;
				if ( ws == 0 || ws[0] == L'\0' )
				{
					NetApiBufferFree( buf );
					return stForeign;
				}

#ifdef UNICODE
				wcscpy( dom, ws );
#else
				wcstombs( dom, ws, sizeof dom );
#endif
				NetApiBufferFree( buf );
				buf = 0;

				// now, get the domain SID
				if ( ! LookupAccountName( 0, dom, buf2, &sidsize, refdom, &refdomsize, &snu ) )
					throw NEWEX32( errNoPrefixSid, "sid::LookupSidType(): cannot get member domain SID", GetLastError() );

				try {
					temp = buf2;
					temp.AppendSubAuthority( 0 );
					if ( EqualPrefix( temp ) )
						return stDomain;
					else
						return stForeign;
				}
				RETHROWEX( "sid::LookupSidType(): compare to domain acct prefix failed" )
			}
		}
		catch ( ex *e )
		{
			e->FKSECADDHOP( "sid::LookupSidType(): compare to machine acct prefix failed" );
			if ( buf != 0 )
				NetApiBufferFree( buf );
			throw;
		}
	}

	return stWellKnown;
}
