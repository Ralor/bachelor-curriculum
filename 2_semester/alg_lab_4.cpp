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

#define iter vector<int>::iterator


iter bin_search(int val, iter l, iter r, iter undef)
{		
	if (l >= r)	return undef;
	iter ser = l + distance(l,r) / 2;
	if (val == *ser) return ser;
	if (val < *ser) return bin_search(val,l,ser,undef);
	if (val > *ser) return bin_search(val,ser+1,r,undef);
}

int solution()
{		
	srand((unsigned int)time(0));
 	
	vector<int> mass;
    int n, val; cin >> n >> val;

	for(int i = 0; i < n; i++)
	{
		//mass.push_back(rand()%30);
		int x; cin >> x; mass.push_back(x);
	}	
	sort(mass.begin(), mass.end());
	for(int i = 0; i < n; i++)
	{
		cout << mass[i] << " ";
	} cout << endl;

	int ans = 
	distance(mass.begin(),
		bin_search(val, mass.begin(), mass.end(), mass.end()));
	
	cout << ((ans == mass.size()) ? -1 : ans);
					
 
    return 0;	
}