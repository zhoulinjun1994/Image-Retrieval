```
This folder is used to test the precision of the algorithm.

可选参数：见程序中“Parameters could be changed here”下面两行
groundtruth_file: 对应2000*100000矩阵，表示测试集和database中的文件相似性，具体格式见样例groundtruth.txt
sort_file: 对应2000*100000矩阵，每行表示对一个测试样本的database集的排序。具体格式见样例sample.txt

这里详细解释一下sort_file的结构：
每一行表示一个测试样本对应的排序，假设为2 4 1 3，则表示第一名为database中的第2个图片，第二名为database中的第4个图片，第三
名为database中的第1个图片，第四名为database中的第3个图片，注意database中的图片序号在这里是从1开始的。

运行方法： python calc_map.py

输出：
AP(1): xxx
AP(2): xxx
......
AP(n): xxx
MAP: xxx

```