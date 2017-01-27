#ifndef _ELIMINATION_H_
#define _ELIMINATION_H_
#include "frwbcm.h"
#include "shape.h"
#include "cblas.h"
#include "matrixclass.h"

void InterfaceElimination(Matrix &matrixdown, Matrix &matrixup, int mi,double pdown, double pup){

    int ma = matrixdown.row - mi;
    int mb = matrixup.row - mi;
    Matrix z1(1,(ma+mb)*(ma+mb));
    Matrix z2(1,(ma+mb)*mi);
    Matrix z3(1, mi*mi);
    Matrix z4(1, mi*(ma+mb));
    for ( int i = 0 ; i < ma+mb ;i++){
        for ( int j = 0; j < ma + mb ; j++){
            if( i < ma ) {
                if ( j < ma){
                    z1.data[0][i*(ma+mb)+j]=matrixdown.data[i][j];
                }else{
                    z1.data[0][i*(ma+mb)+j]=0;
                }
            }else{
                if ( j < ma){
                    z1.data[0][i*(ma+mb)+j] = 0;
                }else{
                    z1.data[0][i*(ma+mb)+j]=matrixup.data[i-ma][j-ma];
                }
            }
        }
    }
    for(int i = 0; i < ma+mb ; i ++){
        for ( int j = 0; j < mi ; j ++){
            if (i < ma){
                z2.data[0][i*mi+j] = matrixdown.data[i][ma+j];
            }else{
                z2.data[0][i*mi+j] = matrixup.data[i-ma][mb+j];
            }
        }
    }
    for (int i = 0 ; i < mi ; i++){
        for (int j = 0 ; j < mi; j ++){
            z3.data[0][i*mi+j] = pdown * matrixdown.data[ma+i][ma+j] + pup * matrixup.data[mb+i][mb+j];
        }
    }
    for ( int i = 0 ; i < mi ; i ++){
        for (int j = 0 ; j < ma + mb; j ++){
            if (j < ma){
                z4.data[0][i*(ma+mb)+j] = pdown * matrixdown.data[ma+i][j];
            }else {
                z4.data[0][i*(ma+mb)+j] = pup * matrixup.data[mb+i][j-ma];
            }
        }
    }
    //std::cout << z4.col << " col z4 " << std::endl;
    lapack_int  g_indx[mi];
    lapack_int size_m = (lapack_int)mi;
    //lapack_int m = (lapack_int)mi;
    //lapack_int n = (lapack_int)(ma+mb);
    LAPACKE_dgetrf(LAPACK_ROW_MAJOR, size_m, size_m, z3.data[0], size_m, g_indx);
//    int INFO = LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'N', m, n, z3.data[0], m, g_indx, z4.data[0], n);
//	std::cout << " ma + mb " << ma + mb << endl;
//	std::cout << " INFO  " << INFO << endl;
    cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,ma+mb,ma+mb,mi,-1,z2.data[0],mi,z4.data[0],ma+mb,1,z1.data[0],ma+mb);
    if(matrixbcm.row && matrixbcm.col){
        matrixbcm.release();
    }
    matrixbcm.resize(ma+mb,ma+mb);
    for(int i = 0 ; i < ma + mb ; i++){
        for( int j = 0 ; j < ma+mb; j ++){
            matrixbcm.data[i][j]=z1.data[0][i*(ma+mb)+j];
        }
    }
    z1.release();
    z2.release();
    z3.release();
    z4.release();
//	std::cout << "e date.............." <<std::endl;
//	std::cout << " smatrixbcm.row " << matrixbcm.row << endl;
//	std::cout << " smatrixbcm.col " << matrixbcm.row << endl;
}
#endif 
