#ifndef _SPARSE_H_
#define _SPARSE_H_

#include <vector>
#include "sdef.h"

struct cs_sparse{
    int nzmax; // maxnum number of entries;
    int m; // number of rows;
    int n; // number of col;
    std::vector<int> p; // column pointers ( size n + 1) or col indices (size nzmax) 
    std::vector<int> i; // row indices , size nzmax
    std::vector<double> x; // numberical values, size nzmax
    int nz; // -1 for CSC, 1 for CSR, 0 for triplet matrix;
};

void matrix_to_csc(cs_sparse &matrixcsc, Matrix &matrix){
    
    matrixcsc.m = matrix.row;
    matrixcsc.n = matrix.col;
    matrixcsc.p.push_back(0);
    int p = 0 ;	
    for(int i = 0 ; i < matrix.col ; i++){
        for(int j = 0 ; j < matrix.row; j++){  
            if(!MbFltEQ(matrix.data[j][i],0)){
                matrixcsc.x.push_back(matrix.data[j][i]);
                matrixcsc.i.push_back(j);
                p = p + 1;
            }
        }
        matrixcsc.p.push_back(p);	
    }
    matrixcsc.nzmax = p;
    matrixcsc.nz = -1;
    matrix.release();
}
#endif	
