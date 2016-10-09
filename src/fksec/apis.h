// apis.h: DO NOT include this file. #include fksec.h instead!



#if ! defined( AFX_APIS_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_ )
#define AFX_APIS_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_
#pragma once



/**@#-*/
namespace fksec {
/**@#+*/

	// functions that extract and set SDs from/to securable objects

	/** retrieves a file's security descriptor

	GetFileSecurity() mimics the behaviour of the NT API function 
	of the same name, with only a few changes. These are:
	<ul><li>Errors are not indicated by a FALSE return value and 
	SetLastError() but are thrown as exceptions</li>
	<li>Instead of a pre-allocated buffer (and its size) for a 
	SECURITY_DESCRIPTOR, the function accepts a reference to an 
	fksec::sd and fills it in</li>
	<li>It automatically enables SeSecurityPrivilege, if a first 
	attempt at calling the NT function results in an 
	ERROR_PRIVILEGE_NOT_HELD</li></ul>
	@param filename the name of the file for which you wish to 
	retrieve the security descriptor
	@param whichParts a set of bits, ORed together, indicating which 
	elements of the security descriptor should be retrieved; valid 
	bits are, as of 24 Dec 1999: DACL_SECURITY_INFORMATION, 
	GROUP_SECURITY_INFORMATION, OWNER_SECURITY_INFORMATION, 
	SACL_SECURITY_INFORMATION. NT5 also defines the following: 
	PROTECTED_DACL_SECURITY_INFORMATION, PROTECTED_SACL_SECURITY_INFORMATION, 
	UNPROTECTED_DACL_SECURITY_INFORMATION, UNPROTECTED_SACL_SECURITY_INFORMATION.
	@param sd a reference to the sd object which is to receive the 
	security descriptor.
	@exceptions errUnreadableSd, plus any that other fksec classes 
	may throw (unlikely, and usually the result of a bug of mine)
	*/
	void GetFileSecurity( const TCHAR *filename, SECURITY_INFORMATION whichParts, sd &sd );

	/** sets a file's security descriptor

	SetFileSecurity() mimics the behaviour of the NT API function 
	of the same name, with only a few changes. These are:
	<ul><li>Errors are not indicated by a FALSE return value and 
	SetLastError() but are thrown as exceptions</li>
	<li>Instead of a SECURITY_DESCRIPTOR, the function accepts 
	a reference to an fksec::sd</li>
	<li>It automatically enables SeSecurityPrivilege, if a first 
	attempt at calling the NT function results in an 
	ERROR_PRIVILEGE_NOT_HELD</li></ul>
	@param filename the name of the file for which you wish to 
	set the security descriptor
	@param whichParts a set of bits, ORed together, indicating which 
	elements of the security descriptor should be set; valid 
	bits are, as of 24 Dec 1999: DACL_SECURITY_INFORMATION, 
	GROUP_SECURITY_INFORMATION, OWNER_SECURITY_INFORMATION, 
	SACL_SECURITY_INFORMATION. NT5 also defines the following: 
	PROTECTED_DACL_SECURITY_INFORMATION, PROTECTED_SACL_SECURITY_INFORMATION, 
	UNPROTECTED_DACL_SECURITY_INFORMATION, UNPROTECTED_SACL_SECURITY_INFORMATION.
	@param sd a reference to the sd object representing the 
	security descriptor to be applied.
	@exceptions errUnwritableSd, plus any that other fksec classes 
	may throw (unlikely, and usually the result of a bug of mine)
	*/
	void SetFileSecurity( const TCHAR *filename, SECURITY_INFORMATION whichParts, const sd &sd );

	/** retrieves a kernel object's security descriptor

	GetKernelObjectSecurity() mimics the behaviour of the NT API 
	function of the same name, with only a few changes. These are:
	<ul><li>Errors are not indicated by a FALSE return value and 
	SetLastError() but are thrown as exceptions</li>
	<li>Instead of a pre-allocated buffer (and its size) for a 
	SECURITY_DESCRIPTOR, the function accepts a reference to an 
	fksec::sd and fills it in</li>
	<li>It automatically enables SeSecurityPrivilege, if a first 
	attempt at calling the NT function results in an 
	ERROR_PRIVILEGE_NOT_HELD</li></ul>
	@param h a handle to the kernel object for which you wish to 
	retrieve the security descriptor
	@param whichParts a set of bits, ORed together, indicating which 
	elements of the security descriptor should be retrieved; valid 
	bits are, as of 24 Dec 1999: DACL_SECURITY_INFORMATION, 
	GROUP_SECURITY_INFORMATION, OWNER_SECURITY_INFORMATION, 
	SACL_SECURITY_INFORMATION. NT5 also defines the following: 
	PROTECTED_DACL_SECURITY_INFORMATION, PROTECTED_SACL_SECURITY_INFORMATION, 
	UNPROTECTED_DACL_SECURITY_INFORMATION, UNPROTECTED_SACL_SECURITY_INFORMATION.
	@param sd a reference to the sd object which is to receive the 
	security descriptor.
	@exceptions errUnreadableSd, plus any that other fksec classes 
	may throw (unlikely, and usually the result of a bug of mine)
	*/
	void GetKernelObjectSecurity( HANDLE h, SECURITY_INFORMATION whichParts, sd &sd );

	/** sets a kernel object's security descriptor

	SetKernelObjectSecurity() mimics the behaviour of the NT API function 
	of the same name, with only a few changes. These are:
	<ul><li>Errors are not indicated by a FALSE return value and 
	SetLastError() but are thrown as exceptions</li>
	<li>Instead of a SECURITY_DESCRIPTOR, the function accepts 
	a reference to an fksec::sd</li>
	<li>It automatically enables SeSecurityPrivilege, if a first 
	attempt at calling the NT function results in an 
	ERROR_PRIVILEGE_NOT_HELD</li></ul>
	@param h a handle to the kernel object for which you wish to 
	set the security descriptor
	@param whichParts a set of bits, ORed together, indicating which 
	elements of the security descriptor should be set; valid 
	bits are, as of 24 Dec 1999: DACL_SECURITY_INFORMATION, 
	GROUP_SECURITY_INFORMATION, OWNER_SECURITY_INFORMATION, 
	SACL_SECURITY_INFORMATION. NT5 also defines the following: 
	PROTECTED_DACL_SECURITY_INFORMATION, PROTECTED_SACL_SECURITY_INFORMATION, 
	UNPROTECTED_DACL_SECURITY_INFORMATION, UNPROTECTED_SACL_SECURITY_INFORMATION.
	@param sd a reference to the sd object representing the 
	security descriptor to be applied.
	@exceptions errUnwritableSd, plus any that other fksec classes 
	may throw (unlikely, and usually the result of a bug of mine)
	*/
	void SetKernelObjectSecurity( HANDLE h, SECURITY_INFORMATION whichParts, const sd &sd );

	/** retrieves a user object's security descriptor

	GetUserObjectSecurity() mimics the behaviour of the NT API 
	function of the same name, with only a few changes. These are:
	<ul><li>Errors are not indicated by a FALSE return value and 
	SetLastError() but are thrown as exceptions</li>
	<li>Instead of a pre-allocated buffer (and its size) for a 
	SECURITY_DESCRIPTOR, the function accepts a reference to an 
	fksec::sd and fills it in</li>
	<li>It automatically enables SeSecurityPrivilege, if a first 
	attempt at calling the NT function results in an 
	ERROR_PRIVILEGE_NOT_HELD</li></ul>
	@param h a handle to the user object for which you wish to 
	retrieve the security descriptor
	@param whichParts a set of bits, ORed together, indicating which 
	elements of the security descriptor should be retrieved; valid 
	bits are, as of 24 Dec 1999: DACL_SECURITY_INFORMATION, 
	GROUP_SECURITY_INFORMATION, OWNER_SECURITY_INFORMATION, 
	SACL_SECURITY_INFORMATION. NT5 also defines the following: 
	PROTECTED_DACL_SECURITY_INFORMATION, PROTECTED_SACL_SECURITY_INFORMATION, 
	UNPROTECTED_DACL_SECURITY_INFORMATION, UNPROTECTED_SACL_SECURITY_INFORMATION.
	@param sd a reference to the sd object which is to receive the 
	security descriptor.
	@exceptions errUnreadableSd, plus any that other fksec classes 
	may throw (unlikely, and usually the result of a bug of mine)
	*/
	void GetUserObjectSecurity( HANDLE h, SECURITY_INFORMATION whichParts, sd &sd );

	/** sets a user object's security descriptor

	SetUserObjectSecurity() mimics the behaviour of the NT API function 
	of the same name, with only a few changes. These are:
	<ul><li>Errors are not indicated by a FALSE return value and 
	SetLastError() but are thrown as exceptions</li>
	<li>Instead of a SECURITY_DESCRIPTOR, the function accepts 
	a reference to an fksec::sd</li>
	<li>It automatically enables SeSecurityPrivilege, if a first 
	attempt at calling the NT function results in an 
	ERROR_PRIVILEGE_NOT_HELD</li></ul>
	@param h a handle to the user object for which you wish to 
	set the security descriptor
	@param whichParts a set of bits, ORed together, indicating which 
	elements of the security descriptor should be set; valid 
	bits are, as of 24 Dec 1999: DACL_SECURITY_INFORMATION, 
	GROUP_SECURITY_INFORMATION, OWNER_SECURITY_INFORMATION, 
	SACL_SECURITY_INFORMATION. NT5 also defines the following: 
	PROTECTED_DACL_SECURITY_INFORMATION, PROTECTED_SACL_SECURITY_INFORMATION, 
	UNPROTECTED_DACL_SECURITY_INFORMATION, UNPROTECTED_SACL_SECURITY_INFORMATION.
	@param sd a reference to the sd object representing the 
	security descriptor to be applied.
	@exceptions errUnwritableSd, plus any that other fksec classes 
	may throw (unlikely, and usually the result of a bug of mine)
	*/
	void SetUserObjectSecurity( HANDLE h, SECURITY_INFORMATION whichParts, const sd &sd );

	/** retrieves a service's security descriptor

	GetServiceObjectSecurity() mimics the behaviour of the NT API 
	function of the same name, with only a few changes. These are:
	<ul><li>Errors are not indicated by a FALSE return value and 
	SetLastError() but are thrown as exceptions</li>
	<li>Instead of a pre-allocated buffer (and its size) for a 
	SECURITY_DESCRIPTOR, the function accepts a reference to an 
	fksec::sd and fills it in</li>
	<li>It automatically enables SeSecurityPrivilege, if a first 
	attempt at calling the NT function results in an 
	ERROR_PRIVILEGE_NOT_HELD</li></ul>
	@param hSvc an SCM handle to the servoce entry for which you 
	wish to retrieve the security descriptor
	@param whichParts a set of bits, ORed together, indicating which 
	elements of the security descriptor should be retrieved; valid 
	bits are, as of 24 Dec 1999: DACL_SECURITY_INFORMATION, 
	GROUP_SECURITY_INFORMATION, OWNER_SECURITY_INFORMATION, 
	SACL_SECURITY_INFORMATION. NT5 also defines the following: 
	PROTECTED_DACL_SECURITY_INFORMATION, PROTECTED_SACL_SECURITY_INFORMATION, 
	UNPROTECTED_DACL_SECURITY_INFORMATION, UNPROTECTED_SACL_SECURITY_INFORMATION.
	@param sd a reference to the sd object which is to receive the 
	security descriptor.
	@exceptions errUnreadableSd, plus any that other fksec classes 
	may throw (unlikely, and usually the result of a bug of mine)
	*/
	void QueryServiceObjectSecurity( SC_HANDLE hSvc, SECURITY_INFORMATION whichParts, sd &sd );

	/** sets a service's security descriptor

	SetServiceSecurity() mimics the behaviour of the NT API function 
	of the same name, with only a few changes. These are:
	<ul><li>Errors are not indicated by a FALSE return value and 
	SetLastError() but are thrown as exceptions</li>
	<li>Instead of a SECURITY_DESCRIPTOR, the function accepts 
	a reference to an fksec::sd</li>
	<li>It automatically enables SeSecurityPrivilege, if a first 
	attempt at calling the NT function results in an 
	ERROR_PRIVILEGE_NOT_HELD</li></ul>
	@param hSvc an SCM handle to the service for which you wish to 
	set the security descriptor
	@param whichParts a set of bits, ORed together, indicating which 
	elements of the security descriptor should be set; valid 
	bits are, as of 24 Dec 1999: DACL_SECURITY_INFORMATION, 
	GROUP_SECURITY_INFORMATION, OWNER_SECURITY_INFORMATION, 
	SACL_SECURITY_INFORMATION. NT5 also defines the following: 
	PROTECTED_DACL_SECURITY_INFORMATION, PROTECTED_SACL_SECURITY_INFORMATION, 
	UNPROTECTED_DACL_SECURITY_INFORMATION, UNPROTECTED_SACL_SECURITY_INFORMATION.
	@param sd a reference to the sd object representing the 
	security descriptor to be applied.
	@exceptions errUnwritableSd, plus any that other fksec classes 
	may throw (unlikely, and usually the result of a bug of mine)
	*/
	void SetServiceObjectSecurity( SC_HANDLE hSvc, SECURITY_INFORMATION whichParts, const sd &sd );

	/** retrieves a registry key's security descriptor

	RegGetKeySecurity() mimics the behaviour of the NT API 
	function of the same name, with only a few changes. These are:
	<ul><li>Errors are not indicated by a FALSE return value and 
	SetLastError() but are thrown as exceptions</li>
	<li>Instead of a pre-allocated buffer (and its size) for a 
	SECURITY_DESCRIPTOR, the function accepts a reference to an 
	fksec::sd and fills it in</li>
	<li>It automatically enables SeSecurityPrivilege, if a first 
	attempt at calling the NT function results in an 
	ERROR_PRIVILEGE_NOT_HELD</li></ul>
	@param hk a handle to the registry key for which you wish to 
	retrieve the security descriptor
	@param whichParts a set of bits, ORed together, indicating which 
	elements of the security descriptor should be retrieved; valid 
	bits are, as of 24 Dec 1999: DACL_SECURITY_INFORMATION, 
	GROUP_SECURITY_INFORMATION, OWNER_SECURITY_INFORMATION, 
	SACL_SECURITY_INFORMATION. NT5 also defines the following: 
	PROTECTED_DACL_SECURITY_INFORMATION, PROTECTED_SACL_SECURITY_INFORMATION, 
	UNPROTECTED_DACL_SECURITY_INFORMATION, UNPROTECTED_SACL_SECURITY_INFORMATION.
	@param sd a reference to the sd object which is to receive the 
	security descriptor.
	@exceptions errUnreadableSd, plus any that other fksec classes 
	may throw (unlikely, and usually the result of a bug of mine)
	*/
	void RegGetKeySecurity( HKEY hk, SECURITY_INFORMATION whichParts, sd &sd );

	/** sets a registry key's security descriptor

	regSetKeySecurity() mimics the behaviour of the NT API function 
	of the same name, with only a few changes. These are:
	<ul><li>Errors are not indicated by a FALSE return value and 
	SetLastError() but are thrown as exceptions</li>
	<li>Instead of a SECURITY_DESCRIPTOR, the function accepts 
	a reference to an fksec::sd</li>
	<li>It automatically enables SeSecurityPrivilege, if a first 
	attempt at calling the NT function results in an 
	ERROR_PRIVILEGE_NOT_HELD</li></ul>
	@param hk a handle to the registry key for which you wish to 
	set the security descriptor
	@param whichParts a set of bits, ORed together, indicating which 
	elements of the security descriptor should be set; valid 
	bits are, as of 24 Dec 1999: DACL_SECURITY_INFORMATION, 
	GROUP_SECURITY_INFORMATION, OWNER_SECURITY_INFORMATION, 
	SACL_SECURITY_INFORMATION. NT5 also defines the following: 
	PROTECTED_DACL_SECURITY_INFORMATION, PROTECTED_SACL_SECURITY_INFORMATION, 
	UNPROTECTED_DACL_SECURITY_INFORMATION, UNPROTECTED_SACL_SECURITY_INFORMATION.
	@param sd a reference to the sd object representing the 
	security descriptor to be applied.
	@exceptions errUnwritableSd, plus any that other fksec classes 
	may throw (unlikely, and usually the result of a bug of mine)
	*/
	void RegSetKeySecurity( HKEY hk, SECURITY_INFORMATION whichParts, const sd &sd );

/**@#-*/
} // namespace fksec
/**@#+*/



#endif // ! defined( AFX_APIS_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_ )
