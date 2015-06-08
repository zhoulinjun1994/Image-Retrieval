import numpy as np
import string

# Parameters could be changed here
groundtruth_file = 'groundtruth.txt';
sort_file = 'sample.txt';

if __name__ == "__main__":
    fin_gt = open(groundtruth_file, "r");
    fin_s = open(sort_file, "r");
    sim = [];
    rank = [];
    MAP = 0;
    test_cnt = 0;
    while(True):
        tmp1 = fin_gt.readline();
        tmp2 = fin_s.readline();
        if(tmp1 == "" or tmp2 == ""):
            break;
        sim = tmp1.strip().split(' ');
        rank = tmp2.strip().split(' ');
        for i in range(0, len(rank)):
            sim[i] = string.atoi(sim[i], 10);
            rank[i] = string.atoi(rank[i], 10);
        AP = 0;
        cnt = 0;
        for i in range(0, len(rank)):
            if(sim[rank[i] - 1] == 1):
                cnt += 1;
                AP += (float(cnt) / float(i+1));
        AP /= float(cnt);
        MAP += AP;
        test_cnt += 1;
        print "AP(" + str(test_cnt) + "): " + str(AP);
    MAP /= float(test_cnt);
    print "MAP: " + str(MAP);
                
        