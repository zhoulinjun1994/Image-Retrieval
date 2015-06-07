```
This folder includes the PCA algorithm.

使用说明：

可修改参数有5个：
input_data : 输入文件，格式见data.txt（两数据间为空格就行，几个都可以）
output_data : 输出文件，格式见result.txt
decimal : 输出结果保留几位小数，格式为"%.4f"
conv_dimension : 目标维数，当大于0时按照该维数运行PCA
cmpl_degree : 数据完整程度（即：保留原矩阵的信息量），如果conv_dimension设为0，则PCA的维数为大于该阈值的最小维数。

运行方法：python pca.py
```
