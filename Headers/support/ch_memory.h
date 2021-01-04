#ifndef MEMORY_H
#define MEMORY_H

#include <memory>

namespace MEM
{
	struct FreeList
	{//Use freed memory to store the pointer to the next block
		FreeList* m_next;
	};
	///Global Allocator
	///Brief Allocates in the global heap memory
	struct GlobalAllocator
	{
	private:
		GlobalAllocator()
		{
			blocksAllocated = 0;
			bytesAllocated = 0;
		}
	public:
		size_t blocksAllocated;
		size_t bytesAllocated;
		
		GlobalAllocator(const GlobalAllocator&) = delete;

		static GlobalAllocator& get()
		{
			static GlobalAllocator s_Instance;
			return s_Instance;
		}

		void* allocate(size_t size);
		void deallocate(void* ptr, size_t size);
		void* reallocate(void* ptr, size_t new_size, size_t old_size);
		static void* l_alloc(void* user_data, void* ptr,
			size_t old_size, size_t new_size);

		void printMemoryUsage();
	};

	///Memory Allocation Arena
	///Brief Allocates memory from a fixed pool.
	///Aligns all memory to 8 bytes
	///Has a min allocation of 64 bytes
	///Puts all free'd blocks on a free list.
	///Falls back to GlobalAllocator when out of memory.
	struct ArenaAllocator
	{
		//Memory usage counter: how much is packed pool and what is the total space of a state
		size_t memBytes, memBlocks, memBytesTotal, memBlocksTotal;
		//m_curr is the adress of the current position in the stack
		void* m_begin;
		void* m_end;
		char* m_curr;
		//The header of freed blocks list: contains all hierarchy of reserved blocks
		FreeList* m_freeListHead;
		//Min size for lua stat is 5-7 KB, 20 with "openlibs"
		static constexpr const int POOL_SIZE = 1024 * 20;
		static constexpr const int ALIGNMENT = 8;
		static constexpr const int MIN_BLOCK_SIZE = ALIGNMENT * 8;

		//Constructor
		ArenaAllocator(void* begin, void* end);
		void reset();
		//Memory functions
		void* allocate(size_t size);
		void deallocate(void* ptr, size_t size);
		void* reallocate(void* ptr, size_t old_size, size_t new_size);

		void printMemoryUsage();

		static size_t sizeForAlloc(size_t size);

		static void* l_alloc(void* user_data, void* ptr,
			size_t old_size, size_t new_size);
	};
}

#endif