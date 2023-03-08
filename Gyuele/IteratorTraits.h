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
#ifndef _VIVITUE_ITERATORTRAITS_H_
#define _VIVITUE_ITERATORTRAITS_H_
#include "Iterator.h"
namespace STL1
{
	template<class T>
	struct ___iterator_traits
	{
		typedef typename T::iterator_category iterator_category;
		typedef typename T::value_type value_type;
		typedef typename T::difference_type difference_type;
		typedef difference_type distance_type;
		typedef typename T::pointer pointer;
		typedef typename T::reference reference;
	};
	template<class T>
	struct ___iterator_traits<T*>
	{
		typedef random_access_iterator_tg iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef difference_type distance_type;
		typedef T* pointer;
		typedef T& reference;
	};
	template<class T>
	struct ___iterator_traits<const T*>
	{
		typedef random_access_iterator_tg iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef difference_type distance_type;
		typedef T* pointer;
		typedef T& reference;
	};
}
#endif