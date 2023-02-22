#pragma once

namespace file
{

HANDLE GetFileData( const char* const filename, void* outData );
void CloseFile( const HANDLE fileHandle );

}
