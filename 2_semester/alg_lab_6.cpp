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
	SubTree(int val)
	{
		value = val;
		left = right = NULL;
	}

	int value;
	
SubTree *left, *right;
};
struct BST
{
	BST()
	{
		top = NULL;
	}
	~BST()
	{
		erase_all(top);
	}

	SubTree *top;

	void push(int value)
	{
		add(top, new SubTree(value) );
	}
	bool search(int value)
	{
		return ( check(top,value) ) ? true : false;
	}
	void print()
	{
		output(top);
	}

	void output(SubTree *current)
	{
		if (current == NULL)
			return;		
		output(current->left); 
		printf("%i ", current->value);
		output(current->right);
	}
	bool check(SubTree *cur, int &value)
	{
		if (cur == NULL) 
			return false;
		if (cur->value == value) 
			return true;
		
		if (value > cur->value)
			return check(cur->right, value);
		else
			return check(cur->left, value);
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
	SubTree * l_rotation(SubTree *target)
	{
		if (target->right == NULL)
			return target;

		SubTree *tmp = target->right;
		target->right = tmp->left;
		tmp->left = target;

		return tmp;
	}
	SubTree * r_rotation(SubTree *target)
	{
		if (target->left == NULL)
			return target;

		SubTree *tmp = target->left;
		target->left = tmp->right;
		tmp->right = target;

		return tmp;
	}
	void add(SubTree *&_cur, SubTree * _new) 
	{
		if (_cur == NULL)
		{
			_cur = _new; 			
			return;
		}		

		if (_new->value < _cur->value)
		{
			add(_cur->left, _new);
			_cur = r_rotation(_cur);
		}
		else
		{
			add(_cur->right, _new);
			_cur = l_rotation(_cur);
		}
	}
};

int solution()
{	
	srand((unsigned int)time(0) );
	
	BST tree;

	int n; cin >> n;
	for(int i = 0; i < n; i++)
		tree.push( rand() % 100 + 1);

	tree.print(); cout << "\n";

	int m; cin >> m;
	for(int i = 0; i < m; i++)
	{
		int tmp = rand() % 100 + 1;
		if (tree.search(tmp) )
		{
			cout << tmp << " found\n";
		}
		else
		{
			cout << tmp << " not found\n";
		}
	}

    return 0;	
}