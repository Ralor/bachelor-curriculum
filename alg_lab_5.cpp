#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <cmath>
#include <map>
#include <set>
#include <cstdio>
#include <ctime>
#pragma comment(linker,"/STACK:256000000")
using namespace std;
#define stop(x) {cout << x; return 0;}
int solution();
int main()
{
#ifndef ONLINE_JUDGE
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);	
#endif 
#ifdef _DEBUG 
	time_t starTime = clock();   
#endif
 
	solution();

#ifdef _DEBUG
	printf("|stop|\n---\n%.0lf msec",((clock() - starTime)/((double)CLOCKS_PER_SEC))*1000);
#endif
	return 0;
} 

struct SubTree
{
	SubTree()
	{
		left = NULL; right = NULL;
	}

	int value;
	SubTree *left, *right;
};
struct MyQueue
{
	MyQueue()
	{
		top = NULL;
	}
	~MyQueue()
	{
		erase_all(top);
	}

	SubTree *top;

	void push(int val)
	{
		SubTree *tmp = new SubTree();
		tmp->value = val;
		top = merge(top, tmp);
	}
	void pop()
	{
		top = merge(top->left, top->right);
	}
	int front()
	{
		return top->value;
	}
	void erase_all(SubTree *current)
	{
		if (current != NULL)
		{
			erase_all(current->left);
			erase_all(current->right);
			delete current;
		}
	}

	SubTree * merge(SubTree * first, SubTree * second) 
	{
		if (first  == NULL) return second;
		if (second == NULL) return first;

		if (first->value > second->value)
			swap (first, second);

		if (rand() % 2)
			swap (first->left, first->right);

		first->left = merge(first->left, second);

		return first;
	}
};

int solution()
{	
	srand((unsigned int)time(0) );
	
	MyQueue u;
	for(int i = 0; i < (int)1e5; i++)
	{
		u.push(rand() % 1000 + 1);
	}
	for(int i = 0; i < (int)1e5; i++)
	{
		printf("%i ",u.front() ); u.pop();
	}

    return 0;	
}