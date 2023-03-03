#pragma once

namespace memory
{

class Heap
{
public:
	static bool		Create();
	static void		Destroy();

	static void*	Alloc( size_t size );
	static void		Free( void* p );

private:
	static const UINT64 MaxHeapSize = 64 * 1024 * 1024; // 64Mb
	static HANDLE	sm_heap;
	static UINT64	sm_heapAllocationsTotal;
#ifdef _DEBUG
	static UINT64	sm_maxHeapUsed;
#endif
};

} // namespace memory
#ifdef __cplusplus
extern "C++"
{

	void* __CRTDECL operator new( size_t size );
	void* __CRTDECL operator new[]( size_t size );
	void __CRTDECL operator delete( void* p );
	void __CRTDECL operator delete( void* p, size_t size );
}
#endif