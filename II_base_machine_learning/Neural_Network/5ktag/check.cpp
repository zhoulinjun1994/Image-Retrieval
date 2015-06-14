#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <fstream>
using namespace std;

const int testnum = 693, trainnum = 2173;
double s[testnum+trainnum][10], score[trainnum], sum[testnum+trainnum];
bool concept[10][trainnum + testnum];
int sorted[trainnum];
const double eps = 1e-8;
const char outfile[10][20] = {
	"./con0.txt", "./con1.txt", "./con2.txt", "./con3.txt", "./con4.txt",
	"./con5.txt", "./con6.txt", "./con7.txt", "./con8.txt", "./con9.txt"};

bool cmp(int p, int q) {return score[p] > score[q] + eps;}

int getmax(int x)
{
	int ans = 0;
	for (int i = 1; i < 10; ++i)
		if (s[x][i] > s[x][ans])
			ans = i;
	return ans;
}

double get_similarity(int x, int y)
{
	double ans = 0, pre = 1;
	for (int i = 0; i < 10; ++i)
	{
		ans += pre * s[x+trainnum][i] * s[y][i] / sum[x+trainnum] / sum[y];
		pre *= 1 - s[x+trainnum][i] * s[y][i] / sum[x+trainnum] / sum[y];
	}
	if (getmax(x+trainnum) != getmax(y))
		return ans / 5;
	return ans;
}

bool cal_ans(int x, int y, int k)
{
	if (k < 10)
		return concept[k][x] && concept[k][y];
	for (int i = 0; i < 10; ++i)
		if (concept[i][x] && concept[i][y])
			return true;
	return false;
}

void read_concept()
{
	memset(concept, 0, sizeof(concept));
	ifstream fin("trainset_txt_img_cat.list");
	string s0, s1;
	int k;
	for (int i = 0; i < trainnum; ++i)
	{
		fin >> s0 >> s1 >> k;
		concept[k-1][i] = 1;
	}
	fin.close();
	fin.open("testset_txt_img_cat.list");
	for (int i = 0; i < testnum; ++i)
	{
		fin >> s0 >> s1 >> k;
		concept[k-1][i+trainnum] = 1;
	}
	fin.close();
}

void output()
{
	int cnt = 0;
	double sum = 0, ap;
	for (int i = 0; i < testnum; ++i)
	{
		ap = 0;
		cnt = 0;
		for (int j = 0; j < trainnum; ++j)
		{
			score[j] = get_similarity(i, j);
			sorted[j] = j;
		}
		sort(sorted, sorted + trainnum, cmp);
		for (int j = 0; j < trainnum; ++j)
			if (cal_ans(i+trainnum, sorted[j], 10))
			{
				ap += (double)(++cnt)/(j+1);
				//if (i == 681)
				//cout << cnt << "/" << j+1 << " " << score[sorted[j]] << endl;
			}
		/*
		if (i==681){
			for (int j = 0; j < 10; ++j)
				cout << s[i+trainnum][j] << " ";
			cout << endl;
			for (int j = 0; j < 10; ++j)
				cout << concept[j][i+trainnum] << " ";
			cout << endl;
		for (int k = 0; k < 10; ++k)
		{
			for (int j = 0; j < 10; ++j)
				cout << s[sorted[k]][j] << " ";
			cout << endl;
			cout << "score = " << get_similarity(i, sorted[k]) << endl;
			for (int j = 0; j < 10; ++j)
				cout << concept[j][sorted[k]] << " ";
			cout << endl;
		}
		}
		*/
		/*
		if (ap/cnt<0.5)
		{
			cout << endl << "ap = " << ap/cnt << " " << i << endl;
			for (int j = 0; j < 10; ++j)
				cout << s[i+trainnum][j] << " ";
			cout << endl;
			for (int j = 0; j < 10; ++j)
				cout << concept[j][i+trainnum] << " ";
			cout << endl;
		}*/
		sum += ap / cnt;
		if (i % 100 == 50)
			printf("%d map : %.3lf\n", i, sum / (i+1));
	}
	printf("MAP = %.3lf\n", sum / testnum);
}

int main()
{
	read_concept();
	for (int i = 0; i < 10; ++i)
	{
		ifstream fin(outfile[i]);
		for (int j = 0; j < trainnum + testnum; ++j)
		{
			fin >> s[j][i];
			sum[j] += s[j][i];
		}
		fin.close();
	}
	output();
	return 0;
}