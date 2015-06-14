#include "neuron.h"

const int testnum = 693, trainnum = 2173;
const int fea_num = 10, tag_num = 5427/*2061*/, train_times = 2;
const double threshold = 0.6;
bool concept[10][trainnum + testnum];
double fea_test[testnum][fea_num], fea_train[trainnum][fea_num];
double tag_test[testnum][tag_num], tag_train[trainnum][tag_num];
int sorted[trainnum];
double score[trainnum], testout[testnum][10], trainout[trainnum][10];
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
		return testout[x][k]>=threshold && trainout[y][k]>=threshold;
	for (int i = 0; i < 10; ++i)
		if (testout[x][i]>=threshold && trainout[y][i]>=threshold)
			return true;
	return false;
}

double get_similarity(int x, int y)
{
	double ans = 0, pre = 1;
	for (int i = 0; i < 10; ++i)
	{
		ans += pre * testout[x][i] * trainout[y][i];
		pre *= 1 - testout[x][i] * trainout[y][i];
	}
	return ans;
}

void test(int x, int k)
{
	for (int i = 0; i < fea_num; ++i)
		mlp[0][i] = neuron(0, i, 0, (k==1?fea_test:fea_train)[x][i]);
	for (int i = 0; i < tag_num; ++i)
		mlp[0][i+fea_num] = neuron(0, fea_num+i, 0, (k==1?tag_test:tag_train)[x][i]);
	mlp[0][fea_num+tag_num] = neuron(0, fea_num+tag_num, 0, 1);
	calc();
}

bool cmp(int p, int q) {return score[p] > score[q] + eps;}
const char outfile[10][20] = {
	"con0.txt", "con1.txt", "con2.txt", "con3.txt", "con4.txt",
	"con5.txt", "con6.txt", "con7.txt", "con8.txt", "con9.txt"};

void test(int j0)
{
	printf("testing %d\n", j0);
	ofstream fout(outfile[j0]);
	psum = nsum = pgood = ngood = 0;
	for (int i = 0; i < trainnum; ++i)
	{
		test(i, 0);
		trainout[i][j0] = (mlp[layers-1][0].y<mlp[layers-1][1].y?1-mlp[layers-1][1].y:mlp[layers-1][0].y);
		if (concept[j0][i])
		{
			++psum;
			if (trainout[i][j0]>=0.6)
				++pgood;
		}
		else
		{
			++nsum;
			if (trainout[i][j0]<=0.6)
				++ngood;
		}
		if (i % 500 == 300)
			printf("now testing %d\n\t%d/%d %d/%d\n", i,pgood,psum,ngood,nsum);
		fout << trainout[i][j0] << endl;
	}
	printf("finish database\n");
	for (int i = 0; i < testnum; ++i)
	{
		test(i, 1);
		testout[i][j0] = (mlp[layers-1][0].y<mlp[layers-1][1].y?1-mlp[layers-1][1].y:mlp[layers-1][0].y);
		fout << testout[i][j0] << endl;
	}
	printf("finish test %d\n", j0);
	fout.close();
	int cnt = 0, posum = 0, nesum = 0, tosum = 0, pogood = 0, negood = 0;
	for (int i = 0; i < testnum; ++i)
	{
		for (int j = 0; j < trainnum; ++j)
		{
			if (cal_ans(i+trainnum, j, j0)==cal_out(i, j, j0))
			{
				++cnt;
				if (cal_ans(i+trainnum, j, j0))
					++pogood;
				else
					++negood;
			}
			if (cal_ans(i+trainnum, j, j0))
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
		for (int j = 0; j < trainnum; ++j)
			if (cal_ans(i+trainnum, j, 10)==cal_out(i, j, 10))
				++cnt;
		if (i % 100 == 50)
			printf("%d correct rate : %.3lf\n", i, (double)cnt / (i+1) / trainnum);
	}
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
	ans[0] = concept[j][x];//*0.9+0.05;
	ans[1]=1-ans[0];
	calc();
	adjust();
	if (concept[j][x]==1)
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
		error_sum = 0;
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
			if (tot % 500 == 300)
			{
				printf("starting %dth training... cur avg sum : %.3lf\n", tot, (double)error_sum / tot);
				printf("\tstatus: %d/%d %d/%d\n",pgood,psum,ngood,nsum);
			}
			modified = false;
			int num = rand()%trainnum;
			while (concept[j][num]!=tot%2)
				num = rand()%trainnum;
			train(num,j);//trainorder[tot], j);
		}
	}
}

void read_concept()
{
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

void read_data(double (*feature)[fea_num], double (*tag)[tag_num], int num, string file)
{
	ifstream fin((file + "_fea.txt").c_str());
	for (int i = 0; i < fea_num; ++i)
		for (int j = 0; j < num; ++j)
			fin >> feature[j][i];
	fin.close();
	fin.open((file + "_tag.txt").c_str());
	for (int i = 0; i < num; ++i)
		for (int j = 0; j < tag_num; ++j)
			fin >> tag[i][j];
	fin.close();
}

void initialize()
{
	if (OUTPUT)
		freopen("result.txt", "w", stdout);
	//concept
	read_concept();
	//database
	//read_data(fea_database, tag_database, id_database, databasenum, "database");
	read_data(fea_test, tag_test, testnum, "testset");
	read_data(fea_train, tag_train, trainnum, "trainset");

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
