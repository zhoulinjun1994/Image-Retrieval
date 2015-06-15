import os
import random
from numpy import ndarray
import numpy as np

if __name__ == '__main__':
    conceptNum = 10;
    label = [];
    L = os.listdir('Concept');
    for filename in L:
        label.append(np.loadtxt('Concept/' + filename));
    trainingSet = np.loadtxt('new_trainid.txt');
    featureList = np.loadtxt('Feature/traindata_image.txt');
    tagList = np.loadtxt('Feature/traindata_text.txt');
    print 'Load Basic Data Finished!'
    
    featureList = ndarray.tolist(featureList);
    tagList = ndarray.tolist(tagList);
    allList = [];
    for i in range(0, len(featureList)):
        allList.append(featureList[i] + tagList[i]);
    allList = np.array(allList);
    np.savetxt('Data/trdata.txt', allList, '%d');
    print 'Basic Features Finished'
    
    for i in range(0, conceptNum):
        reList = [];
        filename = 'Data/redata' + str(i) + '.txt';
        for j in range(0, len(featureList)):
            if(label[i][j] == 1):
                reList.append([1]);
            else:   
                reList.append([0]);
        reList = np.array(reList);
        np.savetxt(filename, reList, '%d');
    '''
    posNum = 10000;
    negNum = 10000;
    graphNum = 4000;
    similar = [];
    unsimilar = [];
    featureData = [];
    tagData = [];
    conclusionData = [];
    for i in range(0, graphNum):
        for j in range(0, i):
            for k in range(0, conceptNum):
                if(label[k][trainingSet[i]] == 1 and label[k][trainingSet[j]] == 1):
                    similar.append([i, j]);
                    break;
            unsimilar.append([i, j]);
    similarPairNum = len(similar);
    unsimilarPairNum = len(unsimilar);
    print 'Construct Similar Matrix Finished!';
    
    for i in range(0, posNum):
        rd = random.randint(0, similarPairNum - 1);
        featureData.append(np.abs(featureList[similar[rd][0]] - featureList[similar[rd][1]]));
        tagData.append(np.abs(tagList[similar[rd][0]] - tagList[similar[rd][1]]));
        conclusionData.append([1.0]);
    for i in range(0, negNum):
        rd = random.randint(0, unsimilarPairNum - 1);
        featureData.append(np.abs(featureList[unsimilar[rd][0]] - featureList[unsimilar[rd][1]]));
        tagData.append(np.abs(tagList[unsimilar[rd][0]] - tagList[unsimilar[rd][1]]));
        conclusionData.append([-1.0]);
    baseData = [];
    for i in range(0, posNum + negNum):
        baseData.append(list(featureData[i]) + list(tagData[i]) + conclusionData[i]);
    baseData = np.array(baseData);
    np.savetxt('testdata.txt', baseData);
    '''
        