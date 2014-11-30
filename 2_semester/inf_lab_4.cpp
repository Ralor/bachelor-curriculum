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

int solution()
{	
	string s, ans = "";
	int i = 1;
	while (cin >> s)
	{
		cout << i++ << ": " << s << "\n";
		if (s.length() > ans.length())
			ans = s;
	}
	reverse(ans.begin(),ans.end());
	cout << ans;
 
    return 0;	
}