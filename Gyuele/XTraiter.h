/*
* Copyright (c) 2015 vivitue
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
* and associated documentation files (the ¡°Software¡±),to deal in the Software without restriction, 
* including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial
* portions of the Software.

* THE SOFTWARE IS PROVIDED ¡°AS IS¡±, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT 
* NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*/
#ifndef _VIVITUE_XTRAITER_H_
#define _VIVITUE_XTRAITER_H_
#include "TypeTraits.h"
#include "IteratorTraits.h"
namespace STL1
{
	template<class T> 
	typename _type_traits<T>::has_trivial_default_constructor ___value_traiter(const T&,const has_trivial_default_constructor_tg&)
	{
		typedef typename _type_traits<T>::has_trivial_default_constructor tmp;
		return tmp();
	}
	template<class T> 
	typename _type_traits<T>::has_trivial_copy_constructor ___value_traiter(const T&,const has_trivial_copy_constructor_tg&)
	{
		typedef typename _type_traits<T>::has_trivial_copy_constructor tmp;
		return tmp();
	}
	template<class T> 
	typename _type_traits<T>::has_trivial_assignment_operator ___value_traiter(const T&,const has_trivial_assignment_operator_tg&)
	{
		typedef typename _type_traits<T>::has_trivial_assignment_operator tmp;
		return tmp();
	}
	template<class T> 
	typename _type_traits<T>::has_trivial_destructor ___value_traiter(const T&,const has_trivial_destructor_tg&)
	{
		typedef typename _type_traits<T>::has_trivial_destructor tmp;
		return tmp();
	}
	template<class T> 
	typename _type_traits<T>::is_POD_type ___value_traiter(const T&,const is_POD_type_tg&)
	{
		typedef typename _type_traits<T>::is_POD_type tmp;
		return tmp();
	}

	template<class Iterator>
	typename ___iterator_traits<Iterator>::iterator_category ___iterator_traiter(const Iterator&)
	{
		typedef typename ___iterator_traits<Iterator>::iterator_category tmp;
		return tmp();
	}
}
#endif