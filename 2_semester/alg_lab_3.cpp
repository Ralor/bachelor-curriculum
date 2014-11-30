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

void mergeSort(int *mass, int &n, int l, int r)
{
	r = min(r,n);
	if (l >= r) return;

	int ser = (l+r)/2;

	mergeSort(mass,n,l,ser);
	mergeSort(mass,n,ser + 1, r);

	int len = r - l + 1;
	int a = l, b = ser + 1;
	int *tmp = new int[len];

	int cur = 0;
	while (a <= ser && b <= r)
		if (mass[a] < mass[b])	 
			tmp[cur++] = mass[a++]; 	
		else	
			tmp[cur++] = mass[b++];			
	
	while (a <= ser)
		tmp[cur++] = mass[a++];
	while (b <= r)
		tmp[cur++] = mass[b++];

	for(cur = 0; cur < len; cur++)
		mass[l + cur] = tmp[cur];

	delete tmp;
}


int solution()
{		
	srand((unsigned int)time(0));
 
    int n; cin >> n;
    cout << "Length == " << n << "\n\n";
 
    int *mass = new int[n];
    for(int i = 0; i < n; i++)
        mass[i] = rand() % 30 + 1;

    cout << "Current mass:" << "\n\t";
    for(int i = 0; i < n; i++)
        cout << mass[i] << " "; cout << endl;    
     
	mergeSort(mass,n,0,n-1);
 
    cout << "Sorted mass:" << "\n\t";
    for(int i = 0; i < n; i++)
        cout << mass[i] << " "; cout << endl;    
 
    return 0;	
}