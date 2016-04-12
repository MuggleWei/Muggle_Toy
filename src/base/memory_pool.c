#include "memory_pool.h"
#include <stdlib.h>
#include <string.h>

void MemoryPoolInit(MemoryPool* pool, unsigned int init_capacity, unsigned int block_size)
{
	init_capacity = init_capacity == 0 ? 8 : init_capacity;

	pool->memory_pool_data_bufs = (uintptr_t*)malloc(sizeof(uintptr_t));
	pool->memory_pool_ptr_buf = (uintptr_t*)malloc(sizeof(uintptr_t) * init_capacity);
	pool->memory_pool_data_bufs[0] = (uintptr_t)malloc(block_size * init_capacity);

	pool->alloc_index = pool->free_index = 0;
	pool->capacity = init_capacity;
	pool->used = 0;

	pool->block_size = block_size;
	pool->num_buf = 1;

#ifndef NDEBUG
	pool->peak = 0;
#endif

	uintptr_t ptr_buf = pool->memory_pool_data_bufs[0];
	unsigned int i;
	for (i = 0; i < init_capacity; ++i)
	{
		pool->memory_pool_ptr_buf[i] = (uintptr_t)((char*)ptr_buf + i * block_size);
	}

}
void MemoryPoolDestroy(MemoryPool* pool)
{
	unsigned int i;
	for (i = 0; i<pool->num_buf; ++i)
	{
		free((void*)pool->memory_pool_data_bufs[i]);
	}
	free((void*)pool->memory_pool_data_bufs);
	free((void*)pool->memory_pool_ptr_buf);

	memset(pool, 0, sizeof(MemoryPool));
}
uintptr_t MemoryPoolAlloc(MemoryPool* pool)
{
	if (pool->used == pool->capacity)
	{
		MemoryPoolEnsureSpace(pool, pool->capacity * 2);
	}
	++pool->used;
#ifndef NDEBUG
	if (pool->used > pool->peak)
	{
		pool->peak = pool->used;
	}
#endif

	uintptr_t ret = pool->memory_pool_ptr_buf[pool->alloc_index];
	pool->alloc_index = (pool->alloc_index + 1) % pool->capacity;
	return ret;
}
void MemoryPoolFree(MemoryPool* pool, void* p_data)
{
	pool->memory_pool_ptr_buf[pool->free_index] = (uintptr_t)p_data;
	pool->free_index = (pool->free_index + 1) % pool->capacity;
	--pool->used;
}

void MemoryPoolEnsureSpace(MemoryPool* pool, unsigned int capacity)
{
	// capacity must increase
	if (capacity < pool->capacity)
	{
		return;
	}

	// allocate new data buffer
	unsigned int delta_size = capacity - pool->capacity;
	uintptr_t* new_bufs = (uintptr_t*)malloc(sizeof(uintptr_t) * (pool->num_buf + 1));
	memcpy(new_bufs, pool->memory_pool_data_bufs, sizeof(uintptr_t) * pool->num_buf);
	new_bufs[pool->num_buf] = (uintptr_t)malloc(pool->block_size * delta_size);

	free((void*)pool->memory_pool_data_bufs);
	pool->memory_pool_data_bufs = new_bufs;

	// get alloc and free section
	uintptr_t *free_section1 = NULL, *free_section2 = NULL;
	uintptr_t *alloc_section1 = NULL, *alloc_section2 = NULL;
	unsigned int num_free_section1 = 0, num_free_section2 = 0;
	unsigned int num_alloc_section1 = 0, num_alloc_section2 = 0;

	if (pool->used == pool->capacity)
	{
		/*
		 *                  fm
		 *  [x] [x] [x] [x] [x] [x] [x] [x] [x] [x] [x] [x]
		 */
		MASSERT(pool->alloc_index == pool->free_index);

		free_section1 = (uintptr_t*)&pool->memory_pool_ptr_buf[pool->free_index];
		num_free_section1 = pool->capacity - pool->free_index;

		free_section2 = (uintptr_t*)&pool->memory_pool_ptr_buf[0];
		num_free_section2 = pool->free_index;
	}
	else if (pool->alloc_index > pool->free_index)
	{
		/*
		 *           f           m
		 *  [0] [0] [x] [x] [x] [0] [0] [0] [0] [0] [0] [0]
		 */
		free_section1 = (uintptr_t*)&pool->memory_pool_ptr_buf[pool->free_index];
		num_free_section1 = pool->alloc_index - pool->free_index;

		alloc_section1 = (uintptr_t*)&pool->memory_pool_ptr_buf[pool->alloc_index];
		num_alloc_section1 = pool->capacity - pool->alloc_index;

		alloc_section2 = (uintptr_t*)&pool->memory_pool_ptr_buf[0];
		num_alloc_section2 = pool->free_index;
	}
	else
	{
		/*
		 *           m           f
		 *  [x] [x] [0] [0] [0] [x] [x] [x] [x] [x] [x] [x]
		 *
		 *                      or
		 *          fm
		 *  [0] [0] [0] [0] [0] [0] [0] [0] [0] [0] [0] [0]
		 */
		free_section1 = (uintptr_t*)&pool->memory_pool_ptr_buf[pool->free_index];
		num_free_section1 = pool->capacity - pool->free_index;

		free_section2 = (uintptr_t*)&pool->memory_pool_ptr_buf[0];
		num_free_section2 = pool->alloc_index;

		alloc_section1 = (uintptr_t*)&pool->memory_pool_ptr_buf[pool->alloc_index];
		num_alloc_section1 = pool->free_index - pool->alloc_index;
	}

	// realloc pointer buffer
	uintptr_t* new_ptr_buf = (uintptr_t*)malloc(sizeof(uintptr_t) * capacity);
	unsigned int offset = 0;

	// copy free section
	pool->free_index = 0;
	memcpy(&new_ptr_buf[offset], (void*)free_section1, sizeof(uintptr_t) * num_free_section1);
	offset += num_free_section1;
	if (num_free_section2 > 0)
	{
		memcpy(&new_ptr_buf[offset], (void*)free_section2, sizeof(uintptr_t) * num_free_section2);
		offset += num_free_section2;
	}

	// copy alloc section
	pool->alloc_index = offset;
	if (num_free_section1 > 0)
	{
		memcpy(&new_ptr_buf[offset], (void*)alloc_section1, sizeof(uintptr_t) * num_alloc_section1);
		offset += num_alloc_section1;
		if (num_alloc_section2 > 0)
		{
			memcpy(&new_ptr_buf[offset], (void*)alloc_section2, sizeof(uintptr_t) * num_alloc_section2);
			offset += num_alloc_section2;
		}
	}

	// init new section
	unsigned int i;
	for (i = 0; i < delta_size; ++i)
	{
		new_ptr_buf[offset + i] = (uintptr_t)((char*)pool->memory_pool_data_bufs[pool->num_buf] + i * pool->block_size);
	}

	free(pool->memory_pool_ptr_buf);
	pool->memory_pool_ptr_buf = new_ptr_buf;

	// update pool data
	++pool->num_buf;
	pool->capacity = capacity;
}