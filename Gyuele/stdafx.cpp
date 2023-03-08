// stdafx.cpp : source file that includes just the standard includes
// Gyuele.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
//struct mem_control_block 
//{
//     int is_available;// available flag (1 avaiable else unavaiable)
//     int size;		  // memory size  (unit: byte)
//};
//
//void free(void *firstbyte) 
//{
//	struct mem_control_block *mcb;
//	mcb = firstbyte - sizeof(struct mem_control_block);
//	mcb->is_available = 1;
//	return;
//}
//void malloc_init()
//{
//	/* grab the last valid address from the OS */
//	last_valid_address = sbrk(0);
//	managed_memory_start = last_valid_address;
//	has_initialized = 1;
//
//}
//void *malloc(long numbytes) 
//{
//     void *current_location;
//     struct mem_control_block *current_location_mcb;
//     void *memory_location;
//     if(!has_initialized) malloc_init();
//     numbytes = numbytes + sizeof(struct mem_control_block);
//     memory_location = 0;
//     current_location = managed_memory_start;
//     /* Keep going until we have searched all allocated space */
//     while(current_location != last_valid_address)
//     {
//         current_location_mcb =(struct mem_control_block *)current_location;
//         if(current_location_mcb->is_available)
//         {
//             if(current_location_mcb->size >= numbytes)
//             {
//                 current_location_mcb->is_available = 0;
//                 memory_location = current_location;
//                 break;
//             }
//         }
//         current_location = current_location +current_location_mcb->size;
//     }
//     if(!memory_location)
//     {
//         sbrk(numbytes);
//         memory_location = last_valid_address;
//         last_valid_address = last_valid_address + numbytes;
//         current_location_mcb = memory_location;
//         current_location_mcb->is_available = 0;
//         current_location_mcb->size = numbytes;
//     }
//     memory_location = memory_location + sizeof(struct mem_control_block);
//     return memory_location;
//}


//Void_t* public_cALLOc(size_t n, size_t elem_size)
//{
//	mstate av;
//	mchunkptr oldtop, p;
//	INTERNAL_SIZE_T bytes, sz, csz, oldtopsize;
//	Void_t* mem;
//	unsigned long clearsize;
//	unsigned long nclears;
//	INTERNAL_SIZE_T* d;
//
//	/* size_t is unsigned so the behavior on overflow is defined.  */
//	bytes = n * elem_size;
//#define HALF_INTERNAL_SIZE_T \
//	(((INTERNAL_SIZE_T) 1) << (8 * sizeof (INTERNAL_SIZE_T) / 2))
//	if (__builtin_expect ((n | elem_size) >= HALF_INTERNAL_SIZE_T, 0)) {
//		if (elem_size != 0 && bytes / elem_size != n) {
//			MALLOC_FAILURE_ACTION;
//			return 0;
//		}
//	}
//
//	__malloc_ptr_t (*hook) __MALLOC_PMT ((size_t, __const __malloc_ptr_t)) =
//		force_reg (__malloc_hook);
//	if (__builtin_expect (hook != NULL, 0)) {
//		sz = bytes;
//		mem = (*hook)(sz, RETURN_ADDRESS (0));
//		if(mem == 0)
//			return 0;
//#ifdef HAVE_MEMCPY
//		return memset(mem, 0, sz);
//#else
//		while(sz > 0) ((char*)mem)[--sz] = 0; /* rather inefficient */
//		return mem;
//#endif
//	}
//
//	sz = bytes;
//
//	arena_get(av, sz);
//	if(!av)
//		return 0;
//
//	/* Check if we hand out the top chunk, in which case there may be no
//	need to clear. */
//#if MORECORE_CLEARS
//	oldtop = top(av);
//	oldtopsize = chunksize(top(av));
//#if MORECORE_CLEARS < 2
//	/* Only newly allocated memory is guaranteed to be cleared.  */
//	if (av == &main_arena &&
//		oldtopsize < mp_.sbrk_base + av->max_system_mem - (char *)oldtop)
//		oldtopsize = (mp_.sbrk_base + av->max_system_mem - (char *)oldtop);
//#endif
//	if (av != &main_arena)
//	{
//		heap_info *heap = heap_for_ptr (oldtop);
//		if (oldtopsize < (char *) heap + heap->mprotect_size - (char *) oldtop)
//			oldtopsize = (char *) heap + heap->mprotect_size - (char *) oldtop;
//	}
//#endif
//	mem = _int_malloc(av, sz);
//
//	/* Only clearing follows, so we can unlock early. */
//	(void)mutex_unlock(&av->mutex);
//
//	assert(!mem || chunk_is_mmapped(mem2chunk(mem)) ||
//		av == arena_for_chunk(mem2chunk(mem)));
//
//	if (mem == 0) {
//		/* Maybe the failure is due to running out of mmapped areas. */
//		if(av != &main_arena) {
//			(void)mutex_lock(&main_arena.mutex);
//			mem = _int_malloc(&main_arena, sz);
//			(void)mutex_unlock(&main_arena.mutex);
//		} else {
//#if USE_ARENAS
//			/* ... or sbrk() has failed and there is still a chance to mmap() */
//			(void)mutex_lock(&main_arena.mutex);
//			av = arena_get2(av->next ? av : 0, sz);
//			(void)mutex_unlock(&main_arena.mutex);
//			if(av) {
//				mem = _int_malloc(av, sz);
//				(void)mutex_unlock(&av->mutex);
//			}
//#endif
//		}
//		if (mem == 0) return 0;
//	}
//	p = mem2chunk(mem);
//
//	/* Two optional cases in which clearing not necessary */
//#if HAVE_MMAP
//	if (chunk_is_mmapped (p))
//	{
//		if (__builtin_expect (perturb_byte, 0))
//			MALLOC_ZERO (mem, sz);
//		return mem;
//	}
//#endif
//
//	csz = chunksize(p);
//
//#if MORECORE_CLEARS
//	if (perturb_byte == 0 && (p == oldtop && csz > oldtopsize)) {
//		/* clear only the bytes from non-freshly-sbrked memory */
//		csz = oldtopsize;
//	}
//#endif
//
//	/* Unroll clear of <= 36 bytes (72 if 8byte sizes).  We know that
//	contents have an odd number of INTERNAL_SIZE_T-sized words;
//	minimally 3.  */
//	d = (INTERNAL_SIZE_T*)mem;
//	clearsize = csz - SIZE_SZ;
//	nclears = clearsize / sizeof(INTERNAL_SIZE_T);
//	assert(nclears >= 3);
//
//	if (nclears > 9)
//		MALLOC_ZERO(d, clearsize);
//
//	else {
//		*(d+0) = 0;
//		*(d+1) = 0;
//		*(d+2) = 0;
//		if (nclears > 4) {
//			*(d+3) = 0;
//			*(d+4) = 0;
//			if (nclears > 6) {
//				*(d+5) = 0;
//				*(d+6) = 0;
//				if (nclears > 8) {
//					*(d+7) = 0;
//					*(d+8) = 0;
//				}
//			}
//		}
//	}
//
//	return mem;
//}
