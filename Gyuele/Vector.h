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

#ifndef _VIVITUE_VECTOR_H_
#define _VIVITUE_VECTOR_H_
#include "Alloc.h"
#include "Algobase.h"
#include "Common.h"
#include "Iterator.h"
namespace STL1
{
	template<class T>
	class Vector
	{
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef ptrdiff_t difference_type;
		typedef value_type* iterator;
		typedef const value_type* const_iterator;
		typedef size_t size_type;
                
		typedef ___reverse_iterator<iterator,value_type,reference,difference_type> reverse_iterator;
		typedef ___reverse_iterator<const_iterator,value_type,const_reference,difference_type> const_reverse_iterator;
	protected:
		typedef _default_alloc_4std_<value_type,alloc> allocator;
		iterator start;
		iterator finish;
		iterator end_of_storage;
	private:
		typedef Vector<T> Self;
	private:
		void Dispose()
		{
			if(0 !=start)
			{
				allocator::deallocate(start,end_of_storage-start);
			}
			this->Reset();
		}
		void Reset()
		{
			start = (iterator)0;
			finish =(iterator) 0;
			end_of_storage=(iterator)0;
		}
	public:
		Vector() : start(0),finish(0),end_of_storage(0){ }
		Vector(size_t n,const T& val)
		{
			start = allocator::allocate(n);
			finish = start + n;
			end_of_storage = finish;
		}
		~Vector()
		{
			Destruct(start,finish,end_of_storage);
			this->Dispose();
		}
	public:
		/// <summary>
		/// copy assignment constructor [deep copy]
		/// 1.A=B (B=X)
		/// 2.when B.SIZE > A.CAPACITY,expand capacity else copy B to A and adjust A.finish
		/// </summary>
		/// <param name="X">another vector</param>
		/// <returns>vector copied</returns>
		Self& operator=(const Self& x)
		{
			if(&x==this)return *this; 
			//if(x.size() >capacity() ||x.capacity() > capacity())
			if(x.size() >capacity())
			{
				Dispose();
				this->start = allocator::allocate(x.capacity());
				this->finish = start + x.size();
				this->end_of_storage = start + x.capacity();
				copy_forward(x.begin(),x.end(),start);
				return *this;
			}
			copy_forward(x.begin(),x.end(),this->start);
			this->finish = start + x.size();
			Destruct(finish,end_of_storage,finish);
			return *this;
		}
		bool operator==(const Self& x)
		{
			return (x.size()==size()) && equal(x.begin(),x.end(),begin());
		}
		bool operator!=(const Self& x)
		{
			return !(*this==x);
		}
	public: // public interfaces
		iterator Begin()
		{
			return this->begin();
		}
		iterator End()
		{
			return this->end();
		}
		reverse_iterator RBegin()
		{
			return this->rbegin();
		}
		bool IsEmpty() const
		{
			return this->empty();
		}
		size_type GetSize() const
		{
			return this->size();
		}
		size_type GetCapacity() const
		{
			return this->capacity();
		}

		/*iterator operator[](size_type i)
		{
			return (start + i); //
		}*/
		reference operator[](size_type i)
		{
			return *(start + i);
		}
		iterator GetIteratorFrom(size_type i)
		{
			return (start + i);
		}
		void Erase(iterator pos)
		{
			this->erase(pos);
		}
		void Erase(iterator first,iterator last)
		{
			this->erase(first,last);
		}
		reference Front()
		{
			return this->front();
		}
		reference Back()
		{
			return this->back();
		}
		void PushBack(const_reference val)
		{
			this->push_back(val);
		}
		void PopBack()
		{
			this->pop_back();
		}
		void Clear()
		{
			this->clear();
		}
		void Swap(Self& v)
		{
			this->swap(v);
		}
		iterator Insert(iterator pos,const_reference val)
		{
			return this->insert(pos,val);
		}
		iterator Insert(iterator pos)
		{
			return this->insert(pos);
		}
		iterator Insert(iterator pos,size_type n,const_reference val)
		{
			return this->insert(pos,n,val);
		}
		size_type LeftCapacity()const
		{
			return this->leftcapacity();
		}
		void Resize(size_type number_of_newsz,const_reference val)
		{
			this->resize(number_of_newsz,val);
		}
		void Resize(size_type number_of_newsz,const_reference val,const_reference)
		{
			this->resize2(number_of_newsz,val);
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
		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}
		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}
		const_reverse_iterator rbegin()const
		{
			return reverse_iterator(end());
		}
		const_reverse_iterator rend()const
		{
			return reverse_iterator(begin());
		}
		bool empty() const
		{
			return start == finish;
		}
		size_type size()const
		{
			return finish - start;
		}
		size_type capacity()const
		{
			return end_of_storage - start;
		}
		size_type leftcapacity()const
		{
			return end_of_storage - finish;
		}
	protected:
		iterator insert(iterator pos,const_reference val)
		{
			size_type n_location = pos - begin();
			insert_aux(pos,val);
			return begin() + n_location;
		}

		iterator insert(iterator pos)
		{
			return insert(pos,T());
		}

		iterator insert(iterator pos,size_type n,const_reference val)
		{
			size_type n_location = pos - begin();
			insert_aux(pos,n,val);
			return begin() + n_location;
		}
	protected:
		void swap(Self& v)
		{
			Gswap(v.start,this->start);
			Gswap(v.finish,this->finish);
			Gswap(v.end_of_storage,this->end_of_storage);
		}
		reference front()
		{
			return *begin();
		}
		reference back()
		{
			return *(end() - 1);
		}
		void push_back(const_reference val)
		{
			insert(end(),val);
		}
		void pop_back()
		{
			Destruct(finish-1);
			--finish;
		}
		void erase(iterator pos)
		{
			copy_forward(pos+1,end(),pos);
			Destruct(finish-1);
			--finish;
		}
		void erase(iterator first,iterator last)
		{
			size_type n=0;//
			size_type t = last - first;
			distance(first,last,n);// last - first;
			copy_forward(first,end(),first);
			Destruct(end()-n,end(),first);
			finish=finish - n;
		}
		
		void resize(size_type number_of_newsz,const_reference val)
		{
			if(number_of_newsz < size())
			{
				erase(begin() + number_of_newsz,end());
				return;
			}
			insert_aux(end(),number_of_newsz-size(),val);
		}
	protected:
		void resize2(size_type number_of_newsz,const_reference val)
		{
			size_type currentsz = size(); 

			// new < size
			if(number_of_newsz < currentsz)
			{
				erase(begin() + number_of_newsz,end());
				return;
			}

			size_type c = this->capacity();
			// new < c
			if(number_of_newsz < c)
			{
				size_type increment = number_of_newsz - currentsz;
				iterator tmpIt = finish;
				advance(finish,increment);
				fill(tmpIt,finish,val);
				return;
			}

			// new >=c   extend newsz to double of number_of_newsz
			size_type newsz = number_of_newsz *2;
			iterator newstart = allocator::allocate(newsz);

			iterator newfinish = newstart;
			advance(newfinish,currentsz);

			// copy old to new
			copy_forward(begin(),end(),newstart);
			
			// fill val to [newfinish,newfinish2)
			iterator newfinish2 = newfinish + (number_of_newsz - currentsz);
			fill(newfinish,newfinish2,val);
			// release old space
			this->Dispose();

			// set the new space iterator
			this->start = newstart;
			this->finish = newfinish2;
			this->end_of_storage = newstart + newsz	;
			
		}
		void clear()
		{
			erase(begin(),end());
		}
	private:
		/// <summary>
		/// insert an element before pos
		/// </summary>
		/// <param name="pos">position will be used to insert</param>
		/// <param name="val">element used to insert</param>
		/// <returns>void</returns>
		void insert_aux(iterator pos,const_reference val)
		{
			if(finish != end_of_storage)
			{
				copy_backward(pos,finish,(finish+1));
				Construct(pos,val);
				++finish;
				return;
			}
			// get old space (numbers of element T)
			size_type number_of_oldsz = this->size(); 
			// set size of newspace(numbers of element T)
			size_type number_of_newsz = number_of_oldsz==0? 1:number_of_oldsz*2;
			// allocate newspace
			iterator newstart = allocator::allocate(number_of_newsz);
			// copy old data to new space [begin,pos)
			iterator newpos = uninitialized_copy(begin(),pos,newstart);
			// construct val on newpos
			Construct(newpos,val);
			// copy old data [pos,finish) to [newpos+1,newend)
			uninitialized_copy(pos,end(),newpos+1);

			// free old space
			this->Dispose();
			// re-adjust position of start
			this->start = newstart; 
			// re-adujst of finish
			this->finish = this->start + number_of_oldsz +1 ; // insert one element
			this->end_of_storage = this->start + number_of_newsz;
		}

		void insert_aux(iterator pos,size_type n,const_reference val)
		{
			if(n<1) return;
			size_type cursor = end_of_storage - finish;
			if(n < cursor)
			{
				copy_backward(pos,finish,(finish+n));
				fill(pos,pos+n,val);
				finish +=n;
				return;
			}
			size_type number_of_oldsz = this->size();
			size_type number_of_newsz = number_of_oldsz + MAX(number_of_oldsz,n);
			iterator newstart = allocator::allocate(number_of_newsz);
			iterator newpos = uninitialized_copy(begin(),pos,newstart);
			fill(newpos,newpos+n,val);
			uninitialized_copy(pos,end(),newpos+n);

			this->Dispose();
			this->start = newstart; // re-adjust position of start
			this->finish = this->start + number_of_oldsz + n;
			this->end_of_storage = this->start + number_of_newsz;
		}
	};
}
#endif
/*
 * Vector.h  
 * Written by vivitue on Jan. 5th 2015. 
 * v1.0.0
 * Updated Sep. 25th 2017
 * all end.
*/