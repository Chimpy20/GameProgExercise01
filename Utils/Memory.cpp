#include "GameProgExercise01.h"
#include "Utils\Memory.h"

namespace memory
{

HANDLE Heap::sm_heap = INVALID_HANDLE_VALUE;
UINT64 Heap::sm_heapAllocationsTotal = 0;
#ifdef _DEBUG
UINT64 Heap::sm_maxHeapUsed = 0;
#endif

bool Heap::Create()
{
	sm_heap = HeapCreate( 0, MaxHeapSize, MaxHeapSize );
	sm_heapAllocationsTotal = 0;
#ifdef _DEBUG
	sm_maxHeapUsed = 0;
#endif
	return( sm_heap != INVALID_HANDLE_VALUE );
}

void Heap::Destroy()
{
	ASSERT( sm_heapAllocationsTotal == 0, "There is still %u bytes allocated on the heap\n", sm_heapAllocationsTotal );
	DEBUG_MESSAGE( "Max heap used was %u bytes\n", sm_maxHeapUsed );
	HeapDestroy( sm_heap );
	sm_heap = INVALID_HANDLE_VALUE;
}

void* Heap::Alloc( size_t size )
{
	ASSERT( sm_heap != INVALID_HANDLE_VALUE, "Heap not created\n" );
	void* const newAllocation = HeapAlloc( sm_heap, 0, size );
	ASSERT( newAllocation != nullptr, "Unable to allocate heap memory of size %u", size );
	sm_heapAllocationsTotal += size;
#ifdef _DEBUG
	if( sm_heapAllocationsTotal > sm_maxHeapUsed )
		sm_maxHeapUsed = sm_heapAllocationsTotal;
#endif
	return newAllocation;
}

void Heap::Free( void* p )
{
	ASSERT( sm_heap != INVALID_HANDLE_VALUE, "Heap not created\n" );
	const size_t memoryFreed = HeapSize( sm_heap, 0, p );
	ASSERT( memoryFreed <= sm_heapAllocationsTotal, "Trying to free more memory than has been allocated\n" );
	sm_heapAllocationsTotal -= memoryFreed;
	HeapFree( sm_heap, 0, p );
}

} // namespace memory


void* operator new( size_t size )
{
	return memory::Heap::Alloc( size );
}

void* operator new[]( size_t size )
{
	return memory::Heap::Alloc( size );
}

void __CRTDECL operator delete( void* p )
{
	memory::Heap::Free( p );
}

void __CRTDECL operator delete( void* p, size_t size )
{
	memory::Heap::Free( p );
}