
#include "frwbcm.h"
#include "shape.h"
#include "mymath.h"
#include "boundarytype.h"
#include "linearequation.h"
#include "quadrangle.h"
#include "triangle.h"
#include "boundaryfrw.h"
#include "tsvsubface.h"
#include "writedata.h"
#include "frustum.h"
#include "prism.h"
#include "tsvprism.h"
#include "cvertex.h"
#include "plane.h"
#include "sweepline.h"
#include "csweepline.h"
#include "boundarybcm.h"
#include "elimination.h"
#include "matrixbcm.h"
#include "probablity.h"
#include "modulation.h"
#include "wvt.h"
#include  "gft.h"
#include "interface.h"
#include "release.h"

/* 
*compute my G,H,g,h
*/
void MatrixGen(){
    int i,j,k,ntotal;
    double P[3],X[4],Y[4],Z[4],FM_PI=4.0*M_PI;
    
    matrix_G = new double *[block_number];
    matrix_H = new double *[block_number];
    vector_g = new double * [block_number];
    vector_h = new double * [block_number];
    cout<<"block_number="<<block_number<<endl;
    
    //must call this before use New_Integration
    gauss();
    //generate matrix_G  matrix_H  vector_g  vector_h
    for(i=0;i<Curblk;i++){
        ntotal=cur_ele_id[i];
        matrix_G[i] = new double[ntotal*ntotal];
        matrix_H[i] = new double[ntotal*ntotal];
        if(Curblk==1 || Curblk==2 && i==pointInMediumId){
            vector_g[i] = new double[ntotal];
            vector_h[i] = new double[ntotal];
        }
        for(j=0;j<ntotal+1;j++){	//cfg
            if(j==ntotal && Curblk==2 && i!=pointInMediumId)
                break;
            for(k=0;k<ntotal;k++){	//elements unit	
                P[0]=gBdElements[i][j].cfg.x; 		P[1]=gBdElements[i][j].cfg.y; 		P[2]=gBdElements[i][j].cfg.z;
                X[0]=gBdElements[i][k].leftdown.x; 	Y[0]=gBdElements[i][k].leftdown.y; 	Z[0]=gBdElements[i][k].leftdown.z;
                X[1]=gBdElements[i][k].rightdown.x; Y[1]=gBdElements[i][k].rightdown.y; Z[1]=gBdElements[i][k].rightdown.z;
                X[2]=gBdElements[i][k].rightup.x; 	Y[2]=gBdElements[i][k].rightup.y; 	Z[2]=gBdElements[i][k].rightup.z;
                X[3]=gBdElements[i][k].leftup.x; 	Y[3]=gBdElements[i][k].leftup.y; 	Z[3]=gBdElements[i][k].leftup.z;
                
                sum0=0; sum1=0;
                if(j!=k)
                    New_Integration(gBdElements[i][k].type,P,X,Y,Z);
                else{				
                    if(gBdElements[i][k].type > 9 && gBdElements[i][k].type < 16) //triangle
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
                }else{
                    vector_g[i][k]=sum0/FM_PI;
                    vector_h[i][k]=sum1/FM_PI;
                }
            }
        }
    }
    //write matrix to file
    if(multiCal ==false)
    write_matrix_res();
}
//for two medium
void MatrixGen_More(){
    //W, W[i][j]=diel(i)*Sj
    matrix_w=new double *[Curmed];
    for(int i=0;i<Curmed;i++){
        matrix_w[i]=new double[cur_ele_id[i]];
        for(int j=0;j<cur_ele_id[i];j++){
            matrix_w[i][j]=tempelec[i]*ele_cover(i,j);
        }
    }
    //A=WG(-1)H
    cout<<"begin to generate A:"<<endl;
    matrix_A = new double**[Curmed];
    for(int i=0;i<Curmed;i++){
        matrix_A[i] = new double*[cur_ele_id[i]];
        for(int j=0;j<cur_ele_id[i];j++)
            matrix_A[i][j]=new double[cur_ele_id[i]];
        //matrix_trans(cur_ele_id[i], matrix_H[i]);
        g_indx[i]=new lapack_int[cur_ele_id[i]];
        lapack_int size_m = (lapack_int)cur_ele_id[i];
        cout<<">>>>>>>>>>>>>>>>G(-1)H"<<endl;
        LAPACKE_dgetrf(LAPACK_ROW_MAJOR, size_m, size_m, matrix_G[i], size_m, g_indx[i]);
        cout<<"G(-1) over"<<endl;
        LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'N', size_m, size_m, matrix_G[i], size_m, g_indx[i], matrix_H[i], size_m);
        cout<<"!!!!!!!!!!!!!!!!over G(-1)H"<<endl;
        //matrix_trans(cur_ele_id[i], matrix_H[i]);
        for(int j=0;j<cur_ele_id[i];j++)
            for(int k=0;k<cur_ele_id[i];k++)
                matrix_A[i][j][k] = matrix_H[i][j*cur_ele_id[i]+k]*matrix_w[i][j];
    }
    //D
    int num_i=cur_ele_id[0]-interface_start_id[0];
    matrix_D = new double*[Curmed];
    for(int i=0;i<Curmed;i++){
        matrix_D[i]=new double[num_i];
        for(int j=0;j<num_i;j++){
            matrix_D[i][j]=matrix_A[i][j+interface_start_id[i]][j+interface_start_id[i]] 
                            / (matrix_A[0][interface_start_id[0]+j][interface_start_id[0]+j] 
                                + matrix_A[1][interface_start_id[1]+j][interface_start_id[1]+j]);
        }
    }
    //M : use A to save
    for(int i=0;i<Curmed;i++){
        for(int j=0;j<cur_ele_id[i];j++){
            for(int k=0;k<cur_ele_id[i];k++){
                if(j==k)
                    continue;
                matrix_A[i][j][k] = (-1.0)*matrix_A[i][j][k]/matrix_A[i][j][j];
            }
            matrix_A[i][j][j]=0;
        }
    }
    matrix_X=new double[num_i*num_i];
    //
    for(int i=0;i<Curmed;i++){
        for(int j=0;j<num_i;j++){
            for(int k=0;k<num_i;k++){
                matrix_A[i][interface_start_id[i]+j][interface_start_id[i]+k] *= matrix_D[i][j];
            }
        }
    }
    for(int i=0;i<num_i;i++){
        for(int j=0;j<num_i;j++){
            matrix_X[i*num_i+j] = matrix_A[0][interface_start_id[0]+i][interface_start_id[0]+j]
                                + matrix_A[1][interface_start_id[1]+i][interface_start_id[1]+j];
            if(i==j)
                matrix_X[i*num_i+j] -= 1;
            matrix_X[i*num_i+j] *= -1;
        }
    }
}

//main
void generatebcm();
void deposite();
void bcmgen(const bool cohesion, const bool standard){
    
//	printf("Generating Boundary elements...\n");
    
    if(standard){
        size_t seg = 0; double h;
        if(standard_h){
            seg = 16; h = double(1) / 32;
        }else if(standard_v){
            seg = 31; h = double(1) / 31;
        }
        std::cout << std::endl;
        if(block_number > 1){
            std::cout << "Generating Two Layer Bcm: " << std::endl;
            for(int i = 1 ; i < seg ; i++){
                std::cout << std::endl;
                std::cout << "Interface Position: " << i << ", " << "Total: " << seg << std::endl;
                std::cout << "Dielectric Pair: " << tempelec[0] << " " << tempelec[1] << std::endl;  
                boundarybcm(standard, i * h, i, seg);
                if(standard_h){
                    sindex = blocks[0].xEle * blocks[0].yEle / 2 ;
                }
                if( seg >=  16 && i < 16 && standard_v){
                    sindex = blocks[0].xEle * blocks[0].yEle  + 2 * blocks[0].yEle * blocks[0].zEle + 2 * blocks[0].xEle * blocks[0].zEle + (15 - i) * blocks[1].yEle + blocks[1].yEle / 2; 
                }else if(seg >= 16 && i >= 16 && standard_v){
                    sindex = blocks[0].xEle * blocks[0].yEle  + blocks[0].yEle * 15 + 15;
                }
                std::cout << "Center index: " << sindex << std::endl;
                generatebcm();
                deposite();
                write_txt();
                release_vector();
            }
            std::cout << "Saved as binary format!" << std::endl;
            write_binary();
        }else{
            std::cout << std::endl;
            std::cout << "Single Dielectric:" << std::endl;
            boundarybcm(false, 0, 0, 0);
            if(standard_h){
                sindex = blocks[0].xEle * blocks[0].yEle / 2 ;
            }else if(standard_v){
                sindex = blocks[0].xEle * blocks[0].yEle  + blocks[0].yEle * blocks[0].zEle / 2;
            }
            std::cout << "Center index: " << sindex << std::endl;
            generatebcm();
            deposite();
            write_txt();
        //	release_vector();
            write_binary();
        }	
    }else{
        boundarybcm(false, 0, 0, 0);
        generatebcm();
        write_txt();
        write_binary();
        release_mem();
    }
    std::cout << "Program End!" <<std::endl;
}

void deposite(){
    for(int i = 0 ; i < boundary_position.row ;i++){
        for(int j = 0 ; j < boundary_position.col ;j++){
            _two_axis.push_back(boundary_position.data[i][j]);
        }
    }
    std::cout << boundary_position.row <<std::endl;
    
    _two_diag.push_back(diag[sindex]);
    std::cout << diag.size() <<std::endl;
    std::cout << matrixbcm.col << std::endl;
    for(int i = 0 ; i < matrixbcm.col; i++){
        _pdf.push_back(matrixbcm.data[sindex][i]);
    }
}
void generatebcm(){
    MatrixGenBcm();
    printf("!!!Finish MatrixGenBcm!!!\n");
    MatrixModulate();
    if(cohesion){
        printf("!!!Begin Cohesion!!!\n");
        MatrixCohesion();
        printf("!!!End Cohesion!!!\n");
        printf("!!!Begin ModulatePosition!!!\n");
        ModulatePosition();
        printf("!!!End ModulatePosition!!!\n");
    }
    //ModulateBCM();
    //deal_matrix();
//	write_bcm();
//	cout<<"FRW-BCM END!"<<endl;
}
void tablegen(){
    //linear_equation_test();

    printf("################################\n");
    tg_init();
    printf("elements generate...\n");
    for(int i=0; i<Curmed; i++){
        switch(trans_area_type[i]){
            case CUBE_TRANS_AREA:				
                cube_point_gen(i);
                EleGen_cube(i);
                break;
            case PRISM_TRANS_AREA:
                EleGen_prism(i);
                break;
            case PRISM_TSV_TRANS_AREA:
                EleGen_tsv_prism(i);
                break;
            case FRUSTUM_TRANS_AREA:
                EleGen_frustum(i);
                break;
            default:
                cout<<"cannot recognize transfer area type, exit...\n";
                exit(1);
        }
        if(Curmed == 1){
            gBdElements[i][cur_ele_id[i]].cfg=observePoint;
            save_ele(i);
        }
    }
    //make sure that the interface division is the same
    if(Curmed == 2)
        unify_interface_divide();
    for(int i=0;i<Curmed;i++){
        cout<<"ele number["<<i<<"]="<<cur_ele_id[i]<<endl;	
        cout<<"interface start id="<<interface_start_id[i]<<endl;
    }
    printf("generating matrix...\n");
    
    if(multiCal ==true){
        for(double y=0.03125;y<1;y=y+0.0625){
            printf("b\n");
            for(double x=y;x<1;x=x+0.0625){
                printf("a\n");
                if(x*x+y*y>0.64){
                    gBdElements[0][cur_ele_id[0]].cfg.x=x;
                    gBdElements[0][cur_ele_id[0]].cfg.y=y;
                    xglobal=x;
                    yglobal=y;
                    MatrixGen();
                    if(Curmed == 2)
                        MatrixGen_More();
                    cout<<"matrix generate over!!!!!!!!!!!!!!"<<endl;
                    if(Curmed == 1)
                        formula_solve_signal();
                    else
                        formula_solve();
                    printf("release memory! <@_@>\n");
                    release_mem();
                    
                }
                
            }
            
        }
        
    }
    else{
        MatrixGen();
        if(Curmed == 2)
            MatrixGen_More();
        cout<<"matrix generate over!!!!!!!!!!!!!!"<<endl;
        if(Curmed ==1)
            formula_solve_signal();
        else
            formula_solve();
        printf("release_memory! <@_@>\n");
        release_mem();
        
    }
}
