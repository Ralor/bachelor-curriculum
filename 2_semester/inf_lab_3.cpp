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
	//freopen("input.txt","r",stdin);
	//freopen("output.txt","w",stdout);	
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

#define read(x,file) fread((void*)&x,sizeof(typeid(x).name()),1,file)
#define write(x,file) fwrite(&x,sizeof(typeid(x).name()),1,file)
#define go(pos,file) fseek(file,pos,SEEK_SET)

void print(FILE *inp)
{
	int pos = ftell(inp);
	rewind(inp);
	int n,m; read(n,inp); read(m,inp);
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < m; j++)
		{
			float tmp; read(tmp,inp);
			printf("%f ",tmp);
		} printf("\n");
	} 
	go(pos,inp);
}


int solution()
{	
	string file_name;
	cout << "\nEnter current file name:\n\n\t---> ";
	cin >> file_name; cout << "\n\n";

	FILE *file = fopen(file_name.c_str(),"rb+");	

#pragma region generateInp
	//int n = 3, m = 5;	
	//write(n,file); write(m,file); 	
	//for(int i = 0; i < n; i++)
	//	for(int j = 1; j <= m; j++)
	//	{
	//		float x = (float)j; write(x,file); 
	//	} cout << "\n";
	//print(file); fclose(file);
#pragma endregion

#pragma region deleteRows
	int n,m; read(n,file); read(m,file);	
	print(file);

	int cur_pos, pen_pos;
	cur_pos = pen_pos = ftell(file);		
	for(int i = 1; i <= n; i++)
	for(int j = 1; j <= m; j++)
	{
		float tmp; read(tmp,file);

		if (j == n)
		{
			j = m;
			go(cur_pos + (m - n + 1)*sizeof(float),file);
		}
		cur_pos = ftell(file);

		go(pen_pos,file); write(tmp,file); pen_pos += sizeof(float);
		go(cur_pos,file);		
	}
	rewind(file); m = n;
	write(n,file); write(m,file);

	cout << "\nTotal " << cur_pos - pen_pos << " bytes of garbage\n\n";
	
	print(file); fclose(file); 

	string new_name;
		cout << "\nEnter new file name:\n\n\t---> "; 
		cin >> new_name;
	rename(file_name.c_str(),new_name.c_str());	
#pragma endregion

	cout << "\n";
	system("PAUSE");

	return 0;	
}