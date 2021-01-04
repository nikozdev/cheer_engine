#include <support/ch_memory.h>

#include <assert.h>
#include <iostream>

void* operator new(size_t size)
{
	void* ptr = malloc(size);
	if (ptr == NULL)
	{
		throw std::bad_alloc();
	}
	MEM::GlobalAllocator::get().bytesAllocated += size;
	MEM::GlobalAllocator::get().blocksAllocated++;
	return ptr;
}

void operator delete(void* memory, size_t size)
{
	MEM::GlobalAllocator::get().bytesAllocated -= size;
	MEM::GlobalAllocator::get().blocksAllocated--;
	free(memory);
}
/*
void* operator new[](size_t size)
{
	void* first = malloc(size);
	return first;
}

void operator delete[](void* memory, size_t size)
{

}*/
/*
void* __CRTDECL operator new(size_t _Size, _Writable_bytes_(_Size) void* _Where) noexcept
{
	MEM::GlobalAllocator::get().bytesAllocated += _Size;
	MEM::GlobalAllocator::get().blocksAllocated++;
	(void)_Size;
	return _Where;
}*/

/// Global Memory Allocator
/// Uses standard new and delete for memory processing
void* MEM::GlobalAllocator::allocate(size_t size)
{
	//printf("MEM::G_ALLOCATOR::ALLOCATE:: allocated %d bytes in the heap.\n",
	//	static_cast<int>(size));
	return ::operator new(size);
}

void MEM::GlobalAllocator::deallocate(void* ptr, size_t size)
{
	assert(ptr != nullptr);
	//printf("MEM::G_ALLOCATOR::DEALLOCATE: dellocated %d bytes from the heap.\n",
	//	static_cast<int>(size));
	::operator delete(ptr, size);
}

void* MEM::GlobalAllocator::reallocate(void* ptr, size_t new_size, size_t old_size)
{
	size_t bytesToCopy = old_size;
	if (new_size < bytesToCopy)
	{
		bytesToCopy = new_size;
	}
	void* newPtr = allocate(new_size);
	memcpy(newPtr, ptr, bytesToCopy);
	deallocate(ptr, old_size);
	return newPtr;
}

void* MEM::GlobalAllocator::l_alloc(void* user_data, void* ptr,
	size_t old_size, size_t new_size)
{
	MEM::GlobalAllocator* pool = static_cast<MEM::GlobalAllocator*>(user_data);

	if (new_size == 0)
	{//We want it take 0 size of bytes
		if (ptr != nullptr)
		{//There is no memory and pointer - do nothing
			pool->deallocate(ptr, old_size);
		}
		return NULL;
	}
	else
	{//Required new size isn't zero
		if (ptr == nullptr)
		{//We have empty pointer, we have required size... ALLOCATE
			return pool->allocate(new_size);
		}
		else
		{//We have a non-empty pointer and new_size.
			return pool->reallocate(ptr, old_size, new_size);
		}
	}
}

void MEM::GlobalAllocator::printMemoryUsage()
{
	std::cout << "MEM::G_ALLOCATOR::CURRENT_USAGE: " <<
		bytesAllocated << " bytes; " << blocksAllocated << " blocks;" << std::endl;
}

///Memory allocation arena
///Constructor takes the bounds of a char array - pool of bytes
MEM::ArenaAllocator::ArenaAllocator(void* begin, void* end) :
	m_begin(begin), m_end(end),
	memBytes(0), memBlocks(0), memBytesTotal(0), memBlocksTotal(0)
{
	reset();
}

void MEM::ArenaAllocator::reset()
{
	m_freeListHead = nullptr;
	m_curr = static_cast<char*>(m_begin);
}

void* MEM::ArenaAllocator::allocate(size_t size)
{
	void* ptr;
	size_t allocBytes = sizeForAlloc(size);
	if ((size <= MIN_BLOCK_SIZE) && m_freeListHead)
	{//We don't need to allocate new memory if we have free blocks in the freeList
		ptr = m_freeListHead; //Get last freed block
		m_freeListHead = m_freeListHead->m_next; //Set next block
		//printf("MEM::ARENA_ALLOCATOR::ALLOCATE: allocated %d bytes from the freeList.\n",
		//	static_cast<int>(size));
		this->memBytes += allocBytes; this->memBlocks++;
		this->memBytesTotal += allocBytes; this->memBlocksTotal++;
	}
	else
	{//There is no free blocks - get from the pool
		/// Allign the allocated memory to ALLIGNMENT = 8 constant : m_curr + (0111 & 1000);
		/// 0000 + 0111 = 0111 -> 0111 & 1000 = 0000 = 0 bit;
		/// 0001 + 0111 = 1000 -> 1000 & 1000 = 1000 = 8 bit;
		/// 0111 + 0111 = 1110 - > 1110 & 1000 = 1000 = 8 bit;
		/// It's just a pointer to alligned memory adress; Only works with power of 2 allignment
		m_curr = (char*)( ((uintptr_t)m_curr + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1) );
		if (m_curr + allocBytes <= m_end)
		{//We are still inside of memory
			ptr = m_curr;
			m_curr += allocBytes;
			//printf("MEM::ARENA_ALLOCATOR::ALLOCATE: allocated %d bytes.\n",
			//	static_cast<int>(allocBytes));
			this->memBytes += allocBytes; this->memBlocks++;
			this->memBytesTotal += allocBytes; this->memBlocksTotal++;
		}
		else
		{//Here is not enough space - call default allocation
			ptr = MEM::GlobalAllocator::get().allocate(size);
			this->memBytesTotal += size;
			this->memBlocksTotal++;
		}
	}
	return ptr;
}

void MEM::ArenaAllocator::deallocate(void* ptr, size_t size)
{
	assert(ptr != nullptr); //Cannot deallocate nullptr
	size_t allocBytes = sizeForAlloc(size);
	assert(allocBytes >= MIN_BLOCK_SIZE);
	if (ptr >= m_begin && ptr <= m_end)
	{//Inside the memory pool
		FreeList* newHead = static_cast<FreeList*>(ptr);
		newHead->m_next = m_freeListHead;
		m_freeListHead = newHead;
		//printf("MEM::ARENA_ALLOCATOR::DEALLOCATE: dellocated %d bytes from the freeList.\n",
		//	static_cast<int>(allocBytes));
		this->memBytes -= allocBytes; this->memBlocks--;
		this->memBytesTotal -= allocBytes; this->memBlocksTotal--;
	}
	else
	{//Ouside of memory
		MEM::GlobalAllocator::get().deallocate(ptr, size);
		this->memBytesTotal -= size;
		this->memBlocksTotal--;
	}
}

void* MEM::ArenaAllocator::reallocate(void* ptr,
	size_t old_size, size_t new_size)
{//Allocate new minimal required space - copy it - deallocate old
	size_t bytesToCopy = old_size;
	if (new_size < bytesToCopy)
	{
		bytesToCopy = new_size;
	}
	void* newPtr = allocate(new_size);
	memcpy(newPtr, ptr, bytesToCopy);	//Copy N bytes from old ptr to new
	deallocate(ptr, old_size);
	return newPtr;
}

void MEM::ArenaAllocator::printMemoryUsage()
{
	std::cout << "MEM::ARENA_ALLOCATOR::CURRENT_USAGE: " << std::endl <<
		"Position in the pool: " << (uintptr_t)m_curr - (uintptr_t)m_begin << std::endl <<
		memBytes << " bytes; " << memBlocks << " blocks;" << std::endl <<
		memBytesTotal << " bytes total; " << memBlocksTotal << " blocks total;" << std::endl;
}

size_t MEM::ArenaAllocator::sizeForAlloc(size_t size)
{//Get minimal block of memory
	if (size < MIN_BLOCK_SIZE)
	{
		size = MIN_BLOCK_SIZE;
	}
	return size;
}
//Lua state allocation function
void* MEM::ArenaAllocator::l_alloc(void* user_data, void* ptr,
	size_t old_size, size_t new_size)
{
	MEM::ArenaAllocator* pool = static_cast<MEM::ArenaAllocator*>(user_data);

	if (new_size == 0)
	{//We want it take 0 size of bytes
		if (ptr != nullptr)
		{//There is no memory and pointer - do nothing
			pool->deallocate(ptr, old_size);
		}
		return NULL;
	}
	else
	{//Required new size isn't zero
		if (ptr == nullptr)
		{//We have empty pointer, we have required size... ALLOCATE
			return pool->allocate(new_size);
		}
		else
		{//We have a non-empty pointer and new_size.
			return pool->reallocate(ptr, old_size, new_size);
		}
	}
}