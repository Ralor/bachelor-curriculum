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
#include <ctime>
#include <fstream>
#pragma comment(linker,"/STACK:256000000")
using namespace std;
#define stop(x) { cout << x; return 0; }
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

int solution()
{
	// very very long real numbers, another hindu code, which can compare it
	string f,s; cin >> f >> s;
	// computing some values
	int f_neg = (f[0] == '-'), 
		s_neg = (s[0] == '-');
	string::iterator dot_f = find(f.begin(),f.end(),'.'), 
					 dot_s = find(s.begin(),s.end(),'.');
	int invert = (f_neg && s_neg);

	// start formatting of strings
	stringstream tmp;
	if (f_neg) f = f.substr(1,f.length() - 1);
	if (s_neg) s = s.substr(1,s.length() - 1);	

	// if no .xxxx in "f" but exist in "s"
	if (dot_f == f.end() && dot_s != s.end())
	{
		f += "."; // dot_f = f.end();
	}
	// same thing, but contrary
	if (dot_s == s.end() && dot_f != f.end())
	{		
		s += "."; // dot_s = s.end();
	} 

	// if fractional parts are not equal
	int dist_f = distance(dot_f,f.end()), dist_s = distance(dot_s,s.end());
	if (dist_f > dist_s)
	{
		for(int i = 0; i < dist_f - dist_s; i++)
			tmp << '0';
		string tmp_loc; tmp >> tmp_loc;
		s += tmp_loc;
	} tmp.flush();
	if (dist_s > dist_f)
	{
		for(int i = 0; i < dist_s - dist_f; i++)
			tmp << '0';
		string tmp_loc; tmp >> tmp_loc;
		f += tmp_loc;
	} tmp.flush();

	// time to make a dicision)
	if (s_neg && !f_neg) stop('>')
	if (f_neg && !s_neg) stop('<')
	if (f.length() > s.length())
		if (!invert)
			stop('>')
		else
			stop('<')
	if (f.length() < s.length()) 
		if (!invert)
			stop('<')
		else
			stop('>')
	if (f.length() == s.length()) 
	{
		if (f == s) stop('=')
		if (f < s)
		{
			if (!invert) 
				stop('<') 
			else 
				stop('>')
		}
		if (f > s)
		{
			if (!invert) 
				stop('>') 
			else 
				stop('<')
		}
	}

	return 0;	
}