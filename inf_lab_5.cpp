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

FILE* open_file(string name, string flags)
{
	return fopen(name.c_str(), flags.c_str() );
}
void close_file(FILE *file_pointer)
{
	fclose(file_pointer);
}

struct Record
{
	Record()
	{
		next = NULL;
	}
	Record(int id, string f_name, string s_name, float value)
	{
		number = id;
		name = f_name;
		surname = s_name;
		mark = value;
		next = NULL;
	}
	
	string name, surname;
	float mark;
	int number;

	Record* next_pointer()
	{
		return next;
	}
	void new_next(Record *pointer)
	{
		next = pointer;
	}	

	Record *next;	
};
struct MyList
{
	MyList()
	{		
		initiated = false;		
		begin = end = current = new Record();
	}

	string name()
	{
		return current->name;
	}
	string surname()
	{
		return current->surname;
	}
	int number()
	{
		return current->number;
	}
	float mark()
	{
		return current->mark;
	}

	void add(Record *rec)
	{		
		if (initiated)
		{
			end->new_next(rec);
			end = rec;
		}
		else
		{
			begin = rec;
			current = rec;
			end = rec;
			initiated = true;
		}
	}
	void print(FILE *outp)
	{
		Record *rec = begin;
		do
		{
			fprintf(outp,"\t%i. %s %s - %.4f\n",
				rec->number,
				rec->name.c_str(),
				rec->surname.c_str(),
				rec->mark);

			rec = rec->next_pointer();
		} while (rec != NULL);
	}
	int find_record(int id)
	{
		Record *seek = begin;
		while (seek != end)
		{
			if (seek->number == id) break;
			seek = seek->next_pointer();
		}

		if (seek->number == id) 
		{
			current = seek;
			return 0;
		}
		else
		{
			return -1;
		}
	}

	Record *current, *begin, *end;	
	bool initiated;
};

int solution()
{	
	MyList group;

	freopen("group.txt","r",stdin);
	
	int id;
	while (cin >> id)
	{
		string name, surname; float mark;
		cin >> name >> surname >> mark;
		group.add( new Record(
			id,name,surname,mark));
	}

	FILE *output = open_file("super_group.txt","w");

	group.find_record(3); 
	fprintf(output,"%s\n\n",group.name().c_str() );
		
	group.print(output);
	close_file(output);

    return 0;	
}