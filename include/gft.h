#ifndef _GFT_H
#define _GFT_H

#include "frwbcm.h"

void formula_solve(){
    //the second part of formula 30: Moi*D
    //in the end, the matrix_A(or called M) holds the result
    int num_i=cur_ele_id[0]-interface_start_id[0];
    for(int i=0;i<Curmed;i++){
        for(int j=0;j<interface_start_id[i];j++){
            for(int k=interface_start_id[i];k<cur_ele_id[i];k++){
                matrix_A[i][j][k] *= matrix_D[i][k-interface_start_id[i]];
            }
        }
    }
    //
    matrix_trans(cur_ele_id[pointInMediumId], matrix_H[pointInMediumId]);
    //now H is the (G(-1)H)_T
    //so Hg-h is the result Pc(pointInMediumId)
    //go on,and in the end, vector_g[pointInMediumId] = Pc(pointInMediumId)
    double *tmp_res;
    tmp_res=new double[cur_ele_id[pointInMediumId]];
    mul_mat_vec(cur_ele_id[pointInMediumId], matrix_H[pointInMediumId], vector_g[pointInMediumId], tmp_res);
    sub_vec(cur_ele_id[pointInMediumId], tmp_res, vector_h[pointInMediumId], vector_g[pointInMediumId]);
    delete[] tmp_res;
    double tmp_sum=0.0;
    for(int i=0;i<cur_ele_id[pointInMediumId];i++)
        tmp_sum += vector_g[pointInMediumId][i];
    cout<<"<<<<<<<<<<<<<<<"<<tmp_sum<<">>>>>>>>>>>>>>"<<endl;
    //begin to compute X(-T)Pci
    //X=X_T
    //&vector_g[pointInMediumId][interface_start_id[pointInMediumId]] holds X(-T)Pci
    double *pci_res;
    pci_res = &vector_g[pointInMediumId][interface_start_id[pointInMediumId]];
    matrix_trans(num_i, matrix_X);
    linear_equation_solve(num_i, matrix_X, pci_res, 2);
    //the real part 2 of formula 30
    final_res = new double[interface_start_id[0] + interface_start_id[1]];
    int cnt=0;
    for(int i=0;i<Curmed;i++){
        for(int j=0;j<interface_start_id[i];j++){
            final_res[cnt+j] = 0.0;
            for(int k=0;k<num_i;k++){
                final_res[cnt+j] += matrix_A[i][j][k+interface_start_id[i]] * pci_res[k];
            }
        }
        cnt = interface_start_id[i];
    }
    //add the first part
    for(int i=0;i<interface_start_id[pointInMediumId];i++){
        if(pointInMediumId == 0)
            final_res[i] += vector_g[0][i];
        else
            final_res[i+interface_start_id[0]] += vector_g[1][i];
    }
    //now final_res holds the final result
    write_res2();
    if(needWeight)
        wvt_solve();
}
void formula_solve_signal(){
    double *tmp_save;
    matrix_trans(cur_ele_id[0], matrix_G[0]);
    matrix_trans(cur_ele_id[0], matrix_H[0]);
    printf("solve G_T*x=g\n");
        //float seconds;
        //struct tms  ti1,ti2;
        //times(&ti1);
    linear_equation_solve(cur_ele_id[0], matrix_G[0], vector_g[0]);
        //(void) times(&ti2);
        //seconds = (float)(ti2.tms_utime - ti1.tms_utime)/60.0;
        //printf("solve equation Time:		 %.2f seconds.\n", seconds);
    printf("y=H_T*x\n");
    tmp_save = new double[cur_ele_id[0]];
    mul_mat_vec(cur_ele_id[0], matrix_H[0], vector_g[0], tmp_save);
    printf("y = y - h\n");		
    sub_vec(cur_ele_id[0], tmp_save, vector_h[0], vector_g[0]);
    delete[] tmp_save;
    printf("start to write result...\n");
    write_res();
    //
    if(needWeight)
        wvt_solve_signal();
}
#endif
