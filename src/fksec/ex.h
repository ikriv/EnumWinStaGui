// ex.h: DO NOT include this file. #include fksec.h instead!



#if ! defined( AFX_EX_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_ )
#define AFX_EX_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_
#pragma once



#define FKSECADDHOP(msg) AddHop( _T( __FILE__ ), __LINE__, _T( msg ) )
#define NEWEX(err,msg) new ex( _T( __FILE__ ), __LINE__, (err), _T( msg ) )
#define NEWEX32(err,msg,err32) new ex( _T( __FILE__ ), __LINE__, (err), _T( msg ), (err32) )
#define RETHROWEX(msg) catch ( ex *e ) { e->AddHop( _T( __FILE__ ), __LINE__, _T( msg ) ); throw; }



/**@#-*/
namespace fksec {
/**@#+*/


/** Errcode enumerates the principal type of an exception object.

  Exception types:

  errNone<br>
  You should never see this; it is there only so I can detect 
  uninitialized error codes during development.

  errBadHopIndex<br>
  This one is thrown if you access the hop list of an exception object 
  with an out-of-range index.

  errTooManySubAuths<br>
  NT defines a limit on the number of subauthorities in a SID 
  (SID_MAX_SUB_AUTHORITIES); errTooManySubAuths is the reward for 
  exceeding this limit.

  errInvalidSid<br>
  This one gets thrown every time you try to use an invalid SID, look 
  up an inexistent user, look up the user name for an inexistent SID, 
  and so on.  "Using" and invalid sid includes using it in an ace, 
  which might get used in an acl, which might be plugged into a security 
  descriptor.

  errInvalidSubAuthIndex<br>
  Thrown if you try to get or set a subauthority of a sid that is not 
  present - basically, an index out-of-range error.

  errNoMemory<br>
  Some of the methods for all classes need dynamically allocated memory, 
  most notably the MakeFoo() functions, where Foo is PSID, PACE, PACL, 
  PSD as appropriate.  Other uses of dynamically allocated memory 
  include buffers for name lookups in the sid class, and chunks for 
  retrieving security descriptors in the apis.cpp functions.  By now 
  you can guess what happens if such memory as is needed cannot be 
  allocated.

  errBufferTooSmall<br>
  This happens when you call an object's StoreFoo() method to fill a 
  buffer supplied by you, and the size you pass is not sufficient.  
  Your choices are to either ask beforehand (all objects support a 
  GetLength() method, and the acl object even has two different ones), 
  allocating a sufficiently large buffer, or to handle the exception, 
  get the required size from the exception's GetData() function, and 
  proceed from there.

  errInvalidAce<br>
  This means that you tried to set invalid inheritance flags, an 
  invalid ACE type, or the like. Invalid users or SIDs get an 
  errInvalidSid instead.

  errInvalidAcl<br>
  Expect this exception when you feed invalid ACLs to an acl object.  
  Note that invalid users or SIDs get an errInvalidSid instead.

  errInvalidAceIndex<br>
  The index passed to GetAce() or SetAce() (of an acl object) is out-of-range.

  errStubbornPriv<br>
  A privilege refused to let us set its state (to enabled or disabled).

  errQueryToken<br>
  A privilege's state could not be retrieved.  This should not happen; 
  if the object cannot open a token handle for TOKEN_QUERY, an errOpenToken 
  exception is thrown, and if the object did successfully open the token 
  handle, this error should not happen.

  errDupTokenHandle<br>
  If an already-opened token handle is passed to a priv object, the object 
  attempts to create a duplicate for its own use.  This error is what you 
  get if the operation fails.

  errOpenToken<br>
  If you leave a priv object's token handle as zero, it attempts to open 
  the current thread or process token.  If this fails, you get an 
  errOpenToken exception.

  errCloseToken<br>
  In theory, this should not happen - assuming, that is, that I don't 
  pass invalid handles around.

  errInvalidPriv<br>
  The privilege you asked for doesn't exist.

  errUnreadableSD<br>
  Thrown by the functions in apis.cpp - a security descriptor could not 
  be read off an object.

  errUnwritableSD<br>
  Thrown by the functions in apis.cpp - a security descriptor could not 
  be written to an object.

  errInvalidSD<br>
  What can I say?  I am really running out of platitudes by now ...

  errNoPrefixSid
  Raised by some sid lookup functions if the attempt of getting a machine's 
  domain SID (with NetUserModalsGet()) fails. Call ex::GetErrWin32() for 
  the precise error code.

  errMaxError<br>
  You should never see this in an exception.  It serves mainly to check 
  indexes into the error string array.

  @author Felix Kasza <a href="mailto:felixk@mvps.org">&lt;felixk@mvps.org&gt;</a>,
  see <a href="http://mvps.org/win32/security/fksec.html">http://mvps.org/win32/security/fksec.html</a>
*/

	enum Errcode
	{
		errNone,
		errBadHopIndex,
		errTooManySubAuths,
		errInvalidSid,
		errInvalidSubAuthIndex,
		errNoMemory,
		errBufferTooSmall,
		errInvalidAce,
		errInvalidAcl,
		errInvalidAceIndex,
		errStubbornPriv,
		errQueryToken,
		errDupTokenHandle,
		errOpenToken,
		errCloseToken,
		errInvalidPriv,
		errUnreadableSD,
		errUnwritableSD,
		errInvalidSD,
		errNoPrefixSid,
		errMaxError
	};

/** fksec throws a heap-allocated instance of ex, the exception class, upon encountering an error.

  The fksec classes use C++ exceptions to report errors, with a very 
  few functions reporting success or failure through normal return values 
  (these are mostly of the "IsValid()" type, which one would expect to 
  return false instead of going bang.)

  An exception object records at least one of the error codes defined in 
  the ex::Errcode enumeration, an accompanying text message, and a list 
  of source-file/line-number/error-message combinations, starting with 
  the source line that threw the exception.  Additionally, the exception 
  object may contain a Win32 error code, and a DWORD of additional data, 
  which is used to communicate minimum required buffer sizes and the like.

  <h4>Catching and handling an fksec exception</h4>

  You should expect every method of every class to throw an exception at 
  you (except where explicitly noted otherwise in a method's discussion 
  below).  That means that all your uses of fksec classes should be 
  wrapped in try … catch blocks:

<pre>
@@    try { sid.StoreSid( thatBuffer, bufferSize ); }
@@    catch ( ex *e )
@@    {
@@        if ( e->GetErr() == something )
@@        {
@@            // handle this exception
@@            e->shoo(); // and delete it
@@        }
@@        else
@@        {
@@            // clean up here
@@            throw;
@@        }
@@    }
</pre>

  In general you will not want to wrap every single method call into its 
  own exception handler; fksec does that internally only in order to 
  pinpoint more precisely where an error occurred or was caught and re-thrown.

  Note that you are expected to delete the fksec exceptions you catch, 
  unless you re-throw them.  Due to the possible conflict between the 
  memory allocation structures of multiple modules linked with a static 
  runtime library, the exception class provides a method to do this 
  safely - see shoo() below.

  <h4>Interpreting the contents of an ex object</h4>

  As mentioned above, the error code, accessible through GetErr() and 
  GetErrString(), is the most important part - it tells you what went 
  wrong.  If the error was caused by the failure of a Win32 API function, 
  GetErrWin32() will return a non-zero value, the result of 
  GetLastError() at the point of failure.  If the error was caused by 
  passing too small a buffer, GetData() tells you how large the buffer 
  must be; this affects mainly the StoreSid(), StoreAce(), StoreAcl(), 
  StoreSd() functions which expect a caller-supplied buffer.

  Finally, the list of source lines where the exception was thrown or 
  re-thrown on its way to you contains messages explaining what the code 
  was trying to do when the error was detected.

  Note that none of this is intended for the user's eyes; handling errors 
  is your job.  Also note carefully that NT exceptions - access violations 
  due to invalid pointers passed in, for example - are not caught or 
  handled by the code.  This is on the to-do list, though.

  @author Felix Kasza <a href="mailto:felixk@mvps.org">&lt;felixk@mvps.org&gt;</a>,
  see <a href="http://mvps.org/win32/security/fksec.html">http://mvps.org/win32/security/fksec.html</a>
*/
	class ex
	{
	public:
		/** Hop describes a catch handler that re-threw the exception pointer. */
		struct Hop
		{
			/** The source file name of the re-throwing catch handler. */
			fkstr file;
			/** The source line number of the re-throwing catch handler. */
			int line;
			/** What the re-throwing catch handler has to say about all this. */
			fkstr msg;
		};

		// --- ctors/dtor ---

		/** The default ctor does precisely nothing.
		@exceptions none */
		ex()												{ }
		
		/** The copy ctor initialises *this to a copy of the exception e.
		@exceptions none */
		ex( const ex &e );
		
		/** Initialises an ex object from bits and pieces supplied by you.  

		ex.h contains the NEWEX and NEWEX32 macros, which you might find 
		useful if you extend the classes and wish to stick with my exceptions.
		@exceptions: none */
		ex( const TCHAR *newFile, int newLine,
			Errcode newErr, const TCHAR *newMsg,
			DWORD newErrWin32 = 0, DWORD newData = 0 );
		
		/** The dtor, well, duh. Supply your own comment here ...
		@exception none */
		virtual ~ex()										{ }
		
		/** shoo() basically executes a delete this, avoiding problems
		with multiple copies of the runtime library.
		@exception none */
		virtual void shoo();

		// --- assignment ---
		
		/** Assignment operator.
		
		I do prefer to have explicit assignment operators (or private, 
		unimplemented, ones), if only to prevent the compiler from doing 
		what it thinks best.
		@exception none */
		const ex &operator=( const ex &e );

		// --- accessors ---
		
		/** AddHop() adds an entry to the hop list.

		AddHop() is the method that adds an entry to the exception's history. 
		(The first entry is usually made during construction of the exception 
		object.) For uses, see any of the other classes' source files.
		@exception none */
		void AddHop( const TCHAR *newFile,
			int newLine, const TCHAR *newMsg );

		/** SetErr() sets the object's error code.

		The operation fails silently if the error code is out-of-range.
		@exception: none. */
		void SetErr( Errcode newErr );
		
		/** GetErr() returns the object's error code.

		@returns a member of the Errcode enumeration giving the reason 
		for which the exception was thrown.
		@exception: none */
		Errcode GetErr() const								{ return err; }
		
		/** GetErrString() retrieves the string name of the object's error code.

		Note that this is just the name of the Errcode constant, <i>not</i> 
		something you can show to a user.
		@returns a const pointer to the string corresponding to the error 
		code in *this, or a const pointer to the string "-unknown-" if the 
		error code is invalid.
		@exception none. */
		const TCHAR *GetErrString() const;

		/** Sets the Win32 error code that, presumably, is the reason 
		for the exception being thrown.
		@exception none. */
		void SetErrWin32( DWORD newErrWin32 )				{ errWin32 = newErrWin32; }

		/** Gets the Win32 error code that, presumably, is the reason 
		for the exception being thrown.
		@returns a Win32 error code.  0 means that there was no Win32 error.
		@exception none. */
		DWORD GetErrWin32() const							{ return errWin32; }

		/** Sets optional exception data.

		Some exceptions communicate a DWORD (say, the minimum size 
		of a buffer that was too small) to the user.  This is how.
		@exception none. */
		void SetData( DWORD newData )						{ data = newData; }

		/** Retrieves optional exception data.

		Some exceptions communicate a DWORD (say, the minimum size 
		of a buffer that was too small) to the user.  This method 
		retrieves that value.  If the return is 0, the value was not set.
		@returns the DWORD containing additional data, or 0 if none 
		(or if 0 was explicitly set).
		@exception none. */
		DWORD GetData() const								{ return data; }

		/** Get number of entries in the hop list

		The hop list has one entry for each catch handler that 
		passed the exception on (by re-throwing it), plus one 
		for the initial throw, assuming that every intermediate 
		handler bothered to add itself to the hop list.  This 
		function retrieves the number of entries in this list.
		@returns the number of entries in the hop list.
		@exception none. */
		size_t GetHopCount() const							{ return hops.size(); }

		/** Get a non-const reference to a hop entry.

		Hop entries are numbered 0 through GetHopCount() - 1.  
		The structure of a hop entry itself is defined in ex.h.
		@returns a non-const reference to a hop entry.
		@exception errBadHopIndex */
		Hop &operator[]( size_t index );

		/** Get a const reference to a hop entry.

		Hop entries are numbered 0 through GetHopCount() - 1.  
		The structure of a hop entry itself is defined in ex.h.
		@returns a const reference to a hop entry.
		@exception errBadHopIndex */
		const Hop &operator[]( size_t index ) const;

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
		friend fkostream &operator<<( fkostream &o, const ex& e );

	private:
		/** The list of catch handlers that were traversed */
		typedef std::vector<Hop> HopList;

		/** The list of catch handlers having seen the exception

		As the exception bubbles its way up the call stack, catch
		handlers add their comments to this list.
		@see #AddHop */
		HopList hops;

		/** The error code. Meanings are listed in the documentation for the
		@$ Errcode
		enumeration. */
		Errcode err;

		/** The Win32 error code that caused the exception, if any; else 0. */
		DWORD errWin32;

		/** Additional data, if any; else 0. */
		DWORD data;

		/** Name strings for the
		@$ Errcode
		enumeration values. */
		static const TCHAR *errStrings[];

		/** The count of string pointers in the errStrings[] array. */
		static const int numErrStrings;
	};

/**@#-*/
} // namespace fksec
/**@#+*/

#endif // ! defined( AFX_SID_H__C2404C08_2791_41F1_A45E_A62EF7364105__INCLUDED_ )
