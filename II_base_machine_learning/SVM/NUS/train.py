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

if __name__ == '__main__':
    x = np.loadtxt('Data/trdata_all.txt');
    y = np.loadtxt('Data/redata7.txt');
    x_train, x_test, y_train, y_test = cv.train_test_split(x, y, test_size=0.2, random_state=42);
    print 'Data Ready!'
    #clf = svm.SVC(kernel='rbf',C=1024, class_weight='auto')
    #clf = lm.LogisticRegression();
    clf = tree.DecisionTreeClassifier();
    #clf = nb.GaussianNB();
    clf = clf.fit(x_train, y_train);
    pre = clf.predict(x_test);
    for i in range(0, 50):
        print pre[i],
        print ' ',
        print y_test[i];
    
    [CC, TP, FN] = calc_correct(ndarray.tolist(pre), ndarray.tolist(y_test));
    print CC;
    print TP;
    print FN;
    
    