// recommended includes in stdafx.h (or in the precompiled header, to be precise):
// windows.h, tchar.h, string, vector, algorithm, exception, sstream, iomanip

#include "stdafx.h"
#define FKSEC_NO_AUTO_INCLUDES 1
#include "fksec.h"
#include "ex.h"
#include "sid.h"
#include "ace.h"
#include "acl.h"

/**@#-*/
using namespace fksec;
/**@#+*/



ace acl::invalidAce;



// --- ctors/dtor ---

// construct an empty acl object
acl::acl()
{
	Init();
}


// construct from another acl object
acl::acl( const acl &a )
{
	Init();
	if ( a.IsValid() )
	{
		aces = a.aces;
		additionalBytes = a.additionalBytes;
	}
	else
		throw NEWEX( errInvalidAcl, "acl::acl(const acl&): invalid ACL" );

}


// construct from a PACL
acl::acl( ACL *a )
{
	ACL_SIZE_INFORMATION asi;
	DWORD i, n;
	void *pace;

	Init();
	if ( IsValidAcl( a ) && a->AclRevision == ACL_REVISION )
	{
		GetAclInformation( a, &asi, sizeof asi, AclSizeInformation );
		additionalBytes = asi.AclBytesFree;
		n = asi.AceCount;
		for ( i = 0; i < n; ++ i )
		{
			if ( ::GetAce( a, i, &pace ) )
			{
				try { AddAce( (DWORD) -1, pace ); }
				catch ( ex *e )
				{
					e->FKSECADDHOP( "acl::ace(ACL *): failed to add an ACE" );
					aces.clear();
					throw;
				}
			}
		}
	}
	else
		throw NEWEX( errInvalidAcl, "acl::acl(ACL *): invalid ACL, or bad revision" );
}


// clean up
acl::~acl()
{
	ReleasePACL();
	aces.clear();
}



// --- assignment ---

// from another acl
const acl &acl::operator=( const acl &s )
{
	if ( this != &s )
	{
		ReleasePACL();
		if ( s.IsValid() )
		{
			aces = s.aces;
			additionalBytes = s.additionalBytes;
		}
		else
			throw NEWEX( errInvalidAcl, "acl::operator=(const acl&): invalid ACL" );
	}

	return *this;
}


// from PACL
const acl &acl::operator=( ACL *s )
{
	ACL_SIZE_INFORMATION asi;
	DWORD i, n;
	void *pace;

	ReleasePACL();
	Init();
	if ( IsValidAcl( s ) )
	{
		GetAclInformation( s, &asi, sizeof asi, AclSizeInformation );
		additionalBytes = asi.AclBytesFree;
		n = asi.AceCount;
		for ( i = 0; i < n; ++ i )
		{
			if ( ::GetAce( s, i, &pace ) )
			{
				try { AddAce( (DWORD) -1, pace ); }
				catch ( ex *e )
				{
					e->FKSECADDHOP( "acl::operator=(ACL *): failed to add an ACE" );
					aces.clear();
					throw;
				}
			}
		}
	}
	else
		throw NEWEX( errInvalidAcl, "acl::operator=(ACL *): invalid ACL" );

	return *this;
}



// --- conversions ---

// return a pointer to an internally-maintained ACL
acl::operator ACL *() const
{
	ACL *p;

	try { p = MakePACL(); }
	RETHROWEX( "acl::operator const ACL *(): MakePACL() failed" )

	return p;
}



// --- accessors ---

// return a ref to the Nth ACE (or to an invalid one if o-o-b)
const ace &acl::GetAce( DWORD index ) const
{
	if ( index >= GetCount() )
		throw NEWEX( errInvalidAceIndex, "acl::GetAce(): ACE index out of range" );

	return aces[index];
}


ace &acl::GetAce( DWORD index )
{
	if ( index >= GetCount() )
		throw NEWEX( errInvalidAceIndex, "acl::GetAce(): ACE index out of range" );

	return aces[index];
}


// return bytes required incl. desired free bytes
DWORD acl::GetSize() const
{
	return GetLength() + additionalBytes;
}


// return count of ACEs
DWORD acl::GetCount() const
{
	return aces.size();
}


// set amount of free bytes desired in NT ACL
void acl::SetFreeBytes( DWORD newAdditionalBytes )
{
	additionalBytes = newAdditionalBytes;
}


// how many bytes are extra?
DWORD acl::GetFreeBytes() const
{
	return additionalBytes;
}



// --- utilities ---

// add an ACE
// index gives the position _before_ which the ACE will be inserted
// (DWORD) -1 inserts at end
// no need to worry about canonical order, you can always canonicalize() the ACL
void acl::AddAce( DWORD index, const void *pace )
{
	AceListIter i;

	if ( index >= aces.size() )
		i = aces.end();
	else
		i = &aces[index];

	aces.insert( i, ace( pace ) );
}


void acl::AddAce( DWORD index, const ace &ace )
{
	AceListIter i;

	if ( index >= aces.size() )
		i = aces.end();
	else
		i = &aces[index];

	aces.insert( i, ace );
}


void acl::AddAce( DWORD index, byte type, byte flags, byte inheritance, ACCESS_MASK mask, PSID psid )
{
	AceListIter i;

	if ( index >= aces.size() )
		i = aces.end();
	else
		i = &aces[index];

	aces.insert( i, ace( type, flags, inheritance, mask, psid ) );
}


void acl::AddAce( DWORD index, byte type, byte flags, byte inheritance, ACCESS_MASK mask, const TCHAR *stringSid )
{
	AceListIter i;

	if ( index >= aces.size() )
		i = aces.end();
	else
		i = &aces[index];

	aces.insert( i, ace( type, flags, inheritance, mask, stringSid ) );
}


void acl::AddAce( DWORD index, byte type, byte flags, byte inheritance, ACCESS_MASK mask, const sid &newSid )
{
	AceListIter i;

	if ( index >= aces.size() )
		i = aces.end();
	else
		i = &aces[index];

	aces.insert( i, ace( type, flags, inheritance, mask, newSid ) );
}


// delete an ACE
void acl::DeleteAce( DWORD index )
{
	if ( index < aces.size() )
		aces.erase( &aces[index] );
	else
		throw NEWEX( errInvalidAceIndex, "acl::DeleteAce(): ACE index out of range" );
}


// delete all ACEs for the specified SID
void acl::DeleteAcesForSid( PSID psid )
{
	sid delSid( psid );

	DeleteAcesForSid( delSid );
}


void acl::DeleteAcesForSid( const TCHAR *stringSid )
{
	sid delSid( stringSid );

	DeleteAcesForSid( delSid );
}


void acl::DeleteAcesForSid( const sid &delSid )
{
	int i;

	for ( i = aces.size() - 1; i >= 0; -- i )
	{
		if ( aces[i].GetSid() == delSid )
			aces.erase( &aces[i] );
	}
}


// normalize ACEs -- ACEs with same type, flags, inheritance,
// and SID are merged by ORing their masks. This means you can
// just blindly add ACEs and then call normalize().
void acl::normalize()
{
	int i, j;

	for ( i = 0; i < aces.size() - 1; ++ i )
	{
		for ( j = aces.size() - 1; j > i; -- j )
		{
			if ( aces[i] == aces[j] )
			{
				aces[i].SetMask( aces[i].GetMask() | aces[j].GetMask() );
				aces.erase( &aces[j] );
			}
		}
	}
}


// sort ACEs into canonical order: deny-ACEs first, then allow-ACEs,
// then all others. This is a stable sort.
void acl::canonicalize()
{
	std::sort( aces.begin(), aces.end() );
}


// how many bytes for an NT-formatted ACL, without extra free space?
DWORD acl::GetLength() const
{
	AceListConstIter i;
	DWORD totsize;

	for ( totsize = 0, i = aces.begin(); i != aces.end(); ++ i )
	{
		try { totsize += (*i).GetLength(); }
		RETHROWEX( "acl::GetLength(): failed to retrieve an ACE's size" )
	}

	totsize += sizeof ACL;
	return totsize;
}


// store a copy of the ACL where the caller wants it
void acl::StoreAcl( ACL *p, DWORD sz ) const
{
	AceListConstIter i;
	void *a;
	DWORD remaining, acelen;

	if ( ! IsValid() )
		throw NEWEX( errInvalidAcl, "acl::StoreAcl(): invalid ACL" );

	remaining = GetSize();
	if ( sz < remaining )
	{
		ex *e = NEWEX( errBufferTooSmall, "acl::StoreAcl(): insufficient buffer, see ex::GetData() for required size" );
		e->SetData( remaining );
		throw e;
	}

	if ( ! ::InitializeAcl( p, remaining, ACL_REVISION ) )
		throw NEWEX32( errInvalidAcl, "acl::StoreAcl(): ::InitializeAcl() failed, see ex::GetErrWin32", GetLastError() );

	p->AceCount = aces.size();
	a = (void *) &p[1]; // point to first ACE slot
	remaining -= sizeof ACL;

	for ( i = aces.begin(); i != aces.end(); ++ i )
	{
		try
		{
			acelen = (*i).GetLength();
			(*i).StoreAce( a, remaining );
		}
		RETHROWEX( "acl::StoreAcl(): ran into a stubborn ACE" )
		a = (void *) ( (byte *) a + acelen );
		remaining -= acelen;
	}

	// at this point, the remaining bytes should be exactly those that
	// the user wanted to be allocated in excess of the minimum.
	if ( remaining != additionalBytes || ! IsValidAcl( p ) )
		throw NEWEX( errInvalidAcl, "acl::StoreAcl(): size calculation incorrect, or invalid ACL produced" );
}


// test for validity
bool acl::IsValid() const
{
	bool result = true;

	try
	{
		for ( AceListConstIter i = aces.begin(); result && i != aces.end(); ++ i )
			result = result && (*i).IsValid();
	}
	catch ( ex *e )
	{
		delete e;
		return false;
	}

	if ( GetLength() > 65535 )
		return false;

	return true;
}


// --- inserters ---

// dump this acl
fkostream &fksec::operator<<( fkostream &o, const acl &a )
{
	o << _T( "acl, " ) << a.aces.size() << _T( " ACEs, " )
		<< a.additionalBytes << _T( " bytes extra space" ) << std::endl;

	for ( AceListConstIter i = a.aces.begin(); i != a.aces.end(); ++ i )
		o << _T( "  " ) << (*i) << std::endl;

	return o;
}



// initialize all members of an acl object
void acl::Init()
{
	aces.clear();
	additionalBytes = 0;
	ClearPACL();
}


// initialize the PACL part
void acl::ClearPACL() const
{
	havePACL = false;
	pacl = 0;
}


// release the PACL part
void acl::ReleasePACL() const
{
	if ( havePACL )
	{
		delete [] (byte *) pacl;
		pacl = 0;
		havePACL = false;
	}
}


// read and convert a "-<uint64>" string
ACL *acl::MakePACL() const
{
	DWORD sz;

	if ( ! havePACL )
	{
		if ( ! IsValid() )
			throw NEWEX( errInvalidAcl, "acl::MakePACL(): invalid ACL" );

		sz = GetSize();

		pacl = (ACL *) new byte[sz];
		if ( pacl == 0 )
			throw NEWEX( errNoMemory, "acl::MakePACL(): no memory for ACL buffer" );

		try { StoreAcl( pacl, sz ); }
		catch ( ex *e )
		{
			e->FKSECADDHOP( "acl::MakePACL(): StoreAcl() is insubordinate" );
			delete (byte *) pacl;
			throw;
		}

		havePACL = true;
	}

	return pacl;
}
