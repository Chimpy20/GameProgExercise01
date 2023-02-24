#pragma once

#ifdef _DEBUG

#define DEBUG_MESSAGE( message, ... ) \
io::OutputMessage( message, __VA_ARGS__ )

#define ASSERT(cond, msg, ...) \
do \
{ \
	if( !( cond ) ) \
	{ \
		io::OutputMessage( "Assert failure: %s at %s %u\n",  #cond, __FILE__, __LINE__ ); \
		io::OutputMessage( msg, __VA_ARGS__ ); \
		DebugBreak(); \
	} \
} while( 0 )

#define ASSERT_HANDLE( handle ) \
do \
{ \
	if( ( FAILED( handle ) ) ) \
	{ \
		io::OutputMessage( "Assert failure: Bad handle at %s %u\n", __FILE__, __LINE__ ); \
		DebugBreak(); \
	} \
} while( 0 )


#else

#define DEBUG_MESSAGE( message, ... ) \
do { (void)sizeof( message ); } while( 0 )

#define ASSERT( cond, msg, ... ) \
do { (void)sizeof( cond ); } while( 0 )

#define ASSERT_HANDLE( handle ) \
do { (void)sizeof( handle ); } while( 0 )

#endif
