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

const int layers = 4, maxn = 1500, maxm = 10000000, maxt = 100000;
const int neu_num[layers] = {maxn + 1, 300 + 1, 60 + 1, 2};
const int inner = 0, outer = 0;
const double erange = 0.1, eps = 1e-8;
const double error_max = maxm * 0.01, ident = 0.95, learning_rate = 2, alfa = 0.3;

int m_num, t_num;
double error_sum, cur_error;
bool modified;
double ans[10];
int correct;
double exp_sum;
int order[maxm];

struct neuron
{
	int f_id, neu_id, layer_id;
	double w[1501], old[1501];
	double y, u, delta;

	neuron(int fid, int nid, int lid, double yinit)	 // 0 for sigmoid, 1 for relu, 2 for softmax
	{
		f_id = fid;
		neu_id = nid;
		layer_id = lid;
		if (lid)
			for (int i = 0; i < neu_num[lid - 1]; ++i)
			{
				w[i] = random() / maxn/maxn;
				old[i] = 0;
				if (random() > 0.5)
					w[i] = -w[i];
			}
		else
			y = yinit;
	}
	neuron()
	{
	}

	void burst();
	void cal_del();
	void adjust();

}	mlp[layers][1501];

double func(int fid, double u)
{
	if (fid == 0)
		return 1.0 / (1 + exp(-u));
	if (fid == 1)
		return u < 0 ? 0 : u;
	if (fid == 2)
	{
		return u;
	}
	return 0;
}

double gradient(int fid, double u, double y)
{
	if (fid == 0)
		return func(0, u) * (1 - func(0, u));
	if (fid == 1)
		return u < 0 ? 0 : 1;
	if (fid == 2)
	{
		//printf("%lf\n", y);
		return y * (1 - y);
	}
	return 0;
}

void neuron::burst()
{
	u = 0;
	for (int i = 0; i < neu_num[layer_id - 1]; ++i)
		u += mlp[layer_id - 1][i].y * w[i];
	if (layer_id < layers - 1 && neu_id == neu_num[layer_id]-1)
		y = 1;
	else
		y = func(f_id, u);
}

void neuron::cal_del()
{
	if (layer_id == layers - 1)
	{
		if (outer != 2)
			delta = (ans[neu_id] - y) * gradient(f_id, u, y);
		else
		{
			delta = 0;
			for (int i = 0; i < neu_num[layers - 1]; ++i)
				delta += ans[i] * ((i == neu_id) - y);
		}
	}
	else
	{
		delta = 0;
		if (neu_id == neu_num[layer_id]-1)
			return;
		for (int i = 0; i < neu_num[layer_id + 1]; ++i)
			delta += mlp[layer_id + 1][i].delta * mlp[layer_id + 1][i].w[neu_id];
		delta *= gradient(f_id, u, y);
	}
}

void neuron::adjust()
{
	for (int i = 0; i < neu_num[layer_id - 1]; ++i)
	{
		neuron *j = &mlp[layer_id - 1][i];
		double newd = old[j->neu_id] * alfa + learning_rate * delta * j->y;
		old[j->neu_id] = newd;
		w[j->neu_id] += newd;
		//printf("%lf\n", newd);
	}
}

void calc()
{
	for (int i = 1; i < layers; ++i)
		for (int j = 0; j < neu_num[i]; ++j)
			mlp[i][j].burst();
	//printf("round\n");
	if (outer == 2)
	{
		exp_sum = 0;
		double tmp = mlp[layers - 1][0].y;
		for (int i = 0; i < neu_num[layers - 1]; ++i)
		{
			if (mlp[layers - 1][i].y > tmp)
				tmp = mlp[layers - 1][i].y;
		}
		for (int i = 0; i < neu_num[layers - 1]; ++i)
		{
			mlp[layers - 1][i].y -= tmp;
			mlp[layers - 1][i].y = exp(mlp[layers - 1][i].y);
			exp_sum += mlp[layers - 1][i].y;
		}
		for (int i = 0; i < neu_num[layers - 1]; ++i)
		{
			mlp[layers - 1][i].y /= exp_sum;
			//printf("%lf\n", mlp[layers - 1][i].y);
		}
	}
	cur_error = 0;
	for (int i = 0; i < neu_num[layers - 1]; ++i)
	{
		if (outer != 2)
		{
			double tmp = ans[i] - mlp[layers - 1][i].y;
			cur_error += tmp * tmp;
		}
		else
			//if (ans[i] > eps)
			{
				cur_error -= ans[i] * log(mlp[layers - 1][i].y);
				//printf("%lf %lf\n", ans[i], mlp[layers - 1][i].y);
			}
	}
	if (cur_error > erange)
		modified = true;
	error_sum += cur_error;
	for (int i = layers - 1; i; --i)
		for (int j = 0; j < neu_num[i]; ++j)
			mlp[i][j].cal_del();
}

void adjust()
{
	for (int i = 1; i < layers; ++i)
		for (int j = 0; j < neu_num[i]; ++j)
			mlp[i][j].adjust();
}
