// sd.h: DO NOT include this file. #include fksec.h instead!



#if ! defined( AFX_SD_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_ )
#define AFX_SD_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_
#pragma once



/**@#-*/
namespace fksec {
/**@#+*/

	class sd
	{
	public:
		// SD types
		enum AbsOrRel { AbsoluteSD, SelfRelativeSD };

		// --- ctors/dtor ---
		// construct an empty sd object
		sd();
		// construct from another sd object
		sd( const sd &s );
		// construct from a PSD
		sd( SECURITY_DESCRIPTOR *s );
		sd( SECURITY_DESCRIPTOR_RELATIVE *s );
		// construct from parts
		sd( WORD newControl, const PSID newOwner, const PSID newGroup, ACL *newDacl, ACL *newSacl );
		// clean up
		virtual ~sd();

		// --- assignment ---
		// from another sd
		const sd &operator=( const sd &s );
		// from PSD
		const sd &operator=( SECURITY_DESCRIPTOR *s );
		const sd &operator=( SECURITY_DESCRIPTOR_RELATIVE *s );

		// --- conversions ---
		// with heavy heart I note that the NT guys have yet to hear of
		// const input arguments. This regrettably compels me to return
		// a non-const pointer, while visions of callers mucking around
		// with my internals gnaw at my viscera.
		// return a pointer to an internally-maintained absolute SD
		operator SECURITY_DESCRIPTOR *() const;
		// return a pointer to an internally-maintained self-relative SD
		operator SECURITY_DESCRIPTOR_RELATIVE *() const;

		// --- accessors ---
		// get flags
		WORD GetControl() const;
		// report which parts of the SD are in use
		SECURITY_INFORMATION GetSecurityInformation() const;
		// get owner reference
		const sid &GetOwnerSid() const;
		sid &GetOwnerSid();
		// get group reference
		const sid &GetGroupSid() const;
		sid &GetGroupSid();
		// get DACL reference
		const acl &GetDacl() const;
		acl &GetDacl();
		// get SACL ref
		const acl &GetSacl() const;
		acl &GetSacl();
		// set flags -- note that some flags are not settable
		// and will be provided by the SD-building functions
		void SetControl( WORD newControl );
		// set/clear owner -- (PSID) 0, (TCHAR *) 0, or an invalid sid will clear
		void ClearOwnerSid();
		void SetOwnerSid( const sid &newSid );
		void SetOwnerSid( const PSID psid );
		void SetOwnerSid( const TCHAR *stringSid );
		// set/clear group -- (PSID) 0, (TCHAR *) 0, or an invalid sid will clear
		void ClearGroupSid();
		void SetGroupSid( const sid &newSid );
		void SetGroupSid( const PSID psid );
		void SetGroupSid( const TCHAR *stringSid );
		// set/clear DACL -- (PACL) 0 will clear
		// "clear" == NULL DACL, which is _not_ the same as an empty ACL
		void ClearDacl();
		void SetDacl( const acl &newAcl );
		void SetDacl( ACL *newAcl );
		bool GetDaclProtection() const;
		void SetDaclProtection( bool newProtection );
		// set/clear SACL -- (PACL) 0 will clear
		// "clear" == NULL SACL, which is _not_ the same as an empty ACL
		void ClearSacl();
		void SetSacl( const acl &newAcl );
		void SetSacl( ACL *newAcl );
		bool GetSaclProtection() const;
		void SetSaclProtection( bool newProtection );

		// --- utilities ---
		// inserter
		friend fkostream &operator<<( fkostream &o, const sd &s );
		// how many bytes for an NT-formatted SD?
		DWORD GetLength() const;
		// store a copy of the SD in NT format
		void StoreSd( SECURITY_DESCRIPTOR *p, DWORD &sz, AbsOrRel sdtype = AbsoluteSD ) const;
		// test for validity
		bool IsValid( bool checkPSD = true ) const;

	private:
		// initialize all members of an acl object
		void Init();
		// initialize the cached SD stuff
		void ClearPSD();
		// release any cached SDs
		void ReleasePSD();
		// prepare the cached SDs
		void MakePSD() const;

		// SD components
		WORD control;
		bool haveOwnerSid; // is the ownerSid meaningful?
		sid ownerSid;
		bool haveGroupSid; // is the groupSid meaningful?
		sid groupSid;
		bool haveDacl;
		bool protectedDacl; // is the DACL protected against inheritance?
		acl dacl;
		bool haveSacl;
		bool protectedSacl; // is the SACL protected against inheritance?
		acl sacl;

		// stuff for implicit conversion to abs or self-rel SD
		mutable bool haveAbsoluteSD;
		mutable SECURITY_DESCRIPTOR *absoluteSD;
		mutable bool haveSelfRelativeSD;
		mutable SECURITY_DESCRIPTOR *selfRelativeSD;

		// one of these is returned when Get()ting comething that is absent
		static sid invalidSid;
		static acl invalidAcl;
	};

/**@#-*/
} // namespace fksec
/**@#+*/

#endif // ! defined( AFX_SD_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_ )
