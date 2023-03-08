#pragma once
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
* GyueleUtility v1.0.0.0 [C++ version]
* Written by vivitue based upon the specification on Jan. 5th 2015
*/

#include "stdafx.h"

#include<windows.h>
#include<assert.h>
#include "RedblackTree.h"
#include "AvlTree.h"
#include "List.h"
#include "Vector.h"
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

// Tree operation test
template<typename T> void Printer(T &i);

class GyueleUtility
{
public:
	GyueleUtility();
	~GyueleUtility();
public:
	void AvlTreeTest();
	void RedblackTreeTest();
	void BSTTreeTest();
	void ListTestingsings();
	void AllocatorTest();
	void ListUtility();
	void VectorUtility();
	void DequeUtility();
	void DequeUtility2();
public:
	void Test();
};
