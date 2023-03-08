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

#ifndef _VIVITUE_LIST_CC_H__
#define _VIVITUE_LIST_CC_H__
#include "Alloc.h"
#include "Construct.h"
#include "Iterator.h"
#include "Common.h"
namespace STL1
{
	template<class T> struct _list_node_;
	template<class T> class ListIterator;
	template<class T,class R,class P> class LIterator;
	template<class T> class ConstListIterator;
	template<class T>
	class List
	{
	public:
		typedef T key_type;
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef value_type value;
	private:
		typedef _list_node_<T> node;
		typedef node* pnode; 
		typedef _default_alloc_4std_<node,alloc> allocator;
		typedef ListIterator<T> iterator;
		typedef LIterator<T,const_reference,const_pointer> const_iterator; // ConstIterator
		typedef List<T> Self;
	public: // public interfaces provided
		size_t GetCount()const
		{
			return this->count;
		}
		bool IsEmpty()const
		{
			return (this->count==0);
		}
		void Add(T val)
		{
			this->insert(begin(),val);
		}
		void Remove(const T& val)
		{
			this->remove(val);
		}
		void Reverse()
		{
			reverse_dft();
		}
		void Sort()
		{
			this->sort2gyue();
		}
		template<class Comparer> 
		void Sort(Comparer cmp)
		{
			this->sort2gyue(cmp);
		}

		void Sort(bool stlversion,int&)
		{
			this->sort_4list_sgi_stl_version();
		}
		void Unique()
		{
			this->unique();
		}
		void UniqueAll()
		{
			this->UniqueAll();
		}
		void Merge(Self& x)
		{
			this->merge(x);
		}
		template<class Comparer>
		void Merge(Self& x,Comparer cmp)
		{
			this->merge(x,cmp);
		}
		iterator Begin()
		{
			return this->begin();
		}
		iterator End()
		{
			return this->end();
		}
		const_iterator Cbegin()
		{
			return this->cbegin();
		}
		const_iterator Cend()
		{
			return this->cend();
		}
		void Splice(iterator pos,Self& lst)
		{
			this->splice(pos,lst);
		}
		void Splice(iterator pos,iterator i,Self& lst)
		{
			this->splice(pos,i,lst);
		}
		void Splice(iterator pos,iterator first,iterator last,Self& lst)
		{
			this->splice(pos,first,last,lst);
		}
		iterator Insert(iterator pos,const value_type& val)
		{
			return this->insert(pos,val);
		}
		void PopFront()
		{
			this->pop_front();
		}

		void PopBack()
		{
			this->pop_back();
		}
		void PushFront(const T& val)
		{
			this->push_front(val);
		}

		void PushBack(const T& val)
		{
			this->push_back(val);
		}

		reference Front()
		{
			return this->front();
		}

		reference Back()
		{
			return this->back();
		}
		void Erase(iterator pos)
		{
			this->erase(pos);
		}

		void Clear()
		{
			this->clear();
		}

	protected:
		size_t count;
		pnode head;
		pnode PNIL;
		iterator beginIt;
		iterator endIt;
	private:
		pnode GetNode()
		{
			return allocator::allocate();
		}
		void PutNode(pnode p)
		{
			allocator::deallocate(p);
		}
	public:
		List() : count(0)
		{
			PNIL = (pnode)0;
			head = GetNode();
			head->next=head;
			head->prev = head;
			//beginIt = iterator(head->next);
			//endIt = iterator(head);
		}
		~List()
		{
			Clear();
			PutNode(head);
		}
	protected:
		iterator begin()
		{
			return iterator(head->next);
		}
		iterator end()
		{
			return iterator(head);
		}
		const_iterator cbegin()
		{
			return const_iterator(head->next);
		}
		const_iterator cend()
		{
			return const_iterator(head);
		}
	_STL1_PROTECTED_:
		/// <summary>
		/// remove position  [before position behind]
		/// 1.link before->next to behind
		/// 2.link behind->prev to before
		/// 3.free position
		/// </summary>
		/// <param name="pos">node iterator will be removed</param>
		void erase(iterator pos)
		{
			((pos.node)->prev)->next = (pos.node)->next; 
			((pos.node)->next)->prev = (pos.node)->prev;
			PutNode(pos.node);
			--count;
		}

		/// <summary>
		/// Insert a node in pos & return iterator of inserted-node
		/// 1. link insertnode->next to pos
		/// 2. link insertnode->prev to pos->prev
		/// 3. link (pos-prev)->next to insertnode
		/// 4. link pos-prev to insertnode
		/// </summary>
		/// <param name="pos">position will be used to insert a new node</param>
		/// <param name="val">value of new insert-node</param>
		/// <returns>iterator of inserted-node</returns>
		iterator insert(iterator pos,const value_type& val)
		{
			pnode insertnode = GetNode();
			Construct(&insertnode->data,val);// insertnode->data = val?  call copy assignment constructor
			((pos.node)->prev)->next = insertnode;
			insertnode->prev = (pos.node)->prev;
			insertnode->next = pos.node;
			(pos.node)->prev = insertnode;
			
			++count;
			return iterator(insertnode);
		}

		/// <summary>
		/// remove the first node
		/// </summary>
		void pop_front()
		{
			erase(begin());
		}

		/// <summary>
		/// remove the last node
		/// </summary>
		void pop_back()
		{
			iterator tmp = end();
			erase(--tmp);
		}
		/// <summary>
		/// insert a node before begin
		/// </summary>
		void push_front(const T& val)
		{
			insert(begin(),val);
		}

		/// <summary>
		/// insert a node before end
		/// </summary>
		void push_back(const T& val)
		{
			insert(end(),val);
		}

		/// <summary>
		/// get the first node value
		/// </summary>
		reference front()
		{
			return *begin();
		}

		/// <summary>
		/// get the last node value
		/// </summary>
		reference back()
		{
			iterator tmp = end();
			--tmp;
			return *tmp;
		}

		void remove(const T& val)
		{
			iterator first = begin();
			while(first!=end())
			{
				if(*first==val)
				{
					erase(first);
					break;
				}
				++first;
			}
		}

		/// <summary>
		/// sort & unique
		/// </summary>
		void uniqueall()
		{
			this->sort2gyue();
			this->unique();
		}

		/// <summary>
		/// remove the adjacent & repeated element
		/// </summary>
		void unique()
		{
			iterator first = begin();
			iterator last = end();
			while(first != last)
			{
				iterator next = first;
				++next;
				if((*first)==(*next))
				{
					erase(next);
				}
				else
				{
					first = next;
				}
			}
		}

		/// <summary>
		/// clear all nodes
		/// </summary>
		void clear()
		{
			pnode tmp = head->next;
			while(tmp !=head )
			{
				pnode delnode = tmp;
				tmp = tmp->next;
				Destruct(&delnode->data);
				PutNode(delnode);
			}
			head->next = head;
			head->prev = head;
			count = 0;
		}
	protected:
		/// <summary>
		/// transfer nodes among in [first,last) to before of pos
		/// </summary>
		/// <param name="pos">position will be used to insert</param>
		/// <param name="first">the first iterator used to be insert</param>
		/// <param name="last">the next position of the last node iterator</param>
		/// <returns>void</returns>
		void transfer(iterator pos,iterator first,iterator last)
		{
			if(first==last) return;
			((first.node)->prev)->next = last.node; // linked
			((last.node)->prev)->next = pos.node;
			((pos.node)->prev)->next = first.node;
			pnode tmp = (pos.node)->prev;
			(pos.node)->prev = (last.node)->prev;
			(last.node)->prev = (first.node)->prev;
			(first.node)->prev = tmp;
		}

		/// <summary>
		/// reverse the whole list
		/// 1. put a node before begin one by one (from the next of begin)
		/// </summary>
		/// <returns>void</returns>
		void reverse_dft()
		{
			if(this->count<2) return;
			iterator first=begin();
			++first;
			while(first!=end())
			{
				iterator xbegin = first;
				iterator yend = ++first;
				transfer(begin(),xbegin,yend);
			}
		}

	protected:
		/// <summary>
		/// splice lst to before of position(lst must be different with list of position)
		/// </summary>
		/// <param name="pos">position will be used to insert</param>
		/// <param name="lst">another list different with pos's list</param>
		/// <returns>void</returns>
		void splice(iterator pos,Self& lst)
		{
			if(lst.IsEmpty()) return;
			transfer(pos,lst.begin(),lst.end());
			this->count+=lst.count; //lst.GetCount();   正确  
			lst.count=0;            //lst.SetCount(0);
		}

		/// <summary>
		/// splice i of lst to before of pos
		/// </summary>
		/// <param name="pos">position will be used to splice</param>
		/// <param name="i">one iterator of list used to splice</param>
		/// <returns>void</returns>
		void splice(iterator pos,iterator i,Self& lst)
		{
			iterator lastOfi = i; // = must be deep copy
			++lastOfi;
			if( pos == i || pos == lastOfi) return;
			transfer(pos,i,lastOfi);
			++count;
			--lst.count;
		}

		/// <summary>
		/// splice 
		/// </summary>
		/// <param name="pos">position will be used to splice</param>
		/// <param name="first"></param>
		/// <param name="last"></param>
		/// <returns>void</returns>
		void splice(iterator pos,iterator first,iterator last,Self&)
		{
			transfer(pos,first,last);
		}


		template<class Comparer> 
		void merge(Self& x,Comparer _cmp)
		{
			iterator first1 = begin();
			iterator last1 = end();
			iterator first2 = x.begin();
			iterator last2 = x.end();
			while (first1 != last1 && first2 != last2)
			{
				if (_cmp(*first2 ,*first1)) 
				{
					iterator next = first2;
					transfer(first1, first2, ++next);
					first2 = next;
				}
				else
				{
					++first1;
				}
			}
			if (first2 != last2)
			{
				transfer(last1, first2, last2);
			}
			count += x.count;
			x.count= 0;
		}

		void merge(Self& x)
		{
			iterator first1 = begin();
			iterator last1 = end();
			iterator first2 = x.begin();
			iterator last2 = x.end();
			while (first1 != last1 && first2 != last2)
			{
				if (*first2 < *first1) 
				{
					iterator next = first2;
					transfer(first1, first2, ++next);
					first2 = next;
				}
				else
				{
					++first1;
				}
			}
			if (first2 != last2)
			{
				transfer(last1, first2, last2);
			}
			count += x.count;
			x.count= 0;
		}

		/// <summary>
		/// swap this & x 
		/// (!including with all data member!) 
		/// </summary>
		void swap(Self& x) 
		{
			//std::swap(head,x.head);
			//std::swap(count,count);
			Gswap(head, x.head);
			Gswap(count, x.count);
		}

	protected:
		/// <summary>
		/// another merge version 
		/// </summary>
		/// <param name="Self">item list used to merge with this</param>
		void mergeX(Self& lst)
		{
			iterator first1 = begin();
			iterator last1 =end();
			iterator first2 = lst.begin();
			iterator last2 = lst.end();
			while(first1 !=last1 && first2 != last2)
			{
				if(*first1>*first2)
				{
					iterator first2Next = first2;
					transfer(first1,first2,++first2Next);
					first2 = first2Next;
				}
				else
				{
					++first1;
				}
			}

			if(first1== last1)
			{
				transfer(first1,first2,last2);
			}
			this->count+=lst.count;
			lst.count = 0;
		}

		/// <summary>
		/// default sort algorithm in stl  ()
		/// !!!Make an unnecessary complicated algorithm to make all confused!
		/// Please refere to the variant alogrithm sort2gyue
		/// </summary>
		void sort_4list_sgi_stl_version()
		{
			if (count < 2) return;
			Self carry;
			Self counterX[64];
			int fill = 0;         // counterX下标控制变量 
			while (!IsEmpty()) 
			{
				carry.splice(carry.begin(), begin(),*this);
				int i = 0;        // 临时的counterX下标控制变量，确定i不会超过64吗？

				                  // 下面这个while循环是将counterX[i]及其之后的非空集合合并到carry
				while(i < fill && !counterX[i].IsEmpty()) 
				{
					counterX[i].merge(carry);   // 将carrry合并到counterX[i],counterX[i]为有序序列
					carry.swap(counterX[i++]);  // counterX[i]与carry交换
				}
				carry.swap(counterX[i]);        // counterX[i]缓存当最新的计算结果
				if (i == fill)                  //表明只有当前的counterX[i]存在数据,fill后移
				{
					++fill;
				}
			} 
			// 将counterX[fill-1]之前的所有数据merge,即可获得最终已排好序数据
			for (int i = 1; i < fill; ++i)
			{
				counterX[i].merge(counterX[i-1]);
			}
			this->swap(counterX[fill-1]);
		}
		/// <summary>
		/// Variant version of merge sort
		/// 1. splice the first node to carry
		/// 2. merge carry to result (result is a ordered sequence)
		/// 3. goto step 1 until count==0
		/// 4. swap result & this
		/// </summary>
		void sort2gyue()
		{
			if (count < 2) return;
			Self carry;	// tmp
			Self result; // tmp result sequence sorted
			while (!IsEmpty()) 
			{
				carry.splice(carry.begin(), begin(),*this); // get the first node to splice to carry
				result.mergeX(carry);                        // merge to result (result is ordered sequence)
			}
			swap(result);									// swap ordered sequence with this
		}

		/// <summary>
		/// Variant version of merge sort
		/// 1. splice the first node to carry
		/// 2. merge carry to result (result is a ordered sequence)
		/// 3. goto step 1 until count==0
		/// 4. swap result & this
		/// </summary>
		template<class Comparer>
		void sort2gyue(Comparer _cmp)
		{
			if (count < 2) return;
			Self carry;	// tmp
			Self result; // tmp result sequence sorted
			while (!IsEmpty()) 
			{
				// get the first node to splice to carry
				carry.splice(carry.begin(), begin(),*this); 
				 // merge to result (result is ordered sequence)
				result.merge(carry,_cmp);                    
			}
			// swap ordered sequence with this
			swap(result);									
		}

	public:
		Self operator=(const Self& x)
		{
			if (this != &x) 
			{
				iterator first1 = begin();
				iterator last1 = end();
				iterator first2 = x.begin();
				iterator last2 = x.end();
				while (first1 != last1 && first2 != last2) *first1++ = *first2++;
				if (first2 == last2)
					erase(first1, last1);
				else
					insert(last1, first2, last2);
			}
			return *this;
		}

	};


	template<class T>
	class ListIterator
	{
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef _list_node_<T>* pnode;
		typedef ListIterator<T> iterator;

		typedef bidirectional_iterator_tg iterator_category;
		typedef ptrdiff_t difference_type;
		typedef difference_type distance_type;
	public:
		pnode node;
	public:
		ListIterator(){ }
		ListIterator(pnode p) : node(p){ }
	public:
		reference operator*()const
		{
			return node->data;
		}
		iterator& operator++()
		{
			node = node->next;
			return *this;
		}
		iterator operator++(int)
		{
			iterator tmp = *this;
			++(*this);
			return tmp;
		}
		iterator& operator--()
		{
			node = node->prev;
			return *this;
		}
		iterator operator--(int)
		{
			iterator tmp = *this;
			--(*this);
			return tmp;
		}
		bool operator==(const iterator& x)const
		{
			return x.node==node;
		}
		bool operator!=(const iterator& x)const
		{
			return x.node!=node;
		}
	};


	template<class T>
	class ConstListIterator
	{
	public:
		typedef T value_type;
		typedef const T* pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef _list_node_<T>* pnode;
		typedef ListIterator<T> iterator;
		typedef ConstListIterator<T> citerator;

		typedef bidirectional_iterator_tg iterator_category;
		typedef ptrdiff_t difference_type;
		typedef difference_type distance_type;
	public:
		pnode node;
	public:
		ConstListIterator(){ }
		ConstListIterator(pnode p) : node(p){ }
		ConstListIterator(const iterator& x) : node(x.node){ }
	public:
		pointer operator->()const
		{
			return &(operator*());
		}
		const_reference operator*()const
		{
			return node->data;
		}
		citerator& operator++()
		{
			node = node->next;
			return *this;
		}
		citerator operator++(int)
		{
			iterator tmp = *this;
			++(*this);
			return tmp;
		}
		citerator& operator--()
		{
			node = node->prev;
			return *this;
		}
		citerator operator--(int)
		{
			iterator tmp = *this;
			--(*this);
			return tmp;
		}
		bool operator==(const citerator& x)const
		{
			return x.node==node;
		}
		bool operator!=(const citerator& x)const
		{
			return x.node!=node;
		}
	};
	
	template<class T,class R,class P>
	class LIterator
	{
	public:
		typedef T value_type;
		typedef P pointer;
		typedef R reference;
		
		typedef LIterator<T,R,P> self;
		typedef LIterator<T,T&,T*> iterator;
		typedef LIterator<T,const T&,const T*> const_iterator;

		typedef bidirectional_iterator_tg iterator_category;
		typedef ptrdiff_t difference_type;
		typedef difference_type distance_type;

		typedef _list_node_<T>* pnode;
	public:
		pnode node;
	public:
		LIterator(){ }
		LIterator(pnode p) : node(p){ }
		LIterator(const iterator& x) : node(x.node){ }
	public:
		reference operator*()const
		{
			return node->data;
		}
		pointer operator->()const
		{
			return &(operator*());
		}
		self& operator++()
		{
			node = node->next;
			return *this;
		}
		self operator++(int)
		{
			iterator tmp = *this;
			++(*this);
			return tmp;
		}
		self& operator--()
		{
			node = node->prev;
			return *this;
		}
		self operator--(int)
		{
			iterator tmp = *this;
			--(*this);
			return tmp;
		}
		bool operator==(const self& x)const
		{
			return x.node==node;
		}
		bool operator!=(const self& x)const
		{
			return x.node!=node;
		}
	};

	template<class T>
	struct _list_node_
	{
		_list_node_* next;
		_list_node_* prev;
		T data;
	};
}
#endif
/*
 * List.h  
 * Written by vivitue on Jan. 5th 2015. 
 * v1.0.0
 * Updated Sep. 25th 2017
 * all end.
*/