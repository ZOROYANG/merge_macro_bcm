#include "class.h"

MATRIX::MATRIX(){}
MATRIX::MATRIX(double **Matrix, int m, int n){
    matrix = Matrix;
    row = m;
    col = n;
}
MATRIX::MATRIX(int m , int n){
    matrix  = new double *[m];
    for (int i = 0 ; i < m ; i++){
        matrix[i] = new double [n];
    }
    row = m ;
    col = n;
}
void MATRIX::RESIZE(int m , int n){
    matrix  = new double *[m];
    for (int i = 0 ; i < m ; i++){
        matrix[i] = new double[n];
    }
    row = m ;
    col = n;
}
MATRIX::~MATRIX(){
    for (int i = 0 ; i < row ; i++){
        if (matrix[i] != NULL){
            delete[] matrix[i];
        }
    }
    delete[] matrix;
}
void MATRIX::RELEASE(){
    for (int i = 0 ; i < row ; i++){
        if (matrix[i] != NULL){
            delete[] matrix[i];
        }
    }
    delete[] matrix;
    }

OMATRIX::OMATRIX(int m){
        vector = new double[m];
        size = m;
}
OMATRIX::~OMATRIX(){
    delete[] vector;
    }
