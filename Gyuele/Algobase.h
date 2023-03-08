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
 * Copyright (c) 2015,2017 vivitue
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

#ifndef _VIVITUE_ALGOBASE_H_
#define _VIVITUE_ALGOBASE_H_
namespace STL1
{
	template<class InputIterator,class Functor>
	Functor ___for_each(InputIterator first,InputIterator last,Functor f)
	{
		while(first != last) f(*first++);
		return f;
	}
	template<class Iterator,class Distance>
	inline void distance(Iterator first,
		Iterator last,
		Distance& n)	
	{
		___distance(first,last,n,___iterator_traiter(first));
	}
	template<class Iterator,class Distance>
	inline void ___distanceX(Iterator first,
		Iterator last,
		Distance& n)
	{
		while(first != last)
		{
			++first;
			++n;
		}
	}
	template<class BidirectionalIterator,class Distance>
	inline void ___distance(BidirectionalIterator first,
		BidirectionalIterator last,
		Distance& n,
		bidirectional_iterator_tg&)
	{
		___distanceX(first,last,n);
	}
	template<class ForwardIterator,class Distance>
	inline void ___distance(ForwardIterator first,
		ForwardIterator last,
		Distance& n,
		forward_iterator_tg&)
	{
		___distanceX(first,last,n);
	}
	template<class InputIterator,class Distance>
	inline void ___distance(InputIterator first,
		InputIterator last,
		Distance& n,
		input_iterator_tg&)
	{
		___distanceX(first,last,n);
	}
	template<class RandomAccessIterator,class Distance>
	inline void ___distance(RandomAccessIterator first,
		RandomAccessIterator last,
		Distance& n,
		random_access_iterator_tg&)
	{
		n +=last - first;
	}
	template<class Iterator,class Distance>
	inline void advance(Iterator& i,Distance n)
	{
		___advance(i,n,___iterator_traiter(i));
	}
	template<class InputIterator,class Distance>
	inline void ___advance(InputIterator& i,
		Distance n,
		input_iterator_tg)
	{
		while(n--) ++i;
	}
	template<class InputIterator,class Distance>
	inline void ___advance(InputIterator& i,
		Distance n,
		bidirectional_iterator_tg)
	{
		if(n>=0)
		{
			while(n--)++i;
		}
		else
		{
			while(n++)--i;
		}
	}
	template<class ForwardIterator,class Distance>
	inline void ___advance(ForwardIterator& i,
		Distance n,
		forward_iterator_tg)
	{
		while(n--) ++i;
	}
	template<class RandomAccessIterator,class Distance>
	inline void ___advance(RandomAccessIterator& i,
		Distance n,
		random_access_iterator_tg)
	{
		i+=n;
	}
	template <class InputIterator1,class InputIterator2,class Predicate>
	inline bool equal(InputIterator1 first1,InputIterator1 last1,
		InputIterator2 first2,InputIterator2 last2,Predicate cmp)
	{
		for( ; first1!=last1;++first1,++first2)
		{
			if(!cmp(*first1,*first2))return false;
		}
		return true;
	}
	template <class InputIterator1,class InputIterator2>
	inline bool equal(InputIterator1 first1,InputIterator1 last1,
		InputIterator2 first2)
	{
		for( ; first1!=last1;++first1,++first2)
		{
			if(*first1!=*first2)return false;
		}
		return true;
	}
	template<class T>
	inline void Gswap(T& t1,T& t2)
	{
		T t =t1;
		t1=t2;
		t2=t;
	}
	
}
#endif