// recommended includes in stdafx.h (or in the precompiled header, to be precise):
// windows.h, tchar.h, string, vector, algorithm, exception, sstream, iomanip

#include "stdafx.h"
#define FKSEC_NO_AUTO_INCLUDES 1
#include "fksec.h"
#include "ex.h"
#include "sid.h"
#include "ace.h"
#include "acl.h"
#include "sd.h"

/**@#-*/
using namespace fksec;
/**@#+*/



sid sd::invalidSid;
acl sd::invalidAcl;




// --- ctors/dtor ---

// construct an empty sd object
sd::sd()
{
	Init();
	ClearPSD();
}


// construct from another sd object
sd::sd( const sd &s )
{
	Init();
	ClearPSD();
	try { *this = s; }
	RETHROWEX( "sd::sd(const sd &): cannot copy source SD" )
}


// construct from a PSD
sd::sd( SECURITY_DESCRIPTOR *s )
{
	Init();
	ClearPSD();
	try { *this = s; }
	RETHROWEX( "sd::sd(SECURITY_DESCRIPTOR *): cannot copy source SD" )
}


sd::sd( SECURITY_DESCRIPTOR_RELATIVE *s )
{
	Init();
	ClearPSD();
	try { *this = s; }
	RETHROWEX( "sd::sd(SECURITY_DESCRIPTOR_RELATIVE *): cannot copy source SD" )
}


// construct from parts
sd::sd( WORD newControl, const PSID newOwner, const PSID newGroup, ACL *newDacl, ACL *newSacl )
{

	ClearPSD();
	Init();

	try { SetControl( newControl ); }
	RETHROWEX( "sd::sd(components): sd::SetControl() complained" );

	if ( newOwner != 0 )
	{
		try { ownerSid = newOwner; }
		RETHROWEX( "sd::sd(components): invalid owner SID" );
		haveOwnerSid = true;
	}
	else
		haveOwnerSid = false;

	if ( newGroup != 0 )
	{
		try { groupSid = newGroup; }
		RETHROWEX( "sd::sd(components): invalid owner SID" );
		haveGroupSid = true;
	}
	else
		haveGroupSid = false;

	if ( newDacl != 0 )
	{
		try { dacl = newDacl; }
		RETHROWEX( "sd::sd(components): cannot copy DACL" );
		haveDacl = true;
		protectedDacl = ( newControl & SE_DACL_PROTECTED ) != 0;
	}
	else
		haveDacl = protectedDacl = false;

	if ( newSacl != 0 )
	{
		try { sacl = newSacl; }
		RETHROWEX( "sd::sd(components): cannot copy SACL" );
		haveSacl = true;
		protectedSacl = ( newControl & SE_SACL_PROTECTED ) != 0;
	}
	else
		haveSacl = protectedSacl = false;
}


// clean up
sd::~sd()
{
	ReleasePSD();
}



// --- assignment ---

// from another sd
const sd &sd::operator=( const sd &s )
{
	if ( this != &s )
	{
		ClearPSD();
		try
		{
			control = s.control;
			haveOwnerSid = s.haveOwnerSid;
			ownerSid = s.ownerSid;
			haveGroupSid = s.haveGroupSid;
			groupSid = s.groupSid;
			haveDacl = s.haveDacl;
			protectedDacl = s.protectedDacl;
			dacl = s.dacl;
			haveSacl = s.haveSacl;
			protectedSacl = s.protectedSacl;
			sacl = s.sacl;
		}
		RETHROWEX( "sd::operator=(const sd &): cannot copy source SD" )
	}

	return *this;
}


// from PSD
const sd &sd::operator=( SECURITY_DESCRIPTOR *s )
{
	BOOL defaulted, present;
	PSID psid;
	PACL pacl;
	DWORD rev;
	WORD c;

	ReleasePSD();
	Init();

	if ( ::GetSecurityDescriptorControl( s, &c, &rev ) )
		SetControl( c );
	else
		throw NEWEX32( errUnreadableSD, "sd::operator=(SECURITY_DESCRIPTOR *): cannot read control word, see ex::GetErrWin32()", GetLastError() );

	psid = 0;
	if ( ::GetSecurityDescriptorOwner( s, &psid, &defaulted ) && psid != 0 )
	{
		ownerSid = psid;
		haveOwnerSid = true;
		if ( defaulted )
			control |= SE_OWNER_DEFAULTED;
	}
	else
		haveOwnerSid = false;

	psid = 0;
	if ( ::GetSecurityDescriptorGroup( s, &psid, &defaulted ) && psid != 0 )
	{
		groupSid = psid;
		haveGroupSid = true;
		if ( defaulted )
			control |= SE_GROUP_DEFAULTED;
	}
	else
		haveGroupSid = false;

	pacl = 0;
	if ( ::GetSecurityDescriptorDacl( s, &present, &pacl, &defaulted ) )
	{
		if ( present && pacl != 0 )
		{
			try { dacl = pacl; }
			RETHROWEX( "sd::operator=(SECURITY_DESCRIPTOR *): cannot copy DACL" );
			haveDacl = true;
			protectedDacl = ( c & SE_DACL_PROTECTED ) != 0;
		}
		else
			haveDacl = protectedDacl = false;
	}
	else
		throw NEWEX32( errUnreadableSD, "sd::operator=(SECURITY_DESCRIPTOR *): cannot read DACL, see ex::GetErrWin32()", GetLastError() );

	pacl = 0;
	if ( ::GetSecurityDescriptorSacl( s, &present, &pacl, &defaulted ) )
	{
		if ( present && pacl != 0 )
		{
			try { sacl = pacl; }
			RETHROWEX( "sd::operator=(SECURITY_DESCRIPTOR *): cannot copy SACL" );
			haveSacl = true;
			protectedSacl = ( c & SE_SACL_PROTECTED ) != 0;
		}
		else
			haveSacl = protectedSacl = false;
	}
	else
		throw NEWEX32( errUnreadableSD, "sd::operator=(SECURITY_DESCRIPTOR *): cannot read SACL, see ex::GetErrWin32()", GetLastError() );

	return *this;
}


const sd &sd::operator=( SECURITY_DESCRIPTOR_RELATIVE *s )
{
	try { *this = (SECURITY_DESCRIPTOR *) s; }
	RETHROWEX( "sd::operator=(SECURITY_DESCRIPTOR_RELATIVE *): cannot copy source SD" )

	return *this;
}


// --- conversions ---

// return a pointer to an internally-maintained absolute SD
sd::operator SECURITY_DESCRIPTOR *() const
{
	try { MakePSD(); }
	RETHROWEX( "sd::operator SECURITY_DESCRIPTOR *(): sd::MakePSD() failed" )
	return absoluteSD;
}


// return a pointer to an internally-maintained self-relative SD
sd::operator SECURITY_DESCRIPTOR_RELATIVE *() const
{
	try { MakePSD(); }
	RETHROWEX( "sd::operator SECURITY_DESCRIPTOR_RELATIVE *(): sd::MakePSD() failed" )
	return (SECURITY_DESCRIPTOR_RELATIVE *) selfRelativeSD;
}



// --- accessors ---

// get flags
WORD sd::GetControl() const
{
	WORD c = control;

	if ( haveDacl )
		c |= SE_DACL_PRESENT;
	else
		c &= ~SE_DACL_PRESENT;
	if ( protectedDacl )
		c |= SE_DACL_PROTECTED;

	if ( haveSacl )
		c |= SE_SACL_PRESENT;
	else
		c &= ~SE_SACL_PRESENT;
	if ( protectedSacl )
		c |= SE_SACL_PROTECTED;

	if ( haveOwnerSid )
		c &= ~ SE_OWNER_DEFAULTED;
	else
		c |= SE_OWNER_DEFAULTED;

	if ( haveGroupSid )
		c &= ~ SE_GROUP_DEFAULTED;
	else
		c |= SE_GROUP_DEFAULTED;

	if ( c & SE_DACL_AUTO_INHERITED )
		c |= SE_DACL_AUTO_INHERIT_REQ;

	if ( c & SE_SACL_AUTO_INHERITED )
		c |= SE_SACL_AUTO_INHERIT_REQ;

	return c;
}


// report which parts of the SD are in use
SECURITY_INFORMATION sd::GetSecurityInformation() const
{
	SECURITY_INFORMATION i = 0;

	if ( haveOwnerSid )
		i |= OWNER_SECURITY_INFORMATION;
	if ( haveGroupSid )
		i |= GROUP_SECURITY_INFORMATION;
	if ( haveDacl )
		i |= protectedDacl? PROTECTED_DACL_SECURITY_INFORMATION: DACL_SECURITY_INFORMATION;
	if ( haveSacl )
		i |= protectedSacl? PROTECTED_SACL_SECURITY_INFORMATION: SACL_SECURITY_INFORMATION;

	return i;
}


// get owner reference
const sid &sd::GetOwnerSid() const
{
	return haveOwnerSid? ownerSid: invalidSid;
}


sid &sd::GetOwnerSid()
{
	ReleasePSD();
	return haveOwnerSid? ownerSid: invalidSid;
}


// get group reference
const sid &sd::GetGroupSid() const
{
	return haveGroupSid? groupSid: invalidSid;
}


sid &sd::GetGroupSid()
{
	ReleasePSD();
	return haveGroupSid? groupSid: invalidSid;
}


// get DACL reference
const acl &sd::GetDacl() const
{
	return haveDacl? dacl: invalidAcl;
}


acl &sd::GetDacl()
{
	ReleasePSD();
	return haveDacl? dacl: invalidAcl;
}


// get SACL ref
const acl &sd::GetSacl() const
{
	return haveSacl? sacl: invalidAcl;
}


acl &sd::GetSacl()
{
	ReleasePSD();
	return haveSacl? sacl: invalidAcl;
}


// set flags -- note that some flags are not settable
// and will be provided by the SD-building functions
void sd::SetControl( WORD newControl )
{
	ReleasePSD();
	control = newControl & ~ ( SE_DACL_PRESENT | SE_SACL_PRESENT |
		SE_DACL_PROTECTED | SE_SACL_PROTECTED | SE_SELF_RELATIVE );
}


// set/clear owner -- (PSID) 0, (TCHAR *) 0, or an invalid sid will clear
void sd::ClearOwnerSid()
{
	haveOwnerSid = false;
	control &= ~ SE_OWNER_DEFAULTED;
	ReleasePSD();
}


void sd::SetOwnerSid( const sid &newSid )
{
	try { ownerSid = newSid; }
	RETHROWEX( "sd::SetOwnerSid(const sid &): assignment failed" )
	haveOwnerSid = true;
	control &= ~ SE_OWNER_DEFAULTED;
	ReleasePSD();
}


void sd::SetOwnerSid( const PSID psid )
{
	if ( psid != 0 )
	{
		try { ownerSid = psid; }
		RETHROWEX( "sd::SetOwnerSid(const PSID): assignment failed" )
		haveOwnerSid = true;
	}
	else
		haveOwnerSid = false;
	control &= ~ SE_OWNER_DEFAULTED;
	ReleasePSD();
}


void sd::SetOwnerSid( const TCHAR *stringSid )
{
	if ( stringSid != 0 )
	{
		try { ownerSid = stringSid; }
		RETHROWEX( "sd::SetOwnerSid(const TCHAR *): assignment failed" )
		haveOwnerSid = true;
	}
	else
		haveOwnerSid = false;
	control &= ~ SE_OWNER_DEFAULTED;
	ReleasePSD();
}


// set/clear group -- (PSID) 0, (TCHAR *) 0, or an invalid sid will clear
void sd::ClearGroupSid()
{
	haveGroupSid = false;
	control &= ~ SE_GROUP_DEFAULTED;
	ReleasePSD();
}


void sd::SetGroupSid( const sid &newSid )
{
	try { groupSid = newSid; }
	RETHROWEX( "sd::SetGroupSid(const sid &): assignment failed" )
	haveGroupSid = true;
	control &= ~ SE_GROUP_DEFAULTED;
	ReleasePSD();
}


void sd::SetGroupSid( const PSID psid )
{
	if ( psid != 0 )
	{
		try { groupSid = psid; }
		RETHROWEX( "sd::SetGroupSid(const PSID): assignment failed" )
		haveGroupSid = true;
	}
	else
		haveGroupSid = false;
	control &= ~ SE_GROUP_DEFAULTED;
	ReleasePSD();
}


void sd::SetGroupSid( const TCHAR *stringSid )
{
	if ( stringSid != 0 )
	{
		try { groupSid = stringSid; }
		RETHROWEX( "sd::SetGroupSid(const TCHAR *): assignment failed" )
		haveGroupSid = true;
	}
	else
		haveGroupSid = false;
	control &= ~ SE_GROUP_DEFAULTED;
	ReleasePSD();
}


// set/clear DACL -- (PACL) 0 will clear
// "clear" == NULL DACL, which is _not_ the same as an empty ACL
void sd::ClearDacl()
{
	haveDacl = false;
	control &= ~ ( SE_DACL_PRESENT | SE_DACL_DEFAULTED );
	ReleasePSD();
}


void sd::SetDacl( const acl &newAcl )
{
	try { dacl = newAcl; }
	RETHROWEX( "sd::SetDacl(const acl &): assignment failed" )
	haveDacl = true;
	control |= SE_DACL_PRESENT;
	ReleasePSD();
}


void sd::SetDacl( ACL *newAcl )
{
	if ( newAcl != 0 )
	{
		try { dacl = newAcl; }
		RETHROWEX( "sd::SetDacl(ACL *): assignment failed" )
		control |= SE_DACL_PRESENT;
		haveDacl = true;
	}
	else
	{
		haveDacl = false;
		control &= ~ ( SE_DACL_PRESENT | SE_DACL_DEFAULTED );
	}
	ReleasePSD();
}


bool sd::GetDaclProtection() const
{
	return haveDacl? protectedDacl: false;
}


void sd::SetDaclProtection( bool newProtection )
{
	ReleasePSD();
	protectedDacl = newProtection;
}


// set/clear SACL -- (PACL) 0 will clear
// "clear" == NULL SACL, which is _not_ the same as an empty ACL
void sd::ClearSacl()
{
	haveSacl = false;
	control &= ~ ( SE_SACL_PRESENT | SE_SACL_DEFAULTED );
	ReleasePSD();
}


void sd::SetSacl( const acl &newAcl )
{
	try { sacl = newAcl; }
	RETHROWEX( "sd::SetSacl(const acl &): assignment failed" )
	control |= SE_SACL_PRESENT;
	haveSacl = true;
	ReleasePSD();
}


void sd::SetSacl( ACL *newAcl )
{
	if ( newAcl != 0 )
	{
		try { sacl = newAcl; }
		RETHROWEX( "sd::SetSacl(ACL *): assignment failed" )
		control |= SE_SACL_PRESENT;
		haveSacl = true;
	}
	else
	{
		haveSacl = false;
		control &= ~ ( SE_SACL_PRESENT | SE_SACL_DEFAULTED );
	}
	ReleasePSD();
}


bool sd::GetSaclProtection() const
{
	return haveSacl? protectedSacl: false;
}


void sd::SetSaclProtection( bool newProtection )
{
	ReleasePSD();
	protectedSacl = newProtection;
}



// --- utilities ---

// inserter
fkostream &fksec::operator<<( fkostream &o, const sd &s )
{
	o << _T( "ACL, control = " ) << s.GetControl() << std::endl;

	o << _T( "  " );
	o << ( s.haveOwnerSid? _T( "has" ): _T( "no" ) ) << _T( " owner, " );
	o << ( s.haveGroupSid? _T( "has" ): _T( "no" ) ) << _T( " group, " );
	o << ( s.haveDacl? _T( "has" ): _T( "no" ) ) <<
		( s.protectedDacl? _T( "protected " ): _T( "" ) ) << _T( " DACL, " );
	o << ( s.haveSacl? _T( "has" ): _T( "no" ) ) <<
		( s.protectedSacl? _T( "protected " ): _T( "" ) ) << _T( " SACL" );
	o << std::endl;

	if ( s.haveOwnerSid )
		o << _T( "Owner: " ) << s.ownerSid << std::endl;

	if ( s.haveGroupSid )
		o << _T( "Group: " ) << s.groupSid << std::endl;

	if ( s.haveDacl )
		o << ( s.protectedDacl? _T( "Protected " ): _T( "" ) ) << _T( "DACL: " ) << s.dacl;

	if ( s.haveSacl )
		o << ( s.protectedSacl? _T( "Protected " ): _T( "" ) ) << _T( "SACL: " ) << s.sacl;

	o << std::endl;

	return o;
}


// how many bytes for an NT-formatted SD?
DWORD sd::GetLength() const
{
	DWORD l;

	l = sizeof SECURITY_DESCRIPTOR;
	if ( haveOwnerSid )
		try { l += ownerSid.GetLength(); }
		RETHROWEX( "sd::GetLength(): cannot size owner SID" )
	if ( haveGroupSid )
		try { l += groupSid.GetLength(); }
		RETHROWEX( "sd::GetLength(): cannot size group SID" )
	if ( haveDacl )
		try { l += dacl.GetSize(); }
		RETHROWEX( "sd::GetLength(): cannot size DACL" )
	if ( haveSacl )
		try { l += sacl.GetSize(); }
		RETHROWEX( "sd::GetLength(): cannot size SACL" )

	return l;
}


// store a copy of the SD in NT format
void sd::StoreSd( SECURITY_DESCRIPTOR *p, DWORD &sz, AbsOrRel sdtype /* = AbsoluteSD */ ) const
{
	byte *next;
	PSID locOwner, locGroup;
	PACL locDacl, locSacl;
	DWORD l;

	if ( ! IsValid( false ) )
		throw NEWEX( errInvalidSD, "sd::StoreSd(): invalid SD" );

	try { l = GetLength(); }
	RETHROWEX( "sd::StoreSd(): cannot compute required buffer size" )

	if ( sz < l )
		throw new ex( _T( __FILE__ ), __LINE__, errBufferTooSmall,
			_T( "sd::StoreSd(): insufficient buffer, see ex::GetData() for required size" ), 0, l );

	if ( ! ::InitializeSecurityDescriptor( p, SECURITY_DESCRIPTOR_REVISION ) )
		throw NEWEX32( errInvalidSD, "sd::StoreSD(): InitializeSecurityDescriptor() failed inexplicably", GetLastError() );

	next = (byte *) &p[1]; // point to right after the SD header
	locOwner = locGroup = locDacl = locSacl = 0;

	if ( haveOwnerSid )
	{
		try
		{
			l = ownerSid.GetLength();
			locOwner = (PSID) next;
			ownerSid.StoreSid( locOwner, l );
			next += l;
		}
		RETHROWEX( "sd::StoreSd(): cannot size or store owner SID" )
	}

	if ( haveGroupSid )
	{
		try
		{
			l = groupSid.GetLength();
			locGroup = (PSID) next;
			groupSid.StoreSid( locGroup, l );
			next += l;
		}
		RETHROWEX( "sd::StoreSd(): cannot size or store group SID" )
	}

	if ( haveDacl )
	{
		try
		{
			l = dacl.GetSize();
			locDacl = (PACL) next;
			dacl.StoreAcl( locDacl, l );
			next += l;
		}
		RETHROWEX( "sd::StoreSd(): cannot size or store DACL" )
	}

	if ( haveSacl )
	{
		try
		{
			l = sacl.GetSize();
			locSacl = (PACL) next;
			sacl.StoreAcl( locSacl, l );
			next += l;
		}
		RETHROWEX( "sd::StoreSd(): cannot size or store SACL" )
	}

	try { p->Control |= GetControl(); }
	RETHROWEX( "sd::StoreSd(): sd::GetControl() failed, but why?" )

	if ( sdtype == AbsoluteSD )
	{
		p->Control &= ~ SE_SELF_RELATIVE;
		p->Owner = locOwner;
		p->Group = locGroup;
		p->Dacl = locDacl;
		p->Sacl = locSacl;
	}
	else
	{
		p->Control |= SE_SELF_RELATIVE;
		p->Owner = (PSID) ( locOwner == 0? 0: (byte *) locOwner - (byte *) p );
		p->Group = (PSID) ( locGroup == 0? 0: (byte *) locGroup - (byte *) p );
		p->Dacl = (PACL) ( locDacl == 0? 0: (byte *) locDacl - (byte *) p );
		p->Sacl = (PACL) ( locSacl == 0? 0: (byte *) locSacl - (byte *) p );
	}
}


// test for validity
bool sd::IsValid( bool checkPSD /* = true */ ) const
{
	// checkPSD is set to false in the IsValid() call in MakePSD()/StoreSd()
	// otherwise, infinite recursion ruins your day
	try
	{
		if ( haveOwnerSid )
			if ( ! ownerSid.IsValid() )
				return false;

		if ( haveGroupSid )
			if ( ! groupSid.IsValid() )
				return false;

		if ( haveDacl )
			if ( ! dacl.IsValid() )
				return false;

		if ( haveSacl )
			if ( ! sacl.IsValid() )
				return false;

		if ( checkPSD )
		{
			this->MakePSD();
			if ( ! ::IsValidSecurityDescriptor( absoluteSD ) )
				return false;
			if ( ! ::IsValidSecurityDescriptor( selfRelativeSD ) )
				return false;
		}
	}
	catch ( ex *e )
	{
		delete e;
		return false;
	}

	return true;
}



// initialize all members of an acl object
void sd::Init()
{
	ClearPSD();
	control = 0;
	haveOwnerSid = haveGroupSid =
		haveDacl = protectedDacl =
		haveSacl = protectedSacl = false;
}


// initialize the cached SD stuff
void sd::ClearPSD()
{
	absoluteSD = 0;
	haveAbsoluteSD = false;
	selfRelativeSD = 0;
	haveSelfRelativeSD = false;
}


// release any cached SDs
void sd::ReleasePSD()
{
	if ( haveAbsoluteSD )
	{
		delete [] (byte *) absoluteSD;
		absoluteSD = 0;
		haveAbsoluteSD = false;
	}
	if ( haveSelfRelativeSD )
	{
		delete [] (byte *) selfRelativeSD;
		selfRelativeSD = 0;
		haveSelfRelativeSD = false;
	}
}


// prepare the cached SDs
void sd::MakePSD() const
{
	DWORD sz = (DWORD) -1;

	if ( ! haveAbsoluteSD )
	{
		try { sz = GetLength(); }
		RETHROWEX( "sd::MakePSD() absolute: cannot size SD" )

		absoluteSD = (SECURITY_DESCRIPTOR *) new byte[sz];
		if ( absoluteSD == 0 )
			throw NEWEX( errNoMemory, "sd::MakePSD() absolute: no memory for SD buffer" );

		try { StoreSd( absoluteSD, sz, AbsoluteSD ); }
		catch ( ex *e )
		{
			e->FKSECADDHOP( "sd::MakePSD() absolute: sd::StoreSd() failed" );
			delete (byte *) absoluteSD;
			throw;
		}

		haveAbsoluteSD = true;
	}

	if ( ! haveSelfRelativeSD )
	{
		if ( sz == (DWORD) -1 )
			try { sz = GetLength(); }
			RETHROWEX( "sd::MakePSD() self-relative: cannot size SD" )

		selfRelativeSD = (SECURITY_DESCRIPTOR *) new byte[sz];
		if ( selfRelativeSD == 0 )
			throw NEWEX( errNoMemory, "sd::MakePSD() self-relative: no memory for SD buffer" );

		try { StoreSd( selfRelativeSD, sz, SelfRelativeSD ); }
		catch ( ex *e )
		{
			e->FKSECADDHOP( "sd::MakePSD() self-relative: sd::StoreSd() failed" );
			delete (byte *) selfRelativeSD;
			throw;
		}

		haveSelfRelativeSD = true;
	}
}
