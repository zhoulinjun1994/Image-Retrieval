import os
import random
from numpy import ndarray
import numpy as np
from sklearn import cross_validation as cv
from sklearn import linear_model as lm
from sklearn import svm
from sklearn import tree
from sklearn import naive_bayes as nb
from sklearn import ensemble as en
from sklearn import neighbors as nn
from sklearn import preprocessing as pp

def calc_correct(predict, actual):
    tp = 0;
    tn = 0;
    fp = 0;
    fn = 0;
    for i in range(0, len(predict)):
        if(predict[i] > 0.5 and actual[i] > 0.5):
            tp += 1;
        elif(predict[i] < 0.5 and actual[i] > 0.5):
            tn += 1;
        elif(predict[i] > 0.5 and actual[i] < 0.5):
            fp += 1;
        else:
            fn += 1;
    truepositive = float(tp) / float(tp + tn);
    falsenegative = float(fn) / float(fn + fp);
    correctness = float(tp + fn) / float(tp + tn + fp + fn);
    return [correctness, truepositive, falsenegative];

def calc_correct2(predict, actual):
    t = 0;
    f = 0;
    for i in range(0, len(predict)):
        if(predict[i] == actual[i]):
            t += 1;
        else:
            f += 1;
    print t;
    print f;
    return float(t) / float(t + f);
    
def groundtruth(y_test, y_train):
    gd = [[] for i in range(0, len(y_test))];
    for i in range(0, len(y_test)):
        for j in range(0, len(y_train)):
            if(y_test[i] == y_train[j]):
                gd[i].append(1);
            else:
                gd[i].append(0);
    gd = np.array(gd);
    np.savetxt('groundtruth_wiki.txt', gd, '%d');

if __name__ == '__main__':
    x_train = np.loadtxt('trainset_tag5k.txt');
    y_train = np.loadtxt('trainset_txt_img_cat.list');
    x_test = np.loadtxt('testset_tag5k.txt');
    y_test = np.loadtxt('testset_txt_img_cat.list');
    #min_max_scaler = pp.MinMaxScaler();
    #x_train = min_max_scaler.fit_transform(x_train);
    #x_test = min_max_scaler.fit_transform(x_test);
    print 'Data Ready!'
    #clf = svm.SVC(kernel='rbf',C=2048, class_weight='auto')
    clf = lm.LogisticRegression(C = 0.01, random_state=42);
    #clf = nb.GaussianNB();
    clf = clf.fit(x_train, y_train);
    pre = clf.predict(x_test);
    for i in range(0, 50):
        print pre[i],
        print ' ',
        print y_test[i];
    
    CC = calc_correct2(ndarray.tolist(pre), ndarray.tolist(y_test));
    print CC;
    #groundtruth(ndarray.tolist(y_test), ndarray.tolist(y_train));
    
    result = [[] for i in range(0, len(pre))];
    for i in range(0, len(pre)):
        flag = [0 for j in range(0, len(y_train))];
        for j in range(0, len(y_train)):
            if(y_train[j] == pre[i]):
                result[i].append(j+1);
                flag[j] = 1;
        for j in range(0, len(y_train)):
            if(flag[j] == 0):
                result[i].append(j+1);
                flag[j] = 1;
    result = np.array(result);
    np.savetxt('result.txt', result, '%d');
    
    
    
    
    
    