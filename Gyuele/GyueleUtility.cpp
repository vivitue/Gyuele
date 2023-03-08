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

#include "StdAfx.h"
#include "GyueleUtility.h"

#include "XTraiter.h"
#include "Construct.h"
#include "Alloc.h"
#include "Algobase.h"
#include "Deque.h"
using namespace STL1;
// Test
// Tesing
template<typename T> bool greaterfunc(T t1,T t2);
template<typename T> void Printer(T &i)
{
	printf("%d ",i);
	//std::cout<<i<<" ";
}


template<class T>bool lesser(T t1,T t2)
{
	if(t1<t2)return true;
	return false;
}

bool (*cmper)(int t1,int t2);


template<typename T>bool greaterX(T t1,T t2)
{
	if(t1>t2)return true;
	return false;
}

void GyueleUtility::Test()
{
	DequeUtility2();		// ok
	DequeUtility();		// ok
	VectorUtility();	// OK
	ListUtility();		// ok
	AvlTreeTest();		// OK
	//RedblackTreeTest();	// OK

	//AllocatorTest();//OK
}
void GyueleUtility::AllocatorTest()
{

	int* p =NULL;
	STL1::Destruct(p);
	size_t t = alloc::RoundUpTest(127);
	void* pc = alloc1::Allocate(127);

	//size_t index = _default_alloc_template<false,0>::GetFreeListIndex(126);

	void * pcdft = TheSecondAllocator<false,0>::Allocate(126);
	

}

void GyueleUtility::ListUtility()
{

	int* pint = (int*)malloc(50*sizeof(int));
	int* pinttmp = pint;

	for(int i=0;i<50;i++)
	{
		*pinttmp = i;
		++pinttmp;
		int tmpresult = pinttmp-pint;
		int sub = tmpresult;
	}
	free(pint);
	
	List<int> lst;
	lst.Insert(lst.Begin(),2);
	lst.Insert(lst.Begin(),9);
	lst.Insert(lst.Begin(),9);
	lst.Insert(lst.Begin(),6);
	lst.Insert(lst.Begin(),1);
	lst.Insert(lst.Begin(),7);
	LIterator<int,const int&,const int*> citerator = lst.Cbegin();

	lst.Unique();
	std::cout<<"unsort : ";
	___for_each(lst.Begin(),lst.End(),Printer<int>);
	
	std::cout<<endl<<endl;
	std::cout<<"sorted : ";
    cmper = greaterX;

	lst.Sort();
	___for_each(lst.Begin(),lst.End(),Printer<int>);
	//lst.sort2gyue();

	List<int> lst2x;
	lst.Reverse();
	lst.Splice(lst.Begin(),lst2x);
	ListIterator<int> item = lst.Begin();
	lst.Erase(item);
	lst.Clear();
	lst.Merge(lst2x);
}
void GyueleUtility::DequeUtility2()
{
	Deque<int> dq;
	for(int i=0;i<10;i++)
	{
		dq.push_front(i);

	}
	___for_each(dq.begin(),dq.end(),Printer<int>);
	printf("\n");
	for(int i=0;i<50;i++)
	{
		int offset = rand()%(dq.size()-1);

		Deque<int>::iterator tmpite = dq.begin()+offset;
		Deque<int>::iterator tmpite2 = dq.insert(tmpite,offset+1,offset+100);
		printf("insert[dq] : [length=%d,offset=%d] tmpite2 :  %d\n ",dq.size(),offset,*tmpite2);
		___for_each(dq.begin(),dq.end(),Printer<int>);
		printf("\n");
	}

	dq.resize(dq.size()/2,0);
	printf("push_front[dq] : ");
	___for_each(dq.begin(),dq.end(),Printer<int>);
	printf("\n");
	dq.clear();
}
void GyueleUtility::DequeUtility()
{
	
	// test complted as follows
	Deque<int> d;
	for(int i=0;i<2000;i++)
	{
		d.push_front(i);
		std::cout<<"push_front : ";
		___for_each(d.begin(),d.end(),Printer<int>);
		std::cout<<endl;
	}
	std::cout<<"push_front : ";
	___for_each(d.begin(),d.end(),Printer<int>);
	std::cout<<endl;
	for(int i=10000;i>8000;i--)
	{
		d.push_back(i);
		std::cout<<"push_back : ";
		___for_each(d.begin(),d.end(),Printer<int>);
		std::cout<<endl;
	}

	std::cout<<"push_back : ";
	___for_each(d.begin(),d.end(),Printer<int>);
	std::cout<<endl;
	for(int i=0;i<2000;i++)
	{
		std::cout<<"pop_front : "<<d.front()<< " "<<endl;;
		d.pop_front();
		___for_each(d.begin(),d.end(),Printer<int>);
		std::cout<<endl;
	}
	std::cout<<endl;
	for(int i=0;i<2000;i++)
	{
		std::cout<<"pop_back : "<<d.back()<< " "<<endl;;
		d.pop_back();
		___for_each(d.begin(),d.end(),Printer<int>);
		std::cout<<endl;
	}
	std::cout<<endl;
}
void GyueleUtility::VectorUtility()
{
	Vector<int> v;
	for(int i=0;i<10;i++)
	{
		v.PushBack(i);
	}

	Vector<int> v2;
	v2.PushBack(12);
	 v=v2;

	Vector<int>::reverse_iterator rite = v.RBegin();

	std::cout<<"push_back : ";

	___for_each(v.Begin(),v.End(),Printer<int>);

	std::cout<<endl;
	std::cout<<"insert 3 element before end : ";
	v.Insert(v.End(),3,10);

	___for_each(v.Begin(),v.End(),Printer<int>);

	std::cout<<endl;
	
	Vector<int>::iterator it2 = v.GetIteratorFrom(2);//v[2];
	std::cout<<"value of it2(iterator it2) : ";
	Printer(*it2);
	std::cout<<endl;
	std::cout<<"insert 4 element before it2 : ";
	v.Insert(it2,4,41);

	___for_each(v.Begin(),v.End(),Printer<int>);

	std::cout<<endl;
	std::cout<<"erase 1 element in it2 : ";
	it2 = v.GetIteratorFrom(2);
	v.Erase(it2);
	___for_each(v.Begin(),v.End(),Printer<int>);
	std::cout<<endl;
	
	std::cout<<"erase 2 element [it2,it2+3) : ";
	it2 = v.GetIteratorFrom(2);
	v.Erase(it2,it2+3);
	___for_each(v.Begin(),v.End(),Printer<int>);
	std::cout<<endl;

	std::cout<<"resize2 : ";
	v.Resize(20,20,1);
	___for_each(v.Begin(),v.End(),Printer<int>);
	std::cout<<endl;
	std::cout<<"resize : ";
	v.Resize(50,50);
	___for_each(v.Begin(),v.End(),Printer<int>);
	std::cout<<endl;

	v.Clear();
}
void GyueleUtility::AvlTreeTest()
{
	//size_t t =ins.RoundUp(128);
	int maxcounter =1000000;
	DWORD ticker = GetTickCount();
	DWORD elapsed =0;

	AvlTree<int,int> avl;
	avl.Insert(6);

	avl.Insert(8);
	avl.Insert(17);
	avl.Insert(37);
	avl.Insert(9);
	avl.Insert(26);
	avl.Insert(25);
	avl.Insert(1);
	avl.Insert(11);
	avl.Insert(2);
	avl.Insert(12);
	avl.Insert(5);
	avl.Insert(10);
	avl.Insert(20);
	avl.Insert(3);
	avl.Insert(4);
	avl.Insert(21);
	avl.Insert(19);
	avl.Insert(18);
	avl.Insert(30);
	avl.Insert(7);

	printf("AvlTree : Regular iterator traverse Cnt = %d: \n",avl.GetCounter());
	___for_each(avl.Begin(),avl.End(),Printer<int>);
	printf("\n");
		
	avl.Delete(6);
	avl.Delete(8);
	avl.Delete(17);
	avl.Delete(37);
	avl.Delete(9);
	avl.Delete(26);
	avl.Delete(25);
	avl.Delete(1);
	avl.Delete(11);
	avl.Delete(2);
	avl.Delete(12);
	avl.Delete(5);
	avl.Delete(10);
	avl.Delete(20);
	avl.Delete(3);
	avl.Delete(4);
	avl.Delete(21);
	avl.Delete(19);
	avl.Delete(18);
	avl.Delete(30);
	avl.Delete(7);

	printf("AvlTree : Delete finished!  Cnt = %d: \n",avl.GetCounter());

	ticker = GetTickCount();
	vector<int> instcollection;
	for(int i=0;i<maxcounter;i++)
	{
		instcollection.push_back(i);
	}

	elapsed=GetTickCount()-ticker;

	printf("AvlTree : vector pushback finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	ticker = GetTickCount();
	random_shuffle(instcollection.begin(),instcollection.end());
	elapsed=GetTickCount()-ticker;
	printf("AvlTree : vector random_shuffle finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);


	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		avl.Insert(instcollection[i]);
	}

	elapsed=GetTickCount()-ticker;
	printf("AvlTree : AvlTree Insert finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		avl.Delete(instcollection[i]);
	}

	elapsed=GetTickCount()-ticker;
	printf("AvlTree : AvlTree Delete finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);
	printf("\n");

	set<int> avlst;

	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		avlst.insert(instcollection[i]);
	}
	elapsed=GetTickCount()-ticker;
	printf("AvlTree : Set Insert finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		avlst.erase(instcollection[i]);
	}
	elapsed=GetTickCount()-ticker;
	printf("AvlTree : Set erase finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	printf("\n");

	ticker = GetTickCount();
	instcollection.clear();
	elapsed=GetTickCount()-ticker;
	printf("AvlTree : Vector clear finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	printf("\n");
}
void GyueleUtility::RedblackTreeTest()
{
	int maxcounter =1000000;
	DWORD ticker = GetTickCount();
	DWORD elapsed =0;

	RedblackTree<int,int> rb;
	rb.Insert(6);

	rb.Insert(8);
	rb.Insert(17);
	rb.Insert(37);
	rb.Insert(9);
	rb.Insert(26);
	rb.Insert(25);
	rb.Insert(1);
	rb.Insert(11);
	rb.Insert(2);
	rb.Insert(12);
	rb.Insert(5);
	rb.Insert(10);
	rb.Insert(20);
	rb.Insert(3);
	rb.Insert(4);
	rb.Insert(21);
	rb.Insert(19);
	rb.Insert(18);
	rb.Insert(30);
	rb.Insert(7);

	printf("Regular iterator traverse Cnt = %d: \n",rb.GetCounter());
	___for_each(rb.Begin(),rb.End(),Printer<int>);
	printf("\n");
		
	rb.Delete(6);
	rb.Delete(8);
	rb.Delete(17);
	rb.Delete(37);
	rb.Delete(9);
	rb.Delete(26);
	rb.Delete(25);
	rb.Delete(1);
	rb.Delete(11);
	rb.Delete(2);
	rb.Delete(12);
	rb.Delete(5);
	rb.Delete(10);
	rb.Delete(20);
	rb.Delete(3);
	rb.Delete(4);
	rb.Delete(21);
	rb.Delete(19);
	rb.Delete(18);
	rb.Delete(30);
	rb.Delete(7);

	printf("Delete finished!  Cnt = %d: \n",rb.GetCounter());

	ticker = GetTickCount();
	vector<int> instcollection;
	for(int i=0;i<maxcounter;i++)
	{
		instcollection.push_back(i);
	}

	elapsed=GetTickCount()-ticker;

	printf("vector pushback finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	ticker = GetTickCount();
	random_shuffle(instcollection.begin(),instcollection.end());
	elapsed=GetTickCount()-ticker;
	printf("vector random_shuffle finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);


	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		rb.Insert(instcollection[i]);
	}

	elapsed=GetTickCount()-ticker;
	printf("Redblack Insert finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		rb.Delete(instcollection[i]);
	}

	elapsed=GetTickCount()-ticker;
	printf("Redblack Delete finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);
	printf("\n");

	set<int> st;

	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		st.insert(instcollection[i]);
	}
	elapsed=GetTickCount()-ticker;
	printf("Set Insert finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		st.erase(instcollection[i]);
	}
	elapsed=GetTickCount()-ticker;
	printf("Set erase finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	printf("\n");

	ticker = GetTickCount();
	instcollection.clear();
	elapsed=GetTickCount()-ticker;
	printf("Vector clear finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	printf("\n");
	/*RedblackTree<int,int>::Iterator it(rb.GetRoot(),rb.GetNil());
	
	it = rb.Begin();
	*it=10;

	it++;
	RedblackTree<int,int>::Iterator it2=it;
	if(it2==it)
	{
		++it;
		int k = *it;
		*it=5;
		___for_each(rb.Begin(),rb.End(),Print<int>);

	}*/
}

void GyueleUtility::BSTTreeTest()
{
	system("mode con cols=120 lines=5000  ");
	printf("\r\n\r\nRedblackTree & AvlTree operation test is beginning........\n\n");
	int maxcounter =1000000;
	DWORD ticker = GetTickCount();
	DWORD elapsed =0;

	RedblackTree<int,int> rb;
	rb.Insert(6);

	rb.Insert(8);
	rb.Insert(17);
	rb.Insert(37);
	rb.Insert(9);
	rb.Insert(26);
	rb.Insert(25);
	rb.Insert(1);
	rb.Insert(11);
	rb.Insert(2);
	rb.Insert(12);
	rb.Insert(5);
	rb.Insert(10);
	rb.Insert(20);
	rb.Insert(3);
	rb.Insert(4);
	rb.Insert(21);
	rb.Insert(19);
	rb.Insert(18);
	rb.Insert(30);
	rb.Insert(7);

	printf("RedblackTree : Regular iterator traverse TotalCnt = %d: \n",rb.GetCounter());
	___for_each(rb.Begin(),rb.End(),Printer<int>);
	printf("\n");
		
	rb.Delete(6);
	rb.Delete(8);
	rb.Delete(17);
	rb.Delete(37);
	rb.Delete(9);
	rb.Delete(26);
	rb.Delete(25);
	rb.Delete(1);
	rb.Delete(11);
	rb.Delete(2);
	rb.Delete(12);
	rb.Delete(5);
	rb.Delete(10);
	rb.Delete(20);
	rb.Delete(3);
	rb.Delete(4);
	rb.Delete(21);
	rb.Delete(19);
	rb.Delete(18);
	rb.Delete(30);
	rb.Delete(7);

	elapsed = GetTickCount()-ticker;
	printf("RedblackTree : Delete finished!  LeftCnt = %d  Elapsed = %dms: \n",rb.GetCounter(),elapsed);

	ticker = GetTickCount();
	vector<int> instcollection;
	for(int i=0;i<maxcounter;i++)
	{
		instcollection.push_back(i);
	}

	elapsed=GetTickCount()-ticker;

	printf("RedblackTree : vector pushback finished!  TotalCnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	ticker = GetTickCount();
	random_shuffle(instcollection.begin(),instcollection.end());
	elapsed=GetTickCount()-ticker;
	printf("RedblackTree : vector random_shuffle finished!  TotalCnt = %d Elapsed = %dms \n",maxcounter,elapsed);


	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		rb.Insert(instcollection[i]);
	}

	printf("\n--------------------------------------------------------------------------\n");
	elapsed=GetTickCount()-ticker;
	printf("RedblackTree : Redblack Insert finished!  TotalCnt = %d Elapsed = %dms \n",rb.GetCounter(),elapsed);

	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		rb.Delete(instcollection[i]);
	}

	elapsed=GetTickCount()-ticker;
	printf("RedblackTree : Redblack Delete finished!  LeftCnt = %d Elapsed = %dms \n",rb.GetCounter(),elapsed);
	printf("--------------------------------------------------------------------------\n");
	printf("\n");

	set<int> st;

	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		st.insert(instcollection[i]);
	}
	elapsed=GetTickCount()-ticker;
	printf("RedblackTree : Set Insert finished!  TotalCnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		st.erase(instcollection[i]);
	}
	elapsed=GetTickCount()-ticker;
	printf("RedblackTree : Set erase finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	printf("RedblackTree : RedblackTree test finished!\n");

	ticker = GetTickCount();
	

	printf("\n\n");

	AvlTree<int,int> avl;
	avl.Insert(6);

	avl.Insert(8);
	avl.Insert(17);
	avl.Insert(37);
	avl.Insert(9);
	avl.Insert(26);
	avl.Insert(25);
	avl.Insert(1);
	avl.Insert(11);
	avl.Insert(2);
	avl.Insert(12);
	avl.Insert(5);
	avl.Insert(10);
	avl.Insert(20);
	avl.Insert(3);
	avl.Insert(4);
	avl.Insert(21);
	avl.Insert(19);
	avl.Insert(18);
	avl.Insert(30);
	avl.Insert(7);

	printf("AvlTree : Regular iterator traverse TotalCnt = %d: \n",avl.GetCounter());
	___for_each(avl.Begin(),avl.End(),Printer<int>);
	printf("\n");
		
	avl.Delete(6);
	avl.Delete(8);
	avl.Delete(17);
	avl.Delete(37);
	avl.Delete(9);
	avl.Delete(26);
	avl.Delete(25);
	avl.Delete(1);
	avl.Delete(11);
	avl.Delete(2);
	avl.Delete(12);
	avl.Delete(5);
	avl.Delete(10);
	avl.Delete(20);
	avl.Delete(3);
	avl.Delete(4);
	avl.Delete(21);
	avl.Delete(19);
	avl.Delete(18);
	avl.Delete(30);
	avl.Delete(7);

	printf("AvlTree : Delete finished!  LeftCnt = %d: \n",avl.GetCounter());

	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		avl.Insert(instcollection[i]);
	}

	elapsed=GetTickCount()-ticker;
	printf("\n--------------------------------------------------------------------------\n");
	printf("AvlTree : AvlTree Insert finished!  TotalCnt = %d Elapsed = %dms \n",avl.GetCounter(),elapsed);

	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		avl.Delete(instcollection[i]);
	}

	elapsed=GetTickCount()-ticker;
	printf("AvlTree : AvlTree Delete finished!  LeftCnt = %d Elapsed = %dms \n",avl.GetCounter(),elapsed);
	printf("--------------------------------------------------------------------------\n");
	printf("\n");

	set<int> avlst;

	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		avlst.insert(instcollection[i]);
	}
	elapsed=GetTickCount()-ticker;
	printf("AvlTree : Set Insert finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	ticker = GetTickCount();
	for(int i=0;i<maxcounter;i++)
	{
		avlst.erase(instcollection[i]);
	}
	elapsed=GetTickCount()-ticker;
	printf("AvlTree : Set erase finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	printf("\n");

	ticker = GetTickCount();
	instcollection.clear();
	elapsed=GetTickCount()-ticker;
	printf("AvlTree : Vector clear finished!  Cnt = %d Elapsed = %dms \n",maxcounter,elapsed);

	printf("\n");
}


GyueleUtility::GyueleUtility(void)
{
}

GyueleUtility::~GyueleUtility(void)
{
}
