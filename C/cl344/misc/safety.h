/****************************************************************************
*																			*
*					  cryptlib Correctness/Safety Header File 				*
*						Copyright Peter Gutmann 1994-2016					*
*																			*
****************************************************************************/

#ifndef _SAFETY_DEFINED

#define _SAFETY_DEFINED

/****************************************************************************
*																			*
*							Design-by-Contract Predicates					*
*																			*
****************************************************************************/

/* Symbolic defines to handle design-by-contract predicates.  If we're 
   really short of code space, we can save a little extra by turning the 
   predicates into no-ops */

#ifndef CONFIG_CONSERVE_MEMORY_EXTRA

#define REQUIRES( x )		if( !( x ) ) retIntError()
#define REQUIRES_N( x )		if( !( x ) ) retIntError_Null()
#define REQUIRES_B( x )		if( !( x ) ) retIntError_Boolean()
#define REQUIRES_V( x )		if( !( x ) ) retIntError_Void()
#define REQUIRES_EXT( x, y )	if( !( x ) ) retIntError_Ext( y )
#define REQUIRES_D( x )		if( !( x ) ) retIntError_Dataptr()
#define REQUIRES_S( x )		if( !( x ) ) retIntError_Stream( stream )

#else

#define REQUIRES( x )
#define REQUIRES_N( x )
#define REQUIRES_B( x )
#define REQUIRES_V( x )
#define REQUIRES_EXT( x, y )
#define REQUIRES_D( x )
#define REQUIRES_S( x )

#endif /* CONFIG_CONSERVE_MEMORY_EXTRA */

#define ENSURES				REQUIRES
#define ENSURES_N			REQUIRES_N
#define ENSURES_B			REQUIRES_B
#define ENSURES_V			REQUIRES_V
#define ENSURES_EXT			REQUIRES_EXT
#define ENSURES_D			REQUIRES_D
#define ENSURES_S			REQUIRES_S

/* A special-case form of the REQUIRES() predicate that's used in functions 
   that acquire a mutex.  There are two versions of this, one for cryptlib
   kernel mutexes, denoted by KRNLMUTEX, and one for native mutexes that are
   only visible inside the kernel, denoted by MUTEX */

#ifndef CONFIG_CONSERVE_MEMORY_EXTRA

#define REQUIRES_KRNLMUTEX( x, mutex ) \
		if( !( x ) ) \
			{ \
			krnlExitMutex( mutex ); \
			retIntError(); \
			}
#define REQUIRES_KRNLMUTEX_V( x, mutex ) \
		if( !( x ) ) \
			{ \
			krnlExitMutex( mutex ); \
			retIntError_Void(); \
			}

#define REQUIRES_MUTEX( x, mutex ) \
		if( !( x ) ) \
			{ \
			MUTEX_UNLOCK( mutex ); \
			retIntError(); \
			}
#else

#define REQUIRES_KRNLMUTEX( x, mutex )
#define REQUIRES_KRNLMUTEX_V( x, mutex )
#define REQUIRES_MUTEX( x, mutex )

#endif /* CONFIG_CONSERVE_MEMORY_EXTRA */

#define ENSURES_KRNLMUTEX	REQUIRES_KRNLMUTEX
#define ENSURES_KRNLMUTEX_V	REQUIRES_KRNLMUTEX_V

#define ENSURES_MUTEX		REQUIRES_MUTEX

/* Another variant of REQUIRES() that releases an object on exit */

#ifndef CONFIG_CONSERVE_MEMORY_EXTRA

#define REQUIRES_OBJECT( x, object ) \
		if( !( x ) ) \
			{ \
			krnlReleaseObject( object ); \
			retIntError(); \
			}
#else

#define REQUIRES_OBJECT( x, object )

#endif /* CONFIG_CONSERVE_MEMORY_EXTRA */

#define ENSURES_OBJECT		REQUIRES_OBJECT

/****************************************************************************
*																			*
*								Bounds Checks								*
*																			*
****************************************************************************/

/* Perform a range check, verifying that { value } falls within 
   { start, end } */

#define rangeCheck( value, start, end ) \
		( ( value ) < ( start ) || ( value ) > ( end ) ) ? FALSE : TRUE

/* Perform a bounds check on indexes into a block of memory, verifying that 
   { start, length } falls within { 0, totalLength }.  There are two 
   versions of this, the default which requires a nonzero start offset and 
   the special-case variant which allows a zero start offset, which is used 
   for situations like optionally MIME-wrapped data which have a nonzero 
   offset if there's a MIME header to be skipped but a zero offset if it's 
   unencapsulated data */

#define boundsCheck( start, length, totalLength ) \
		( ( start ) <= 0 || ( length ) < 1 || \
		  ( start ) + ( length ) > ( totalLength ) ) ? FALSE : TRUE
#define boundsCheckZ( start, length, totalLength ) \
		( ( start ) < 0 || ( length ) < 1 || \
		  ( start ) + ( length ) > ( totalLength ) ) ? FALSE : TRUE

/* Perform a bounds check on pointers to blocks of memory, verifying that an
   inner block of memory is contained entirely within an outer block of 
   memory:

			innerPtr
				v ----- innerLength ---->
		+-------+-----------------------+-------+
		|		|						|		|
		+-------+-----------------------+-------+
		^ --------------- length --------------->
	startPtr

   This is used for pointers to specific objects within a large encoded data
   block.  Since it's a fairly complex set of checks, it's implemented as a
   function in int_api.c */

CHECK_RETVAL_BOOL \
BOOLEAN pointerBoundsCheck( IN_OPT const void *data,
							IN_LENGTH_Z const int dataLength,
							IN_OPT const void *innerData,
							IN_LENGTH_SHORT_Z const int innerDataLength );

/****************************************************************************
*																			*
*							Pointer Validity Checks							*
*																			*
****************************************************************************/

/* Check the validity of a pointer passed to a cryptlib function.  Usually
   the best that we can do is check that it's not NULL, but some OSes allow
   for better checking than this, for example that it points to a block of
   readable or writeable memory.  Under Windows IsBadReadPtr() will always
   succeed if the size is 0, so we have to add a separate check to make sure
   that it's non-NULL.

   For any OS, we check not just for the specific value NULL but for anything
   that appears to be pointing into an unlikely memory range.  This is used
   to catch invalid pointers to elements inside structures, for example:

	struct foo_struct *fooPtr; 
	
	function( &fooPtr->element ); 
	
   where fooPtr is NULL, which will pass in a small integer value as the 
   pointer.  While it won't catch most invalid pointers, it's at least a bit 
   more useful than just checking for NULL.

   There are additional caveats with the use of the Windows memory-checking
   functions.  In theory these would be implemented via VirtualQuery(),
   however this is quite slow, requiring a kernel transition and poking
   around with the page protection mechanisms.  Instead, they try and read
   or write the memory, with an exception handler wrapped around the access.
   If the exception is thrown, they fail.  The problem with this way of
   doing things is that if the memory address is a stack guard page used to
   grow the stack (when the system-level exception handler sees an access to
   the bottom-of-stack guard page, it knows that it has to grow the stack)
   *and* the guard page is owned by another thread, IsBadXxxPtr() will catch 
   the exception and the system will never see it, so it can't grow the 
   stack past the current limit (note that this only occurs if the guard 
   page that we hit is owned by a different thread; if we own in then the
   kernel will catch the STATUS_GUARD_PAGE_VIOLATION exception and grow the
   stack as required).  In addition if it's the last guard page then instead 
   of getting an "out of stack" exception, it's turned into a no-op.  The 
   second time the last guard page is hit, the application is terminated by 
   the system, since it's passed its first-chance exception.

   A variation of this is that the calling app could be deliberately passing
   a pointer to a guard page and catching the guard page exception in order
   to dynamically generate the data that would fill the page (this can 
   happen for example when simulating a large address space with pointer 
   swizzling), but this is a pretty weird programming technique that's 
   unlikely to be used with a crypto library.

   A lesser problem is that there's a race condition in the checking in 
   which the memory can be unmapped between the IsBadXxxPtr() check and the 
   actual access, but you'd pretty much have to be trying to actively 
   subvert the checks to do something like this.

   For these reasons we use these functions mostly for debugging, wrapping
   them up in assert()s in most cases where they're used.  Under Windows 
   Vista they've actually been turned into no-ops because of the above 
   problems, although it's probable that they'll be replaced by code to 
   check for NULL pointers, since Microsoft's docs indicate that this much 
   checking will still be done.  In addition the type of checking seems to
   be a function of the Visual C++ libraries used rather than the OS, since
   VC++ 6 applications still perform the full readability check even under
   Windows 7 and 8.
   
   If necessary we could also replace the no-op'd out versions with the 
   equivalent code:

	inline BOOL IsBadReadPtr( const VOID *lp, UINT_PTR ucb )
		{
		__try { memcmp( p, p, cb ); }
		__except( EXCEPTION_EXECUTE_HANDLER ) { return( FALSE ); }
		return( TRUE );
		}

	inline BOOL IsBadWritePtr( LPVOID lp, UINT_PTR ucb )
		{
		__try { memset( p, 0, cb ); }
		__except( EXCEPTION_EXECUTE_HANDLER ) { return( FALSE ); }
		return( TRUE );
		} 

   In a number of cases the code is called as 
   isXXXPtr( ptr, sizeof( ptrObject ) ), which causes warnings about 
   constant expressions, to avoid this we define a separate version 
   isXXXPtrConst() that avoids the size check.
   
   Under Unix we could in theory check against _etext but this is too 
   unreliable to use, with shared libraries the single shared image can be 
   mapped pretty much anywhere into the process' address space and there can 
   be multiple _etext's present, one per shared library, it fails with 
   SELinux (which is something you'd expect to see used in combination with 
   code that's been carefully written to do things like perform pointer 
   checking), and who knows what it'll do in combination with different 
   approaches to ASLR.  Because of its high level of nonportability (even on 
   the same system it can break depending on whether something like SELinux 
   is enabled or not) it's too dangerous to enable its use */

#define isValidPointer( ptr )	( ( uintptr_t ) ( ptr ) > 0x0FFFF )

#if defined( __WIN32__ ) || defined( __WINCE__ )
  /* The use of code analysis complicates the pointer-checking macros
	 because they read memory that's uninitialised at that point.  This is
	 fine because we're only checking for readability/writeability, but the
	 analyser doesn't know this and flags it as an error.  To avoid this,
	 we remove the read/write calls when running the analyser */
  #ifdef _PREFAST_
	#define isReadPtr( ptr, size )	( isValidPointer( ptr ) )
	#define isWritePtr( ptr, size )	( isValidPointer( ptr ) )
	#define isReadPtrDynamic( ptr, size ) \
									( isValidPointer( ptr ) && ( size ) > 0 )
	#define isWritePtrDynamic( ptr, size ) \
									( isValidPointer( ptr ) && ( size ) > 0 )
  #else
	#define isReadPtr( ptr, size )	( isValidPointer( ptr ) && \
									  !IsBadReadPtr( ( ptr ), ( size ) ) )
	#define isWritePtr( ptr, size )	( isValidPointer( ptr ) && \
									  !IsBadWritePtr( ( ptr ), ( size ) ) )
	#define isReadPtrDynamic( ptr, size ) \
									( isValidPointer( ptr ) && ( size ) > 0 && \
									  !IsBadReadPtr( ( ptr ), ( size ) ) )
	#define isWritePtrDynamic( ptr, size ) \
									( isValidPointer( ptr ) && ( size ) > 0 && \
									  !IsBadWritePtr( ( ptr ), ( size ) ) )
  #endif /* _PREFAST_ */
#elif defined( __UNIX__ ) && 0		/* See comment above */
  extern int _etext;

  #define isReadPtr( ptr, size )	( isValidPointer( ptr ) && \
									  ( void * ) ( ptr ) > ( void * ) &_etext )
  #define isWritePtr( ptr, size )	( isValidPointer( ptr ) && \
									  ( void * ) ( ptr ) > ( void * ) &_etext )
  #define isReadPtrDynamic( ptr, size )	\
									( isValidPointer( ptr ) && \
									  ( void * ) ( ptr ) > ( void * ) &_etext && \
									  ( size ) > 0 )
  #define isWritePtrDynamic( ptr, size ) \
									( isValidPointer( ptr ) && \
									  ( void * ) ( ptr ) > ( void * ) &_etext && \
									  ( size ) > 0 )
#else
  #define isReadPtr( ptr, type )	( isValidPointer( ptr ) )
  #define isWritePtr( ptr, type )	( isValidPointer( ptr ) )
  #define isReadPtrDynamic( ptr, size )	\
									( isValidPointer( ptr ) && ( size ) > 0 )
  #define isWritePtrDynamic( ptr, size ) \
									( isValidPointer( ptr ) && ( size ) > 0 )
#endif /* Pointer check macros */

/****************************************************************************
*																			*
*								Loop Bounds Checks							*
*																			*
****************************************************************************/

/* Loop bounds used when a more specific constant upper bound isn't 
   available.  The following bounds on loop iterations apply:

	FAILSAFE_SMALL: Expect 1 but can have a few more.
	FAILSAFE_MED: Expect 10-20 but can have a few more.
	FAILSAFE_LARGE: Expect many, but not too many.

  In addition to these values there's a special value 
  FAILSAFE_ITERATIONS_MAX which is equivalent to the ASN.1 (1...MAX) 
  construct in setting an upper bound on loop iterations without necessarily 
  setting any specific limit:

	FAILSAFE_MAX: A value that's unlikely to be reached during normal 
				  operation, but that also won't result in an excessive 
				  stall if it's exceeded */

#define FAILSAFE_ITERATIONS_SMALL	10
#define FAILSAFE_ITERATIONS_MED		50
#define FAILSAFE_ITERATIONS_LARGE	1000
#define FAILSAFE_ITERATIONS_MAX		min( INT_MAX, 100000L )

/* Pseudo-constants used for array bounds-checking.  These provide a more
   precise limit than the FAILSAFE_ITERATIONS_xxx values above.  We subtract
   one from the total count because static arrays are always overallocated 
   with two extra dummy elements at the end */

#define FAILSAFE_ARRAYSIZE( array, elementType ) \
		( ( sizeof( array ) / sizeof( elementType ) ) - 1 )

/* In order to provide its availability guarantees, all loops in cryptlib 
   are statically bounded and double-indexed in case of a fault in the
   primary loop index.  In addition the loops are indexed in opposite
   directions to prevent compilers from combining the two loop index 
   variables into one.  So instead of:

	for( i = 0; i < max; i++ )

   the loop construct used is:

	for( i = 0,		_iterationCount = FAILSAFE_ITERATIONS_MED;
		 i < max && _iterationCount > 0;
		 i++,		_iterationCount-- )

   (in practice the static bounds check is performed before the dynamic one).

   In order to hide the resulting complexity and to ensure consistent
   implementation, the overall construct is manged through macros so that
   the above becomes:

	LOOP_MED( i = 0, i < max, i++ )
		{
		<loop body>;
		}
	ENSURES( LOOP_BOUND_OK );

   First we define the loop variables and conditions that we need.  Since we
   can have nested loops, we also define alternative values for a total of 
   up to three levels of nesting */

#define LOOP_ITERATOR				_iterationCount
#define LOOP_BOUND_INIT( value )	_iterationCount = ( value )
#define LOOP_BOUND_CHECK			( _iterationCount > 0 )
#define LOOP_BOUND_INC				_iterationCount--
#define LOOP_BOUND_OK				LOOP_BOUND_CHECK

#define LOOP_ITERATOR_ALT			_innerIterationCount
#define LOOP_BOUND_INIT_ALT( value ) _innerIterationCount = ( value )
#define LOOP_BOUND_CHECK_ALT		( _innerIterationCount > 0 )
#define LOOP_BOUND_INC_ALT			_innerIterationCount--
#define LOOP_BOUND_OK_ALT			LOOP_BOUND_CHECK_ALT

#define LOOP_ITERATOR_ALT2			_innerInnerIterationCount
#define LOOP_BOUND_INIT_ALT2( value ) _innerInnerIterationCount = ( value )
#define LOOP_BOUND_CHECK_ALT2		( _innerInnerIterationCount > 0 )
#define LOOP_BOUND_INC_ALT2			_innerInnerIterationCount--
#define LOOP_BOUND_OK_ALT2			LOOP_BOUND_CHECK_ALT2

/* With the above we can now create the building blocks for the loops, the
   basic universal form and then more specific forms built on top of that */

#define LOOP_EXT( a, b, c, bound ) \
		for( LOOP_BOUND_INIT( bound ), ( a ); \
			 LOOP_BOUND_CHECK && ( b ); \
			 LOOP_BOUND_INC, ( c ) )
#define LOOP_EXT_ALT( a, b, c, bound ) \
		for( LOOP_BOUND_INIT_ALT( bound ), ( a ); \
			 LOOP_BOUND_CHECK_ALT && ( b ); \
			 LOOP_BOUND_INC_ALT, ( c ) )
#define LOOP_EXT_ALT2( a, b, c, bound ) \
		for( LOOP_BOUND_INIT_ALT2( bound ), ( a ); \
			 LOOP_BOUND_CHECK_ALT2 && ( b ); \
			 LOOP_BOUND_INC_ALT2, ( c ) )

#define LOOP_SMALL( a, b, c )	LOOP_EXT( a, b, c, FAILSAFE_ITERATIONS_SMALL )
#define LOOP_MED( a, b, c )		LOOP_EXT( a, b, c, FAILSAFE_ITERATIONS_MED )
#define LOOP_LARGE( a, b, c )	LOOP_EXT( a, b, c, FAILSAFE_ITERATIONS_LARGE )
#define LOOP_MAX( a, b, c )		LOOP_EXT( a, b, c, FAILSAFE_ITERATIONS_MAX )

#define LOOP_SMALL_ALT( a, b, c ) \
								LOOP_EXT_ALT( a, b, c, FAILSAFE_ITERATIONS_SMALL )
#define LOOP_MED_ALT( a, b, c ) \
								LOOP_EXT_ALT( a, b, c, FAILSAFE_ITERATIONS_MED )
#define LOOP_LARGE_ALT( a, b, c ) \
								LOOP_EXT_ALT( a, b, c, FAILSAFE_ITERATIONS_LARGE )
#define LOOP_MAX_ALT( a, b, c ) \
								LOOP_EXT_ALT( a, b, c, FAILSAFE_ITERATIONS_MAX )

#define LOOP_LARGE_ALT2( a, b, c ) \
								LOOP_EXT_ALT2( a, b, c, FAILSAFE_ITERATIONS_LARGE )

/* Finally, we need a few specialised subtypes to handle constructs like:

	for( ; i < max ; i++ )

   or even:

	for( ; i < max ; )

   (used when the loop variable is initialised dynamically and the increment
   is part of a conditional in the loop body) */

#define LOOP_EXT_INITCHECK( a, b, bound ) \
		for( LOOP_BOUND_INIT( bound ), ( a ); \
			 LOOP_BOUND_CHECK && ( b ); \
			 LOOP_BOUND_INC )
#define LOOP_EXT_INITINC( a, c, bound ) \
		for( LOOP_BOUND_INIT( bound ), ( a ); \
			 LOOP_BOUND_CHECK; \
			 LOOP_BOUND_INC, ( c ) )
#define LOOP_EXT_CHECK( b, bound ) \
		for( LOOP_BOUND_INIT( bound ); \
			 LOOP_BOUND_CHECK && ( b ); \
			 LOOP_BOUND_INC )
#define LOOP_EXT_CHECKINC( b, c, bound ) \
		for( LOOP_BOUND_INIT( bound ); \
			 LOOP_BOUND_CHECK && ( b ); \
			 LOOP_BOUND_INC, ( c ) )

#define LOOP_EXT_INITCHECK_ALT( a, b, bound ) \
		for( LOOP_BOUND_INIT_ALT( bound ), ( a ); \
			 LOOP_BOUND_CHECK_ALT && ( b ); \
			 LOOP_BOUND_INC_ALT )
#define LOOP_EXT_CHECK_ALT( b, bound ) \
		for( LOOP_BOUND_INIT_ALT( bound ); \
			 LOOP_BOUND_CHECK_ALT && ( b ); \
			 LOOP_BOUND_INC_ALT )
#define LOOP_EXT_CHECKINC_ALT( b, c, bound ) \
		for( LOOP_BOUND_INIT_ALT( bound ); \
			 LOOP_BOUND_CHECK_ALT && ( b ); \
			 LOOP_BOUND_INC_ALT, ( c ) )

#define LOOP_SMALL_INITCHECK( a, b ) \
								LOOP_EXT_INITCHECK( a, b, FAILSAFE_ITERATIONS_SMALL ) 
#define LOOP_MED_INITCHECK( a, b ) \
								LOOP_EXT_INITCHECK( a, b, FAILSAFE_ITERATIONS_MED ) 
#define LOOP_LARGE_INITCHECK( a, b ) \
								LOOP_EXT_INITCHECK( a, b, FAILSAFE_ITERATIONS_LARGE ) 
#define LOOP_MAX_INITCHECK( a, b ) \
								LOOP_EXT_INITCHECK( a, b, FAILSAFE_ITERATIONS_MAX ) 

#define LOOP_MED_INITINC( a, c ) \
								LOOP_EXT_INITINC( a, c, FAILSAFE_ITERATIONS_MED )

#define LOOP_SMALL_CHECK( b )	LOOP_EXT_CHECK( b, FAILSAFE_ITERATIONS_SMALL )
#define LOOP_MED_CHECK( b )		LOOP_EXT_CHECK( b, FAILSAFE_ITERATIONS_MED )
#define LOOP_LARGE_CHECK( b )	LOOP_EXT_CHECK( b, FAILSAFE_ITERATIONS_LARGE )
#define LOOP_MAX_CHECK( b )		LOOP_EXT_CHECK( b, FAILSAFE_ITERATIONS_MAX )

#define LOOP_SMALL_CHECKINC( b, c ) \
								LOOP_EXT_CHECKINC( b, c, FAILSAFE_ITERATIONS_SMALL )
#define LOOP_MED_CHECKINC( b, c ) \
								LOOP_EXT_CHECKINC( b, c, FAILSAFE_ITERATIONS_MED )
#define LOOP_LARGE_CHECKINC( b, c ) \
								LOOP_EXT_CHECKINC( b, c, FAILSAFE_ITERATIONS_LARGE )
#define LOOP_MAX_CHECKINC( b, c ) \
								LOOP_EXT_CHECKINC( b, c, FAILSAFE_ITERATIONS_MAX )

#define LOOP_MED_CHECK_ALT( b ) \
								LOOP_EXT_CHECK_ALT( b, FAILSAFE_ITERATIONS_MED )
#define LOOP_LARGE_CHECK_ALT( b ) \
								LOOP_EXT_CHECK_ALT( b, FAILSAFE_ITERATIONS_LARGE )
#define LOOP_MAX_CHECK_ALT( b )	LOOP_EXT_CHECK_ALT( b, FAILSAFE_ITERATIONS_MAX )

#define LOOP_MED_INITCHECK_ALT( a, b ) \
								LOOP_EXT_INITCHECK_ALT( a, b, FAILSAFE_ITERATIONS_MED ) 
#define LOOP_MAX_INITCHECK_ALT( a, b ) \
								LOOP_EXT_INITCHECK_ALT( a, b, FAILSAFE_ITERATIONS_LARGE ) 

#define LOOP_MED_CHECKINC_ALT( b, c ) \
								LOOP_EXT_CHECKINC_ALT( b, c, FAILSAFE_ITERATIONS_MED )
#define LOOP_LARGE_CHECKINC_ALT( b, c ) \
								LOOP_EXT_CHECKINC_ALT( b, c, FAILSAFE_ITERATIONS_LARGE )
#define LOOP_MAX_CHECKINC_ALT( b, c ) \
								LOOP_EXT_CHECKINC_ALT( b, c, FAILSAFE_ITERATIONS_MAX )

/****************************************************************************
*																			*
*								Safe Pointers								*
*																			*
****************************************************************************/

/* Error-detecting function and data pointers.  We store two copies of the 
   pointer, the value itself and its bitwise inverse.  If on retrieving them 
   their XOR isn't all-ones then one of the values has been corrupted and 
   the pointer isn't safe to dereference.  

   A linked list of items using safe pointers looks as follows:

	DATAPTR --> listItem {
					...
					DATAPTR --> listItem {
					...				...
					}				DATAPTR --> listItem {
									}				...
													DATAPTR --> NULL
													}

   Walking down a list of safe pointers works as follows:

	LOOP_LARGE( listPtr = DATAPTR_GET( listHead ),
				listPtr != NULL,
				listPtr = DATAPTR_GET( listPtr->next ) )

   When traversing a list (meaning walking from one link to the next looking
   for a particular entry), the access pattern is as above.  When processing
   entries (meaning working with the elements of listPtr), the pattern 
   becomes:

	LOOP_LARGE( listPtr = DATAPTR_GET( listHead ),
				listPtr != NULL,
				listPtr = DATAPTR_GET( listPtr->next ) )
		{
		REQUIRES( sanityCheckListEntry( listPtr ) );

		body;
		}

   Sometimes we need to have both a LIST_ITEM and the DATAPTR that refers 
   to it available.  This occurs when we're working with a mixture of 
   internal-access (LIST_ITEM) and external-access (DATAPTR) functions.  
   The following loop structure allows for this dual access:

	LOOP_LARGE( ( listCursor = listHead,
				  listPtr = DATAPTR_GET( listHead ) ),
				listPtr != NULL,
				( listCursor = listPtr->next,
				  listPtr = DATAPTR_GET( listPtr->next ) ) )

   Dealing with some types of loops is now especially difficult because a 
   pointer can become NULL even after it's been checked for being non-NULL.  
   To see how this can happen, consider a loop to find the end of a linked 
   list:

	LOOP( listPtr = DATAPTR_GET( list ),
		  DATAPTR_ISSET( listPtr->next ),
		  listPtr = DATAPTR_GET( listPtr->next ) )

   Since DATAPTR_GET() can return NULL on a fault, the DATAPTR_ISSET() check
   works as a loop condition check but doesn't guarantee that listPtr is 
   non-NULL in the loop body.  To see how this happens, consider the C loop 
   structure:

	for( init; check; increment )
		body

   This is evaluated as:

	init;
	while( check )
		{
		body;
	continue_label:
		increment;
		}

  So the above loop will be evaluated as:

	listPtr = DATAPTR_GET( list ),
	while( DATAPTR_ISSET( listPtr->next ) )
		{
		body;
		listPtr = DATAPTR_GET( listPtr->next );
		}

   If the DATAPTR_GET() in the increment returns NULL then the check
   DATAPTR_ISSET() will dereference a NULL pointer.

   The workaround for this is to rewrite the loop as:

	LOOP( listPtr = DATAPTR_GET( list ),
		  listPtr != NULL && DATAPTR_ISSET( listPtr->next ),
		  listPtr = DATAPTR_GET( listPtr->next ) )
		{
		body;
		}
	ENSURES( LOOP_BOUND_OK );
	ENSURES( listPtr != NULL && DATAPTR_ISNULL( listPtr->next ) );

   An alternative workaround, using a temporary variable, is:

	LOOP( ( prevElementPtr = NULL, listPtr = DATAPTR_GET( list ) ), 
		  listPtr != NULL,
		  ( prevElementPtr = listPtr,
			listPtr = DATAPTR_GET( listPtr->next ) ) )
		{
		body;
		}
	ENSURES( LOOP_BOUND_OK );

   The pattern for fetching an attribute from a function that returns a 
   pointer to the attribute is:

	attributeListPtr = findAttribute( attributeListHead, attributeType );
	ENSURES( attributeListPtr != NULL );
	REQUIRES( sanityCheckAttribute( attributeListPtr ) );

   First, we need to define the safe pointer type, a primary copy consisting 
   of a pointer and the alternative copy as an equivalent-sized integer.  We 
   could alternately store two copies of the same value but that wouldn't 
   detect identical corruption on both values.  We could also mask the value 
   with a secret seed generated at runtime, but that's more useful for 
   preventing pointer-overwriting attacks than detecting corruption, and 
   it's not clear whether that's a real threat.  Finally, we could actually 
   store a triple modular-redundant copy, but if we're trying to deal with 
   that level of corruption then there are likely to be all sorts of other 
   problems as well that we'd need to handle.
   
   Function pointers get a bit more difficult because there's no equivalent 
   to the 'void *' universal data pointer.  The ostensible reason for this
   is that function and data pointers may not be of the same size, an example
   being IA64, which as part of its "totally idiotic calling conventions"
   (Linus) turns pointers into handles.  Since sizeof( data_ptr ) may not be
   the same as sizeof( fn_ptr ), it follows logically (if you're a compiler
   author, who run on their own very special type of logic) that we can't 
   have a 'void *' equivalent for function pointers.  Because of this, we 
   have to declare a generic void-ish function, typedef'd as 
   FNPTR_FUNCTION_TYPE, and then cast every assignment to/from it to avoid 
   compiler warnings.
   
   An additional complication with function pointers is that for the 
   aforementioned idiotic implementations there's no equivalent to a 
   uintptr_t.  However, this doesn't really affect us because we're not 
   storing a function pointer in a uintptr_t, just using it as a check
   value.  In this case the worst that can happen is that a few bits of a
   not-quite-a-pointer implementation of a function pointer don't get
   checked, but on any sane architecture we'll be OK */

#define FNPTR_TYPE				uintptr_t
#define DATAPTR_TYPE			uintptr_t

typedef void ( *FNPTR_FUNCTION_TYPE )( void );

typedef struct { 
	void *dataPtr; 
	uintptr_t dataCheck; 
	} DATAPTR;
typedef struct { 
	FNPTR_FUNCTION_TYPE fnPtr;
	uintptr_t fnCheck; 
	} FNPTR;

/* Initialisers for the safe pointer data.  This is now a scalar so we need 
   both a means of setting it to NULL and a NULL-equivalent value that we
   can use in places where we'd normally use the constant NULL */

#define FNPTR_INIT				{ NULL, ( FNPTR_TYPE ) ~0 }
#define DATAPTR_INIT			{ NULL, ( DATAPTR_TYPE ) ~0 }

extern const DATAPTR DATAPTR_NULL;
extern const FNPTR FNPTR_NULL;

/* Check safe pointers.  There are now three checks that can be performed 
   rather than the usual two NULL/non-NULL ones, since there's a third 
   possibility, "not valid".  The latter is required because pointers are 
   now tri-state, valid and non-NULL, valid and NULL, and invalid, which is 
   reported as NULL by FN/DATAPTR_GET() and so would be indistinguishable 
   from valid and NULL */

#define FNPTR_ISVALID( name ) \
		( ( ( ( FNPTR_TYPE ) ( name.fnPtr ) ) ^ ( name.fnCheck ) ) == ~0 )
#define DATAPTR_ISVALID( name ) \
		( ( ( ( DATAPTR_TYPE ) ( name.dataPtr ) ) ^ ( name.dataCheck ) ) == ~0 )
#define DATAPTR_ISVALID_PTR( name ) \
		( ( ( ( DATAPTR_TYPE ) ( name->dataPtr ) ) ^ ( name->dataCheck ) ) == ~0 )

#define FNPTR_ISSET( name ) \
		( FNPTR_ISVALID( name ) && ( name.fnPtr ) != NULL )
#define DATAPTR_ISSET( name ) \
		( DATAPTR_ISVALID( name ) && ( name.dataPtr ) != NULL )
#define DATAPTR_ISSET_PTR( name ) \
		( DATAPTR_ISVALID_PTR( name ) && ( name->dataPtr ) != NULL )

#define FNPTR_ISNULL( name ) \
		( FNPTR_ISVALID( name ) && ( name.fnPtr ) == NULL )
#define DATAPTR_ISNULL( name ) \
		( DATAPTR_ISVALID( name ) && ( name.dataPtr ) == NULL )
#define DATAPTR_ISNULL_PTR( name ) \
		( DATAPTR_ISVALID_PTR( name ) && ( name->dataPtr ) == NULL )

/* Finally, we need to set and get safe pointers.  The macros are used as:

	DATAPTR ptrStorage;

	FNPTR_SET( ptrStorage, functionAddress );
	DATAPTR_SET( ptrStorage, dataAddress );

	const PTR_TYPE functionPtr = FNPTR_GET( ptrStorage );
	REQUIRES( functionPtr != NULL );
	PTR_TYPE dataPtr = DATAPTR_GET( ptrStorage );
	REQUIRES( dataPtr != NULL ); */

#define FNPTR_GET( name ) \
		( FNPTR_ISVALID( name ) ? ( name.fnPtr ) : NULL )
#define DATAPTR_GET( name ) \
		( DATAPTR_ISVALID( name ) ? ( name.dataPtr ) : NULL )
#define DATAPTR_GET_PTR( name ) \
		( DATAPTR_ISVALID_PTR( name ) ? ( name->dataPtr ) : NULL )

#define FNPTR_SET( name, value ) \
			{ \
			name.fnPtr = ( FNPTR_FUNCTION_TYPE ) value; \
			name.fnCheck = ( ( FNPTR_TYPE ) ( value ) ) ^ ~0; \
			}
#define DATAPTR_SET( name, value ) \
			{ \
			name.dataPtr = value; \
			name.dataCheck = ( ( DATAPTR_TYPE ) ( value ) ) ^ ~0; \
			}
#define DATAPTR_SET_PTR( namePtr, value ) \
			{ \
			namePtr->dataPtr = value; \
			namePtr->dataCheck = ( ( DATAPTR_TYPE ) ( value ) ) ^ ~0; \
			}

/* Finally, since DATAPTRs are now scalar values rather than pointers, we 
   have to replace some operations that work on pointers with macros that
   deal with the use of scalars */

#define DATAPTR_SAME( name1, name2 ) \
		( ( ( name1 ).dataPtr ) == ( ( name2 ).dataPtr ) )

/****************************************************************************
*																			*
*								Safe Bitflags								*
*																			*
****************************************************************************/

/* Safe bitflags.  These are particularly critical because some of them have
   a considerable influence over how objects are used, for example 
   CONTEXT_FLAG_DUMMY (software vs. external hardware context), 
   DEVICE_FLAG_ACTIVE (device has been unlocked via PIN/password and is 
   ready for use), ENVELOPE_FLAG_ISDEENVELOPE (whether the envelope is used 
   for enveloping or de-enveloping) and so on.  In addition since the flags
   are all packed into a single integer value, corruption of that value will
   upset a large range of flags.

   Because of this critical nature we protect flags by making them safe
   objects, in the same way the pointers are protected */

typedef struct {
	int flagValue, flagCheckValue;
	} SAFE_FLAGS;

/* Initialisers for the safe flags.  We need both static and dynamic 
   initialision mechanisms */

#define SAFE_FLAGS_INIT( value )		{ ( value ), ~( value ) }
#define INIT_FLAGS( flags, value ) \
		( flags ).flagValue = ( value ), ( flags ).flagCheckValue = ~( value )

/* Initialise and check safe flags.  The check doubles as a sanity-check,
   so it both verifies that the flags are valid in the sense of not having 
   been corrupted and that they've been set to an allowed value */

#define CHECK_FLAGS( flags, minRange, maxRange ) \
		( ( ( flags ).flagValue ^ ( flags ).flagCheckValue ) == ~0 && \
		  ( ( flags ).flagValue >= ( minRange ) && \
			( flags ).flagValue <= ( maxRange ) ) )

/* Get, set, and clear flags.  The XXX_FLAG() and XXX_FLAGS() operations do 
   the same thing, so one is aliased to the other.  The macros are used as:

	SAFE_FLAGS flags;
	
	INIT_FLAGS( flags, XXX_FLAG_NONE );
	CHECK_FLAGS( flags, XXX_FLAG_NONE, XXX_FLAG_MAX );
	
	SET_FLAG( flags, XXX_FLAG_YYY );
	if( TEST_FLAG( flags, XXX_FLAG_YYY )
		do_something;
	CLEAR_FLAG( flags, XXX_FLAG_YYY ); */

#define GET_FLAG( flags, value ) \
		( ( flags ).flagValue & ( value ) )
#define SET_FLAG( flags, value ) \
		( flags ).flagValue |= ( value ), ( flags ).flagCheckValue &= ~( value )
#define CLEAR_FLAG( flags, value ) \
		( flags ).flagValue &= ~( value ), ( flags ).flagCheckValue |= ( value )
#define GET_FLAGS		GET_FLAG
#define SET_FLAGS		SET_FLAG
#define CLEAR_FLAGS		CLEAR_FLAG

/* Test a flag.  Unlike the other macros there are two distinct versions of 
   this, TEST_FLAG() checks that a single flag value (or one of a set of 
   values) is set, TEST_FLAGS() checks that all specified flags are set */

#define TEST_FLAG		GET_FLAG
#define TEST_FLAGS( flags, mask, reqFlags ) \
		( GET_FLAG( flags, mask ) == ( reqFlags ) )

/****************************************************************************
*																			*
*								Safe Booleans								*
*																			*
****************************************************************************/

/* Boolean constants.  Since the traditional TRUE = 1, FALSE = 0 only has a 
   single-bit difference between the two and it's going to be used to decide
   things like "access authorised" or "cryptographic verification succeeded",
   we define our own value for TRUE that minimises the chances of a simple
   fault converting one value to another.  In addition we explicitly check
   for equality to TRUE rather than just "is non-zero".

   The bit pattern in the TRUE value is chosen to minimise the chances of an
   SEU or similar fault flipping the value into something else that looks 
   valid.  The bit pattern is:

	0000 0000 1111 1111 0011 0011 1100 1100 || \
	  0	   0	F	 F	  3	   3	C	 C

	0000 1111 0011 1100 0101 0110 1001 1111
	  0	   F	3	 C	  5	   6	9	 F

   with the more important patterns at the LSB end, so we use the best
   subset of patterns no matter what the word size is */

#ifdef TRUE
  #undef TRUE
#endif /* TRUE */
#if INT_MAX > 0xFFFFFFFFL
  #define TRUE			0x00FF33CC0F3C569F
#elif INT_MAX > 0xFFFF
  #define TRUE			0x0F3C569F
#else
  #define TRUE			0x569F
#endif /* System-specific word size */
#if defined( _MSC_VER ) && VC_GE_2010( _MSC_VER )
  /* VC warns about #if FALSE vs. #ifdef FALSE, since FALSE == 0 */
  #pragma warning( push )
  #pragma warning( disable : 4574 )
#endif /* VS 2010 and above */
#ifdef FALSE
  #if FALSE != 0
	#error Value of FALSE is nonzero, this isnt a boolean FALSE value.
  #endif /* FALSE sanity check */
#else
  #define FALSE			0
#endif /* FALSE */
#if defined( _MSC_VER ) && VC_GE_2010( _MSC_VER )
  #pragma warning( pop )
#endif /* VS 2010 and above */

/* The fault-detecting value of TRUE is OK for internal use, but for 
   external use we still have to use TRUE = 1, for which we define an
   alternative constant to make it explicit that this is the external-
   use TRUE */

#define TRUE_ALT		1

/* Error-detecting boolean variables, used for critical values where we 
   don't want to risk a single bit-flip converting a value from one to the
   other.  In this case we also define HA_FALSE to an SEU-immune data value
   rather than allowing it to be all zeroes.
   
   We also mix in an additional value, currently just set to the constant
   HA_CONST, to deal with data-injection attacks in which an attacker tries
   to set a boolean flag to a particular value.   In practice this should
   be some unpredictable value set at runtime, but for now it's just a 
   no-op placeholder */

#define SAFE_BOOL_TRUE		TRUE
#if INT_MAX > 0xFFFFFFFFL
  #define SAFE_BOOL_FALSE	0x3300CCFF0FC3F596
#elif INT_MAX > 0xFFFF
  #define SAFE_BOOL_FALSE	0x0FC3F596
#else
  #define SAFE_BOOL_FALSE	0xF596
#endif /* System-specific word size */
#define SAFE_BOOL_CONST		0

typedef struct {
		int value1, value2;
		} SAFE_BOOLEAN;

#define BOOL_SET( name ) \
		{ \
		( name )->value1 = SAFE_BOOL_TRUE; \
		( name )->value2 = SAFE_BOOL_TRUE ^ SAFE_BOOL_CONST; \
		}
#define BOOL_CLEAR( name ) \
		{ \
		( name )->value1 = SAFE_BOOL_FALSE; \
		( name )->value2 = ~SAFE_BOOL_FALSE ^ SAFE_BOOL_CONST; \
		}

#define BOOL_ISSET( name )		( ( ( name )->value1 ^ \
									( name )->value2 ^ SAFE_BOOL_CONST ) == 0 )
#define BOOL_ISCLEAR( name )	( ( ( name )->value1 ^ \
									( name )->value2 ^ SAFE_BOOL_CONST ) == ~0 )
#define BOOL_ISVALID( name )	( BOOL_ISSET( name ) || BOOL_ISCLEAR( name ) )

#endif /* _SAFETY_DEFINED */
