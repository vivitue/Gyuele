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

#ifndef _VIVITUE_ITERATOR_H_
#define _VIVITUE_ITERATOR_H_

namespace STL1
{
	struct input_iterator_tg{ };
	struct output_iterator_tg{ };
	struct forward_iterator_tg{ };
	struct bidirectional_iterator_tg{ };
	struct random_access_iterator_tg{ };
	template<class T,class Distance> struct input_iterator{ };
	template<class T,class Distance> struct output_iterator{ };
	template<class T,class Distance> struct forward_iterator{ };
	template<class T,class Distance> struct bidirectional_iterator{ };
	template<class T,class Distance> struct random_access_iterator{ };

	template<class T,class Distance>
	inline input_iterator_tg iterator_category(const input_iterator<T,Distance>& x)
	{
		return input_iterator_tg();
	}

	template<class T,class Distance>
	inline output_iterator_tg iterator_category(const output_iterator<T,Distance>& x)
	{
		return output_iterator_tg();
	}
	template<class T,class Distance>
	inline forward_iterator_tg iterator_category(const forward_iterator<T,Distance>& x)
	{
		return forward_iterator_tg();
	}
	template<class T,class Distance>
	inline bidirectional_iterator_tg iterator_category(const bidirectional_iterator<T,Distance>& x)
	{
		return bidirectional_iterator_tg();
	}
	template<class T,class Distance>
	inline random_access_iterator_tg iterator_category(const random_access_iterator<T,Distance>& x)
	{
		return random_access_iterator_tg();
	}

	template<class RandomAccessIterator,class T,class Reference,class Distance>
	class ___reverse_iterator
	{
	public:
		typedef ___reverse_iterator<RandomAccessIterator,T,Reference,Distance> Self;
		typedef RandomAccessIterator iterator;
		typedef Reference reference;
	protected:
		iterator current;
	public:
		___reverse_iterator(){ }
		___reverse_iterator(iterator i) : current(i) { }
		iterator base()
		{
			return current;
		}
		reference operator*()const
		{
			return *(current - 1);
		}
		Self& operator++()
		{
			--current;
			return *this;
		}
		Self operator++(int)
		{
			Self tmp = *this;
			--current;
			return tmp;
		}
		Self& operator--()
		{
			++current;
			return *this;
		}
		Self operator--(int)
		{
			Self tmp = *this;
			++current;
			return *this;
		}
		Self operator+(Distance n)
		{
			return Self(current - n);
		}
		Self& operator+=(Distance n)
		{
			current-=n;
			return *this;
		}
		Self operator-(Distance n)
		{
			return Self(current+n);
		}
		Self& operator-=(Distance n)
		{
			current+=n;
			return *this;
		}
		reference operator[](Distance n)
		{
			return *(*this + n)
		}
	};
}


#endif