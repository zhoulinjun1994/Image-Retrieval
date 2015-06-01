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

multi-layer neural network
输入：1500维向量，每个的输入值为两张图片tag和feature的差的平方
输出：0~1的实数，两张图片的共有concept的比例
训练数据：30000*30000对图片中随机挑取10000000对。
测试：对每个test-database对，跑一遍这个网络，最后用输出的实数从大到小排序。

结构：
1500+1-->300+1-->60+1-->1

```