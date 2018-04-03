#!/usr/bin/env python3
# @author = 53 68 79 61 6D 61 6C 
# date	  = 31/03/2018

from scipy.sparse import random
from scipy.sparse import csr_matrix
from scipy import stats
import numpy as np
import sys
from math import log

class CustomRandomState(object):
    def randint(self, k):
        i = np.random.randint(k)
        return i - i % 2

def main():
    V = [10**x for x in [1, 2, 4, 6, 8, 10]] [int(sys.argv[1])]
    #V = 7
    base = 2
    c = 2
    E = [1.1, 3, log(log(V, base), base), log(V, base) / c, log(V, base)] [int(sys.argv[2])] / V

    print(V)
    print(E)
    rs = CustomRandomState()
    rvs = stats.poisson(25, loc=10).rvs
    S = random(V, V, density=E, format='csr', random_state=rs, data_rvs=rvs)
    print(type(S))
    with open("testcase.data", "w") as f:
        f.write("{} {} \n".format(len(S.indices), len(S.indptr)))
        f.write(" ".join([str(x) for x in S.indptr]) + '\n')
        f.write(" ".join([str(x) for x in S.indices]) + '\n')
        f.write(" ".join([str(int(x)) for x in S.data]) + '\n')
    
    S = S.tocsc()
    
    with open("answer.data", "w") as f:
        # f.write("{} {} \n".format(len(S.indices), len(S.indptr)))
        f.write("\n")
        f.write(" ".join([str(x) for x in S.indptr]) + '\n')
        f.write(" ".join([str(x) for x in S.indices]) + '\n')
        f.write(" ".join([str(int(x)) for x in S.data]) + '\n')

if __name__ == "__main__":
    main()
