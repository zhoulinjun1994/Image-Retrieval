#include "neuron.h"

const int overallnum = 269648, databasenum = 100000, testnum = 2000, trainnum = 30000;
const int fea_num = 500, tag_num = 1000, train_times = 3000;
bool concept[10][overallnum];
int fea_database[databasenum][fea_num], fea_test[testnum][fea_num], fea_train[trainnum][fea_num];
bool tag_database[databasenum][tag_num], tag_test[testnum][tag_num], tag_train[trainnum][tag_num];
int id_database[databasenum], id_test[testnum], id_train[trainnum];
int sorted[databasenum];
double score[databasenum];

int sqr(int x) {return x * x;}
int cal_ans(int x, int y)
{
	for (int i = 0; i < 10; ++i)
		if (concept[i][x] && concept[i][y])
			return 1;
	return 0;
}

double test(int x, int y)
{
	for (int i = 0; i < fea_num; ++i)
		mlp[0][i] = neuron(0, i, 0, sqr(fea_test[x][i] - fea_database[y][i]));
	for (int i = 0; i < tag_num; ++i)
		mlp[0][i] = neuron(0, fea_num+i, 0, sqr(tag_test[x][i] - tag_database[y][i]));
	mlp[0][fea_num+tag_num] = neuron(0, fea_num+tag_num, 0, 1);
	calc();
	return mlp[layers-1][0].y-mlp[layers-1][1].y;
}

bool cmp(int p, int q) {return score[p] > score[q] + eps;}

void test()
{
	printf("testing\n");
	int cnt = 0, c1 = 0, c2 = 0;
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 1000; ++j)
		{
			//if (j%100==0)cout << "at " << j << endl;
			int x, y;
			while (1)
			{
				x = rand()%testnum, y = rand()%databasenum;
				if ((cal_ans(id_test[x], id_database[y])==1) == (i^j)%2)
					break;
			}
			score[j] = test(x, y);
			if ((score[j]>0) == (cal_ans(id_test[x], id_database[y])==1))
			{
				++cnt;
				if (score[j]>0)
					++c1;
				else
					++c2;
			}
		}
		printf("correct rate : %.6lf\n", (double)cnt / 1000 / (i+1));
		printf("c1 = %d c2 = %d\n", c1, c2);
	}
}

void output()
{
	for (int i = 0; i < testnum; ++i)
	{
		for (int j = 0; j < databasenum; ++j)
		{
			score[j] = test(i, j);
			sorted[j] = j;
		}
		sort(sorted, sorted + databasenum, cmp);
		for (int j = 0; j < databasenum; ++j)
			printf("%d ", id_database[sorted[j]]);
		printf("\n");
	}
}

void train(int x, int y)
{
	for (int i = 0; i < fea_num; ++i)
		mlp[0][i] = neuron(0, i, 0, sqr(fea_train[x][i] - fea_train[y][i]));
	for (int i = 0; i < tag_num; ++i)
		mlp[0][i] = neuron(0, fea_num+i, 0, sqr(tag_train[x][i] - tag_train[y][i]));
	mlp[0][fea_num+tag_num] = neuron(0, fea_num+tag_num, 0, 1);
	ans[0] = cal_ans(id_train[x], id_train[y]);ans[1]=1-ans[0];
	calc();
	adjust();
	{
		if ((mlp[layers-1][0].y>=mlp[layers-1][1].y) == (ans[0]>1-eps))
			++correct;
	}
}

void train()
{
	correct = 0;
	error_sum = 0;
	for (int tot = 0; tot < train_times; ++tot)
	{
		if (tot % 100 == 50)
			printf("starting %dth training... cur avg sum : %.3lf\n", tot, (double)error_sum / tot);
		modified = false;
		int x, y;
		while (1)
		{
			x = rand()%trainnum, y = rand()%trainnum;
			//if ((cal_ans(id_train[x], id_train[y])==1) == tot%2)
				break;
		}
		train(x, y);
	}
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

void read_data(int (*feature)[fea_num], bool (*tag)[tag_num], int *id, int num, string prefix)
{
	ifstream fin(("Feature/" + prefix + "data_image.txt").c_str());
	for (int i = 0; i < num; ++i)
		for (int j = 0; j < fea_num; ++j)
			fin >> feature[i][j];
	fin.close();
	fin.open(("Feature/" + prefix + "data_text.txt").c_str());
	for (int i = 0; i < num; ++i)
		for (int j = 0; j < tag_num; ++j)
			fin >> tag[i][j];
	fin.close();
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
	read_data(fea_database, tag_database, id_database, databasenum, "database");
	read_data(fea_test, tag_test, id_test, testnum, "test");
	read_data(fea_train, tag_train, id_train, trainnum, "train");

	for (int i = 1; i < layers; ++i)
		for (int j = 0; j < neu_num[i]; ++j)
			mlp[i][j] = neuron((i < layers - 1 ? inner : outer), j, i, 0);
	printf("init done\n");
}

int main()
{
	srand(0);//(unsigned)time(NULL));
	initialize();
	train();
	test();
	//output();
	return 0;
}
