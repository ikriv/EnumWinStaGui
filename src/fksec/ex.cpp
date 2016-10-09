// recommended includes in stdafx.h (or in the precompiled header, to be precise):
// windows.h, tchar.h, string, vector, algorithm, exception, sstream, iomanip

#include "stdafx.h"
#define FKSEC_NO_AUTO_INCLUDES 1
#include "fksec.h"
#include "ex.h"

/**@#-*/
using namespace fksec;
/**@#+*/



// --- ctors/dtor ---

ex::ex( const TCHAR *newFile, int newLine, Errcode newErr,
	const TCHAR *newMsg, DWORD newErrWin32 /* = 0 */, DWORD newData /* = 0 */ )
{
	Hop h;
	h.file = newFile;
	h.line = newLine;
	h.msg = newMsg;
	hops.push_back( h );

	if ( newErr >= errNone && newErr < errMaxError )
		err = newErr;
	errWin32 = newErrWin32;
	data = newData;
}


ex::ex( const ex &e )
{
	hops = e.hops;
	err = e.err;
	errWin32 = e.errWin32;
	data = e.data;
}


// shoo() basically executes a delete this
void ex::shoo()
{
	delete this;
}



// --- assignment ---

const ex &ex::operator=( const ex &e )
{
	if ( this != &e )
	{
		hops = e.hops;
		err = e.err;
		errWin32 = e.errWin32;
		data = e.data;
	}

	return *this;
}



// --- accessors ---

void ex::AddHop( const TCHAR *newFile, int newLine, const TCHAR *newMsg )
{
	Hop h;
	h.file = newFile;
	h.line = newLine;
	h.msg = newMsg;
	hops.push_back( h );
}


void ex::SetErr( Errcode newErr )
{
	if ( newErr >= errNone && newErr < errMaxError )
		err = newErr;
}


const TCHAR *ex::GetErrString() const
{
	if ( err >= numErrStrings )
		return _T( "-unknown-" );

	return errStrings[err];
}


const ex::Hop &ex::operator[]( size_t index ) const
{
	if ( index >= hops.size() )
		throw NEWEX( errBadHopIndex, "Out-of-range index when accessing exception hops" );

	return hops[index];
}


ex::Hop &ex::operator[]( size_t index )
{
	if ( index >= hops.size() )
		throw NEWEX( errBadHopIndex, "Out-of-range index when accessing exception hops" );

	return hops[index];
}



// --- inserters ---

/** Dumps this exception to a stream.

An fkostream is a synonym for a std::ostream or a std::wostream, 
depending on the Unicode settings. operator<< writes a 
human-readable representation of the exception to such a stream, 
useful for diagnostics.  Note that the output takes multiple 
lines and ends with a newline.
@returns the fkostream reference that was passed in, as usual
for inserters.
@exception unpredictable, depends on the iostreams library. */
fkostream &fksec::operator<<( fkostream &o, const ex& e )
{
	int i;

	o << _T( "exception: " ) << e.GetErr() << _T( ", " ) <<
		e.GetErrString() << _T( " (Win32: " ) << e.GetErrWin32() <<
		_T( ", data: " ) << e.GetData() << _T( ")" ) << std::endl;

	o << _T( "hop line source-file" ) << std::endl << _T( "         message" ) << std::endl;

	for ( i = 0; i < e.GetHopCount(); ++ i )
		o << std::setw( 3 ) << i << _T( " " ) << std::setw( 4 ) <<
			std::setiosflags( o.left ) << e[i].line << _T( " " ) <<
			e[i].file << std::endl << _T( "         " ) << e[i].msg << std::endl;

	return o;
}



const TCHAR *ex::errStrings[] = {
	_T( "errNone" ),
	_T( "errBadHopIndex" ),
	_T( "errTooManySubAuths" ),
	_T( "errInvalidSid" ),
	_T( "errInvalidSubAuthIndex" ),
	_T( "errNoMemory" ),
	_T( "errBufferTooSmall" ),
	_T( "errInvalidAce" ),
	_T( "errInvalidAcl" ),
	_T( "errInvalidAceIndex" ),
	_T( "errStubbornPriv" ),
	_T( "errQueryToken" ),
	_T( "errDupTokenHandle" ),
	_T( "errOpenToken" ),
	_T( "errCloseToken" ),
	_T( "errInvalidPriv" ),
	_T( "errUnreadableSD" ),
	_T( "errUnwritableSD" ),
	_T( "errInvalidSD" ),
	_T( "errNoPrefixSid" ),
	_T( "errMaxError" )
};

const int ex::numErrStrings = sizeof errStrings / sizeof errStrings[0];
