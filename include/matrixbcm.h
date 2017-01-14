#ifndef _MATRIXBCM_H
#define _MATRIXBCM_H
#include "frwbcm.h"
#include "shape.h"
#include "Matrix_Sparse.h"
#include "Configure.h"
#include "FDMSolver.h"
#include "matrixclass.h"


/* 
*compute G,H,BCM matrix
*/
void MatrixGenBcm(){
    printf("\nbasic info:\nlength: %lf %lf %lf\n", _length[0], _length[1], _length[2]);
    for(int i = 0; i < ele_x.size(); ++ i){
        printf("%d segs in x dir.: %d\n", i, ele_x[i]);
    }
    for(int i = 0; i < ele_y.size(); ++ i){
        printf("%d segs in y dir.: %d\n", i, ele_y[i]);
    }
    for(int i = 0; i < ele_z.size(); ++ i){
        printf("%d segs in z dir.: %d\n", i, ele_z[i]);
    }
    printf("Curmed: %d\n", Curmed);
    for(int i = 0; i < Curmed; ++ i){
        printf("tempelec: %lf", tempelec[i]);
    }
    printf("\n");
    
    printf("Areas:\n");
    for(int i = 0; i < Areas.size(); ++ i){
        for(int j = 0; j < Areas[i].size(); ++ j){
            printf("%lf ", Areas[i][j]);
        }
        printf("\n");
    }
    
    // add by [ym]
    if(useFDM && Curmed == 1){
        double ori[3] = {0., 0., 0.};
        int sa[3] = {ele_x[0], ele_y[0], ele_z[0]};
        double diele = tempelec[0];
        double len[3];
        for(int i = 0; i < 3; ++ i){
            len[i] = _length.at(i);
        }
        printf("%lf %lf %lf\n", len[0], len[1], len[2]);
        int** cs;
        double** co;
        double** cl;
        
        if(conductors.size() == 0){
            Configure mp(ori, len, sa, diele);
            mp.Debug();
            FDM_Solver fdmsp(&mp);
            fdmsp.Debug();
            puts("solve begin");
            fdmsp.solve();
            puts("solve end");
            
            // add by [ym], attention getCap return type!
            if(blocks.size() == 1){
                printf("!!!begin passing matrix!!!\n");
                std::vector<std::vector<double> > tempvec = fdmsp.getCap();
                double** A = new double*[tempvec.size()];
                for(int u = 0; u < tempvec.size(); ++ u){
                    A[u] = new double[tempvec[u].size()];
                    
                }
                printf("!!!finish allocation!!!\n");
                
                for(int u = 0; u < tempvec.size(); ++ u){
                    for(int v = 0; v < tempvec[u].size(); ++ v){
                        A[u][v] = tempvec[u][v];
                    }
                    
                }
                printf("!!!finish passing matrix!!!\n");
                matrixbcm.data = A;
                matrixbcm.row = cur_ele_id[0];
                matrixbcm.col = cur_ele_id[0];
                
            }
        }else{
            co = new double*[conductors.size()];
            for(int i = 0; i < conductors.size(); ++ i){
                co[i] = new double[3];
            }
            
            cl = new double*[conductors.size()];
            for(int i = 0; i < conductors.size(); ++ i){
                cl[i] = new double[3];
            }
            
            cs = new int*[conductors.size()];
            for(int i = 0; i < conductors.size(); ++ i){
                cs[i] = new int[3];
            }
            
            int k = 0;
            for(int i = 0; i < blocks.size(); ++ i){
                for(int j = 0; j < conductors.size(); ++ j){
                    if(conductors[j].layerNum == i + 1){
                        cs[k][0] = conductors[j].xEle;
                        cs[k][1] = conductors[j].yEle;
                        cs[k][2] = conductors[j].zEle;
                        
                        co[k][0] = conductors[j].geo.basePoint.x;
                        co[k][1] = conductors[j].geo.basePoint.y;
                        co[k][2] = conductors[j].geo.basePoint.z;
                        
                        cl[k][0] = conductors[j].geo.xAxis.x;
                        cl[k][1] = conductors[j].geo.yAxis.y;
                        cl[k][2] = conductors[j].geo.zAxis.z;
                        
                        ++ k;
                    }
                }
            }
            
            Configure mp(ori, len, sa, conductors.size(), co, cl, cs, diele);
            //mp.Debug();
            FDM_Solver fdmsp(&mp);
            //printf("%d\n", fdmsp.A11.i.size());
            //for (auto x : fdmsp.A11.i) printf("%d ", x); puts("");
            puts("solve begin");
            fdmsp.solve();
            puts("solve end");
            //fdmsp.solve();
            
            if(blocks.size() == 1){
                printf("!!!begin passing matrix!!!\n");
                std::vector<std::vector<double> > tempvec = fdmsp.getCap();
                double** A = new double*[tempvec.size()];
                for(int u = 0; u < tempvec.size(); ++ u){
                    A[u] = new double[tempvec[u].size()];
                    
                }
                printf("!!!finish allocation!!!\n");
                
                for(int u = 0; u < tempvec.size(); ++ u){
                    for(int v = 0; v < tempvec[u].size(); ++ v){
                        A[u][v] = tempvec[u][v];
                    }
                    
                }
                printf("!!!finish passing matrix!!!\n");
                matrixbcm.data = A;
                matrixbcm.row = cur_ele_id[0];
                matrixbcm.col = cur_ele_id[0];
            }
            
            printf("!!!HI!!!\n");
            for(int i = 0; i < conductors.size(); ++ i){
                delete[] cs[i];
                delete[] cl[i];
                delete[] co[i];
            }
            
            if(conductors.size() == 1){
                delete cs;
                delete cl;
                delete co;
            }else if(conductors.size() > 1){
                delete[] cs;
                delete[] cl;
                delete[] co;
            }
            printf("!!!HI!!!\n");
        }
        
        return;
    }
    
    int j,k,ntotal;
    double P[3],X[4],Y[4],Z[4],FM_PI=4.0*M_PI;
    matrix_G = new double *[Curmed];
    matrix_H = new double *[Curmed];
    
    //must call this before use New_Integration
    gauss();
    for(int i = 0  ; i < Curmed ; i++){
        ntotal=cur_ele_id[i];
        matrix_G[i] = new double[ntotal*ntotal];
        matrix_H[i] = new double[ntotal*ntotal];
        for(j=0;j<ntotal+1;j++){	//cfg
            if(j==ntotal)
                break;
            for(k=0;k<ntotal;k++){	//elements unit	
                P[0]=gBdElementsbcm[i][j].cfg.x;
                P[1]=gBdElementsbcm[i][j].cfg.y;
                P[2]=gBdElementsbcm[i][j].cfg.z;
                X[0]=gBdElementsbcm[i][k].leftdown.x; 
                Y[0]=gBdElementsbcm[i][k].leftdown.y; 
                Z[0]=gBdElementsbcm[i][k].leftdown.z;
                X[1]=gBdElementsbcm[i][k].rightdown.x; 
                Y[1]=gBdElementsbcm[i][k].rightdown.y; 
                Z[1]=gBdElementsbcm[i][k].rightdown.z;
                X[2]=gBdElementsbcm[i][k].rightup.x; 
                Y[2]=gBdElementsbcm[i][k].rightup.y; 
                Z[2]=gBdElementsbcm[i][k].rightup.z;
                X[3]=gBdElementsbcm[i][k].leftup.x; 
                Y[3]=gBdElementsbcm[i][k].leftup.y; 
                Z[3]=gBdElementsbcm[i][k].leftup.z;
                
                sum0=0; sum1=0;
                if(j!=k)
                    New_Integration(gBdElementsbcm[i][k].type,P,X,Y,Z);
                else{				
                    if(gBdElementsbcm[i][k].type > 9 && gBdElementsbcm[i][k].type < 16)
                        sum0=tri(X,Y,Z);
                    else
                        sum0=quadbak(P,X,Y,Z);
                        sum1=0;
                }
                if(j<ntotal){
                    matrix_G[i][j*ntotal+k]=sum0/FM_PI;
                    matrix_H[i][j*ntotal+k]=sum1/FM_PI;				
                    if(j==k)
                        matrix_H[i][j*ntotal+k]+=0.5;
                }
            }
        }
    }		
    matrix_w=new double *[Curmed];
    for(int i=0;i<Curmed;i++){
        matrix_w[i]=new double[cur_ele_id[i]];
        for(int j=0;j<cur_ele_id[i];j++){
            matrix_w[i][j]=tempelec[i]*ele_coverbcm(i,j);
        }
    }
    //cout<<"Generating BCM for two mediums:"<<endl;
    matrix_A = new double**[Curmed];
    for(int i=0;i<Curmed;i++){
        matrix_A[i] = new double*[cur_ele_id[i]];
        for(int j=0;j<cur_ele_id[i];j++)
            matrix_A[i][j]=new double[cur_ele_id[i]];
        g_indx[i]=new lapack_int[cur_ele_id[i]];
        lapack_int size_m = (lapack_int)cur_ele_id[i];
        LAPACKE_dgetrf(LAPACK_ROW_MAJOR, size_m, size_m, matrix_G[i], size_m, g_indx[i]);
        LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'N', size_m, size_m, matrix_G[i], size_m, g_indx[i], matrix_H[i], size_m);
        for(int j=0;j<cur_ele_id[i];j++)
            for(int k=0;k<cur_ele_id[i];k++)
                matrix_A[i][j][k] = matrix_H[i][j*cur_ele_id[i]+k]*matrix_w[i][j];
    }

    if(blocks.size() == 1){
        matrixbcm.data = matrix_A[0];
        matrixbcm.row = cur_ele_id[0];
        matrixbcm.col = cur_ele_id[0];
    }else{
        Matrix matrixup,matrixdown;
        for (int i = 1  ; i < Curmed ; i++){
            if(i > 1){
                matrixdown.release();
                matrixdown = matrixbcm;
            }else{
                matrixdown.data = matrix_A[0];
                matrixdown.row = cur_ele_id[0];
                matrixdown.col = cur_ele_id[0];
            }
    //		std::cout << " down size " << matrixdown.row << std::endl;
    //		std::cout << " down  size " << matrixdown.col << std::endl;
            int mi = BdElementInterSize[i-1];
    //		std::cout << " inter size " << mi << std::endl;
            matrixup.data = matrix_A[i];
            matrixup.row = cur_ele_id[i];
            matrixup.col = cur_ele_id[i];
    //		std::cout << " down size " << matrixup.row << std::endl;
    //		std::cout << " down  size " << matrixup.col << std::endl;
            InterfaceElimination(matrixdown,matrixup,mi,tempelec[i-1],tempelec[i]);	
        }
    }
}
#endif	
