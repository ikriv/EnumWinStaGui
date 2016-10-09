// fksec.h: This file pulls in all the other bits and pieces that make up the fksec namespace.



#if ! defined( AFX_FKSEC_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_ )
#define AFX_FKSEC_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_
#pragma once


#pragma comment( lib, "kernel32.lib" )
#pragma comment( lib, "advapi32.lib" )
#pragma comment( lib, "user32.lib" )

#if ( defined( UNICODE ) != defined( _UNICODE ) )
#error UNICODE and _UNICODE must either both be #defined, or neither of them may be #defined.
#endif

#if ( defined( UNICODE ) == defined( _MBCS ) )
#error Only one of _UNICODE and _MBCS may be #defined.
#endif


#ifndef _WINDOWS_
#include <windows.h>
#endif // ! _WINDOWS_

#ifndef _INC_TCHAR
#include <tchar.h>
#endif

#ifndef _EXCEPTION_
#include <exception>
#endif // ! _EXCEPTION_

#ifndef _STRING_
#include <string>
#endif // ! _STRING_

#ifndef _VECTOR_
#include <vector>
#endif // ! _VECTOR_

#ifndef _ALGORITHM_
#include <algorithm>
#endif // ! _ALGORITHM_

#ifndef _OSTREAM_
#include <ostream>
#endif // ! _SSTREAM_

#ifndef _IOMANIP_
#include <iomanip>
#endif // ! _IOMANIP_



#ifndef lenof
#define lenof(x) (sizeof (x)/sizeof ((x)[0]))
#endif



/**@#-*/
namespace fksec {
/**@#+*/

#ifdef _UNICODE
	typedef std::wstring fkstr;
	typedef std::wostream fkostream;
#else
	typedef std::string fkstr;
	typedef std::ostream fkostream;
#endif

/**@#-*/
} // namespace fksec
/**@#+*/

// ------------------ Begin IVK addition /*!*/
#pragma comment( lib, "netapi32.lib" ) 

// substitute for constants not defined in WinNT 4.0
#ifndef PROTECTED_DACL_SECURITY_INFORMATION
    #define PROTECTED_DACL_SECURITY_INFORMATION 0
#endif

#ifndef PROTECTED_SACL_SECURITY_INFORMATION
    #define PROTECTED_SACL_SECURITY_INFORMATION 0
#endif
// ------------------ end IVK addition /*!*/


#ifndef FKSEC_NO_AUTO_INCLUDES
#include "ex.h"
#include "priv.h"
#include "sid.h"
#include "ace.h"
#include "acl.h"
#include "sd.h"
#include "apis.h"
#endif



#endif // ! defined( AFX_FKSEC_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_ )
