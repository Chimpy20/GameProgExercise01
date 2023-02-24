#include "GameProgExercise01.h"
#include "Utils\File.h"

namespace utils
{

namespace file
{

static const UINT32 BufferSize = 1024 * 1024;
static unsigned char g_tempReadBuffer[ BufferSize ];


HANDLE GetFileData( const char* const filename, void** outData, DWORD* size )
{
	HANDLE fileHandle = CreateFileA( filename, GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL );

	if( fileHandle != INVALID_HANDLE_VALUE )
	{
		DWORD fileSize = GetFileSize( fileHandle, NULL );
		ASSERT( fileSize <= BufferSize, "File too large for buffer.\n" );
		if( fileSize > BufferSize )
		{
			fileSize = BufferSize;
		}
		DWORD bytesRead = 0;
		bool result = ReadFile( fileHandle, g_tempReadBuffer, fileSize, &bytesRead, NULL );
		ASSERT( result, "Unable to read from file %s.\n", filename );
		if( result )
		{
			*outData = g_tempReadBuffer;
			*size = fileSize;
		}
	}

	return fileHandle;
}

void CloseFile( const HANDLE fileHandle )
{
	CloseHandle( fileHandle );
}

} // namespace file

} // namespace utils