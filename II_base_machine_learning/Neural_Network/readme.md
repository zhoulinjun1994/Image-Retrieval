```
This folder includes the Neural Network algorithm.

我把这里先当做草稿理顺一下思路吧= =

concept中有10个文件。每个文件有269648行，每行是0或者1表示id为行数的图片是否存在某个concept。

数据集有三个，分别为database，test以及train。数据集大小分别为100000，2000和30000。
每个数据集有三个文件叫做id，data_image以及data_text，分别为图片的编号，图片的特征（500维的int向量）以及图片的tag（1000长度的01串）。
groundtruth为一个2000*100000的矩阵，表示测试集中的图片是否和database中的图片有共同的concept。

tag里面为1000个tag的具体含义。

imageid有269648行，每行代表的是图片的编号。在数据集中使用的是这个编号，所以还要做一层的转换。

目标：已知训练集的concept，和所有数据的id，tag和feature。
对每个测试集中的图片，对database集中的图片按照与它的相关度进行排序。

solution:

old中：
multi-layer neural network
输入：1500维向量，每个的输入值为两张图片tag和feature的差的平方
输出：0~1的实数，两张图片的共有concept的比例
训练数据：30000*30000对图片中随机挑取10000000对。
测试：对每个test-database对，跑一遍这个网络，最后用输出的实数从大到小排序。
结构：
1500+1-->300+1-->60+1-->1
结果：前5000次训练并没有什么效果，全部都输出0.5，可能是训练数据太少。
但是测试的话需要跑2000*100000次网络，从时间消耗上来说已经不可行。

mlp_1中：
输入：1500维向量，每个输入为每张图片的feature和tag。其中feature除以了10，并且与1取了min。
输出：两个实数，表示是否有该concept的概率。
训练数据：全部，1次。每次随机找一个出来，正负例间隔。
测试：对每个concept，每个test和每个database都跑一遍，记录正确率（概率>=0.6当做是属于concept）。
结构：
1500+1-->300+1-->60+1-->2
结果：见log.txt。MAP为0.48，每个concept的正确率在0.8~0.9左右。

update:
NUS可以跑到0.73：修改了get_similarity函数：本来是都>=0.6的话加1否则加对应乘积再求和作为相似度。现在是把10维向量当做概率，算至少有一个concept相同的概率。
训练2轮的话可以到0.74。
文件及输出结果见NUSupdate。

2ktag中是把wiki的文章中选了tfidf最大的2k个以及cnn的10个输出作为mlp的输入训练。MAP=0.83。
5ktag中把tag的个数扩展到5k个，训练轮数改为2次。得出来的结果经过check.cpp之后可以得到0.908的MAP。

新增check.cpp，编译运行可以算出MAP。

```