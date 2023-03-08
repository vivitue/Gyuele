/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 *
 * Copyright (c) 2015 vivitue
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */
#ifndef _VIVITUE_ALLOC_H_
#define _VIVITUE_ALLOC_H_
#include "Common.h"
#include "malloc.h"

namespace STL1
{
	template<bool threadsafe,int inst>
	class TheSecondAllocator // __default_alloc_template
	{
	public:
		static size_t RoundUpTest(size_t sz)
		{
			return RoundUp(sz);
		}
	private:
		enum { ALIGN = 8,MAXBYTES = 128,INTERVAL = MAXBYTES/ALIGN };
		/// <summary>
		/// Note :    adjust bytes to satisfy the condition of (bytes%8==0)
        /// </summary>
        /// <param name="nsize">size needs to be adjusted</param>
		/// <returns>adjusted size (bytes%8==0)</returns>
		static size_t RoundUp(size_t bytes)
		{
			if(bytes % ALIGN == 0) return bytes;
			return (ALIGN - bytes % ALIGN) + bytes;  // (bytes+ALIGN-1)&~(ALIGN-1)
		}

		/// <summary>
		/// Note : get specified index of node from freeList with bytes-size
        /// </summary>
        /// <param name="bytes"></param>
		/// <returns>index of bytes in freeList</returns>
		static size_t GetFreeListIndex(size_t bytes)
		{
			return (bytes+ALIGN-1)/ALIGN -1;
		}

		/// <summary>
		/// Note :    nsize must be adjusted (nsize % 8==0) first before passing to Refill
        /// 1. Get the target index of nsize
		/// 2. assign the target index block chunk as the head address
		/// 3. Link the left (num-1) blocks to the head(target index) behind
		/// 4. Assign next pointer to null of the last block
        /// </summary>
        /// <param name="nsize">memory size which has been adjusted to nsize % 8==0</param>
        /// <returns>pointer needed</returns>
		static void* Refill(size_t nsize)
		{
			size_t num = _DEFAULT_EXTENTION_NODE; //default extention node number 20
			char* chunk = ChunkAlloc(nsize, num);
			if(1 == num)return chunk;
			Node* result = 0;
			Node* volatile* targetNode = freeList + GetFreeListIndex(nsize);

			result = (Node*) chunk;// return the first block

			// the left blocks fill to freeList   index = GetFreeListIndex(nsize)
			*targetNode = (Node*)(chunk + nsize); // head in index  GetFreeListIndex(nsize)
			Node* next= *targetNode;
			Node* tmp =0;

			// link the (num-1) blocks to targetNode behind
			for(int i = 0;i < num-1;++i)
			{
				tmp = next;
				next =(Node*)((char*)next + nsize);
				tmp->next = next;
			}
			tmp->next = 0;// the next of the last node

			return result;
		}

		/// <summary>
		/// Note : memory pool 
		/// 1.leftsz >= totalsz(requireSize),get totalsz& return,else goto 2
		/// 2.left >nsize,get n = leftsz/nsize nodes & return, else goto 3
		/// 3.request bytesRequire=2*totalsz + heapSize/2,and linked left to freeList[index] behind (leftsz%8==0)
		/// 4.bytesRequire>0,get totalsz & return,give (bytesRequire - totalsz) to memory pool,else goto 5
		/// 5.bytesRequire==0,find node from space>=nsize of freeList,if success,get 1 node & return,else goto 6
		/// 6.malloc memory from the first allocator for help
        /// </summary>
        /// <param name="nsize">memory size which has been adjusted to nsize % 8==0</param>
		/// <param name="n">total blocks of nsize</param>
        /// <returns>pointer needed</returns>
		static char* ChunkAlloc(size_t nsize,size_t& n)
		{
			char* result = 0;
			size_t totalsz = nsize * n;
			size_t leftsz = end - begin;
			if(leftsz >=totalsz)
			{
				result = begin;
				begin += totalsz;
				return result;
			}
			if(leftsz >=nsize)
			{
				n = leftsz/nsize;
				totalsz = nsize * n;
				result = begin;
				begin += totalsz;
				return result;
			}
			size_t bytesRequire = 2*totalsz + RoundUp(heapSize>>4);// 2 times + (micro-increment=heapSize/2   (heapSize/2)%8==0
			if(leftsz > 0)
			{
				Node* volatile* leftlist = freeList + GetFreeListIndex(leftsz);// linked left to freeList[index] behind (leftsz%8==0)
				((Node*)begin)->next = *leftlist;	
				*leftlist = (Node*)begin;		
			}
			begin = (char*)malloc(bytesRequire);
			if(0 == begin)
			{
				Node* volatile* tmplist;
				Node* p;
				for(int i = nsize;i<= MAXBYTES ;i +=ALIGN)
				{
					tmplist = freeList + GetFreeListIndex(i);
					p = *tmplist;
					if(0 != p)
					{
						*tmplist = p->next;
						begin = (char*)p;		// find an avaiable node
						end = begin + i;
						return ChunkAlloc(nsize, n);// 
					}
				}
				// use the first allocator for help if there is not any avaiable node to use
				begin = (char*)TheFirstAllocator<threadsafe,inst>::Allocate(bytesRequire);
			}
			
			heapSize +=bytesRequire;
			end = begin + bytesRequire;
			return ChunkAlloc(nsize,n); 
		}
	private:
		union Node
		{
			union Node* next;
			char data[1];
		};
	private:
		static Node* volatile freeList[INTERVAL]; 
		static char* begin;       // begin cursor of the memory pool
		static char* end;         // end cursor of the memory pool
		static size_t heapSize;   // total heap size until now
	public:
		/// <summary>
		/// Note :  when sz>128 ,use the first allocator else use the second allocator
		/// when sz<=128
        /// 1. get the target address of the node index of requiredsize-sz
		/// 2. assign result use the target node(not null) & remove the head node of the target nodelist
		/// 3. (target node null) refill when there is not any nodes existed for using
        /// </summary>
        /// <param name="sz">memory size</param>
        /// <returns>pointer needed</returns>
		static void* Allocate(size_t sz)
		{
			// if(threadsafe) lock here
			if(sz > MAXBYTES) return TheFirstAllocator<threadsafe,inst>::Allocate(sz);

			Node* result = 0;
			Node* volatile* targetNode = freeList + GetFreeListIndex(sz);
			result = *targetNode;
			if(0 == result)
			{
				//freelist unsatisfies the requirement
				return Refill(RoundUp(sz));
			}
			// freelist satisfies the requirement
			*targetNode = result->next; // remove head
			return result;
		}
		
		static void* Reallocate(void* p,size_t oldsz,size_t newsz)
		{
			void* result =0;
			if(oldsz> MAXBYTES && newsz>MAXBYTES)
			{
				return realloc(p,newsz);
			}
			if(RoundUp(oldsz) == RoundUp(newsz)) return p;

			result = Allocate(newsz);
			size_t cpsz = newsz >oldsz? oldsz : newsz;
			memcpy(result,p,cpsz);
			Deallocate(p,oldsz);
			return result;
		}

		/// <summary>
		/// Note :  when sz>128 ,use the first allocator else use the second allocator
		/// when sz<=128
		/// 1. Recycle node p to the target node of freeList (add p to the head of target node of freeList)
        /// </summary>
        /// <param name="p">free node</param>
		/// <param name="sz">size of node for freeing</param>
        /// <returns>void</returns>
		static void Deallocate(void* p,size_t sz)
		{
			Node* freeTarget = (Node*)p;
			if(sz > MAXBYTES)
			{
				TheFirstAllocator<threadsafe,inst>::Deallocate(p,sz);
				return;
			}
			
			Node* volatile* target = freeList + GetFreeListIndex(sz);
			freeTarget->next = *target;
			*target = freeTarget;

		}


	};

	template<bool threadsafe,int inst>
	class TheFirstAllocator //__malloc_alloc_template
	{
	private:
		// outofmemory event only for single method's registration.
		// multi-methods can't be bound (bind) [Not supported]
		static SetHandler outOfMemoryHandler;
	private:

		/// <summary>
		/// Note :  malloc for outofmemory
        /// </summary>
		/// <param name="sz">size of node</param>
        /// <returns>target pointer</returns>
		static void* Allocate4Outofmemory(size_t sz)
		{
			void* result = 0;
			while(true)
			{
				if(0 == outOfMemoryHandler)_BAD_MEM_EXCEPTION_;
				outOfMemoryHandler();// triger client's call of registered method
				result = malloc(sz); // malloc
				if(0 !=result)break;
			}
			return result;
		}

		/// <summary>
		/// Note :  realloc for outofmemory
        /// </summary>
		/// <param name="sz">size of node</param>
        /// <returns>target pointer</returns>
		static void* Reallocate4Outofmemory(void* p,size_t sz)
		{
			void* result = 0;
			while(true)
			{
				if(0 == outOfMemoryHandler)_BAD_MEM_EXCEPTION_;
				outOfMemoryHandler();// triger client's call of registered method
				result = realloc(p,sz); // malloc
				if(0 !=result)break;
			}
			return result;
		}

	public:
		/// <summary>
		/// malloc for the first allocator
        /// </summary>
		/// <param name="sz">size of node</param>
        /// <returns>target pointer</returns>
		static void* Allocate(size_t sz)
		{
			void* result = malloc(sz);
			if(0==result) result = Allocate4Outofmemory(sz);
			return result;

		}

		/// <summary>
		/// single node only supported for free
        /// </summary>
		/// <param name="p">target pointer for free</param>
		/// <param name="sz">size of node</param>
        /// <returns>void</returns>
		static void Deallocate(void* p,size_t sz)
		{
			free(p);
		}


		/// <summary>
		/// realloc for the first allocator
        /// </summary>
		/// <param name="p">the onld pointer existed</param>
		/// <param name="sz">new space size required</param>
        /// <returns>target pointer</returns>
		static void* Reallocate(void*p,size_t sz)
		{
			void* result = realloc(sz);
			if( 0 ==result) result = Reallocate4Outofmemory(p,sz);
			return result;
		}
		
		/// <summary>
		/// Register out of memory event
		/// Note :  event mechanism provided with function-pointer
        /// </summary>
        /// <param name="handler">function pointer passed in from outside</param>
        /// <returns>the old function pointer internal using</returns>
		static SetHandler RegisterOutofMemoryEvent(SetHandler handler)
		{
			SetHandler old = outOfMemoryHandler;
			outOfMemoryHandler = handle;
			return old;
		}
	};

	template<class T,class Alloc>
	class _default_alloc_4std_
	{
	public:
		static T* allocate(size_t n)
		{
			if(0==n) return 0;
			return (T*)Alloc::Allocate(n * sizeof(T));
		}
		static T* allocate()
		{
			return (T*)Alloc::Allocate(sizeof(T));
		}
		static void deallocate(T* p,size_t n)
		{
			if(0!=n)
			{
				Alloc::Deallocate(p,n * sizeof(T));
			}
		}
		static void deallocate(T* p)
		{
			Alloc::Deallocate(p,sizeof(T));
		}
	};

	template <bool threadsafe, int inst>
	char* TheSecondAllocator<threadsafe, inst>::begin = 0;

	template<bool threadsafe,int inst>
	char* TheSecondAllocator<threadsafe, inst>::end = 0;

	template<bool threadsafe,int inst>
	size_t TheSecondAllocator<threadsafe, inst>::heapSize = 0;

	template<bool threadsafe,int inst>
	typename TheSecondAllocator<threadsafe,inst>::Node* volatile
	TheSecondAllocator<threadsafe,inst>::freeList
	[TheSecondAllocator<threadsafe,inst>::INTERVAL]
	={ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	template<bool threadsafe,int inst>
	SetHandler TheFirstAllocator<threadsafe,inst>::outOfMemoryHandler =0;

	typedef TheSecondAllocator<_IS_ALLOC_THREADSAFE_ALIVE_,0> alloc; // default second allocator
	typedef TheFirstAllocator<_IS_ALLOC_THREADSAFE_ALIVE_,0> alloc1; // default first allocator

}
#endif
/*
* Alloc.h  
* Written by vivitue on Jan. 5th 2015. 
* v1.0.0
* Updated Sep. 25th 2017
* all end.
*/