#ifndef _RELEASE_H
#define _RELEASE_H
#include "frwbcm.h"
/*
* release memory
*/
extern bool frw;
extern bool bcm;
void release_vector(){
    vector<int>().swap(ele_x);
    vector<int>().swap(ele_y);
    vector<int>().swap(ele_z);
    vector<vector<double>>().swap(segments_x);
    vector<vector<double>>().swap(segments_y);
    vector<vector<double>>().swap(segments_z);
    boundary_position.release();
    matrixbcm.release();
    vector<double>().swap(high);
    vector<double>().swap(diag);
    vector<int>().swap(condp);
    vector<vector<double>>().swap(Areas);
    std::vector<TBDELEINFO>().swap(BdElementsbcm);
    std::vector<TBDELEINFO>().swap(BdElementInter);
    std::vector<DPLANE>().swap(dinterfaces);
    std::vector<std::vector<TBDELEINFO> >().swap(gBdElementsbcm);
    std::vector<std::vector<DPLANE> >().swap(dblocks);
    std::vector<std::vector<DPLANE> >().swap(dconductors);
    vector<vector<double>>().swap(boundaryposition);
    boundaryposition.resize(3);
    vector<int>().swap(BdElementInterSize);
    BdElementInterSize.resize(MAX_BLOCK_NUM);
    vector<BLOCKINFO>().swap(blocks);
    vector<CONDUCTORINFO>().swap(conductors);

    for(int i=0;i<static_cast<int>(Curmed);i++){
        if(matrix_G[i]!=NULL){
            delete[] matrix_G[i];
        }
        if(matrix_H[i]!=NULL){
            delete[] matrix_H[i];
        }
    }
    delete[] matrix_G;
    delete[] matrix_H;	
        //for 2 mediums
    for(int i=0;i<static_cast<int>(Curmed);i++){
        if(matrix_w[i]!=NULL)
            delete[] matrix_w[i];
        if(matrix_A[i]!=NULL){
            for(int j=0;j<cur_ele_id[i];j++){
                if(matrix_A[i][j]!=NULL)
                    delete[] matrix_A[i][j];
            }
            delete[] matrix_A[i];
        }
    }
    for(int i = 0 ; i < static_cast<int>(Curmed) + 1 ; i++){
        if(g_indx[i] != NULL){
            delete [] g_indx[i];
        }
    }
    for(int i = 0 ; i< MAX_BLOCK_NUM; i++){ 
        cur_ele_id[i] = 0;
    }
}

void release_mem(){
    //int j,k;
    int j;
    if(frw){
        for(int i=0;i<static_cast<int>(Curmed);i++){
            if(gBdElements[i]!=NULL)
                delete[] gBdElements[i];
            if(Curmed == 1 || (Curmed==2 && i==pointInMediumId)){
                if(vector_g[i]!=NULL)
                    delete[] vector_g[i];
                if(vector_h[i]!=NULL)
                    delete[] vector_h[i];
            }
            if(matrix_G[i]!=NULL){
                delete[] matrix_G[i];
            }
            if(matrix_H[i]!=NULL){
                delete[] matrix_H[i];
            }
        }
        delete[] gBdElements;
        delete[] tempblock2;
        delete[] vector_g;
        delete[] vector_h;
        delete[] matrix_G;
        delete[] matrix_H;	
        //for 2 mediums
        if(Curmed == 2){
            for(int i=0;i<static_cast<int>(Curmed);i++){
                if(matrix_w[i]!=NULL)
                    delete[] matrix_w[i];
                if(matrix_A[i]!=NULL){
                    for(j=0;j<cur_ele_id[i];j++){
                        if(matrix_A[i][j]!=NULL)
                            delete[] matrix_A[i][j];
                    }
                    delete[] matrix_A[i];
                }
                if(matrix_D[i]!=NULL)
                    delete[] matrix_D[i];
            }
            delete[] matrix_X;
            delete[] matrix_D;		
            delete[] matrix_w;
            delete[] final_res;
            delete[] poly_in_interface;
            delete[] pddlInterfacePoint3DBuf;
        }
    }
}
#endif
