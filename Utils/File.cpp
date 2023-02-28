#include "GameProgExercise01.h"
#include "Containers\List.h"
#include "Utils\File.h"

namespace utils
{

namespace file
{

struct FileInfo
{
	FileInfo()
	{
		m_fileHandle = INVALID_HANDLE_VALUE;
		m_size = 0;
		m_data = nullptr;
	}
	HANDLE		m_fileHandle;
	DWORD		m_size;
	void*		m_data;
};

// This can't exist statically since new is overridden, and the list class uses new at creation time.
containers::List< FileInfo >* g_fileInfoList = nullptr;
UINT g_totalSizeOfOpenFiles = 0;

BOOL InitialiseFileSystem()
{
	g_fileInfoList = new containers::List< FileInfo >();
	ASSERT( g_fileInfoList != nullptr, "Unable to create file list.\n" );

	return FALSE;
}

HANDLE GetFileData( const char* const filename, void** outData, DWORD* size )
{
	FileInfo fileInfo;
	fileInfo.m_fileHandle = CreateFileA( filename, GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL );

	if( fileInfo.m_fileHandle != INVALID_HANDLE_VALUE )
	{
		fileInfo.m_size = GetFileSize( fileInfo.m_fileHandle, NULL );
		g_totalSizeOfOpenFiles += fileInfo.m_size;
		DWORD bytesRead = 0;
		fileInfo.m_data = (void*)(new CHAR[ fileInfo.m_size ]);
		ASSERT( fileInfo.m_data != nullptr, "unable to allocate memory of %u bytes for reading file data.\n", fileInfo.m_size );
		bool result = ReadFile( fileInfo.m_fileHandle, fileInfo.m_data, fileInfo.m_size, &bytesRead, NULL );
		ASSERT( result, "Unable to read from file %s.\n", filename );
		ASSERT( bytesRead == fileInfo.m_size, "Mismatch between file size and number of read bytes.\n" );
		if( result )
		{
			*outData = fileInfo.m_data;
			*size = fileInfo.m_size;
			g_fileInfoList->push_back( fileInfo );
		}
		else
		{
			if( fileInfo.m_data != nullptr )
				delete fileInfo.m_data;
		}
	}

	return fileInfo.m_fileHandle;
}

void CloseFile( const HANDLE fileHandle )
{
	containers::List< FileInfo >::iterator itor = g_fileInfoList->begin();
	while( itor != nullptr )
	{
		FileInfo& fileInfo = *itor;
		if( fileInfo.m_fileHandle == fileHandle )
		{
			delete fileInfo.m_data;
			ASSERT( fileInfo.m_size <= g_totalSizeOfOpenFiles, "Closing a file that is bigger than the size of all open files.\n" );
			g_totalSizeOfOpenFiles -= fileInfo.m_size;
			CloseHandle( fileInfo.m_fileHandle );
			g_fileInfoList->erase( itor );
			return;
		}
	}

	DEBUG_MESSAGE( "Unable to find file to close.\n" );
}

BOOL ShutdownFileSystem()
{
	if( g_totalSizeOfOpenFiles != 0 )
	{
		io::OutputMessage( "There are still %u bytes allocated from open files.\n", g_totalSizeOfOpenFiles );
		return TRUE;
	}

	g_fileInfoList->clear();

	delete g_fileInfoList;

	return FALSE;
}

} // namespace file

} // namespace utils