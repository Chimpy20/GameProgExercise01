#include "GameProgExercise01.h"
#include "Utils\IO.h"
#include "Utils\printf.h"

namespace io
{

static const UINT MaxMessageSize = 1024;

void FormatString( CHAR* const string, UINT maxChars, const CHAR* const format, ... )
{
	va_list args;
	va_start( args, format );
	vsnprintf_( string, maxChars, format, args );
	va_end( args );
}

void FormatStringAppend( CHAR* const string, UINT maxChars, const CHAR* const format, ... )
{
	UINT length = strnlen_( string, maxChars );
	if( length < maxChars )
	{
		CHAR* const stringWithOffset = string + length;
		maxChars -= length;
		va_list args;
		va_start( args, format );
		vsnprintf_( stringWithOffset, maxChars, format, args );
		va_end( args );
	}
}

void OutputMessage( const CHAR* const message, ... )
{
	CHAR messageBuffer[ MaxMessageSize ];

	va_list args;
	va_start( args, message );
	vsnprintf_( messageBuffer, MaxMessageSize, message, args );
	va_end( args );

	OutputDebugStringA( messageBuffer );
}

}