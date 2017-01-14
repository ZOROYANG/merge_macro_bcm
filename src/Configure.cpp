/*
 * Configure.cpp
 *
 *  Created on: 8 Jul, 2016
 *      Author: ming
 */

#include "Configure.h"

Configure::Configure() {
    // TODO Auto-generated constructor stub
    for(int i = 0; i < 3; ++ i){
        this->origin[i] = 0.;
        this->length[i] = 0.;
        this->seg_axis[i] = 0;
        
    }
    this->num_cond = 0;
    this->cond_ori = NULL;
    this->cond_len = NULL;
    this->cond_seg = NULL;
    this->diel = 1.;
}

Configure::Configure(double* ori, double* len, int* sa, double d){
    for(int i = 0; i < 3; ++ i){
        this->origin[i] = ori[i];
        this->length[i] = len[i];
        this->seg_axis[i] = sa[i];
    }
    this->diel = d;
}

Configure::Configure(double* ori, double* len, int* sa, int nc, double** co, double** cl, int** cs, double d){
    for(int i = 0; i < 3; ++ i){
        this->origin[i] = ori[i];
        this->length[i] = len[i];
        this->seg_axis[i] = sa[i];
    }   
    
    this->num_cond = nc;
    
    this->cond_ori = new double*[num_cond];
    for(int i = 0; i < num_cond; ++ i){
        cond_ori[i] = new double[3];
    }
    for(int i = 0; i < num_cond; ++ i){
        for(int j = 0; j < 3; ++ j){
            cond_ori[i][j] = co[i][j];
        }
    }
    
    this->cond_len = new double*[num_cond];
    for(int i = 0; i < num_cond; ++ i){
        cond_len[i] = new double[3];
    }
    for(int i = 0; i < num_cond; ++ i){
        for(int j = 0; j < 3; ++ j){
            cond_len[i][j] = cl[i][j];
        }
    }
    
    this->cond_seg = new int*[num_cond];
    for(int i = 0; i < num_cond; ++ i){
        cond_seg[i] = new int[3];
    }
    for(int i = 0; i < num_cond; ++ i){
        for(int j = 0; j < 3; ++ j){
            cond_seg[i][j] = cs[i][j];
        }
    }
    
    this->diel = d;
}


Configure::~Configure() {
    // TODO Auto-generated destructor stub
    for(int i = 0; i < num_cond; ++ i){
        delete[] cond_ori[i];
        delete[] cond_len[i];
        delete[] cond_seg[i];
    }
    delete[] cond_ori;
    delete[] cond_len;
    delete[] cond_seg;

}

void Configure::Debug(){
    if(num_cond > 0){
        this->debg.Conf_print(this->origin, this->length, this->seg_axis, this->num_cond, this->cond_ori, this->cond_len, this->cond_seg, this->diel);
    }else{
        this->debg.Conf_print(this->origin, this->length, this->seg_axis, this->diel);
    }
    
    return;
}

int Configure::getNumCond(){
    return num_cond;
}