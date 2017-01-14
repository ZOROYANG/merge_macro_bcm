#ifndef _COHESION_H_
#define _COHESION_H_

#include "position.h"

void matrix_cohesion(){
    
    PositionRange();
    int row;
    int col;
    row = matrixbcm.row;
    col = matrixbcm.col;
    for(int i = 0 ; i < mrange.size() ;i++){
        for(int j = 0 ; j < mrange[i].size(); j++){
            if(mrange[i][j].conductorid != MAXNUM){
                col = col - (mrange[i][j].end - mrange[i][j].start);
            }
        }
    }
    Matrix matrix_col(row, col);
    int sum = 0; 
    int colposition = 0 ;
    for(int i = 0 ; i < mrange.size() ; i++){
        for(int j = 0 ; j < mrange[i].size() ; j++){
            int start = mrange[i][j].start;
            int end = mrange[i][j].end;
            int layerid = mrange[i][j].layerid;
            int conductorid = mrange[i][j].conductorid;
            if(conductorid != MAXNUM){
                for(int k = 0 ; k < row ; k++){
                    for(int h = start - 1 ; h < end; h++){
                        sum += matrixbcm.data[k][h];
                    }
                    matrix_col.data[k][colposition] = sum;
                    sum = 0 ;
                }
                for(int k = 0 ; k < mrange[i+1].size() ; k++){
                    if(mrange[i+1].layerid == layerid && 
                    mrange[i+1].conductorid == conductorid){
                        start = mrange[i+1][k].start;
                        end = mrange[i+1][k].endl;
                        for(int kk = 0 ; kk < row ; kk++){
                            for(int h = start - 1 ; h < end; h++){
                                sum += matrixbcm.data[kk][h];
                            }
                            matrix_col.data[kk][colposition] += sum;
                            sum = 0 ;
                        }
                        break;
                    }
                }
                colposition = colposition + 1;
            }else if(conductorid == MAXNUM){
                for(int k = 0 ; k < row ; k++){
                    for(int h = start - 1 ; h < end; h++){
                        matrix_col.data[k][colposition] = matrix_col[k][h];
                        colposition = colposition + 1;
                    }
                }
            }
        }
    }
    matrixbcm.release();
    Matrix matrix_row(col, col);
    int rowposition = 0 ;
    for(int i = 0 ; i < mrange.size() ; i++){
        for(int j = 0 ; j < mrange[i].size() ; j++){
            int start = mrange[i][j].start;
            int end = mrange[i][j].end;
            int layerid = mrange[i][j].layerid;
            int conductorid = mrange[i][j].conductorid;
            if(conductorid != MAXNUM){
                for(int k = 0 ; k < col ; k++){
                    for(int h = start - 1 ; h < end; h++){
                        sum += matrix_col.data[h][k];
                    }
                    matrix_row.data[rowposition][k] = sum;
                    sum = 0 ;
                }
                for(int k = 0 ; k < mrange[i+1].size() ; k++){
                    if(mrange[i+1].layerid == layerid && 
                    mrange[i+1].conductorid == conductorid){
                        start = mrange[i+1][k].start;
                        end = mrange[i+1][k].endl;
                        for(int kk = 0 ; kk < col ; kk++){
                            for(int h = start - 1 ; h < end; h++){
                                sum += matrix_col.data[h][kk];
                            }
                            matrix_row.data[rowposition][kk] += sum;
                            sum = 0 ;
                        }
                        break;
                    }
                }
                rowposition = rowposition + 1;
            }else if(conductorid == MAXNUM){
                for(int k = 0 ; k < row ; k++){
                    for(int h = start - 1 ; h < end; h++){
                        matrix_row.data[rowposition][k] = matrix_col.data[h][k];
                        rowposition = rowposition + 1;
                    }
                }
            }
        }
    }
    matrix_col.release();
    matrixbcm = matrix_row;
}
