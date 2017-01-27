#ifndef _WVT_H
#define _WVT_H

#include "frwbcm.h"

void wvt_init(int bid){	
    double P[3],X[4],Y[4],Z[4],FM_PI=4.0*M_PI;
    int n=cur_ele_id[bid];
    for(int i=0;i<MAX_AXIS_NUM;i++){
        vector_wg[i]=new double[n];
        vector_tg[i]=new double[n];
        vector_tg[i+3]=new double[n];
        vector_wh[i]=new double[n];
        vector_th[i]=new double[n];
        vector_th[i+3]=new double[n];
        pointForW[i] = pointForW[i+3] = observePoint;
    }
    pointForW[0].x += STEP_H;
    pointForW[1].y += STEP_H;
    pointForW[2].z += STEP_H;
    pointForW[3].x -= STEP_H;
    pointForW[4].y -= STEP_H;
    pointForW[5].z -= STEP_H;
    //
    //compute vector_tg, vector_th
    for(int i=0;i<2*MAX_AXIS_NUM;i++){
        for(int k=0;k<n;k++){	//elements unit	
            P[0]=pointForW[i].x; 		P[1]=pointForW[i].y; 		P[2]=pointForW[i].z;
            X[0]=gBdElements[bid][k].leftdown.x; 	Y[0]=gBdElements[bid][k].leftdown.y; 	Z[0]=gBdElements[bid][k].leftdown.z;
            X[1]=gBdElements[bid][k].rightdown.x; 	Y[1]=gBdElements[bid][k].rightdown.y; 	Z[1]=gBdElements[bid][k].rightdown.z;
            X[2]=gBdElements[bid][k].rightup.x; 	Y[2]=gBdElements[bid][k].rightup.y; 	Z[2]=gBdElements[bid][k].rightup.z;
            X[3]=gBdElements[bid][k].leftup.x; 		Y[3]=gBdElements[bid][k].leftup.y; 		Z[3]=gBdElements[bid][k].leftup.z;
            sum0=0; sum1=0;
            New_Integration(gBdElements[bid][k].type,P,X,Y,Z);
            vector_tg[i][k]=sum0/FM_PI;
            vector_th[i][k]=sum1/FM_PI;
        }
    }
    //vector_wg, vector_wh
    for(int i=0;i<MAX_AXIS_NUM;i++){
        for(int j=0;j<n;j++){
            vector_wg[i][j] = vector_tg[i][j]-vector_tg[i+3][j];
            vector_wh[i][j] = vector_th[i][j]-vector_th[i+3][j];
        }
    }
}
//to compute wvt for multi medium
void wvt_solve(){
    int bid=pointInMediumId;
    int n=cur_ele_id[bid];
    int sid=interface_start_id[bid];
    int num_i=n-sid;
    wvt_init(bid);
    double *tmp_res, *pci_res;
    tmp_res=new double[n];
    for(int i=0;i<MAX_AXIS_NUM;i++){
        mul_mat_vec(n, matrix_H[bid], vector_wg[i], tmp_res);
        sub_vec(n, tmp_res, vector_wh[i], vector_wg[i]);
        //
        pci_res = &vector_wg[i][sid];
        LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'N', (lapack_int)num_i, 1, matrix_X, (lapack_int)num_i, g_indx[2], pci_res, 1);
        //the real part 2 of formula 30
        final_wvt[i]=new double[interface_start_id[0] + interface_start_id[1]];
        int cnt=0;
        for(int j=0;j<static_cast<int>(Curmed);j++){
            for(int k=0;k<interface_start_id[j];k++){
                final_wvt[i][cnt+k] = 0.0;
                for(int r=0;r<num_i;r++){
                    final_wvt[i][cnt+k] += matrix_A[j][k][r+interface_start_id[j]] * pci_res[r];
                }
            }
            cnt = interface_start_id[j];
        }
        //add the first part
        for(int j=0;j<sid;j++){
            if(pointInMediumId == 0)
                final_wvt[i][j] += vector_wg[i][j];
            else
                final_res[j+interface_start_id[0]] += vector_wg[i][j];
        }
        for(int j=0;j<interface_start_id[0] + interface_start_id[1];j++)
            final_wvt[i][j] /= (2*STEP_H);
    }
    delete[] tmp_res;
    //write wvt result
    FILE* fp=fopen("res/wvt.txt", "w+");
    if(fp==NULL){
        cout<<"open file wvt.txt error,exit..."<<endl;
        exit(1);
    }
    for(int j=0;j<interface_start_id[0] + interface_start_id[1];j++){
        //fprintf(fp, "(%e, %e, %e)\n", final_wvt[0][j], final_wvt[1][j], final_wvt[2][j]);
        fprintf(fp, "%e %e %e\n", final_wvt[0][j], final_wvt[1][j], final_wvt[2][j]);
    }
    fclose(fp);
    cout<<"<<wvt now is generated!"<<endl;
}
//compute weight vector
void wvt_solve_signal(){
    wvt_init(0);
    //solve G_T*x = vector_wg[i]	
    double *tmp_save;
    tmp_save = new double[cur_ele_id[0]];
    lapack_int size_m = (lapack_int)cur_ele_id[0];
    for(int i=0;i<MAX_AXIS_NUM;i++){
        LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'N', size_m, 1, matrix_G[0], size_m, g_indx[0], vector_wg[i], 1);
        mul_mat_vec(cur_ele_id[0], matrix_H[0], vector_wg[i], tmp_save);
        sub_vec(cur_ele_id[0], tmp_save, vector_wh[i], vector_wg[i]);
        for(int j=0;j<cur_ele_id[0];j++)
            vector_wg[i][j] /= (2*STEP_H);
    }
    delete[] tmp_save;
    //write wvt result
    FILE* fp=fopen("res/wvt.txt", "w+");
    if(fp==NULL){
        cout<<"open file wvt.txt error,exit..."<<endl;
        exit(1);
    }
    for(int j=0;j<cur_ele_id[0];j++){
        //fprintf(fp, "(%e, %e, %e)\n", vector_wg[0][j], vector_wg[1][j], vector_wg[2][j]);
        fprintf(fp, "%e %e %e\n", vector_wg[0][j], vector_wg[1][j], vector_wg[2][j]);
    }
    fclose(fp);
    cout<<"<<wvt now is generated!"<<endl;
}
#endif
