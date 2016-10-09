// ace.h: DO NOT include this file. #include fksec.h instead!



#if ! defined( AFX_ACE_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_ )
#define AFX_ACE_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_
#pragma once



/**@#-*/
namespace fksec {
/**@#+*/

	class ace
	{
	public:
		// --- ctors/dtor ---
		// construct an empty ace object
		ace();
		// construct from another ace object
		ace( const ace &a );
		// construct from a PACE
		ace( const void *a );
		// construct from parts
		ace( byte type, byte flags, byte inheritance, ACCESS_MASK mask, const sid &newSid );
		// construct from parts and a name or text SID
		ace( byte type, byte flags, byte inheritance, ACCESS_MASK mask, const TCHAR *stringSid );
		// clean up
		virtual ~ace();

		// --- assignment ---
		// from another ace
		const ace &operator=( const ace &s );
		// from PACE
		const ace &operator=( const void *s );

		// --- conversions ---
		// return a pointer to an internally-maintained ACE
		operator const void *() const;

		// --- comparisons ---
		// returns 1 if all of type, flags, inheritance, and sid are equal
		int operator==( const ace &r );
		// returns 1 if any of type, flags, inheritance, and sid are unequal
		int operator!=( const ace &r );
		// operator<() orders by type -- ACCESS_DENIED_ACE_TYPE 0x1,
		// ACCESS_DENIED_OBJECT_ACE_TYPE 0x6, ACCESS_ALLOWED_ACE_TYPE 0x0,
		// ACCESS_ALLOWED_COMPOUND_ACE_TYPE 0x4, ACCESS_ALLOWED_OBJECT_ACE_TYPE 0x5,
		// SYSTEM_AUDIT_ACE_TYPE 0x2, SYSTEM_ALARM_ACE_TYPE 0x3,
		// SYSTEM_AUDIT_OBJECT_ACE_TYPE 0x7, SYSTEM_ALARM_OBJECT_ACE_TYPE 0x8,
		// all others
		// if types are equal, the lhs is considered smaller than the rhs
		bool operator<( const ace &r );

		// --- accessors ---
		byte GetType() const { return aceType; }
		byte GetFlags() const { return aceFlags; }
		ACCESS_MASK GetMask() const { return aceMask; }
		DWORD GetSize() const { return GetLength(); }
		byte GetInheritance() const { return aceInheritance; }
		const sid &GetSid() const { return aceSid; }
		void SetType( byte newType );
		void SetFlags( byte newFlags );
		void SetMask( ACCESS_MASK newMask );
		void SetInheritance( byte newInheritance );
		void SetSid( const sid &newSid );
		void SetSid( const TCHAR *stringSid );

		// --- utilities ---
		// how many bytes for an NT-formatted ACE?
		DWORD GetLength() const;
		// store a copy of the ACE where the caller wants it
		void StoreAce( void *p, DWORD sz ) const;
		// test for validity
		bool IsValid() const;

		// --- inserters ---
		// dump the ace
		friend fkostream &operator<<( fkostream &o, const ace &a );

		// ACE sort order control
		enum AceSortOrder { asoDefault, asoNT4, asoNT5 };
		static AceSortOrder GetAceSortOrder();
		static SetAceSortOrder( AceSortOrder aso );

	private:
		// initialize the PACE part
		void ClearPACE() const;
		// release the PACE part
		void ReleasePACE() const;
		// read and convert a "-<uint64>" string
		void *MakePACE() const;

		// keep track of the current sort order.
		// if it asoDefault when it is needed, it is set
		// according to the OS version.
		static AceSortOrder currentAceSortOrder;

		// keep track of components we have
		bool haveAceType, haveAceFlags, haveAceMask, haveAceInheritance;
		// no "haveAceSid" -- aceSid.IsValid() serves here

		// ACE components
		byte aceType;
		byte aceFlags;
		byte aceInheritance;
		ACCESS_MASK aceMask;
		sid aceSid;

		// do we have a current NT-formatted ACE?
		// true also implies that pace points to new-ed memory; false implies
		// the opposite. Do not rely on pace being NULL or non-NULL!
		mutable bool havePACE;
		// NT-formatted ACE
		mutable ACCESS_ALLOWED_ACE *pace;
	};

/**@#-*/
} // namespace fksec
/**@#+*/

#endif // ! defined( AFX_ACE_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_ )
