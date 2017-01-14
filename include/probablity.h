#ifndef _PROBABLITY_H_
#define _PROBABLITY_H_

#include "frwbcm.h"

void deal_matrix(){

        
    int row = matrixbcm.row;
    int col = matrixbcm.col;
    double positivesum = 0.0 ;
    double rowsum = 0.0;
    int countpnum = 0;
    double positiveave = 0.0 ;
    double rowave = 0.0 ;
    for(int i = 0 ; i < row ; i++){
        for(int j = 0 ; j < col ; j++){
            if(MbFltGT(matrixbcm.data[i][j], 0.0) && i != j){
                //positivesum += matrixbcm.data[i][j];
                matrixbcm.data[i][j] = 0.0 ;
                countpnum++;
            }else{
                rowsum += matrixbcm.data[i][j];
            }
        }
        //positiveave = positivesum / countpnum ;
        rowave = rowsum / (col - countpnum);
        for(int k = 0 ; k < col ;k++){
            if(!(MbFltEQ(matrixbcm.data[i][k],0.0))){
                matrixbcm.data[i][k] -= rowave;
            }
        }
        rowsum = 0.0;
        positivesum = 0.0;
        countpnum = 0;
    }
    std::cout<<"deal done !" << std::endl;
}
#endif
