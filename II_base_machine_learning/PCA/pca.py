import numpy as np
import numpy.linalg as nplg
import sys

# PARAMETERS COULD BE CHANGED HERE!!!

input_data = 'data.txt'
output_data = 'result.txt'
decimal = '%.4f';
conv_dimension = 2;
cmpl_degree = 0.9;


#suppose E(a) = 0 and E(b) = 0
def calc_cov(a, b):
    assert(len(a) == len(b));
    return np.dot(a, b) / (len(a) - 1);
#Given a matrix and its rank, return a new matrix whose rank is convW
def PCA(matrix, H, W, convW):
    matrix_tmp = np.matrix(matrix);
    E = matrix.mean(axis = 0);
    print "Calculate expectations finished!"
    for i in range(0, W):
        matrix[:, i] -= E[i];
    CovXY = np.zeros([W, W]);
    need_to_deal = W * (W + 1) / 2;
    now_deal = 0;
    for i in range(0, W):
        for j in range(0, i+1):
            CovXY[i][j] = calc_cov(matrix[:, i], matrix[:, j]);
        now_deal += (i+1);
        sys.stdout.write(str(round(100 * float(now_deal) / float(need_to_deal), 2)) + "%\r");
        sys.stdout.flush();
    for i in range(0, W):
        for j in range(i+1, W):
            CovXY[i][j] = CovXY[j][i];
    print "Calculate covariances finished!"
    (eigvalue, eigvector) = nplg.eig(CovXY);
    print "Calculate eigvalues finished!"
    completeness = 0;
    if(convW > 0):
        eigSum = 0;
        for i in range(0, convW):
            eigSum += eigvalue[i];
        completeness = eigSum / np.sum(eigvalue);              
    else:
        eigSum = 0;
        for i in range(0, W):
            eigSum += eigvalue[i];
            completeness = eigSum / np.sum(eigvalue);
            if(completeness > cmpl_degree):
                convW = i + 1;
                break;
    print "Data Completeness: ",
    print completeness; 
    print "Converted Dimension: ",
    print convW;
    TransM = eigvector[:, :convW];     
    return np.dot(matrix_tmp, TransM);
if __name__ == "__main__":
    M = np.loadtxt(input_data)
    (H, W) = np.shape(M);
    result = PCA(M, H, W, conv_dimension);
    np.savetxt(output_data, result, decimal);
    print "PCA Algorithm Finished!"