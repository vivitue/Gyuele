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

#ifndef _VIVITUE_REDBLACKTREE_CPLUSPLUS_H__
#define _VIVITUE_REDBLACKTREE_CPLUSPLUS_H__
#include <malloc.h>
#include "Iterator.h"

#define _THROW_BADMEMORY_EXCEPTION(msg) throw exception(msg)

namespace STL1
{
	//using namespace std;


	template<class TKey,class TValue> class RedblackTree;
	template<class TKey,class TRefOfKey,class TPtrOfKey,class TValue> class RedblackIterator;
	enum RedblackNodeColor;
	enum ErrCode;
	template<class TKey,class TValue>struct Rbnode;

	// Global inline template declarations
	template<class TKey,class TValue> inline Rbnode<TKey,TValue>* GetMaximum(Rbnode<TKey,TValue>* node,Rbnode<TKey,TValue>* nil);
	template<class TKey,class TValue> inline Rbnode<TKey,TValue>* GetMaximum(Rbnode<TKey,TValue>* node,Rbnode<TKey,TValue>* nil);
	template<class TKey,class TValue> inline Rbnode<TKey,TValue>* GetSuccessor(Rbnode<TKey,TValue>* node,Rbnode<TKey,TValue>* nil);
	template<class TKey,class TValue> inline Rbnode<TKey,TValue>* GetPredecessor(Rbnode<TKey,TValue>* node,Rbnode<TKey,TValue>* nil);
	template<class TKey,class TValue> inline Rbnode<TKey,TValue>* LeftRotate(Rbnode<TKey,TValue>* root,Rbnode<TKey,TValue>* node,Rbnode<TKey,TValue>* nil);
	template<class TKey,class TValue> inline Rbnode<TKey,TValue>* RightRotate(Rbnode<TKey,TValue>* root,Rbnode<TKey,TValue>* node,Rbnode<TKey,TValue>* nil);

	// Redblack tree 
	template<class TKey,class TValue>
	class RedblackTree
	{
	public:
		// type definitions for the compatibility of original stl-codes
		typedef TKey key_type;
		typedef TValue value_type;
		typedef TKey* pointer;
		typedef const TKey* const_pointer;
		typedef TKey& reference;
		typedef const TKey& const_reference;

		typedef key_type Key;
		typedef value_type Value;
		typedef size_t Count;
		typedef Rbnode<Key,Value> Rbnode,*PRbnode,*prbnode,*pRbnode;

		// redblack tree interator <!use ConstIterator instead of Interator!>
		typedef RedblackIterator<Key,reference,pointer,Value> Iterator;
		typedef RedblackIterator<Key,const_reference,const_pointer,Value> ConstIterator;

	private:
		PRbnode nuln;			// null pointer of PRbnode
		// swap t1 & t2
		template<typename T>void Rbswaper(T& t1,T& t2)
		{
			T t =t1;
			t1=t2;
			t2=t;
		}
		// swap t1 & t2 (t2 must be equal to t2sibling)
		template<typename T>void DRbswaper(T& t1,T& t2,T& t2sibling)
		{
			T t =t2;
			t2=t1;
			t2sibling=t1;
			t1=t;
		}
	protected:
		PRbnode PNIL;			// nil node pointer(please refer to Introduction to Algrothem)
		PRbnode root;			// root of the redblack tree
		Count counter;			// node counter
		Iterator beginIt;		// regular begin interator
		Iterator endIt;			// iterator of the past-of-end
		ConstIterator cbeginIt;	// const begin interator
		ConstIterator cendIt;
	private:
		// fuction pointer & callback methods are for the realization of c++ event mechanism
		typedef enum Command{ MemoryReleaseRequest=0,ClientResponsibility }TestCmd;
		typedef bool (RedblackTree<Key,Value>::*MemoryRequestedHandler)();// member function pointer declaration
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
			memoryRequestedEvent=&RedblackTree<Key,Value>::OnOutofMemory;
		}

		void AutoUnregisterEvents()
		{
			memoryRequestedEvent=0;
			MemoryHandler=0;
		}
	private:
		// Create one redblack node with red color (default color)
		PRbnode CreateRbnode()
		{
			PRbnode node = (PRbnode)malloc(sizeof(Rbnode));
			if(node==nuln)
			{
				if(memoryRequestedEvent!=0)
				{
					bool isok = (this->*memoryRequestedEvent)();
					if(isok)
					{
						node = (PRbnode)malloc(sizeof(Rbnode));
						if(node==nuln)
						{
							_THROW_BADMEMORY_EXCEPTION("RedblackTree CreateNode : out of memory!");
						}
					}
					else
					{
						_THROW_BADMEMORY_EXCEPTION("RedblackTree CreateNode : out of memory!");

					}
				}
				else
				{
					_THROW_BADMEMORY_EXCEPTION("RedblackTree CreateNode : out of memory!");
				}
			}
			node->color = Red;
			node->lchild=PNIL;
			node->parent=PNIL;
			node->rchild=PNIL;

			return node;
		}

		// preinitialization
		void PreInit()
		{
			MemoryHandler=0;
			memoryRequestedEvent = (MemoryRequestedHandler)0;

			nuln =(PRbnode)0;			
			PNIL = (PRbnode)malloc(sizeof(Rbnode));
			if(PNIL==nuln)
			{
				_THROW_BADMEMORY_EXCEPTION("CreateNode : out of memory!");
			}

			PNIL->lchild = PNIL;
			PNIL->rchild=PNIL;
			PNIL->parent=PNIL;
			PNIL->key=-1;
			PNIL->color = Black;

			root = PNIL;

			endIt = Iterator(PNIL,PNIL);
			cendIt = ConstIterator(PNIL,PNIL);
			counter =0;
			RegisterEvents();
		}

		// destroy the whole tree
		void Clear(PRbnode rt)
		{
			if(rt!=PNIL)
			{
				Clear(rt->lchild);
				Clear(rt->rchild);
				free(rt);
				rt=PNIL;
			}
		}
		// swap values of u and v
		void Transplant(PRbnode u,PRbnode v)
		{
			Rbswaper<Key>(u->key,v->key);
			Rbswaper<Value>(u->value,v->value);
		}
		PRbnode GetGrandfather(PRbnode node)
		{
			return node->parent->parent;
		}
		PRbnode GetBrother(PRbnode node)
		{
			if(node==node->parent->lchild)
			{
				return node->parent->rchild;
			}
			return node->parent->lchild;
		}
		PRbnode GetUncle(PRbnode node)
		{
			if(node->parent==node->parent->parent->lchild)
			{
				return node->parent->parent->rchild;
			}
			return node->parent->parent->lchild;
		}
		PRbnode SearchKey(Key key)
		{
			PRbnode top=root;
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
		PRbnode GetInsertParent(Key key)
		{
			PRbnode prt = PNIL;
			PRbnode top = root;
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

		void RbInsertFixUp(PRbnode fixnd)
		{
			PRbnode uncle =PNIL;
			PRbnode fixnode=fixnd;
			PRbnode ret=root;
			while(fixnode->parent->color==Red)
			{
				// 
				uncle = GetUncle(fixnode);
				if(fixnode->parent==fixnode->parent->parent->lchild)
				{
					if(uncle->color==Red)
					{
						DRbswaper(uncle->parent->color,uncle->parent->lchild->color,uncle->parent->rchild->color);
						fixnode = fixnode->parent->parent;
					}
					else
					{
						if(fixnode==fixnode->parent->lchild)
						{
							Rbswaper(fixnode->parent->color,fixnode->parent->parent->color);
							ret = RightRotate<Key,Value>(ret,fixnode->parent->parent,PNIL);
						}
						else
						{
							fixnode=fixnode->parent;
							ret = LeftRotate<Key,Value>(ret,fixnode,PNIL);
						}
					}
				}
				else
				{
					if(uncle->color==Red)
					{
						DRbswaper(uncle->parent->color,uncle->parent->lchild->color,uncle->parent->rchild->color);
						fixnode = fixnode->parent->parent;
					}
					else
					{
						if(fixnode==fixnode->parent->rchild)
						{
							Rbswaper(fixnode->parent->color,fixnode->parent->parent->color);
							ret = LeftRotate<Key,Value>(ret,fixnode->parent->parent,PNIL);
						}
						else
						{
							fixnode=fixnode->parent;
							ret = RightRotate<Key,Value>(ret,fixnode,PNIL);
						}
					}
				}
			}
			if(ret->color!=Black) ret->color=Black;
			root = ret;
		}
		void RbnDeleteFixUp(PRbnode fixnd)
		{
			PRbnode ret = root;
			PRbnode fixnode = fixnd;
			PRbnode delnode = fixnode;
			while(fixnode!=ret && fixnode->color==Black)
			{
				PRbnode brother = GetBrother(fixnode);
				if(fixnode==fixnode->parent->lchild)
				{
					if(brother->color==Red)
					{
						Rbswaper(brother->color,brother->parent->color);
						ret = LeftRotate<Key,Value>(ret,brother->parent,PNIL);
					}
					else
					{
						// 
						if(fixnode->parent->color==Red)
						{
							if(brother->lchild->color==Black
								&& brother->rchild->color==Black)
							{
								Rbswaper(brother->color,brother->parent->color);
								fixnode=ret;//break
							}
						}
						else
						{
							if(brother->lchild->color==Black
								&& brother->rchild->color==Black)
							{
								brother->color=Red;
								fixnode=fixnode->parent;// pointer backtrack
							}
						}

						if(brother->rchild->color==Red)
						{
							Rbswaper(brother->color,brother->parent->color);
							brother->rchild->color = Black;
							ret = LeftRotate<Key,Value>(ret,brother->parent,PNIL);
							fixnode=ret;		// Break
						}
						else
						{
							if(brother->lchild->color==Red)
							{
								Rbswaper(brother->color,brother->lchild->color);
								ret = RightRotate(ret,brother,PNIL);
							}
						}
					}
				}
				else
				{
					if(brother->color==Red)
					{
						Rbswaper(brother->color,brother->parent->color);
						ret = RightRotate(ret,brother->parent,PNIL);
					}
					else
					{
						// 
						if(fixnode->parent->color==Red)
						{
							if(brother->lchild->color==Black
								&& brother->rchild->color==Black)
							{
								Rbswaper(brother->color,brother->parent->color);
								fixnode=ret;//break
							}
						}
						else
						{
							if(brother->lchild->color==Black
								&& brother->rchild->color==Black)
							{
								brother->color=Red;
								fixnode=fixnode->parent;// pointer backtrack
							}
						}

						if(brother->lchild->color==Red)
						{
							Rbswaper(brother->color,brother->parent->color);
							brother->lchild->color = Black;
							ret = RightRotate(ret,brother->parent,PNIL);
							fixnode=ret;		// Break
						}
						else
						{
							if(brother->rchild->color==Red)
							{
								Rbswaper(brother->color,brother->rchild->color);
								ret = LeftRotate(ret,brother,PNIL);
							}
						}
					}
				}
			}
			if(ret->color!=Black) ret->color=Black;
			root = ret;
		}

	public:

		RedblackTree()
		{
			PreInit();
		}
		~RedblackTree()
		{
			Dispose();
		}
	public:
		// Clear the whole tree
		void Clear()
		{
			if(root!=PNIL)
			{
				Clear(root);
				counter=0;
			}
		}
		// release all the used memory
		void Dispose()
		{
			Clear();
			if(nuln!=PNIL)
			{
				free(PNIL);
				PNIL = nuln;
			}
			AutoUnregisterEvents();
		}

	public:
		const PRbnode GetNil() const
		{
			return PNIL;
		}
		PRbnode GetRoot() const
		{
			return root;
		}

		Iterator Begin() const
		{
			PRbnode min = GetMinimum(root,PNIL);
			return Iterator(min,PNIL);
		}
		Iterator End() const
		{
			return endIt;
		}
		ConstIterator CBegin() const
		{
			PRbnode min = GetMinimum(root,PNIL);
			return ConstIterator(min,PNIL);
		}
		ConstIterator CEnd() const
		{
			return cendIt;
		}

		ConstIterator Find(const Key& key)
		{
			PRbnode node = SearchKey(key);
			return ConstIterator(node,PNIL);

		}
		int GetCounter()
		{
			return (int)this->counter;
		}
	protected:
		// Get iterator of key
		Iterator Find(const Key& key,int)
		{
			PRbnode node = SearchKey(key);
			return Iterator(node,PNIL);
		}
	public:
		int Insert(Key key,Value data=0)
		{
			PRbnode ret = root;
			if(ret==PNIL)
			{
				ret = CreateRbnode();
				ret->value=data;
				ret->key=key;
				ret->color = Black;
				root = ret;
				++counter;
				return (int)Success;
			}

			PRbnode insertparent= GetInsertParent(key);
			if(insertparent==PNIL)
			{
				return (int)KeyExisted;
			}

			PRbnode rbn = CreateRbnode();
			rbn->value=data;
			rbn->key=key;

			rbn->parent=insertparent;

			if(rbn->key>insertparent->key)
			{
				insertparent->rchild = rbn;
			}
			else
			{
				insertparent->lchild = rbn;
			}
			RbInsertFixUp(rbn);
			++counter;
			return (int)Success;
		}
		int Delete(Key key)
		{
			PRbnode ret=root;
			PRbnode knode =SearchKey(key);
			if(knode==PNIL)
			{
				return (int)KeyNotExisted;
			}
			PRbnode deln = knode;
			if(deln->lchild!=PNIL && deln->rchild!=PNIL)
			{
				PRbnode successor = GetSuccessor(deln,PNIL);
				Transplant(deln,successor);
				deln = successor;
			}

			PRbnode tmpchild = deln;
			if(deln->lchild==PNIL && deln->rchild==PNIL)
			{
				if(deln->color==Black)
				{
					RbnDeleteFixUp(deln);
				}

				if(deln==deln->parent->lchild)
				{
					deln->parent->lchild=PNIL;
				}
				else
				{
					deln->parent->rchild=PNIL;
				}
			}
			else
			{
				if(deln->lchild!=PNIL)
				{
					Transplant(deln,deln->lchild);
					deln=deln->lchild;
					tmpchild->lchild=PNIL;
				}
				else
				{
					Transplant(deln,deln->rchild);
					deln=deln->rchild;
					tmpchild->rchild=PNIL;
				}
			}
			free(deln);
			if(--counter==0) root = PNIL;
		}

	};

	// Iterator of redblack tree
	template<class TKey,class TRefOfKey,class TPtrOfKey,class TValue>
	class RedblackIterator
	{
	public:
		typedef bidirectional_iterator_tg iterator_category;
		typedef TKey value_type;		// value type of Tkey rather than value type of TValue
		typedef ptrdiff_t difference_type;
		typedef difference_type distance_type;
		typedef TRefOfKey reference;
		typedef const TRefOfKey const_reference;
		typedef TPtrOfKey pointer;
		typedef const TPtrOfKey const_pointer;

		typedef TKey Key;
		typedef TValue Value;
		typedef RedblackIterator<Key,reference,pointer,Value> Self;
		typedef RedblackIterator<Key,Key&,Key*,Value> Iterator;
		typedef RedblackIterator<Key,const Key&,const Key*,Value> ConstIterator;
		typedef Rbnode<Key,Value>* PRbnode;
	protected:
		PRbnode node;	// current node
		PRbnode nil;	// nil node
	public:
		// value can be allowed to change 
		Value& GetValue()const //
		{
			return node->value;
		}
		// key can't be allowed to change 
		Key GetKey() const
		{
			return node->key;
		}
	private:
		void Increment()
		{
			node = GetSuccessor<Key,Value>(node,nil);
		}
		void Decrement()
		{
			node = GetPredecessor<Key,Value>(node,nil);
		}
	public:
		// member variable can't be changed
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
		RedblackIterator(){ }
		RedblackIterator(PRbnode cur,PRbnode nl) : node(cur),nil(nl){ }
		RedblackIterator(const Self& self):node(self.node),nil(self.nil){ }
	};

	// node color
	typedef enum RedblackNodeColor
	{
		Red=0,
		Black
	}Color;
	enum ErrCode
	{
		MinID=-1,
		Success=0,
		KeyExisted,
		KeyNotExisted,
		MaxID
	};
	// redblack tree node structure
	template<class TKey,class TValue>
	struct Rbnode
	{
		TKey key;
		Color color;
		TValue value;
		Rbnode* lchild;
		Rbnode* rchild;
		Rbnode* parent;
	};

	// Global template methods
	template<class TKey,class TValue> 
	inline Rbnode<TKey,TValue>* GetMinimum(Rbnode<TKey,TValue>* node,Rbnode<TKey,TValue>* nil)
	{
		typedef Rbnode<TKey,TValue>* PRbnode;
		PRbnode top=node;
		PRbnode min = node;
		while(top!=nil)
		{
			min=top;
			top=top->lchild;
		}
		return min;
	}
	template<class TKey,class TValue> 
	inline Rbnode<TKey,TValue>* GetMaximum(Rbnode<TKey,TValue>* node,Rbnode<TKey,TValue>* nil)
	{
		typedef Rbnode<TKey,TValue>* PRbnode;
		PRbnode top=node;
		PRbnode max = node;
		while(top!=nil)
		{
			max=top;
			top=top->rchild;
		}
		return max;
	}
	template<class TKey,class TValue> 
	inline Rbnode<TKey,TValue>* GetSuccessor(Rbnode<TKey,TValue>* node,Rbnode<TKey,TValue>* nil)
	{
		typedef Rbnode<TKey,TKey>* PRbnode;
		PRbnode top = node;
		PRbnode sor = top;
		if(top->rchild!=nil)
		{
			sor = GetMinimum<TKey,TValue>(top->rchild,nil);
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

	template<class TKey,class TValue> 
	inline Rbnode<TKey,TValue>* GetPredecessor(Rbnode<TKey,TValue>* node,Rbnode<TKey,TValue>* nil)
	{
		typedef Rbnode<TKey,TValue>* PRbnode;
		PRbnode top = node;
		PRbnode por = top;

		if(top->lchild!=nil)
		{
			por = GetMaximum<TKey,TValue>(top->lchild,nil);
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
	template<class TKey,class TValue> 
	inline Rbnode<TKey,TValue>* LeftRotate(Rbnode<TKey,TValue>* root,Rbnode<TKey,TValue>* node,Rbnode<TKey,TValue>* nil)
	{
		typedef Rbnode<TKey,TValue>* PRbnode;
		if(node==nil || node->rchild==nil) return root;

		PRbnode ret=root;

		PRbnode x=node;
		PRbnode y=x->rchild;

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
	template<class TKey,class TValue> 
	inline Rbnode<TKey,TValue>* RightRotate(Rbnode<TKey,TValue>* root,Rbnode<TKey,TValue>* node,Rbnode<TKey,TValue>* nil)
	{
		typedef Rbnode<TKey,TValue>* PRbnode;
		if(node==nil||node->lchild==nil) return root;
		PRbnode ret=root;

		PRbnode x=node;
		PRbnode y=x->lchild;

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

}

#endif
/*
* RedblackTree.h -  
* Written by vivitue based upon the specification on Jan. 5th 2015. 
* v1.0.0
* all end.
*/