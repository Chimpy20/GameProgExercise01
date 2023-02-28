#pragma once

namespace utils
{
namespace file
{

BOOL InitialiseFileSystem();
HANDLE GetFileData( const char* const filename, void** outData, DWORD* size );
void CloseFile( const HANDLE fileHandle );
BOOL ShutdownFileSystem();

} // namespace file
} // namespace utils