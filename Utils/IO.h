#pragma once

namespace io
{

void FormatString( CHAR* const message, UINT maxChars, const CHAR* const format, ... );
void FormatStringAppend( CHAR* const message, UINT maxChars, const CHAR* const format, ... );
void OutputMessage( const CHAR* const message, ... );

} // namespace io