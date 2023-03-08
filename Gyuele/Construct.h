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
#ifndef _VIVITUE_CONSTRUCT_H_
#define _VIVITUE_CONSTRUCT_H_
#include <new.h>
#include "XTraiter.h"
namespace STL1
{
	template<class T1,class T2>
	inline void Construct(T1* p,const T2& val)
	{
		new (p) T1(val);// placement new
	}

	template<class T>
	inline void Construct(T* p)
	{
		new (p) T1();
	}

	template<class T>
	inline void Destruct(T* p)
	{
		DestructHelper(p);
	}

	template<class ForwardIterator,class T>
	inline void Destruct(ForwardIterator first,ForwardIterator last,T* p)
	{
		DestructHelper(first,last,___value_traiter(p,has_trivial_destructor_tg()));
	}
	template<class T>
	inline void DestructHelper(T *p)
	{

		DestructHelper(p,___value_traiter(p,has_trivial_destructor_tg()));
	}

	template<class T>
	inline void DestructHelper(T *p,const _false_type&)
	{
		p->~T();
	}
	template<class T>
	inline void DestructHelper(T* p,const _true_type&){ }
    
	template<class ForwardIterator>
	inline void DestructHelper(ForwardIterator first,ForwardIterator last,const _false_type&)
	{
		for( ;first!=last;++first)
		{
			DestructHelper(&(*first),_false_type());
		}
	}
	template<class ForwardIterator>
	inline void DestructHelper(ForwardIterator,ForwardIterator,const _true_type&){ }

	template<class ForwardIterator,class T>
	ForwardIterator uninitialized_fill_n(ForwardIterator first,size_t n,const T& val)
	{
		while(n--)
		{
			Construct(&(*first),val);
			++first;
		}
		return first;
	}

	/// <summary>
	/// copy It1's elements among [first,last) to [x,result) of It2's
	/// </summary>
	/// <param name="first"></param>
	/// <param name="last"></param>
	/// <param name="result"></param>
	/// <returns>iterator of It2</returns>
	template<class BidirectionalIterator1,class BidirectionalIterator2>
	BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
		BidirectionalIterator1 last,
		BidirectionalIterator2 result)
	{
		while(first != last)
		{
			--result;
			--last;
			*result = *last; // *--result = *--last;
		}
		return result;
	}
	

	/// <summary>
	/// copy It1's elements among [first,last) to [result,x) of It2's
	/// </summary>
	/// <param name="first"></param>
	/// <param name="last"></param>
	/// <param name="result"></param>
	/// <returns>iterator of It2</returns>
	template<class BidirectionalIterator1,class BidirectionalIterator2>
	BidirectionalIterator2 copy_forward(BidirectionalIterator1 first,
		BidirectionalIterator1 last,
		BidirectionalIterator2 result)
	{
		return uninitialized_copy(first,last,result);
	}
	/// <summary>
	/// copy It1's elements among [first,last) to [result,x) of It2's
	/// </summary>
	/// <param name="first"></param>
	/// <param name="last"></param>
	/// <param name="result"></param>
	/// <returns>iterator of It2</returns>
	template<class InputIterator,class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first,
		InputIterator last,
		ForwardIterator result)
	{
		while(first!=last)
		{
			*result = *first;
			++result;
			++first; // *result++=*first++;
		}
		return result;
	}
	template<class ForwardIterator,class T>
	inline void fill(ForwardIterator first,ForwardIterator last,T& val)
	{
		while(first!=last)
		{
			*first = val;
			++first; // *first++=val;
		}
	}

}
#endif