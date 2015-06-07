#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <map>
using namespace std;

const int allnum = 269648, databasenum = 100000, testnum = 2000, trainnum = 30000;
map<long long, int> p;

void doit(const char *f, const char *out, const int n)
{
	ifstream fin(f);
	ofstream fout(out);
	for (int i = 0; i < n; ++i)
	{
		long long tmp;
		fin >> tmp;
		if (p.find(tmp) == p.end())
			printf("in %s : row %d not found in whole database!!! named %lld\n", f, i, tmp);
		fout << p[tmp] << endl;
	}
	fin.close();
	fout.close();
}

int main()
{
	ifstream fin("ImageId.txt");
	for (int i = 0; i < allnum; ++i)
	{
		long long tmp;
		fin >> tmp;
		if (p.find(tmp) != p.end())
			printf("id of row %d is the same as row %d!!! named %lld\n", i, p[tmp], tmp);
		p[tmp] = i;
	}
	fin.close();
	doit("Feature/databaseid.txt", "new_databaseid.txt", databasenum);
	doit("Feature/testid.txt", "new_testid.txt", testnum);
	doit("Feature/trainid.txt", "new_trainid.txt", trainnum);
	return 0;
}