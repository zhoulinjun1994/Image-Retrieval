```
tag.cpp
将train的text按照类别合并成10个文件，然后计算tfidf。若某一单词在某一文章的tfidf > 1e-4，则保留为tag（共2000+个），存在tag2.txt。每个tag在每类文章中，出现该tag的文章数参见tags2_stat_2.txt。

TE.cpp
在train和test中寻找上文计算出的tag出现次数，然后保存在某文章中该tag出现次数*1000/文章词数，这样可以保持在一个科学的数量级。结果在testset_tag.txt和trainset_tag.txt


```
