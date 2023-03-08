// Gyuele.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GyueleUtility.h"
#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include "Alloc.h"
class tt2;
void sizeoftest();
class tt2
{
private:
	int a;

public:
	tt2() 
	{
		printf("tt2 construct!");
	}
	void print1(){ printf("cout tt2");}
	void print2(){ printf("cout a=%d",a);}
	void print3(){ printf("cout a=%d",this->a);}
};
class bb
{
public:
	static tt2 t;
	const static int b2=0;
};

int _tmain(int argc, _TCHAR* argv[])
{
	int *p = new int[4];
	int *p1=p;
	++p1;
	++p1;
	int n = distance(p,p1);

	delete p;
	sizeoftest();
	GyueleUtility g;
	g.Test();
	tt2* pt=NULL;
	pt->print1();
	pt->print2();

	return 0;
}

void sizeoftest()
{
	int boolval =(int)true;
	int intsize = sizeof(int);		// 4
	int floatsize = sizeof(float);  // 4
	int* p;
	int intpsize = sizeof(p);		// 4
	int charsize = sizeof(char);	// 1
	int doublesize = sizeof(double);// 8
	int shortsize = sizeof(short);	// 2        short int∂Ã’˚–Õ

	int end =0;
}
void TEST2()
{
		int compi=0;
#ifdef __cpluscplus
	printf("c++");
#else
	printf("c");
#endif
}

