// recommended includes in stdafx.h (or in the precompiled header, to be precise):
// windows.h, tchar.h, string, vector, algorithm, exception, sstream, iomanip

#include "stdafx.h"
#define FKSEC_NO_AUTO_INCLUDES 1
#include "fksec.h"
#include "ex.h"
#include "sid.h"
#include "ace.h"

/**@#-*/
using namespace fksec;
/**@#+*/



ace::AceSortOrder ace::currentAceSortOrder = asoDefault;



// --- ctors/dtor ---

// construct an empty ace object
ace::ace()
  :	aceType( 0 ),
	aceFlags( 0 ),
	aceMask( 0 ),
	aceInheritance( 0 ),
	aceSid(),
	haveAceType( false ),
	haveAceMask( false ),
	haveAceFlags( false ),
	haveAceInheritance( false )
{
	ClearPACE();
}


// construct from another ace object
ace::ace( const ace &a )
  :	aceType( a.aceType ),
	aceFlags( a.aceFlags ),
	aceMask( a.aceMask ),
	aceInheritance( a.aceInheritance ),
	aceSid( a.aceSid ),
	haveAceType( a.haveAceType ),
	haveAceMask( a.haveAceMask ),
	haveAceFlags( a.haveAceFlags ),
	haveAceInheritance( a.haveAceInheritance )
{
	ClearPACE();
}


// construct from a PACE
ace::ace( const void *a )
  :	aceType( ((ACE_HEADER *) a)->AceType ),
	aceFlags( ((ACE_HEADER *) a)->AceFlags & ~ VALID_INHERIT_FLAGS ),
	aceMask( ((ACCESS_ALLOWED_ACE *) a)->Mask ),
	aceInheritance( ((ACE_HEADER *) a)->AceFlags & VALID_INHERIT_FLAGS ),
	aceSid( &((ACCESS_ALLOWED_ACE *) a)->SidStart) ,
	haveAceType( true ),
	haveAceMask( true ),
	haveAceFlags( true ),
	haveAceInheritance( true )
{
	ClearPACE();
}


// construct from parts
ace::ace( byte type, byte flags, byte inheritance, ACCESS_MASK mask, const sid &newSid )
  :	aceType( type ),
	aceFlags( flags ),
	aceMask( mask ),
	aceInheritance( inheritance ),
	aceSid( newSid ),
	haveAceType( true ),
	haveAceMask( true ),
	haveAceFlags( true ),
	haveAceInheritance( true )
{
	ClearPACE();
}


// construct from parts and a name or text SID
ace::ace( byte type, byte flags, byte inheritance, ACCESS_MASK mask, const TCHAR *stringSid )
  :	aceType( type ),
	aceFlags( flags ),
	aceMask( mask ),
	aceInheritance( inheritance ),
	aceSid( stringSid ),
	haveAceType( true ),
	haveAceMask( true ),
	haveAceFlags( true ),
	haveAceInheritance( true )
{
	ClearPACE();
}


// clean up
ace::~ace()
{
	ReleasePACE();
}



// --- assignment ---

// from another ace
const ace &ace::operator=( const ace &s )
{
	if ( this != &s )
	{
		ReleasePACE();
		aceType = s.aceType;
		aceFlags = s.aceFlags;
		aceMask = s.aceMask;
		aceInheritance = s.aceInheritance;
		aceSid = s.aceSid;
		haveAceType = s.haveAceType;
		haveAceMask = s.haveAceMask;
		haveAceFlags = s.haveAceFlags;
		haveAceInheritance = s.haveAceInheritance;
	}
	return *this;
}


// from PACE
const ace &ace::operator=( const void *s )
{
	ReleasePACE();
	aceType = ((ACE_HEADER *) s)->AceType;
	aceFlags = ((ACE_HEADER *) s)->AceFlags & ~ VALID_INHERIT_FLAGS;
	aceMask = ((ACCESS_ALLOWED_ACE *) s)->Mask;
	aceInheritance = ((ACE_HEADER *) s)->AceFlags & VALID_INHERIT_FLAGS;
	aceSid = &((ACCESS_ALLOWED_ACE *) s)->SidStart;
	haveAceType = true;
	haveAceMask = true;
	haveAceFlags = true;
	haveAceInheritance = true;
	return *this;
}



// --- conversions ---

// return a pointer to an internally-maintained ACE
ace::operator const void *() const
{
	const void *p;

	try { p = MakePACE(); }
	RETHROWEX( "ace::operator const void *()" )

	return p;
}



// --- comparisons ---

// returns 1 if all of type, flags, inheritance, and sid are equal
int ace::operator==( const ace &r )
{
	if ( ! IsValid() || ! r.IsValid() )
		throw NEWEX( errInvalidAce, "ace::operator==(): invalid ACE" );

	if ( aceType == r.aceType && aceFlags == r.aceFlags &&
		aceInheritance == r.aceInheritance && aceSid == r.aceSid )
		return 1;

	return 0;
}


// returns 1 if any of type, flags, inheritance, and sid are unequal
int ace::operator!=( const ace &r )
{
	if ( ! IsValid() || ! r.IsValid() )
		throw NEWEX( errInvalidAce, "ace::operator!=(): invalid ACE" );

	if ( aceType != r.aceType || aceFlags != r.aceFlags ||
		aceInheritance != r.aceInheritance || aceSid != r.aceSid )
		return 1;

	return 0;
}


// operator<() orders ACEs.  The algorithm is empirically derived,
// as none of the three I found in MS docs produces the same results
// as the GUI utilities of NT.
bool ace::operator<( const ace &r )
{
	// the lt* flags determine the relation for single criteria;
	// their precedence is then determined by the ACE sort order
	int ltAutoInherited; // == -1 if LHS.auto-i < RHS.auto-i (not auto-i goes first)
	int ltType; // == -1 if LHS.type < RHS.type (deny goes first)
	int ltSid; // == -1 if LHS.sid < RHS.sid
	int ltEffective; // == -1 if LHS.effective < RHS.effective (effective goes first)

	static int typeMap[ACCESS_MAX_MS_ACE_TYPE + 1] = { -1 }; // basically, a collating seq for ACE types

	// why do I init explicitly instead of setting up an initialized array?
	// Because this is easier to read. Bear with me.
	if ( typeMap[0] == -1 )
	{
		typeMap[ACCESS_DENIED_ACE_TYPE]				= 0;
		typeMap[ACCESS_DENIED_OBJECT_ACE_TYPE]		= 1;
		typeMap[ACCESS_ALLOWED_ACE_TYPE]			= 2;
		typeMap[ACCESS_ALLOWED_COMPOUND_ACE_TYPE]	= 3;
		typeMap[ACCESS_ALLOWED_OBJECT_ACE_TYPE]		= 4;
		typeMap[SYSTEM_AUDIT_ACE_TYPE]				= 5;
		typeMap[SYSTEM_ALARM_ACE_TYPE]				= 6;
		typeMap[SYSTEM_AUDIT_OBJECT_ACE_TYPE]		= 7;
		typeMap[SYSTEM_ALARM_OBJECT_ACE_TYPE]		= 8;
	}

	if ( ! haveAceType || aceType >= lenof( typeMap ) ||
		! r.haveAceType || r.aceType >= lenof( typeMap ) )
		throw NEWEX( errInvalidAce, "ace::operator<(): invalid ACE type" );

	if ( aceType == r.aceType )
		ltType = 0;
	else
		ltType = ( typeMap[aceType] < typeMap[r.aceType] )? -1: 1;

	// now check inheritance bits
	if ( ( aceInheritance & INHERITED_ACE ) == ( r.aceInheritance & INHERITED_ACE ) )
		ltAutoInherited = 0; // equality?
	else // not same, no-auto-i is smaller
	{
		// if LHS.auto-i is set, then LHS is NOT < RHS
		ltAutoInherited = ( aceInheritance & INHERITED_ACE )? 1: -1;
	}

	// check SIDs
	if ( aceSid < r.aceSid )
		ltSid = -1;
	else if ( aceSid == r.aceSid )
		ltSid = 0;
	else
		ltSid = 1;

	// now check whether this ACE is an effective one, or an inherit-only
	if ( ( aceInheritance & INHERIT_ONLY_ACE ) == ( r.aceInheritance & INHERIT_ONLY_ACE ) )
		ltEffective = 0;
	else // not same; which one is effective?
		ltEffective = ( aceInheritance & INHERIT_ONLY_ACE )? 1: -1;

	switch ( GetAceSortOrder() )
	{
	case asoNT4:
		// NT4: deny before allow; effective before inherit-only
		if ( ltType != 0 )
			return ltType < 0;
		if ( ltSid != 0 )
			return ltSid < 0;
		return ltEffective < 0;

	case asoNT5:
	default:
		// NT5: no-auto-i before auto-i; deny before allow; effective before inherit-only
		if ( ltAutoInherited != 0 )
			return ltAutoInherited < 0;
		if ( ltType != 0 )
			return ltType < 0;
		if ( ltSid != 0 )
			return ltSid < 0;
		return ltEffective < 0;
	}
}



// --- accessors ---

void ace::SetType( byte newType )
{
	if ( newType < ACCESS_MIN_MS_OBJECT_ACE_TYPE ||
		newType > ACCESS_MAX_MS_OBJECT_ACE_TYPE )
	{
		ReleasePACE();
		aceType = newType;
		haveAceType = true;
	}
	else
		throw NEWEX( errInvalidAce, "ace::SetType(): invalid ACE flags" );
}


void ace::SetFlags( byte newFlags )
{
	if ( ( newFlags & VALID_INHERIT_FLAGS ) == 0 )
	{
		ReleasePACE();
		aceFlags = newFlags;
		haveAceFlags = true;
	}
	else
		throw NEWEX( errInvalidAce, "ace::SetFlags(): invalid ACE flags" );
}


void ace::SetMask( ACCESS_MASK newMask )
{
	ReleasePACE();
	aceMask = newMask;
	haveAceMask = true;
}


void ace::SetInheritance( byte newInheritance )
{
	if ( ( newInheritance & ~ VALID_INHERIT_FLAGS ) == 0 )
	{
		ReleasePACE();
		aceInheritance = newInheritance;
		haveAceInheritance = true;
	}
	else
		throw NEWEX( errInvalidAce, "ace::SetInheritance(): invalid ACE inheritance flags" );
}


void ace::SetSid( const sid &newSid )
{
	if ( newSid.IsValid() )
	{
		ReleasePACE();
		aceSid = newSid;
	}
	else
		throw NEWEX( errInvalidSid, "ace::SetSid(): invalid SID" );
}


void ace::SetSid( const TCHAR *stringSid )
{
	sid newSid;

	try { newSid = stringSid; }
	RETHROWEX( "ace::SetSid(const TCHAR *): invalid SID" )

	if ( newSid.IsValid() )
	{
		ReleasePACE();
		aceSid = newSid;
	}
	else
		throw NEWEX( errInvalidSid, "ace::SetSid(): invalid SID" );
}



// --- utilities ---

// how many bytes for an NT-formatted ACE?
DWORD ace::GetLength() const
{
	return sizeof ACCESS_ALLOWED_ACE + aceSid.GetLength() -
		sizeof ( ((ACCESS_ALLOWED_ACE *) 0)->SidStart );
}


// store a copy of the ACE where the caller wants it
void ace::StoreAce( void *p, DWORD sz ) const
{
	DWORD sidSize, l;

	if ( ! IsValid() )
		throw NEWEX( errInvalidAce, "ace::StoreAce(): invalid ACE" );

	l = GetLength();
	if ( sz < l )
		throw new ex( _T( __FILE__ ), __LINE__, errBufferTooSmall,
			_T( "ace::StoreAce(): insufficient buffer, see ex::GetData() for required size" ), 0, l );

	((ACCESS_ALLOWED_ACE *) p)->Header.AceFlags = aceFlags | aceInheritance;
	((ACCESS_ALLOWED_ACE *) p)->Header.AceSize = GetLength();
	((ACCESS_ALLOWED_ACE *) p)->Header.AceType = aceType;
	((ACCESS_ALLOWED_ACE *) p)->Mask = aceMask;
	sidSize = sz - sizeof ACE_HEADER - sizeof ACCESS_MASK;
	try { aceSid.StoreSid( &((ACCESS_ALLOWED_ACE *) p)->SidStart, sidSize ); }
	RETHROWEX( "ace::StoreAce(): invalid SID" )
}


// test for validity
bool ace::IsValid() const
{
	return haveAceFlags && haveAceType && haveAceInheritance && aceSid.IsValid();
}



// return sort order for ACEs
// will never return asoDefault
ace::AceSortOrder ace::GetAceSortOrder()
{
	if ( currentAceSortOrder == asoDefault )
	{
		OSVERSIONINFO ov = { sizeof ov };
		currentAceSortOrder = asoNT5;
		if ( GetVersionEx( &ov ) )
		{
			if ( ov.dwMajorVersion < 5 )
				currentAceSortOrder = asoNT4;
		}
	}

	return currentAceSortOrder;
}



// set ACE sort order
// if set to asoDefault, the next call to GetAceSortOrder()
// will resolve to either asoNT4 or asoNT5
ace::SetAceSortOrder( AceSortOrder aso )
{
	currentAceSortOrder = aso;
}



// --- inserters ---
// dump the ace
fkostream &fksec::operator<<( fkostream &o, const ace &a )
{
	o << _T( "ace: type " );
 
	if ( a.haveAceType )
		o << _T( "0x" ) << std::hex << std::setfill( _T( '0' ) ) << std::setw( 2 ) << (int) a.aceType << std::dec << std::setfill( _T( ' ' ) );
	else
		o << _T( "-NA-" );

	o << _T( ", flags: " );
	if ( a.haveAceFlags )
		o << _T( "0x" ) << std::hex << std::setfill( _T( '0' ) ) << std::setw( 2 ) << ( a.aceFlags & 0xff ) << std::dec << std::setfill( _T( ' ' ) );
	else
		o << _T( "-NA-" );

	o << _T( ", inheritance: " );
	if ( a.haveAceInheritance )
		o << _T( "0x" ) << std::hex << std::setfill( _T( '0' ) ) << std::setw( 2 ) << ( a.aceInheritance & 0xff ) << std::dec << std::setfill( _T( ' ' ) );
	else
		o << _T( "-NA-" );

	o << _T( ", mask: " );
	if ( a.haveAceMask )
		// with printf, this would be: printf( "0x%08lX", aceMask );
		// Brave New World of C++! This stream stuff deserves retroactive abortion.
		o << _T( "0x" ) << std::hex << std::setfill( _T( '0' ) ) << std::setw( 8 ) << a.aceMask << std::dec << std::setfill( _T( ' ' ) );
	else
		o << _T( "----NA----" );

	o << _T( ", sid: " );
	if ( a.aceSid.IsValid() )
		o << a.aceSid;
	else
		o << _T( "-none-" );

	return o;
}



// initialize the PACE part
void ace::ClearPACE() const
{
	havePACE = false;
	pace = 0;
}


// release the PACE part
void ace::ReleasePACE() const
{
	if ( havePACE )
	{
		delete [] (byte *) pace;
		havePACE = false;
	}
}


// make sure we have a current NT ACE
void *ace::MakePACE() const
{
	DWORD sz;

	if ( ! havePACE )
	{
		if ( ! IsValid() )
			throw NEWEX( errInvalidAce, "ace::MakePACE(): invalid ACE" );

		sz = GetLength();

		pace = (ACCESS_ALLOWED_ACE *) new byte[sz];
		if ( pace == 0 )
			throw NEWEX( errNoMemory, "ace::MakePACE(): no memory for ACE buffer" );

		try { StoreAce( pace, sz ); }
		catch ( ex *e )
		{
			e->FKSECADDHOP( "ace::MakePACE()" );
			delete (byte *) pace;
			throw;
		}

		havePACE = true;
	}

	return (void *) pace;
}
