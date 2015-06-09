#include "neuron.h"

const int overallnum = 269648, databasenum = 100000, testnum = 2000, trainnum = 30000;
const int fea_num = 500, tag_num = 1000, train_times = 1;
bool concept[10][overallnum];
double fea_database[databasenum][fea_num], fea_test[testnum][fea_num], fea_train[trainnum][fea_num];
bool tag_database[databasenum][tag_num], tag_test[testnum][tag_num], tag_train[trainnum][tag_num];
int id_database[databasenum], id_test[testnum], id_train[trainnum];
int sorted[databasenum];
double score[databasenum], testout[testnum][10], databaseout[databasenum][10];
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

bool cal_out(int x, int y, int k)
{
	if (k < 10)
		return testout[x][k]>=0.6 && databaseout[y][k]>=0.6;
	for (int i = 0; i < 10; ++i)
		if (testout[x][i]>=0.6 && databaseout[y][i]>=0.6)
			return true;
	return false;
}

double get_similarity(int x, int y)
{
	double ans = 0;
	for (int i = 0; i < 10; ++i)
		if (testout[x][i]>=0.6 && databaseout[y][i]>=0.6)
			ans += 1;
		else
			ans += testout[x][i] * databaseout[y][i];
	return ans;
}

void test(int x, int k)
{
	for (int i = 0; i < fea_num; ++i)
		mlp[0][i] = neuron(0, i, 0, (k==1?fea_test:fea_database)[x][i]);
	for (int i = 0; i < tag_num; ++i)
		mlp[0][i+fea_num] = neuron(0, fea_num+i, 0, (k==1?tag_test:tag_database)[x][i]);
	mlp[0][fea_num+tag_num] = neuron(0, fea_num+tag_num, 0, 1);
	calc();
}

bool cmp(int p, int q) {return score[p] > score[q] + eps;}

void test(int j0)
{
	printf("testing %d\n", j0);
	psum = nsum = pgood = ngood = 0;
	for (int i = 0; i < databasenum; ++i)
	{
		test(i, 0);
		databaseout[i][j0] = (mlp[layers-1][0].y<mlp[layers-1][1].y?1-mlp[layers-1][1].y:mlp[layers-1][0].y);
		if (concept[j0][id_database[i]])
		{
			++psum;
			if (databaseout[i][j0]>=0.6)
				++pgood;
		}
		else
		{
			++nsum;
			if (databaseout[i][j0]<=0.6)
				++ngood;
		}
		if (i % 10000 == 5000)
			printf("now testing %d\n\t%d/%d %d/%d\n", i,pgood,psum,ngood,nsum);
	}
	printf("finish database\n");
	for (int i = 0; i < testnum; ++i)
	{
		test(i, 1);
		testout[i][j0] = (mlp[layers-1][0].y<mlp[layers-1][1].y?1-mlp[layers-1][1].y:mlp[layers-1][0].y);
	}
	printf("finish test %d\n", j0);
	int cnt = 0, posum = 0, nesum = 0, tosum = 0, pogood = 0, negood = 0;
	for (int i = 0; i < testnum; ++i)
	{
		for (int j = 0; j < databasenum; ++j)
		{
			if (cal_ans(id_test[i], id_database[j], j0)==cal_out(i, j, j0))
			{
				++cnt;
				if (cal_ans(id_test[i], id_database[j], j0))
					++pogood;
				else
					++negood;
			}
			if (cal_ans(id_test[i], id_database[j], j0))
				++posum;
			else
				++nesum;
			++tosum;
		}
		if (i % 100 == 50)
			printf("%d correct rate : [%.3lf %.3lf %.3lf]\n", i, (double)cnt/tosum, (double)pogood/posum,(double)negood/nesum);
	}
}

void output()
{
	int cnt = 0;
	printf("testing all\n");
	for (int i = 0; i < testnum; ++i)
	{
		for (int j = 0; j < databasenum; ++j)
			if (cal_ans(id_test[i], id_database[j], 10)==cal_out(i, j, 10))
				++cnt;
		if (i % 100 == 50)
			printf("%d correct rate : %.3lf\n", i, (double)cnt / (i+1) / databasenum);
	}
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
			if (cal_ans(id_test[i], id_database[j], 10))
				ap += (double)(++cnt)/(j+1);
		sum += ap / cnt;
		if (i % 100 == 50)
			printf("%d map : %.3lf\n", i, sum / (i+1));
	}
	printf("MAP = %.3lf\n", sum / testnum);
}

void train(int x, int j)
{
	for (int i = 0; i < fea_num; ++i)
		mlp[0][i] = neuron(0, i, 0, fea_train[x][i]);
	for (int i = 0; i < tag_num; ++i)
		mlp[0][i+fea_num] = neuron(0, fea_num+i, 0, tag_train[x][i]);
	mlp[0][fea_num+tag_num] = neuron(0, fea_num+tag_num, 0, 1);
	//for (int i = 0; i < 10; ++i)
	//	ans[i] = concept[i][id_train[x]];
	ans[0] = concept[j][id_train[x]];
	ans[1]=1-ans[0];
	calc();
	adjust();
	if (concept[j][id_train[x]]==1)
	{
		if (mlp[layers-1][0].y>=mlp[layers-1][1].y)
			++pgood;
		++psum;
	}
	else
	{
		if (mlp[layers-1][0].y<=mlp[layers-1][1].y)
			++ngood;
		++nsum;
	}
}

void train(int j)
{
	printf("\n\ntraining %d\n", j);
	psum = nsum = pgood = ngood = 0;
	error_sum = 0;
	for (int k = 0; k < train_times; ++k)
	{
		int cur = 0;
		for (int i = 0; i < trainnum; ++i)
		{
			trainorder[i] = i;
			swap(trainorder[i], trainorder[rand()%(i+1)]);
			//if (concept[j][id_train[i]]==1)
			//	swap(trainorder[i], trainorder[cur++]);
		}
		for (int tot = 0; tot < trainnum; ++tot)
		{
			if (tot % 1000 == 500)
			{
				printf("starting %dth training... cur avg sum : %.3lf\n", tot, (double)error_sum / tot);
				printf("\tstatus: %d/%d %d/%d\n",pgood,psum,ngood,nsum);
			}
			modified = false;
			int num = rand()%trainnum;
			while (concept[j][id_train[num]]!=tot%2)
				num = rand()%trainnum;
			train(num,j);//trainorder[tot], j);
		}
		error_sum = 0;
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

void read_data(double (*feature)[fea_num], bool (*tag)[tag_num], int *id, int num, string prefix)
{
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

	printf("init done\n");
}

int main()
{
	srand(0);//(unsigned)time(NULL));
	initialize();
	for (int k = 0; k < 10; ++k)
	{
		for (int i = 1; i < layers; ++i)
			for (int j = 0; j < neu_num[i]; ++j)
				mlp[i][j] = neuron((i < layers - 1 ? inner : outer), j, i, 0);
		train(k);
		test(k);
	}
	output();
	return 0;
}
