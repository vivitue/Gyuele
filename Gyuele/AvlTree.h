/*
* Copyright (c) 2015 vivitue
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
* and associated documentation files (the “Software”),to deal in the Software without restriction, 
* including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial
* portions of the Software.

* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT 
* NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*/

#ifndef _VIVITUE_AVLTREE_CPLUSPLUS_H__
#define _VIVITUE_AVLTREE_CPLUSPLUS_H__
#include <malloc.h>
#include "Iterator.h"
#include "Common.h"

#ifdef _USING_STD_TEST_
#include<xutility>
using namespace std;
#endif

#define _THROW_BADMEMORY_EXCEPTION_AVLT(msg) throw exception(msg)

namespace STL1
{

	template<class T1,class T2=int>struct Avlnode;
	template<class TKey,class THeight=int>class AvlTree;
	template<class TKey,class TRefOfKey,class TPtrOfKey,class THeight=int>class AvlIterator;

	template<class TKey,class THeight>inline Avlnode<TKey,THeight>* GetPredecessor(Avlnode<TKey,THeight>* node,Avlnode<TKey,THeight>* nil);
	template<class TKey,class THeight>inline Avlnode<TKey,THeight>* GetSuccessor(Avlnode<TKey,THeight>* node,Avlnode<TKey,THeight>* nil);
	template<class TKey,class THeight>inline Avlnode<TKey,THeight>* GetMinimum(Avlnode<TKey,THeight>* node,Avlnode<TKey,THeight>* nil);
	template<class TKey,class THeight>inline Avlnode<TKey,THeight>* GetMaximum(Avlnode<TKey,THeight>* node,Avlnode<TKey,THeight>* nil);
	template<class TKey,class THeight>inline Avlnode<TKey,THeight>* RightRotate(Avlnode<TKey,THeight>* root,Avlnode<TKey,THeight>* node,Avlnode<TKey,THeight>* nil);
	template<class TKey,class THeight>inline Avlnode<TKey,THeight>* LeftRotate(Avlnode<TKey,THeight>* root,Avlnode<TKey,THeight>* node,Avlnode<TKey,THeight>* nil);

	// AvlTree
	template<class TKey,class THeight>
	class AvlTree
	{
	public:
		typedef TKey key_type;
		typedef TKey value_type;
		typedef TKey* pointer;
		typedef const TKey* const_pointer;
		typedef TKey& reference;
		typedef const TKey& const_reference;

		typedef THeight Height;
		typedef key_type Key;
		typedef value_type Value;
		typedef size_t Count;

		typedef Avlnode<Key,Height> Avlnode,*pavlnode,*PAvlnode,*pAvlnode;

		typedef AvlIterator<Key,reference,pointer,Height> Iterator;
		typedef AvlIterator<Key,const_reference,const_pointer,Height> ConstIterator;

	private:
		PAvlnode nuln;			// null pointer of Avlnode
	protected:
		PAvlnode PNIL;			// nil node pointer(please refer to Introduction to Algrothem)
		PAvlnode root;			// root of the avl tree
		Count counter;			// node counter

		Iterator beginIt;		// regular begin interator
		Iterator endIt;			// iterator of the past-of-end
		ConstIterator cbeginIt;	// const begin interator
		ConstIterator cendIt;
	public:
		const PAvlnode GetNil() const
		{
			return PNIL;
		}
		PAvlnode GetRoot() const
		{

			return root;
		}

		Iterator Begin()
		{
			PAvlnode min = GetMinimum(root,PNIL);
			return Iterator(min,PNIL);
		}
		Iterator End()
		{
			return endIt;
		}
		ConstIterator Begin() const
		{
			PAvlnode min = GetMinimum(root,PNIL);
			return ConstIterator(min,PNIL);
		}
		ConstIterator End() const
		{
			return cendIt;
		}

		ConstIterator Find(const Key& key)
		{
			PAvlnode node = SearchKey(key);
			return ConstIterator(node,PNIL);

		}
		int GetCounter()
		{
			return (int)this->counter;
		}
	protected:
		Iterator Find(const Key& key,int)
		{
			PAvlnode node = SearchKey(key);
			return Iterator(node,PNIL);
		}

	private:
		// fuction pointer & callback methods are for the realization of c++ event mechanism
		typedef enum Command{ MemoryReleaseRequest=0,ClientResponsibility }TestCmd;
		typedef bool (AvlTree<Key,Height>::*MemoryRequestedHandler)();// member function pointer declaration
		MemoryRequestedHandler memoryRequestedEvent;				  // event of out-of-memory (actually,this is a function pointer here)
		int (*MemoryHandler)(int);									  // normal function pointer
	private:
		// callback messagers for lacking memory process
		bool OnOutofMemory()
		{
			if(MemoryHandler!=0)
			{
				int result = MemoryHandler(MemoryReleaseRequest);
				if(result==0) return true;
			}
			return false;
		}

		void RegisterEvents()
		{
			memoryRequestedEvent=&AvlTree<Key,Height>::OnOutofMemory;
		}

		void AutoUnregisterEvents()
		{
			memoryRequestedEvent=0;
			MemoryHandler=0;
		}
	private:

		PAvlnode CreateAvlnode()
		{
			PAvlnode pavln = (PAvlnode)malloc(sizeof(Avlnode));
			if(pavln==nuln)
			{
				if(memoryRequestedEvent!=0)
				{
					bool isok = (this->*memoryRequestedEvent)();
					if(isok)
					{
						pavln = (PAvlnode)malloc(sizeof(Avlnode));
						if(pavln==nuln)
						{
							_THROW_BADMEMORY_EXCEPTION_AVLT("CreateAvlnode : out of memory!");
						}
					}
					else
					{
						_THROW_BADMEMORY_EXCEPTION_AVLT("CreateAvlnode : out of memory!");

					}
				}
				else
				{
					_THROW_BADMEMORY_EXCEPTION_AVLT("CreateAvlnode : out of memory!");
				}
			}
			pavln->height=0;
			pavln->key=-1;

			pavln->parent=PNIL;
			pavln->lchild=PNIL;
			pavln->rchild=PNIL;
			return pavln;
		}
		void PreInit()
		{
			counter =0;
			MemoryHandler=0;
			memoryRequestedEvent = (MemoryRequestedHandler)0;

			nuln = (PAvlnode)0;
			PNIL = (PAvlnode)malloc(sizeof(Avlnode));
			PNIL->key=-1;
			PNIL->height=-1;
			PNIL->lchild=PNIL;
			PNIL->rchild=PNIL;
			PNIL->parent=PNIL;

			root = PNIL;

			endIt = Iterator(PNIL,PNIL);
			cendIt = ConstIterator(PNIL,PNIL);
			RegisterEvents();

		}

		void Clear(PAvlnode rt)
		{
			if(rt!=PNIL)
			{
				Clear(rt->lchild);
				Clear(rt->rchild);
				free(rt);
				rt=PNIL;
			}
		}
	public:
		AvlTree()
		{
			PreInit();
		}
		~AvlTree()
		{
			Dispose();
		}
	public:
		void Clear()
		{
			Clear(root);
		}

		void Dispose()
		{
			AutoUnregisterEvents();
			Clear();
			if(PNIL!=nuln)
			{
				free(PNIL);
				PNIL=nuln;
			}
			counter=0;
		}
	public:
		// Event principle in C++  (for matching c++ new-handler)
		// Subscribe inner memory process event,TFunctor should be matched with the signature of MemoryHandler
		template<class TFunctor>
		void SubscribeMemoryRequestEvent(TFunctor handle)
		{
			MemoryHandler=handle;
		}
	public:
		int GetBlancefactor(PAvlnode node)
		{
			return (int)(node->lchild->height - node->rchild->height);
		}
	private:
		PAvlnode SearchKey(Key key)
		{
			PAvlnode top=root;
			while(top!=PNIL)
			{
				if(top->key<key)
				{
					top=top->rchild;
				}
				else if(top->key>key)
				{
					top=top->lchild;
				}
				else
				{
					break;
				}
			}
			return top;
		}

		// swap key of u & v
		void Transplant(PAvlnode u,PAvlnode v)
		{
			Key tk=u->key;

			u->key=v->key;
			v->key = tk;

		}
		void Swapheight(PAvlnode u,PAvlnode v)
		{
			Height h = u->height;
			u->height=v->height;
			v->height=h;
		}
		Height GetHeight(PAvlnode node)
		{
			return node->height;
		}
		void Cutleaf(PAvlnode deln)
		{
			bool b = (deln==deln->parent->lchild);
			if(b)
			{
				deln->parent->lchild = PNIL;
			}
			else
			{
				deln->parent->rchild = PNIL;
			}

		}

		PAvlnode GetBrother(PAvlnode node)
		{
			if(node==node->parent->lchild)
			{
				return node->parent->rchild;
			}
			return node->parent->lchild;
		}
		PAvlnode GetInsertParent(Key key)
		{
			PAvlnode prt = PNIL;
			PAvlnode top = root;
			while(top!=PNIL)
			{
				prt =top;
				if(top->key<key)
				{
					top=top->rchild;
				}
				else if(top->key>key)
				{
					top=top->lchild;
				}
				else
				{
					prt = PNIL; 
					break;
				}
			}
			return prt;
		}

		PAvlnode GetGrandfather(PAvlnode node)
		{
			return node->parent->parent;
		}
	private:
		void AvlnRightBrotherSecondaryFixUp(PAvlnode subroot)
		{
			PAvlnode  fixroot = subroot;

			int factor = GetBlancefactor(fixroot);
			if(factor!=1) return ;

			PAvlnode  fixnode = fixroot->lchild;//令其为左子树

			PAvlnode srtparent = fixroot->parent;
			fixroot->parent=PNIL;


			// 二次调整算法
			// fixnode为左孩子
			// 1. 如果 fixnode的平衡因子为-1则调整 ，此时fixnode = fixnode->rchild 调整指针下移一层 
			// 2. 如果fixnode的平衡因子为0或1，且fixnode等于initialfixnode，则程序退出
			//                                 且fixnode不等于initialfixnode，以fixnode.p右旋，//指针自动退至上一层

			// fixnode为右孩子
			// 1. 如果 fixnode的平衡因子为1则调整 ，此时fixnode = fixnode->lchild 调整指针下移一层 
			// 2. 如果fixnode的平衡因子为0或-1，且fixnode等于initialfixnode，则程序退出
			//                                  且fixnode不等于initialfixnode，以fixnode.p左旋 //指针自动退至上一层

			// 程序循环的条件是 fixnode.p等于传入根subroot
			while(true)
			{
				factor = GetBlancefactor(fixnode);
				if(fixnode==fixnode->parent->lchild)
				{
					if(factor==-1)
					{
						fixnode = fixnode->rchild;
					}
					else
					{
						if(fixnode->parent==fixroot)
						{
							break;
						}
						else
						{
							Swapheight(fixnode,fixnode->parent);// 交换fixnode与其父的高度然后以其父右旋
							fixroot = RightRotate(fixroot,fixnode->parent,PNIL);
						}
					}
				}
				else
				{
					if(factor==1)
					{
						fixnode=fixnode->lchild;
					}
					else
					{
						Swapheight(fixnode,fixnode->parent);// 交换fixnode与其父的深度然后以其父左旋
						fixroot = LeftRotate(fixroot,fixnode->parent,PNIL);

					}
				}
			}
			fixroot->parent = srtparent;
		}

		void AvlnLeftBrotherSecondaryFixUp(PAvlnode subroot)
		{
			PAvlnode  fixroot = subroot;

			int factor = GetBlancefactor(fixroot);
			if(factor!=-1) return ;

			PAvlnode  fixnode = fixroot->rchild;//令其为右子树

			PAvlnode srtparent = fixroot->parent;
			fixroot->parent=PNIL;

			// 二次调整算法
			// fixnode为左孩子
			// 1. 如果 fixnode的平衡因子为-1则调整 ，此时fixnode = fixnode->rchild 调整指针下移一层 
			// 2. 如果fixnode的平衡因子为0或1，且fixnode等于initialfixnode，则程序退出
			//                                 且fixnode不等于initialfixnode，以fixnode.p右旋，//指针自动退至上一层

			// fixnode为右孩子
			// 1. 如果 fixnode的平衡因子为1则调整 ，此时fixnode = fixnode->lchild 调整指针下移一层 
			// 2. 如果fixnode的平衡因子为0或-1，且fixnode等于initialfixnode，则程序退出
			//                                  且fixnode不等于initialfixnode，以fixnode.p左旋 //指针自动退至上一层

			// 程序循环的条件是 fixnode.p等于传入根subroot
			while(true)
			{
				factor = GetBlancefactor(fixnode);
				if(fixnode==fixnode->parent->rchild)
				{
					if(factor==1)
					{
						fixnode = fixnode->lchild;
					}
					else
					{
						if(fixnode->parent==fixroot)
						{
							break;
						}
						else
						{
							Swapheight(fixnode,fixnode->parent);// 交换fixnode与其父的高度然后以其父左旋
							fixroot = LeftRotate(fixroot,fixnode->parent,PNIL);
						}
					}
				}
				else
				{
					if(factor==-1)
					{
						fixnode=fixnode->rchild;
					}
					else
					{
						Swapheight(fixnode,fixnode->parent);// 交换fixnode与其父的深度然后以其父左旋
						fixroot = RightRotate(fixroot,fixnode->parent,PNIL);
					}
				}
			}
			fixroot->parent = srtparent;

		}
	private:
		void AvlnInsertFixUp(PAvlnode fixnd)
		{
			PAvlnode ret = root;
			PAvlnode fixnode = fixnd;
			PAvlnode g=GetGrandfather(fixnd);

			int bfactor =-1;
			while(fixnode->parent!=PNIL)
			{
				g=GetGrandfather(fixnode);
				bfactor = GetBlancefactor(g);	
				if(fixnode->parent == g->lchild)
				{
					if(bfactor==0)
					{
						++fixnode->parent->height;
						fixnode =fixnode->parent;
						if(fixnode->parent->parent==PNIL)
						{
							++fixnode->parent->height;
							break;
						}
					}
					else if(bfactor==1)
					{
						if(fixnode==fixnode->parent->lchild)
						{
							ret = RightRotate(ret,g,PNIL);
							++fixnode->parent->height;
							--g->height;
							break;
						}
						else
						{	
							Swapheight(fixnode,fixnode->parent);
							ret = LeftRotate(ret,fixnode->parent,PNIL);
							fixnode = fixnode->lchild;
						}
					}
					else
					{
						++fixnode->parent->height;
						break;
					}
				}
				else
				{
					if(bfactor==0)
					{
						++fixnode->parent->height;
						fixnode =fixnode->parent;
						if(fixnode->parent->parent==PNIL)
						{
							++fixnode->parent->height;
							break;
						}
					}
					else if(bfactor==1)
					{
						++fixnode->parent->height;
						break;
					}
					else
					{
						if(fixnode==fixnode->parent->lchild)
						{
							Swapheight(fixnode,fixnode->parent);
							ret = RightRotate(ret,fixnode->parent,PNIL);
							fixnode = fixnode->rchild;
						}
						else
						{
							ret = LeftRotate(ret,g,PNIL);
							++fixnode->parent->height;
							--g->height;
							break;
						}
					}
				}
			}
			root = ret;
		}

		void AvlnDeleteFixUp(PAvlnode fixnd)
		{
			PAvlnode ret = root;
			PAvlnode fixnode = fixnd;
			PAvlnode delnode = fixnode;
			int bfactor =-1;

			while(fixnode->parent!=PNIL)
			{
				bfactor = GetBlancefactor(fixnode->parent);

				if(fixnode==fixnode->parent->lchild)
				{
					if(bfactor==0) 
					{
						if(fixnode->height>0)--fixnode->height;
						break;
					}
					else if(bfactor==1)
					{
						if(fixnode->lchild!=PNIL || fixnode->rchild!=PNIL) --fixnode->height;
						fixnode=fixnode->parent;
						if(fixnode==ret) --fixnode->height;
					}
					else
					{
						PAvlnode brother =GetBrother(fixnode);
						int brotherblnsfactor = GetBlancefactor(brother);

						if(brotherblnsfactor == 0)
						{
							Swapheight(fixnode->parent,brother);
							ret = LeftRotate(ret,fixnode->parent,PNIL);
							if(fixnode->lchild!=PNIL || fixnode->rchild!=PNIL) --fixnode->height;
							break;
						}
						else if(brotherblnsfactor==1)
						{
							AvlnRightBrotherSecondaryFixUp(brother);
							Swapheight(brother,brother->lchild);
							ret = RightRotate(ret,brother,PNIL);
						}
						else
						{
							Swapheight(fixnode->parent,brother);
							if(fixnode->height>0)  --fixnode->height;
							--fixnode->parent->height;
							ret = LeftRotate(ret,fixnode->parent,PNIL);
							fixnode = fixnode->parent->parent;
						}
					}
				}
				else
				{
					if(bfactor==0)
					{
						if(fixnode->height>0)--fixnode->height;
						break;
					}
					else if(bfactor==1)
					{
						PAvlnode brother =GetBrother(fixnode);
						int brotherblnsfactor = GetBlancefactor(brother);

						if(brotherblnsfactor == 0)
						{
							Swapheight(fixnode->parent,brother);
							ret = RightRotate(ret,fixnode->parent,PNIL);
							if(fixnode->lchild!=PNIL || fixnode->rchild!=PNIL) --fixnode->height;
							break;
						}
						else if(brotherblnsfactor==1)
						{
							Swapheight(fixnode->parent,brother);
							if(fixnode->height>0)  --fixnode->height;
							--fixnode->parent->height;
							ret = RightRotate(ret,fixnode->parent,PNIL);
							fixnode = fixnode->parent->parent;

						}
						else
						{
							AvlnLeftBrotherSecondaryFixUp(brother);
							Swapheight(brother,brother->rchild);
							ret= LeftRotate(ret,brother,PNIL);
						}
					}
					else
					{
						if(fixnode->lchild!=PNIL || fixnode->rchild!=PNIL) --fixnode->height;
						fixnode=fixnode->parent;
						if(fixnode==ret) --fixnode->height;
					}
				}
			}
			root = ret;
		}
	public:
		int Insert(Key key)
		{
			PAvlnode ret = root;

			if(ret==PNIL)
			{
				PAvlnode ret = CreateAvlnode();
				ret->key=key;
				root = ret;
				++counter;
				return 0;
			}

			PAvlnode insertparent= GetInsertParent(key);

			// 插入节点存在 
			if(insertparent==PNIL)
			{
				return 1;
			}

			++counter;

			PAvlnode rbn = CreateAvlnode();

			rbn->key=key;

			rbn->parent=insertparent;

			if(rbn->key>insertparent->key)
			{
				// 右边插入
				insertparent->rchild = rbn;
				// 插入节点父节点存在另一孩子 树高度不变，不用调整
				if(rbn->parent->lchild!=PNIL) return 0;

			}
			else
			{
				insertparent->lchild = rbn;
				if(rbn->parent->rchild!=PNIL) return 0;	

			}

			if(insertparent->parent==PNIL)
			{
				rbn->parent->height++;
				return 0;
			}
			AvlnInsertFixUp(rbn);
			return 0;
		}
		int Delete(Key key)
		{
			PAvlnode ret=root;
			PAvlnode knode =SearchKey(key);
			if(knode==PNIL) return 1;

			--counter;
			PAvlnode deln = knode;

			if(deln->parent==PNIL && deln->lchild==PNIL && deln->rchild==PNIL)
			{
				free(deln);
				root = PNIL;
				return 0;
			}

			if(deln->lchild!=PNIL && deln->rchild!=PNIL)
			{
				PAvlnode sor = GetSuccessor(deln,PNIL);
				Transplant(deln,sor);				// 交换deln与sor数据，并令sor为册除节点
				deln = sor;
			}

			int bfactor = GetBlancefactor(deln);
			if(bfactor==-1 || bfactor==1)
			{
				if(deln->lchild!=PNIL)
				{
					Transplant(deln,deln->lchild);
					deln = deln->lchild;
				}
				else
				{
					Transplant(deln,deln->rchild);
					deln = deln->rchild;
				}
			}

			bfactor = GetBlancefactor(deln->parent);

			if(bfactor==0)
			{
				PAvlnode delntmp = deln;
				if(deln==deln->parent->lchild)
				{
					deln = deln->parent->lchild;
					delntmp->parent->lchild=PNIL;

				}
				else
				{
					deln = deln->parent->rchild;
					delntmp->parent->rchild=PNIL;
				}
				free(deln);
				return 0;
			}

			AvlnDeleteFixUp(deln);

			Cutleaf(deln);

			free(deln);

			return 0;
		}

	};

	// Iterator of avltree
	template<class TKey,class TRefOfKey,class TPtrOfKey,class THeight>
	class AvlIterator
	{
	public:
#ifdef _USING_STD_TEST_
		typedef std::bidirectional_iterator_tag iterator_category;
#else
		typedef bidirectional_iterator_tg iterator_category;
#endif
		typedef TKey key_type;
		typedef TKey value_type;		// 
		typedef ptrdiff_t difference_type;
		typedef difference_type distance_type;
		typedef TRefOfKey reference;
		typedef const TRefOfKey const_reference;
		typedef TPtrOfKey pointer;
		typedef const TPtrOfKey const_pointer;

		typedef key_type Key;
		typedef value_type Value;
		typedef THeight Height;

		typedef Avlnode<Key,Height>* PAvlnode;
		typedef AvlIterator<Key,reference,pointer,Height> Self;
		typedef AvlIterator<Key,Key&,Key*,Height> Iterator;
		typedef AvlIterator<Key,const Key&,const Key*,Height> ConstIterator;
	protected:
		PAvlnode node;	// current node
		PAvlnode nil;	// nil node
	private:
		void Increment()
		{
			node = GetSuccessor<Key,Height>(node,nil);
		}
		void Decrement()
		{
			node = GetPredecessor<Key,Height>(node,nil);
		}
	public:
		reference operator*() const
		{
			return node->key;
		}
		pointer operator->() const
		{
			return &node->key;
		}
		Self& operator++()
		{
			Increment();
			return *this;
		}
		Self operator++(int)
		{
			Self tmp = *this;
			Increment();
			return tmp;
		}
		Self& operator--()
		{
			Decrement();
			return *this;
		}
		Self operator--(int)
		{
			Self tmp = *this;
			Decrement();
			return tmp;
		}
		bool operator==(Self rhs)const
		{
			return node==rhs.node;
		}
		bool operator!=(Self rhs)const
		{
			return node!=rhs.node;
		}

	public:
		AvlIterator(){ }
		AvlIterator(PAvlnode cur,PAvlnode nl) : node(cur),nil(nl){ }
		AvlIterator(const Self& self) : node(self.node),nil(self.nil){ }
	};

	template<class TKey,class THeight>
	inline Avlnode<TKey,THeight>* GetSuccessor(Avlnode<TKey,THeight>* node,Avlnode<TKey,THeight>* nil)
	{
		typedef Avlnode<TKey,THeight>* PAvlnode;
		PAvlnode top = node;
		PAvlnode sor = top;
		if(top->rchild!=nil)
		{
			sor = GetMinimum(top->rchild,nil);
			return sor;
		}
		sor = top->parent;
		while(sor!=nil && top==sor->rchild)
		{
			top = sor;
			sor=sor->parent;
		}
		return sor;
	}

	template<class TKey,class THeight>
	inline Avlnode<TKey,THeight>* GetPredecessor(Avlnode<TKey,THeight>* node,Avlnode<TKey,THeight>* nil)
	{
		typedef Avlnode<TKey,THeight>* PAvlnode;
		PAvlnode top = node;
		PAvlnode por = top;

		if(top->lchild!=nil)
		{
			por = GetMaximum(top->lchild,nil);
			return por;
		}

		por = top->parent;

		while(por!=nil && top==por->lchild)
		{
			top = por;
			por=por->parent;
		}
		return por;
	}


	template<class TKey,class THeight>
	inline Avlnode<TKey,THeight>* GetMinimum(Avlnode<TKey,THeight>* node,Avlnode<TKey,THeight>* nil)
	{
		typedef Avlnode<TKey,THeight>* PAvlnode;
		PAvlnode top=node;
		PAvlnode min = node;
		while(top!=nil)
		{
			min=top;
			top=top->lchild;
		}
		return min;
	}

	template<class TKey,class THeight>
	inline Avlnode<TKey,THeight>* GetMaximum(Avlnode<TKey,THeight>* node,Avlnode<TKey,THeight>* nil)
	{
		typedef Avlnode<TKey,THeight>* PAvlnode;
		PAvlnode top=node;
		PAvlnode max = node;
		while(top!=nil)
		{
			max=top;
			top=top->rchild;
		}
		return max;
	}
	template<class TKey,class THeight>
	inline Avlnode<TKey,THeight>* RightRotate(Avlnode<TKey,THeight>* root,Avlnode<TKey,THeight>* node,Avlnode<TKey,THeight>* nil)
	{
		if(node==nil||node->lchild==nil) return root;
		typedef Avlnode<TKey,THeight>* PAvlnode;

		PAvlnode ret=root;

		PAvlnode x=node;
		PAvlnode y=x->lchild;

		x->lchild=y->rchild;
		if(y->rchild!=nil)
		{
			y->rchild->parent = x;
		}
		y->parent = x->parent;
		if(x->parent!=nil)
		{
			if(x == x->parent->rchild)
			{
				x->parent->rchild=y;
			}
			else if(x==x->parent->lchild)
			{
				x->parent->lchild=y;
			}
		}

		y->rchild=x;
		x->parent=y;

		if(y->parent==nil) ret=y;

		return ret;
	}
	template<class TKey,class THeight>
	inline Avlnode<TKey,THeight>* LeftRotate(Avlnode<TKey,THeight>* root,Avlnode<TKey,THeight>* node,Avlnode<TKey,THeight>* nil)
	{
		if(node==nil || node->rchild==nil)return root;

		typedef Avlnode<TKey,THeight>* PAvlnode;
		PAvlnode ret=root;

		PAvlnode x=node;
		PAvlnode y=x->rchild;

		x->rchild=y->lchild;
		if(y->lchild!=nil)
		{
			y->lchild->parent = x;
		}
		y->parent = x->parent;
		if(x->parent!=nil)
		{
			if(x == x->parent->lchild)
			{
				x->parent->lchild=y;
			}
			else if(x==x->parent->rchild)
			{
				x->parent->rchild=y;
			}
		}

		y->lchild=x;
		x->parent=y;


		if(y->parent==nil)
		{
			ret=y;
		}
		return ret;
	}
	// AvlTree node structure
	template<class T1,class T2>
	struct Avlnode
	{
		T1 key;				// node info
		T2 height;			// node height
		Avlnode* parent;	// parent
		Avlnode* lchild;	// left child
		Avlnode* rchild;	// right child
	};
}

#endif
/*
* AvlTree.h -  
* Written by vivitue based upon the specification on Jan. 5th 2015. 
* v1.0.0
* all end.
*/
