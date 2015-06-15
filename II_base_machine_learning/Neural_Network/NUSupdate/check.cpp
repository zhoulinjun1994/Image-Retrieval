#include <cstdio>
#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>
#define random() (double)rand() / RAND_MAX
#define OUTPUT (0)
using namespace std;

const int overallnum = 269648, databasenum = 100000, testnum = 2000, trainnum = 30000;
const int fea_num = 500, tag_num = 1000, train_times = 3;
const double eps = 1e-10;
bool concept[10][overallnum];
//double fea_database[databasenum][fea_num], fea_test[testnum][fea_num], fea_train[trainnum][fea_num];
//bool tag_database[databasenum][tag_num], tag_test[testnum][tag_num], tag_train[trainnum][tag_num];
int id_database[databasenum], id_test[testnum], id_train[trainnum];
int sorted[databasenum];
double score[databasenum], testout[testnum][10], databaseout[databasenum][10];
double testsum[testnum], databasesum[databasenum];
int trainorder[trainnum];
int psum, nsum, pgood, ngood;

int sqr(int x) {return x * x;}
bool cal_ans(int x, int y, int k)
{
	if (k < 10)
		return concept[k][x] && concept[k][y];
	for (int i = 0; i < 10; ++i)
		if (concept[i][x] && concept[i][y])
			return true;
	return false;
}
/*
bool cal_out(int x, int y, int k)
{
	if (k < 10)
		return testout[x][k]>=0.6 && databaseout[y][k]>=0.6;
	for (int i = 0; i < 10; ++i)
		if (testout[x][i]>=0.6 && databaseout[y][i]>=0.6)
			return true;
	return false;
}
*/

double get_similarity(int x, int y)
{
	double ans = 0, pre = 1;
	for (int i = 0; i < 10; ++i)
	{
		ans += pre * testout[x][i] * databaseout[y][i];// /testsum[x]/databasesum[y];
		pre *= 1 - testout[x][i] * databaseout[y][i];// /testsum[x]/databasesum[y];
	}
	return ans;
}

bool cmp(int p, int q) {return score[p] > score[q] + eps;}
const char outfile[10][20] = {
	"con0.txt", "con1.txt", "con2.txt", "con3.txt", "con4.txt",
	"con5.txt", "con6.txt", "con7.txt", "con8.txt", "con9.txt"};

void output()
{
	int cnt = 0;
	/*
	printf("testing all\n");
	for (int i = 0; i < testnum; ++i)
	{
		for (int j = 0; j < databasenum; ++j)
			if (cal_ans(id_test[i], id_database[j], 10)==cal_out(i, j, 10))
				++cnt;
		if (i % 100 == 50)
			printf("%d correct rate : %.3lf\n", i, (double)cnt / (i+1) / databasenum);
	}
	*/
	double sum = 0, ap;
	for (int i = 0; i < testnum; ++i)
	{
		ap = 0;
		cnt = 0;
		for (int j = 0; j < databasenum; ++j)
		{
			score[j] = get_similarity(i, j);
			sorted[j] = j;
		}
		sort(sorted, sorted + databasenum, cmp);
		for (int j = 0; j < databasenum; ++j)
			if (cal_ans(id_test[i], id_database[sorted[j]], 10))
				ap += (double)(++cnt)/(j+1);
		sum += ap / cnt;
		if (i % 100 == 50)
		{
			printf("%d map : %.3lf\n", i, sum / (i+1));
			//printf("%.3lf %d\n", ap, cnt);
		}
	}
	printf("MAP = %.3lf\n", sum / testnum);
}

const string concept_prefix = "Concept/Labels_";
const char concept_suffix[10][20] = {
"animal.txt", "buildings.txt", "clouds.txt", "grass.txt", 
"lake.txt", "person.txt", "plants.txt", "sky.txt", "water.txt", "window.txt"};

void read_concept(int ind)
{
	ifstream fin((concept_prefix + concept_suffix[ind]).c_str());
	for (int i = 0; i < overallnum; ++i)
		fin >> concept[ind][i];
	fin.close();
}

void read_data(/*double (*feature)[fea_num], bool (*tag)[tag_num], */int *id, int num, string prefix)
{
	/*
	ifstream fin(("Feature/" + prefix + "data_image.txt").c_str());
	for (int i = 0; i < num; ++i)
		for (int j = 0; j < fea_num; ++j)
		{
			fin >> feature[i][j];
			if (feature[i][j]>10)
				feature[i][j]=1;
			else
				feature[i][j]/=10.0;
		}
	fin.close();
	fin.open(("Feature/" + prefix + "data_text.txt").c_str());
	for (int i = 0; i < num; ++i)
		for (int j = 0; j < tag_num; ++j)
			fin >> tag[i][j];
	fin.close();
	*/
	ifstream fin;
	fin.open(("new_" + prefix + "id.txt").c_str());
	for (int i = 0; i < num; ++i)
		fin >> id[i];
	fin.close();
}

void initialize()
{
	if (OUTPUT)
		freopen("result.txt", "w", stdout);
	//concept
	for (int i = 0; i < 10; ++i)
		read_concept(i);
	//database
	
	read_data(/*fea_database, tag_database, */id_database, databasenum, "database");
	read_data(/*fea_test, tag_test, */id_test, testnum, "test");
	read_data(/*fea_train, tag_train, */id_train, trainnum, "train");
	
	for (int j = 0; j < 10; ++j)
	{
		ifstream fin(outfile[j]);
		for (int i = 0; i < databasenum; ++i)
		{
			fin >> databaseout[i][j];
			databasesum[i] += databaseout[i][j];
		}
		for (int i = 0; i < testnum; ++i)
		{
			fin >> testout[i][j];
			testsum[i] += testout[i][j];
		}
		fin.close();
	}
	printf("init done\n");
}

int main()
{
	initialize();
	output();
	return 0;
}
