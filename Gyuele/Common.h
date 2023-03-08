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
#ifndef _VIVITUE_COMMON_H_
#define _VIVITUE_COMMON_H_
namespace STL1
{
#define _VIVITUE_STL_BEGIN_NAMESPACE_ namespace VISTL{
#define _VIVITUE_STL_END_NAMESPACE_ }
#define _TEMPLATE_FULL_SPECIALIZATION_ template<>
#define USIZE size_t
#define _BAD_MEM_EXCEPTION_ throw exception("out of memory")
#define _DEFAULT_EXTENTION_NODE 20
#define _IS_ALLOC_THREADSAFE_ALIVE_ false
#define _STL1_PROTECTED_ public         // set this flag protected
#define _STL1_PRIVATE_ private
#define _STL1_PUBLIC_ public
#define _STL1_STD_NULL_ 0
#define _STL1_STD_MAXDQ_BYTESZ 16

#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define _USING_STD_TEST_FLAG_ 9

	typedef void (*SetHandler)();

#if (_USING_STD_TEST_FLAG_ >10)
#define _USING_STD_TEST_
#endif
}
#endif