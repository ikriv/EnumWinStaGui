// sid.h: DO NOT include this file. #include fksec.h instead!



#if ! defined( AFX_SID_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_ )
#define AFX_SID_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_
#pragma once


/**@#-*/
namespace fksec {
/**@#+*/

/** The sid class mimics an NT SID object (Security Identifier).

SIDs are strings of numbers that uniquely (one hopes) identify an 
entity within a certain context.  Their major features are an "authority" 
which is basically the subsystem or general area issuing a SID, and a 
series of DWORDs (RIDs) which, in a hierarchical manner, narrow down to 
a single entry.  Here, for example, is my SID when I log on as 
Administrator, expressed as a string:

<pre>
@@	S-1-5-21-484763869-764733703-1202660629-500
</pre>

All users in my domain would have an identical SID, except for the last 
value.  This last value is technically just one of 5 RIDs that are 
present, but in practice we call the last RID "the RID," and what goes 
before it is the "prefix."  Here, the prefix is at the same time a domain 
SID, because it identifies the domain in which user 500 lives, and a 
domain SID that is part of a complete, larger SID is often called a 
"domain prefix."

An object of class sid encapsulates just such a SID and happily converts 
between NT-formatted SIDs, user/group/domain/… names, and the textual 
representation above, which is what you can see in the HKEY_USERS key of 
your registry, too.  It also provides a const PSID conversion operator so 
you can plug an sid into any place where you would normally use a const PSID.

<h4>A note on the validity of sid instances</h4>

A sid object may, at any time, be in an invalid state, or in a valid one. 
Without goint into details, it becomes valid as soon as it has an sia 
(SID_IDENTIFIER_AUTHORITY) value and at least one RID. Using an invalid 
sid object anywhere is very likely to reward you with an exception, so 
make sure of your exception handling.

@author Felix Kasza <a href="mailto:felixk@mvps.org">&lt;felixk@mvps.org&gt;</a>,
see <a href="http://mvps.org/win32/security/fksec.html">http://mvps.org/win32/security/fksec.html</a>
*/

	class sid
	{
	public:
		/** The SidType enumeration, used by LookupSidType(), provides 
		values which identify a SID as local or remote.

		In particular:

		stWellKnown refers to SIDs which are always local and which do 
		not belong to any account domain. Examples are the SIDs for 
		Everyone, LocalSystem.

		stLocal refers to SIDs residing in the local machine's accounts 
		database -- local users and local groups, mostly.

		stDomain is used for accounts which reside in the domain 
		controller's accounts database, including domain users and global 
		groups.

		stForeign is the tag for SIDs whose domain prefix does not match 
		the local prefixes, and for which no domain can be identified.
		@see #LookupSidType */
		enum SidType { stWellKnown, stLocal, stDomain, stForeign };

		// --- ctors/dtor ---

		/** Construct *this as an empty sid object

		*this is initially invalid.
		@exceptions none */
		sid();
		
		/** Construct *this as a copy of another sid object

		*this becomes a copy of the argument, including its valid state.
		@exceptions none */
		sid( const sid &s );
		
		/** Construct *this as a copy of a SID

		Parses the SID and stashes its contents in *this.  Note that the 
		sid class does not wrap the PSID; you can free the PSID after 
		this function returns.
		@exceptions none */
		sid( const PSID s );

		/** Construct *this by interpreting a string SID or looking a
		name up in the accounts database

		The ctor first tries to parse the string as a text-SID ("S-1-5-...");
		if that fails, it calls LookupAccountName() with server == NULL 
		(i.e., it asks the local machine to find the name).  If this works,
		the SID is stored in *this.
		@exceptions: errNoMemory, errInvalidSid (both from LookupName()) */
		sid( const TCHAR *name );
		
		/** Construct *this from bits and pieces

		This ctor works in a manner similar to ::AllocateAndInitializeSid(), 
		except that it allocates no memory.
		@exceptions none */
		sid( SID_IDENTIFIER_AUTHORITY newsia, DWORD nSubAuths,
			DWORD subAuth0 = 0, DWORD subAuth1 = 0, DWORD subAuth2 = 0,
			DWORD subAuth3 = 0, DWORD subAuth4 = 0, DWORD subAuth5 = 0,
			DWORD subAuth6 = 0, DWORD subAuth7 = 0 );
		
		/** Clean up *this

		The dtor releases any internal buffers (like the one that the
		object may have used for providing PSID conversion).
		@exceptions none. */
		virtual ~sid();

		// --- assignment ---

		/** Set *this to a copy of a sid, including the valid/invalid state
		@returns a const reference to *this.
		@exceptions: none */
		const sid &operator=( const sid &s );

		/** Set *this to a copy of a SID

		Parses the SID and stashes its contents in *this.  Note that the 
		sid class does not wrap the PSID; you can free the PSID after 
		this function returns.
		@returns a const reference to *this.
		@exceptions none */
		const sid &operator=( const PSID s );

		/** Set *this to a SID derived from interpreting a string SID or 
		looking a name up in the accounts database

		The operator first tries to parse the string as a text-SID ("S-1-5-...");
		if that fails, it calls LookupAccountName() with server == NULL 
		(i.e., it asks the local machine to find the name).  If this works,
		the SID is stored in *this.
		@returns a const reference to *this.
		@exceptions: errNoMemory, errInvalidSid (both from LookupName()) */
		const sid &operator=( const TCHAR *name );

		// --- conversions ---

		/** Return a pointer to an NT-formatted SID

		Creates an NT-formatted SID in storage internal to the object, and 
		returns a pointer to that memory. The returned pointer is only 
		guaranteed to be valid until the next method is invoked on the 
		object.

		Note: a PSID is essentially a typedef for a void*. This may lead to
		conversions where you do not want them.
		@returns a const pointer to SID (PSID) which is typedef-ed as 
		a const pointer to void.
		@exceptions errInvalidSid, errNoMemory */
		operator const PSID() const;

		// --- comparisons ---

		/** Compare two sid objects for less-than

		Compares *this with r and returns the result.

		The < operator compares by the following arbitrary rules: If
		*this has less subauthorities than r, *this is smaller. If *this
		has more subauthorities than r, r is smaller. Else, if the
		numerical value of this->sia is less than r.sia, *this is smaller;
		if this->sia is higher than r.sia, r is smaller. If, after this,
		the subauthority count and the SIA have proved equal, the lists
		of subauthorities are compared left to right. The relation between
		the first two unequal subauthorities determines the final result.
		If no unequal subauthorities exist, the SIDs are equal, and the
		result is false (since "==" implies "not <").
		@returns true if *this is "less" than r, or false if not.
		@exceptions: errInvalidSid */
		bool operator<( const sid &r ) const;

		/** Compare two sid objects for equality

		Compares *this with r and returns the result.

		The == operator considers two SIDs (and sids) to be equal if and 
		only if they have the same authority value, subauthority count, 
		and subauthority lists.
		@returns true if the sids are equal, or false if not.
		@exceptions: errInvalidSid */
		bool operator==( const sid &r ) const;

		/** Compare two sid objects for inequality

		Compares *this with r and returns the result.

		The != operator considers two SIDs (and sids) to be equal if and 
		only if they have the same authority value, subauthority count, 
		and subauthority lists.
		@returns true if the sids are not equal, or false if they are equal.
		@exceptions: errInvalidSid */
		bool operator!=( const sid &r ) const;

		/** Compare the prefixes of two sid objects for equality

		Compares the prefix of *this with the prefix of r and returns the result.

		The method considers two SIDs (and sids) to be equal if and 
		only if they have the same authority value, subauthority count, 
		and subauthority lists, except for the last subauthority value,
		which is ignored in both sid objects.
		@returns 1 if the sids have the same prefix, or 0 if not.
		@exceptions: errInvalidSid */
		bool EqualPrefix( const sid &r ) const;


		// --- utilities ---

		/** GetLength() computes the number of bytes needed to store 
		*this as an NT SID
		@returns the number of bytes required for *this as an NT SID
		@exceptions none */
		DWORD GetLength() const;

		/** Returns the SID_IDENTIFIER_AUTHORITY value as a 64-bit integer
		@returns the integer equivalent of the SIA, with proper byte ordering
		@exceptions none */
		__int64 GetSidIdentifierAuthority() const;

		/** Truncates *this by removing the last subauthority value

		One of the uses for this method is getting a domain SID from 
		a user or group SID.
		@exceptions errInvalidSid */
		void RemoveLastRid();

		/** Appends a RID to the end of this sid

		Given a domain SID and a user RID (some NetGroup*() functions 
		return just those), this method can be used to build a full
		SID for the user (or group).
		@exceptions errInvalidSid */
		void AppendSubAuthority( DWORD rid );

		/** Sets a new value for an existing RID

		Note that subauthority index values range from 0 through 
		GetSubAuthorityCount() - 1. This method can not be used to 
		lengthen or truncate a sid.
		@exceptions errInvalidSid, errInvalidSubAuthIndex */
		void SetSubAuthority( DWORD index, DWORD rid );

		/** Returns a specified subauthority value

		Note that subauthority index values range from 0 through 
		GetSubAuthorityCount() - 1.
		@returns a DWORD with the index<sup>th</sup> subauthority value
		@exceptions errInvalidSid, errInvalidSubAuthIndex */
		DWORD GetSubAuthority( DWORD index ) const;

		/** Returns the current supauthority count
		@returns a DWORD with the sid's current subauthority count
		@exceptions errInvalidSid */
		DWORD GetSubAuthorityCount() const;

		/** Test *this for validity

		If the sid is initialized, it builds an NT SID and runs it through
		::IsValidSid(). If both tests are met, the sid is considered valid.
		This function is guaranteed to never throw an exception.
		@returns true if the sid is valid, else false
		@exceptions none */
		bool IsValid() const;

		/** Translate SID_NAME_USE to text description

		SnuToText() looks up a short textual representation for the
		SID_NAME_USE argument value. This method does not return localized
		strings.
		@returns a const TCHAR* to the description string, or to the string 
		"*SNU?*" if the argument is an unknown SID_NAME_USE value.
		@exceptions none */
		static const TCHAR *SnuToText( SID_NAME_USE snu );

		/** Return the string representation of *this

		Produces a string representation of the sid object's contents 
		in the same format as NT5's ::ConvertSidToStringSid(), but does
		not rely on running under NT5.

		If the sid is not currently valid, the returned string has the 
		value "*invalid*". No exceptions are thrown.
		@returns an instance of fkstr (which maps to std::string or 
		std::wstring, depending on the UNICODE setting) containing
		the stringified sid.
		@exceptions none */
		fkstr ConvertToStringSid() const;

		/** Store a SID into a client-provided buffer

		StoreSid() fills a client-allocated buffer with the NT-formatted 
		SID corresponding to *this' contents, unless size id less than
		needed; in that case, an exception is raised.
		@exceptions errInvalidSid, errBufferTooSmall */
		void StoreSid( PSID ps, DWORD size ) const;

		/** Set *this to the SID represented by a stringified SID

		This method is the complement to ConvertToStringSid(). It attempts
		to parse the format produced by that function and, if successful,
		sets *this to the parsed SID. If unsuccessful, an exception is
		raised, and the state of *this is unchanged.
		@exceptions errInvalidSid */
		void ConvertFromStringSid( const TCHAR *stringsid );

		/** Sets *this to the SID of an account looked up on a server

		LookupName() uses ::LookupAccountName to query the named server
		(local machine if server == NULL or points to an empty string) for
		the SID of the account named. If successful, *this is set to this
		SID, else an exception is raised.
		@exception errNoMemory, errInvalidSid */
		void LookupName( const TCHAR *server, const TCHAR *name );

		/** Retrieves the account name and domain for the sid

		This method uses ::LookupAccountSid() to query the named server 
		(local machine if server == NULL or points to an empty string) 
		for the name and domain of the account represented by the sid.
		@returns no direct return; side effects: name, domain, snu changed 
		if successful.
		@exception errInvalidSid, errNoMemory */
		void LookupSid( const TCHAR *server, fkstr &name,
			fkstr &domain, SID_NAME_USE &snu ) const;

		/** Determines the server on which the SID resides

		This method finds out whether the SID is in the local SAM, 
		whether it comes from the domain that this machine is a server 
		of, or whether it is a "special" SID such as the ones in the 
		BUILTIN domain.
		@returns a sid::SidType enumeration value representing the SID 
		status.
		@exceptions errInvalidSid, errNoPrefixSid, errTooManySubauths 
		(from AppendSubAuthority() only) */
		SidType LookupSidType();

		// --- inserters ---

		/** Inserts a textual representation, suitable for debugging, 
		into an output stream

		Note that fkostream is a typedef for either std::ostream or 
		std::wostream, depending on the UNICODE setting.
		@exceptions none */
		friend fkostream &operator<<( fkostream &o, const sid &s );

	private:
		/** Initializes all members of a sid object to zero and sets 
		its state to invalid
		@exceptions none */
		void Init();

		/** Sets psid to NULL and havePSID to false

		This method does not release memory allocated for psid. 
		Use with care.
		@exceptions none */
		void ClearPSID() const;

		/** Releases psid memory, if any, and sets psid to NULL and 
		havePSID to false
		@exceptions none */
		void ReleasePSID() const;

		/** Reads and converts a "-<uint64>" string

		CvsGetUInt64() expects str to point at a string consisting of 
		a hyphen and a number. The number itself may be decimal (default), 
		octal (if there is a leading zero), or hex (leading zero followed 
		by a lower- or uppercase 'x'). The method advances str past the
		characters it has successfully read.
		
		The return value is false if either the input string's syntax 
		does not match the method's expectations, or if the number (or 
		the part following the base-prefix, if any) has no convertible 
		digits. Otherwise, the return value is true, and the unsigned 
		__int64 reference r stores the result.
		@returns true if a number was successfully converted (in which 
		case str points at the first unconverted character, and r holds 
		the result); or false if a syntax error was encountered.
		@exceptions none */
		bool CvsGetUInt64( const TCHAR *&str, unsigned __int64 &r );

		/** Generates a PSID from *this

		MakePSID() takes no action if havePSID is already true. If 
		havePSID is false, the method allocates a buffer for a SID and 
		fills it with the SID components in *this. The member psid is 
		set to the address of the buffer, and havePSID is set to true.
		@returns the value of psid, pointing to the SID.
		@exceptions errInvalidSid, errNoMemory */
		PSID MakePSID() const;


		/** isValid remains false until a valid SID can be built. */
		bool isValid;

		// SID components

		/** sia contains the sid's SID_IDENTIFIER_AUTHORITY value.

		sia is mutable because ::InitializeSid() wants a non-const 
		pointer. Feh! */
		mutable SID_IDENTIFIER_AUTHORITY sia;

		/** subAuthCount gives the current number of subauthorities in *this. */
		DWORD subAuthCount;

		/** subAuth[] contains the list of subauthorities. */
		DWORD subAuth[SID_MAX_SUB_AUTHORITIES];

		/** havePSID indicates whether psid is valid and points to an 
		NT-formatted SID reflecting the contents of *this.

		If havePSID is false, psid is invalid, even if non-NULL. Do 
		not rely on whether psid is NULL or not!
		@see #MakePSID
		@see #ReleasePSID */
		mutable bool havePSID;

		/** psid points to a new()ed chunk of memory which holds an
		NT-formatted SID representing *this.

		If havePSID is false, psid is not to be relied on! */
		mutable PSID psid;
	};

/**@#-*/
} // namespace fksec
/**@#+*/

#endif // ! defined( AFX_SID_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_ )
