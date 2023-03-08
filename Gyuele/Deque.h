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
 * Copyright (c) 2017 vivitue
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

#ifndef _VIVITUE_DEQUE_H_
#define _VIVITUE_DEQUE_H_
#include "Alloc.h"
#include "Iterator.h"
#include "Common.h"
namespace STL1
{
	
	template<class T,class Ref,class Ptr> class DequeIterator;
	template<class T>
	class Deque
	{
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef const value_type* const_pointer;
		typedef const value_type& const_reference;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef size_type SizeType;
	
	private:
		static size_type dft_bufer_size()
		{
			return ___buffer_size2<value_type>();
		}
		static size_type dft_map_size()
		{
			return ___buffer_size2<pointer>();
		}
		typedef _default_alloc_4std_<value_type,alloc> allocator;
		typedef _default_alloc_4std_<pointer,alloc> map_allocator;
	public:
		typedef DequeIterator<value_type,reference,pointer> iterator;
		typedef DequeIterator<value_type,const_reference,const_pointer> const_iterator;
	protected:
		typedef pointer* map_pointer;
	protected:
		iterator start;			// the first buffer
		iterator finish;		// the last buffer
		size_type length;		// number of deque's element 
		map_pointer map;		// map first addr
		size_type map_size;		// current map size (used map size : finish.node -start.node +1)
	public:
		Deque() : start(),finish(),length(0),map(0),map_size(0){ }
	private:
		void deallocate_buf_at_begin()
		{
			map_pointer tmp = start.node;
			allocator::deallocate(*tmp,dft_bufer_size());
			++start.node;
			if(empty())
			{
				map_allocator::deallocate(map,map_size);
				start = iterator();
				finish = start;
				map =0;
				map_size = 0;
			}
			else
			{
				start = iterator(*start.node,start.node);
			}
			
		}

		void deallocate_buf_at_end()
		{
			map_pointer tmp = finish.node;
			allocator::deallocate(*tmp,dft_bufer_size());
			--finish.node;
			if(empty())
			{
				map_allocator::deallocate(map,map_size);
				start = iterator();
				finish = start;
				map =0;
				map_size = 0;
			}
			else
			{
				finish = iterator(*finish.node +dft_bufer_size() ,finish.node);
			}
			
		}
		void allocate_buf_at_begin()
		{
			// 获取缓存
			size_type bfsz = dft_bufer_size();
			// 申请一个缓存空间
			pointer bufp = allocator::allocate(bfsz);
			if(empty())
			{
				// 当前的map为空 获取默认map大小
				map_size = dft_map_size();
				// 申请map的空间
				map = map_allocator::allocate(map_size);
				
				size_type pos = map_size/2;
				// 在map_size/2的位置连接新申请的缓存
				map[pos] = bufp;
				//令buffer的bfsz/2+1的位置为current的位置
				start = iterator(bufp +bfsz/2 +1,(map + pos));
				// 终点==起点
				finish = start;
				return;
			}

			if(start.node==map)
			{
				// 申请新的空间，复制数据，释放旧的空间
				// 当前总的节点数
				size_type oldsz= finish.node - start.node +1;
  
				size_type newmpsz = oldsz  * 2; // 增量 2倍＋　
				map_pointer newmp = map_allocator::allocate(newmpsz);
				// [start.node,finish.node+1) to [newmp+newmpsz/4+1,)
				copy_forward(start.node,finish.node+1,newmp + newmpsz/4+1);

				map_allocator::deallocate(map,map_size);
				map_size = newmpsz;
				if(map_size==476)
				{
					map_size=476;
				}

				map = newmp;
				map[newmpsz/4] = bufp;
				start = iterator(bufp + bfsz,map + newmpsz/4);

				// finish location : map + newmpsz/4 + 1 + (oldsz -1)
				finish = iterator(finish.current,map + newmpsz/4 + oldsz);
			}
			else
			{
				//map前面的结点还够用 ，连接bufer到start的前一个位置
				--start.node;
				*(start.node) =bufp;
				// start.current指向buffer最后一个元素的下一个位置
				start = iterator(bufp+bfsz,start.node);
			}
		}
		void allocate_buf_at_end()
		{
			size_type bfsz = dft_bufer_size();
			pointer bufp = allocator::allocate(bfsz);
			if(empty())
			{
				map_size = dft_map_size();
				map = map_allocator::allocate(map_size);
	
				map[map_size/2] = bufp;
				start = iterator(bufp +bfsz/2 -1,(map + map_size/2));
				finish = start;
				return;
			}
			if(finish.node==(map+map_size-1))
			{
				// 申请新的空间，复制数据，释放旧的空间
				size_type oldsz= finish.node - start.node + 1;

				size_type newmpsz = oldsz* 2; // 增量 2倍
				map_pointer newmp = map_allocator::allocate(newmpsz);

				// copy [start.node,finish.node] to [newmp + newmpsz/4,last)
				copy_forward(start.node,finish.node + 1,newmp + newmpsz/4);

				// release old space
				map_allocator::deallocate(map,map_size);

				map_size = newmpsz;

				map = newmp;
				map[newmpsz/4+oldsz] = bufp;
				start = iterator(start.current,map + newmpsz/4);
				finish = iterator(bufp,map + newmpsz/4 + oldsz);
			}
			else
			{
				++finish.node;
				*(finish.node) =bufp;
				finish = iterator(bufp,finish.node);
			}
		}
		void ExtendStartEdgeAccording()
		{
			if(start.current==start.first)
			{
				allocate_buf_at_begin();
			}
		}
		void ExtendFinishEdgeAccording()
		{
			if(finish.current==finish.last)
			{
				allocate_buf_at_end();
			}
		}
	public:
		SizeType GetMapsize() const
		{
			return map_size;
		}
		SizeType GetUsedMapsize() const
		{
			if(empty())return 0;
			return finish.node - start.node +1 ;
		}

	_STL1_PROTECTED_:
		iterator begin()
		{
			return start;
		}
		const_iterator begin()const
		{
			return start;
		}
		iterator end()
		{
			return finish;
		}
		const_iterator end()const
		{
			return finish;
		}
		bool empty()const
		{
			return length==0;
		}
		size_type size()const
		{
			return length;
		}
		size_type capacity()const
		{
			return size_type(-1);
		}
		reference front()
		{
			return *begin();
		}
		reference back()
		{
			return *(end()-1);
		}
		void push_front(const_reference x)
		{
			ExtendStartEdgeAccording();
			--start.current;
			Construct(start.current,x);
			++length;
			ExtendFinishEdgeAccording();
		}

		void push_back(const_reference x)
		{
			if(empty())allocate_buf_at_end();
			Construct(finish.current,x);
			++finish.current;
			++length;
			ExtendFinishEdgeAccording();
		}
		void pop_front()
		{
			if(empty())return;
			--length;
			Destruct(start.current);
			++start.current;
			if(empty() ||
				start.current==start.last)
			{
				deallocate_buf_at_begin();
			}
		}
		void pop_back()
		{
			if(empty())return;
			if(finish.current==finish.first)
			{
				deallocate_buf_at_end();
			}
			--length;
			--finish.current;
			Destruct(finish.current);
			if(empty())
			{
				deallocate_buf_at_end();
			}
		}
		void resize(size_type newsz,const_reference x)
		{
			if(newsz == size()) return;
			if(newsz < size())
			{
				erase(begin()+newsz,end());
			}
			else
			{
				insert(end(),newsz - size(),x);
			}
		}
		void resize(size_type newsz)
		{
			resize(newsz,T());
		}
		void clear()
		{
			while(!empty())
			{
				pop_front();
			}
		}

	_STL1_PROTECTED_:
		// copy the shortest sequence is more efficient
		void erase(iterator position)
		{
			if(end()-position>position-begin())
			{
				// copy [begin,position) to [begin+1,position+1)
				copy_backward(begin(),position,position+1);
				pop_front();
			}
			else
			{
				// copy [position+1,end) to [position,end()+1)
				copy_forward(position+1,end(),position);
				pop_back();
			}
		}
		void erase(iterator first,iterator last)
		{
			difference_type d = last - first;
			if(end()-last>first-begin())
			{
				copy_backward(begin(),first,last);
				while(d--)pop_front();
			}
			else
			{
				// copy [last,end) to [first,end()-offset
				copy_forward(last,end(),first);
				while(d--)pop_back();
			}
		}

		/// <summary>
		/// Insert a node before pos & return iterator of inserted-node
		/// </summary>
		/// <param name="pos">position behind of inserted node</param>
		/// <param name="x">value of new insert-node</param>
		/// <returns>iterator of inserted-node</returns>
		iterator insert(iterator position,const_reference x)
		{
			if(position==begin())
			{
				push_front(x);
				return begin();
			}
			if(position==end())
			{
				push_back(x);
				return end() -1;
			}

			difference_type index = position - begin();
			T x_copy = x;
			if (index < length / 2)
			{
				push_front(*begin());
				copy_forward(begin() + 2, begin() + index + 1, begin() + 1); 
			} 
			else 
			{
				push_back(*(end() - 1));
				copy_backward(begin() + index, end() - 2, end() - 1); 
			}
			*(begin() + index) = x_copy;
			return begin() + index;
			//difference_type offset=0;
			//if(end() - position > position - begin())
			//{
			//	offset = position - begin();
			//	push_front(*begin());
			//	
			//	position = begin() + offset+1; // position.node maybe changed

			//	// copy [begin()+2,position) to [begin+1,position-1)
			//	copy_forward(begin()+2,position,begin()+1);

			//	*(position-1) = x;
			//	return position-1;
			//}
			//else
			//{
			//	offset =end() - position;
			//    push_back(back());
			//	// copy [position,end()-2) to [position+1,end()-1)
			//	//copy_forward(begin()+2,position,begin()+1);				
			//	position = end() - offset+1;
			//	copy_backward(position,end()-2,end()-1);
			//	*position = x;
			//	return position;
			//}
		}

		iterator insert(iterator position,size_type n,const_reference x)
		{
			if(position==begin())
			{
				while(n--)push_front(x);
				return begin();
			}
			if(position==end())
			{
				while(n--)push_back(x);
				return end() -1;
			}

			size_type i =n;
			difference_type end_offset = end() - position;
			difference_type begin_offset = position - begin();

			if(end_offset > begin_offset)
			{
				while(i--)push_front(*begin());

				// copy [begin+n,position) to [begin,position-n)
				// 
				//position = begin() + begin_offset + n;
				position=end() - end_offset;
				copy_forward(begin() + (n+1),position,begin()+1);
				// reset i
				i = n;
				// set tmp = position -1 & construct n on[position-5,position)
				iterator tmp = position;
				while(i--) *--tmp = x;
				return position+1;
			}
			else
			{
				while(i--)push_back(back());
				
				//position = end() - (end_offset + n);
				position = begin() + begin_offset;
				copy_backward(position,end() - n-1,end()-1);

				i = n;
				iterator tmp = position;
				while(i--) *tmp++ = x;
				return tmp-1;
			}
		}

		iterator insert_debug(iterator position,size_type n,const_reference x)
		{
			if(position==begin())
			{
				while(n--)push_front(x);
				return begin();
			}
			if(position==end())
			{
				while(n--)push_back(x);
				return end() -1;
			}

			size_type i =n;
			difference_type end_offset = end() - position;
			difference_type begin_offset = position - begin();
			if(begin_offset==1571)
			{
				int comehere=-1;
			}
			if(end_offset > begin_offset)
			{
				while(i--)push_front(*begin());

				// copy [begin+n,position) to [begin,position-n)
				// 
				//position = begin() + begin_offset + n;
				position=end() - end_offset;
				copy_forward(begin() + (n+1),position,begin()+1);
				// reset i
				i = n;
				// set tmp = position -1 & construct n on[position-5,position)
				iterator tmp = position;
				while(i--)
				{
					*--tmp = x;
				}
				return position+1;
			}
			else
			{
				while(i--)push_back(back());

				iterator ty = position;
				
				//position = end() - (end_offset + n);
				position = begin() + begin_offset;

				if(ty.current!=position.current)
				{
					int testflag = *position.current;
				}
				copy_backward(position,end() - n-1,end()-1);

				i = n;
				iterator tmp = position;
				difference_type nxf = end() - position;
				difference_type nxf2 = position - begin();
				difference_type j = length-1;
				iterator jiter =begin();
				while(j--)
				{
					++jiter;
					if(jiter==position)
					{
						++jiter;
						--jiter;

					}
					if(j==956)
					{
						T xcp2 = *jiter;
						*jiter = xcp2;
					}
					T xcp = *jiter;
					*jiter = xcp;
					if((int)(&*jiter)==0x00000687)
					{
						*jiter = 687;
					}
					Construct(&*jiter,x);
					//*jiter = x;
				}
				while(i--)
				{

					*tmp = x;
					++tmp;
				}
				return tmp-1;
			}
		}

		iterator insert(iterator position)
		{
			return insert(position,T());

		}

		
		void insert(iterator position,const_pointer first,const_pointer last)
		{
			// 充分考虑了copy效率的问题
			difference_type n = 0;
			distance(first,last,n);

			difference_type begin_offset =  position - begin(); // distance of [begin,position)
			difference_type end_offset = length - begin_offset; // end() - position [position,end)

			difference i = 0;// temp variable
			// 后段数据比前段数据多 从前段插入 (copy的数据少)
			if(end_offset > begin_offset)
			{
				if( n < begin_offset ) // 插入的数据小于前段数据量
				{
					while( i++ < n) push_front(*(begin() + n -1));
					
					// begin+2n为新的起点，终点为
					copy_forward(begin()+n + n,begin()+begin_offset+n,begin() + n);
					copy_forward(first,last,begin() + begin_offset);
					//return begin() + begin_offset;
				}
				else
				{
					// 插入的数据大于前段数据量
					const_pointer p = last - begin_offset;
					// [first,p)的数据反向压入deque的front
					while( p!= first ) push_front(*--p);
					// 剩下[p,last)总长为begin_offset的数据
					// 从(,begin() + n -1]反向压力front,压begin_offset个数据，正好压到原来的begin处
					while( i++ < begin_offset) push_front(*(begin() + n -1));
					// 剩下[p,last)总长为begin_offset的数据，copy到[begin+n,)位置
					copy_forward(last-begin_offset,last,begin()+n);
				}
			}
			else
			{
				// 后段的数据比前段的数据少，后端压入
				difference_type orginal_lens =  length;
				if( n < end_offset) // 总个数小于后段数据
				{
					//将后段的[end-n,end)的数据顺序压入，由于压一个数据总长为增1，故压入的数据要增加，每压一个增1
					while(i < n) push_back( *(begin()+ orginal_lens - n + i++));
					// 反向copy[position,begin() + original_lens - n)的数据到[,begin+original_lens)
					copy_backward(begin() + begin_offset,begin() + original_lens - n,begin()+original_lens);
					// 将[first,last)的数据copy到[position,)的位置
					copy_forward(first,last,begin() + begin_offset);
				}
				else
				{
					// 后段的数据小于n
					const_pointer p = first + end_offset;
					// 将[p,last)数据压入   总个数为last - p
					while( p!= last)push_back(*p++);
					// 还有end_offset个数据压入 [pos,last)段的数据
					while( i < end_offset) push_back(*(begin() + begin_offset + i++));
					// copy [first,first+end_offset)的数据到[pos,)的位置 总计end_offset个
					copy_forward(first,first+end_offset,begin() + begin_offset);
				}
			}
		}
		void insert(iterator position,const_iterator first,const_iterator last)
		{
			
		}
	};

	template<class T,class Ref,class Ptr>
	class DequeIterator
	{
	public:
		typedef T value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef const reference const_reference;
		typedef const pointer const_pointer;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef random_access_iterator_tg iterator_category;

		typedef DequeIterator<T,Ref,Ptr> Self;
		typedef DequeIterator<T,T&,T*> iterator;
		typedef DequeIterator<T,const T&,const T*> const_iterator;
		
	protected:
		typedef T** map_pointer;
	public:
		T* current;  
		T* first;
		T* last;
		map_pointer node;
		
	public:
		// *y+dft_bufer_size 最后一个元素的下一个位置
		DequeIterator(T* x,map_pointer y) :current(x),first(*y),last(*y + dft_bufer_size()),node(y){ }
		DequeIterator() : node(0),first(0),last(0),current(0){ }
		DequeIterator(const Self& x) : node(x.node),current(x.current),first(x.first),last(x.last){ }
	private:
		static size_type dft_bufer_size()
		{
			return ___buffer_size2<value_type>();
		}
	private:
		void increment()
		{
			++current;// 可以加到最后一个元素的下一个位置，如果到达最后一个元素的下一个位置，需要跳过该节点
			if(current==last)
			{
				set_node(node+1);
				current = first;
			}
		}
		void decrement()
		{
			// 如果current与first相等，对finish指针来说，需要跳到上一个节点，否则一直执行减减操作
			if(current==first)
			{
				set_node(node-1);
				current = last;
			}
			--current;
		}
		/// <summary>
		/// iterator shift n-position  
		/// (n : supported negtive parameter) means moving forward & backword
		/// </summary>
		/// <param name="n">shift position</param>
		void _xor_advance(difference_type n)
		{
			difference_type bfsz = dft_bufer_size();
		    difference_type lens = n + current - first;
			//difference_type num = lens >= 0? lens/bfsz : -(-lens + bfsz -1)/bfsz;
			difference_type num = lens >= 0? lens/bfsz : (lens+1)/bfsz - 1;
			if(num == 0)
			{
				current+=n;
				return;
			}
			else if(num > 0)
			{
				set_node(node + num);// node location
				current = first + lens - num * bfsz;
				//current = first + lens%bfsz;// first location
			}
			else
			{
				set_node(node + num);// node location
				//current = last + lens%bfsz;
				current = first + lens - num * bfsz;
			}
		}
		void set_node(map_pointer target)
		{
			node = target;
			first = *target;
			last = first + difference_type(dft_bufer_size());
		}
	public:
		reference operator*()const
		{
			return *current;
		}
		pointer operator->()const
		{
			return &(operator*());
		}
		Self& operator++()
		{
			increment();
			return *this; 

		}
		Self operator++(int)
		{
			Self tmp = *this;
			++(*this);
			return tmp;
		}
		Self& operator--()
		{
			decrement();
			return *this;
		}
		Self operator--(int)
		{
			Self tmp = *this;
			this->decrement();
			return tmp;
		}
		Self& operator+=(difference_type n)
		{
			this->_xor_advance(n);
			return *this;
		}
		Self& operator-=(difference_type n)
		{
			return operator+=(0-n);
		}
		Self operator+(difference_type n)
		{
			Self tmp = *this;
			return tmp+=n;
		}
		Self operator-(difference_type n)
		{
			Self tmp = *this;
			return tmp-=n;
		}
		difference_type operator-(const Self x)const
		{
			difference_type tmp = difference_type(dft_bufer_size() * (node - x.node - 1) +(current - first) + (x.last - x.current));
			difference_type result =  node == x.node ? current - x.current : tmp;
			return result;

			/*difference_type n = 0;
			if(x==*this)return n;

			if(x>*this)
			{
				Self tmp = *this;
				while(tmp!=x)
				{
					++tmp;
					++n;
				};
				n=0-n;
			}
			else
			{
				while(x!=*this)
				{
					++x;
					++n;
				}
			}
			return n;*/
		}
		reference operator[](difference_type n)const
		{
			return *(*this + n);
		}
		bool operator==(const Self& x)const
		{
			return current==x.current;
		}
		bool operator!=(const Self& x)const
		{
			return !(*this==x);
		}
		bool operator<(const Self& x)const
		{
			return (node == x.node) ? current<x.current : node<x.node;
		}
		bool operator>(const Self& x)const
		{
			return !(*this<x);
		}
	};
	inline size_t ___buffer_size(size_t n,size_t sz)
	{
		return n!=0?n : (sz<4096 ? size_t(4096/sz) : size_t(1));
	}

	template<class T>
	inline size_t ___buffer_size2()
	{
		size_t max = _STL1_STD_MAXDQ_BYTESZ;
		return sizeof(T)<max ? size_t(max/sizeof(T)) : 1;
	}
}
#endif
/*
 * Deque.h  
 * Written by vivitue on Oct. 19th 2017. 
 * v1.0.0
 * Updated Oct. 19th 2017
 * all end.
*/