// acl.h: DO NOT include this file. #include fksec.h instead!



#if ! defined( AFX_ACL_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_ )
#define AFX_ACL_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_
#pragma once



/**@#-*/
namespace fksec {
/**@#+*/

	typedef std::vector<ace> AceList;
	typedef AceList::iterator AceListIter;
	typedef AceList::reverse_iterator AceListRevIter;
	typedef AceList::const_iterator AceListConstIter;

	class acl
	{
	public:
		// --- ctors/dtor ---
		// construct an empty acl object
		acl();
		// construct from another acl object
		acl( const acl &a );
		// construct from a PACL -- NT wants a non-const ptr, alas
		acl( ACL *a );
		// clean up
		virtual ~acl();

		// --- assignment ---
		// from another acl
		const acl &operator=( const acl &s );
		// from PACL
		const acl &operator=( ACL *s ); // why oh why does NT want a non-const ACL*?

		// --- conversions ---
		// return a pointer to an internally-maintained ACL
		// ask me if I wished that the NT guys used "const"!
		operator ACL *() const;

		// --- accessors ---
		// return a ref to the Nth ACE (or to an invalid one if o-o-b)
		const ace &GetAce( DWORD index ) const;
		ace &GetAce( DWORD index );
		// return bytes required incl. desired free bytes
		DWORD GetSize() const;
		// return minimum bytes required for this ACL in NT format
		DWORD GetMinimumSize() const { return GetLength(); }
		// return count of ACEs
		DWORD GetCount() const;
		// set amount of free bytes desired in NT ACL
		void SetFreeBytes( DWORD newAdditionalBytes );
		// how many bytes are extra?
		DWORD GetFreeBytes() const;

		// --- utilities ---
		// add an ACE
		// index gives the position _before_ which the ACE will be inserted
		// (DWORD) -1 inserts at end
		// no need to worry about canonical order, you can always canonicalize() the ACL
		void AddAce( DWORD index, const void *pace );
		void AddAce( DWORD index, const ace &ace );
		void AddAce( DWORD index, byte type, byte flags, byte inheritance, ACCESS_MASK mask, PSID psid );
		void AddAce( DWORD index, byte type, byte flags, byte inheritance, ACCESS_MASK mask, const TCHAR *stringSid );
		void AddAce( DWORD index, byte type, byte flags, byte inheritance, ACCESS_MASK mask, const sid &newSid );
		// delete an ACE
		void DeleteAce( DWORD index );
		// delete all ACEs for the specified SID
		void DeleteAcesForSid( PSID psid );
		void DeleteAcesForSid( const TCHAR *stringSid );
		void DeleteAcesForSid( const sid &delSid );
		// normalize ACEs -- ACEs with same type, flags, inheritance,
		// and SID are merged by ORing their masks. This means you can
		// just blindly add ACEs and then call normalize().
		void normalize();
		// sort ACEs into canonical order: deny-ACEs first, then allow-ACEs,
		// then all others. This is a stable sort.
		void canonicalize();
		// how many bytes for an NT-formatted ACL?
		DWORD GetLength() const;
		// store a copy of the ACL where the caller wants it
		void StoreAcl( ACL *p, DWORD sz ) const;
		// test for validity
		bool IsValid() const;

		// --- inserters ---
		// dump this acl
		friend fkostream &operator<<( fkostream &o, const acl &a );

	private:
		// initialize all members of an acl object
		void Init();
		// initialize the PACL part
		void ClearPACL() const;
		// release the PACL part
		void ReleasePACL() const;
		// read and convert a "-<uint64>" string
		ACL *MakePACL() const;

		// ACL components
		AceList aces;
		// how many bytes to reserve in addition to the needed minimum
		// when formatting an NT ACL
		DWORD additionalBytes;

		// This pseudo-ACE is returned when an index is out of range
		static ace invalidAce;

		// do we have a current NT-formatted ACL?
		// true also implies that pacl points to new-ed memory; false implies
		// the opposite. Do not rely on pacl being NULL or non-NULL!
		mutable bool havePACL;
		// NT-formatted ACL
		mutable ACL *pacl;
	};

/**@#-*/
} // namespace fksec
/**@#+*/

#endif // ! defined( AFX_ACL_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_ )
