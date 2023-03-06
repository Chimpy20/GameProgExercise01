#include "GameProgExercise01.h"
#include "crtfuncs.h"

void WINAPI wWinMainCRTStartup()
{
	STARTUPINFO				startupInfo = { sizeof( STARTUPINFO ),0 };

	GetStartupInfo( &startupInfo );

	WCHAR* commandLine = GetCommandLine();

	// Skip past program name (first token in command line).
	if( *commandLine == '"' ) // check for and handle quoted program name
	{
		// Scan, and skip over, subsequent characters until another
		// double-quote or a null is encountered
		while( *commandLine && ( *commandLine != '"' ) )
			commandLine++;

		// If we stopped on a double-quote (usual case), skip over it.
		if( *commandLine == '"' )
			commandLine++;
	}
	else
	{
		// First token wasn't a quote
		while( *commandLine > ' ' )
			commandLine++;
	}

	// Skip past any white space preceeding the second token.
	while( *commandLine && ( *commandLine <= ' ' ) )
		commandLine++;

	int result = wWinMain( GetModuleHandle( NULL ), 0, commandLine, startupInfo.dwFlags & STARTF_USESHOWWINDOW
		? startupInfo.wShowWindow : SW_SHOWDEFAULT );

	ExitProcess( result );
}

#ifdef __cplusplus
extern "C"
{
#pragma function(memset)
	void* __cdecl memset( _Out_writes_bytes_all_( count ) void* dest, _In_ int value, _In_ size_t count )
	{
		char* bytes = (char*)dest;
		while( count-- )
		{
			*bytes++ = (char)value;
		}
		return dest;
	}

#ifdef _DEBUG
	void* __cdecl memcpy( _Out_writes_bytes_all_( _Size ) void* _Dst, _In_reads_bytes_( _Size ) void const* _Src, _In_ size_t _Size )
	{
		char* pszDest = (char*)_Dst;
		const char* pszSource = (const char*)_Src;
		if( ( pszDest != NULL ) && ( pszSource != NULL ) )
		{
			while( _Size ) //till cnt
			{
				//Copy byte by byte
				*( pszDest++ ) = *( pszSource++ );
				--_Size;
			}
		}
		return _Dst;
	}
#endif //_DEBUG

	int _fltused = 0;

	int _purecall( void )
	{
		ASSERT( FALSE, "Pure virtual function called.\n" );
		return 0;
	}

	void __cdecl _wassert(
		_In_z_ wchar_t const* _Message,
		_In_z_ wchar_t const* _File,
		_In_   unsigned       _Line )
	{
	}

	typedef void ( *AtExitFunc )( void );
	int __cdecl atexit( AtExitFunc f )
	{
		return 0;
	}

	ULONG _tls_index = 0;

	int const StaticNotInitialized = 0;
	int const StaticInitialized = -1;
	int const EpochStart = INT_MIN;

	DWORD const XpTimeout = 100;

	CRITICAL_SECTION _Tss_mutex;
	CONDITION_VARIABLE _Tss_cv;

	int _Init_global_epoch = EpochStart;
	__declspec( thread ) int _Init_thread_epoch = EpochStart;

	void __cdecl _Init_thread_lock( void )
	{
		EnterCriticalSection( &_Tss_mutex );
	}

	void __cdecl _Init_thread_unlock( void )
	{
		LeaveCriticalSection( &_Tss_mutex );
	}

	void __cdecl _Init_thread_wait( DWORD const TimeOut )
	{
		SleepConditionVariableCS( &_Tss_cv, &_Tss_mutex, TimeOut );


		_Init_thread_lock();
		WaitForSingleObjectEx( 0, TimeOut, FALSE );
		_Init_thread_unlock();
	}

	void __cdecl _Init_thread_notify( void )
	{
		WakeAllConditionVariable( &_Tss_cv );
	}

	void __cdecl _Init_thread_header( int* const Static )
	{
		_Init_thread_lock();

		if( *Static == StaticNotInitialized )

		{
			*Static = StaticInitialized;
		}
		else
		{
			//NOTE: Fix for WinXP....
			_Init_thread_wait( XpTimeout );
			while( *Static == StaticInitialized )
			{
				if( *Static == StaticNotInitialized )
				{
					*Static = StaticInitialized;
					_Init_thread_unlock();
				}
			}

			_Init_thread_epoch = _Init_global_epoch;
		}

		_Init_thread_unlock();
	}

	void __cdecl _Init_thread_footer( int* const Static )
	{
		_Init_thread_lock();
		{
			++_Init_global_epoch;
			*Static = _Init_global_epoch;
		}
		_Init_thread_unlock();

		_Init_thread_notify();
	}

	void __cdecl _Init_thread_abort( int* const Static )
	{
		_Init_thread_lock();
		{
			*Static = StaticNotInitialized;
		}
		_Init_thread_unlock();

		_Init_thread_notify();
	}
}

#endif //__cplusplus
