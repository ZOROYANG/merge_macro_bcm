/* Functions in this file are the write funcition*/
#ifndef _WRITEDATA_H
#define _WRITEDATA_H

#include "frwbcm.h"
#include "sparse.h"
#include <stdlib.h>
extern bool standard_v;
extern bool standard_h;

extern bool binary;
extern bool readable;
cs matrixcsc;
extern size_t sindex;

void write_txt(){

    if (readable){
        std::cout<<"BCM will be saved as txt format!" << std::endl;
        std::string path = "res/" + filename+ ".txt";
        ofstream fid(path.c_str(),ios::binary);
        //ofstream fid(path.c_str(),ios::app);
    
        fid << "one time " << std::endl;	
        if(standard_v || standard_h){
        for(int j=0;j<static_cast<int>(_two_axis.size());j++){
            fid << _two_axis[j] << " ";
                //fid.write((char*)(&_two_axis[j]),sizeof(_two_axis[j]));
        }
        fid << " axis size" << _two_axis.size() << std::endl;
        fid << endl;
        for(int i = 0 ; i <static_cast<int>(_two_diag.size()); i++){
            fid << _two_diag[i] << std::endl;
        //	fid.write((char*)(&_two_diag[i]),sizeof(_two_diag[i]));
        }
        fid << "pdf size" << _pdf.size() << std::endl;
        for(int k=0;k<static_cast<int>(_pdf.size());k++)
            fid << _pdf[k] << " " ;
        fid.close();	
        for(int j=0;j<boundary_position.row;j++){
            for(int k=0;k<boundary_position.col;k++){
                //float p = float(boundary_position.data[j][k]);
                //fid.write((char*)(&p),sizeof(p));
            //	fid.write((char*)(&boundary_position.data[j][k]),sizeof(boundary_position.data[j][k]));
                fid << boundary_position.data[j][k] << " " ;
                }
                fid << std::endl;
        }
        
        std::cout << " bbbbbbbbbbbbbbbbbbb" << std::endl;
        //fid.write((char*)(&diag[index]),sizeof(diag[index]));
        fid << diag[sindex] << std::endl;
        for(int k=0;k<matrixbcm.col;k++)
            //fid.write((char*)(&matrixbcm.data[index][k]),sizeof(matrixbcm.data[index][k]));
            fid << matrixbcm.data[sindex][k] << " " ;
    //	std::cout << " ccccccc" << std::endl;
        fid << std::endl;
        fid << std::endl;
        fid << std::endl;
        fid << std::endl;
        fid.close();
        }else if(!standard_v && !standard_h){
        fid << blocks.size() << std::endl;
        fid << matrixbcm.row << std::endl;

        for(int i = 0 ; i < static_cast<int>(_length.size()) ;i++){
            fid << _length[i] << " " ;
        }
        fid << std::endl;		
        for(int i = 0 ; i < static_cast<int>(ele_x.size()); i++){
            size_t x = size_t(ele_x[i]);
            fid << x << " " ;
            //fid.write((char*)(&x),sizeof(x));
        }
        fid << std::endl;
        for(int i = 0 ; i < static_cast<int>(ele_y.size()); i++){
            size_t y = size_t(ele_y[i]);
            fid << y << " " ;
            //fid.write((char*)(&y),sizeof(y));
        }
        fid << std::endl;
        for(int i = 0 ; i < static_cast<int>(ele_z.size()); i++){
            size_t z = size_t(ele_z[i]);
            fid << z << " " ;
            //fid.write((char*)(&z),sizeof(z));
        }
        fid << std::endl;

        for(int i = 0 ; i < static_cast<int>(high.size()) ; i++){
            fid << high[i] <<" " ;
        }
        fid << std::endl;		
        for(int i = 0 ; i< static_cast<int>(segments_x.size()); i++){
            for(int j = 0 ; j < static_cast<int>(segments_x[i].size()); j++){
                fid << segments_x[i][j] << " " ;
                //fid.write((char*)(&segments_x[i][j]),sizeof(segments_x[i][j]));
            }
            fid << std::endl;
        }
        for(int i = 0 ; i< static_cast<int>(segments_y.size()); i++){
            for(int j = 0 ; j < static_cast<int>(segments_y[i].size()); j++){
                fid << segments_y[i][j] << " " ;
                //fid.write((char*)(&segments_y[i][j]),sizeof(segments_y[i][j]));
            }
            fid << std::endl;
        }
        for(int i = 0 ; i< static_cast<int>(segments_z.size()) ; i++){
            for(int j = 0 ; j < static_cast<int>(segments_z[i].size()); j++){
                fid << segments_z[i][j] << " " ;
                //fid.write((char*)(&segments_z[i][j]),sizeof(segments_z[i][j]));
            }
            fid << std::endl;
        }
        
        for(int i = 0 ; i < 3 ; i++){
            for(int j = 0 ; j < static_cast<int>(segments[i].size()); j++)
                fid << segments[i][j] << " ";
                fid<< std::endl;
        }

        for(int i = 0 ; i < static_cast<int>(Areas.size()) ;i++){
            for( int j = 0 ; j < static_cast<int>(Areas[i].size()) ;j++){
                fid << Areas[i][j] << " " ;
            }
            fid << std::endl;
        }
        
        for(int i = 0; i < static_cast<int>(condp.size()); i++)
            fid << condp[i] << " condp  " ;
            fid << std::endl;
    
        fid << boundary_position.row << " " << boundary_position.col << std::endl;
        int k = 0;
        fid << "*********************" <<std::endl;
        for(int j=0;j<boundary_position.row;j++){
            for(k=0;k<boundary_position.col;k++)
                fid << boundary_position.data[j][k] << " " ;
                fid << std::endl;
        }
        fid << "*********************" <<std::endl;
        std::cout << boundary_position.data[0][920] << std::endl;
        std::cout << boundary_position.data[1][920] << std::endl;
        std::cout << boundary_position.data[2][920] << std::endl;
        for(int j=0;j<matrixbcm.row;j++){
            for(int k=0;k < matrixbcm.col;k++)
                fid << matrixbcm.data[j][k] << " " ;
                fid << std::endl;
        }
        fid.close();
        }
    }//std::cout<<"BCM will be saved as binary format!" << std::endl;
}
void write_binary(){

/*	std::cout << "write date.............." <<std::endl;
    std::cout << " matrixbcm.row " << matrixbcm.row << endl;
    std::cout << " matrixbcm.col " << matrixbcm.row << endl;

    for(int j=0;j<boundary_position.row;j++){
        for(int k=0;k<boundary_position.col;k++)
            std::cout << boundary_position.data[j][k] << " " ;
            std::cout<< std::endl;
    }
    std::cout<< "position " << std::endl;

    for(int j=0;j <matrixbcm.row;j++){
        double sumcol = 0;	
        for(int k=0;k<matrixbcm.col;k++){
            //if( j != k && matrixbcm.data[j][k] > 0){
            //	matrixbcm.data[j][k] = 0 ;
            //}
            sumcol = sumcol + matrixbcm.data[j][k];
        }
        std::cout << sumcol << endl;
    }*/
    if (binary){
        //std::cout<<"BCM will be saved as binary format!" << std::endl;
    
        if(!standard_v && !standard_h){
        std::string path = "res/" + filename+ ".db";
        ofstream fid(path.c_str(),ios::binary);
        size_t size = blocks.size();
        size_t cn = size_t(cond_number);
        fid.write((char*)(&size),sizeof(size));
        fid.write((char*)(&cn),sizeof(cn));
        size_t dimen = size_t(matrixbcm.row);
        fid.write((char*)(&dimen),sizeof(dimen));
        for(int i = 0 ; i < static_cast<int>(ele_x.size()); i++){
            size_t x = size_t(ele_x[i]);
            fid.write((char*)(&x),sizeof(x));
        }
        for(int i = 0 ; i < static_cast<int>(ele_y.size()); i++){
            size_t y = size_t(ele_y[i]);
            fid.write((char*)(&y),sizeof(y));
        }
        for(int i = 0 ; i < static_cast<int>(ele_z.size()); i++){
            size_t z = size_t(ele_z[i]);
            fid.write((char*)(&z),sizeof(z));
        }
        for(int i =0 ; i < static_cast<int>(_length.size()); i++){
            fid.write((char*)(&_length[i]),sizeof(_length[i]));
        }	
        for(int i = 0 ; i < static_cast<int>(Areas.size()) ;i++){
            for( int j = 0 ; j < static_cast<int>(Areas[i].size()) ;j++){
                fid.write((char*)(&Areas[i][j]),sizeof(Areas[i][j]));
            }
        }
        for(int i = 0 ; i < static_cast<int>(high.size()) ; i++){
            fid.write((char*)(&high[i]), sizeof(high[i]));
        }
        for(int i = 0 ; i< block_number ; i++){
            for(int j = 0 ; j < static_cast<int>(segments_x[i].size()); j++){
                fid.write((char*)(&segments_x[i][j]),sizeof(segments_x[i][j]));
            }
        }
        for(int i = 0 ; i< block_number ; i++){
            for(int j = 0 ; j < static_cast<int>(segments_y[i].size()); j++){
                fid.write((char*)(&segments_y[i][j]),sizeof(segments_y[i][j]));
            }
        }
        for(int i = 0 ; i< block_number ; i++){
            for(int j = 0 ; j < static_cast<int>(segments_z[i].size()); j++){
                fid.write((char*)(&segments_z[i][j]),sizeof(segments_z[i][j]));
            }
        }
        for(int i = 0 ; i < static_cast<int>(condp.size()); i++){
            size_t cp = size_t(condp[i]);
            fid.write((char*)(&cp),sizeof(cp));
        }
        for(int j=0;j<boundary_position.row;j++){
            for(int k=0;k<boundary_position.col;k++){
                fid.write((char*)(&boundary_position.data[j][k]),sizeof(boundary_position.data[j][k]));
            }
        }
        for(int j = 0 ; j < static_cast<int>(diag.size()); j++){
            fid.write((char*)(&diag[j]),sizeof(diag[j]));
        }
        for(int j=0;j<matrixbcm.row;j++){
            for(int k=0;k<matrixbcm.col;k++)
                fid.write((char*)(&matrixbcm.data[j][k]),sizeof(matrixbcm.data[j][k]));
        }
        fid.close();
        }if(standard_v || standard_h){
        std::string path, path_axis, path_diag;
        
        if(standard_v && block_number == 1){
            path = "res/single_vertical.db";
            path_axis = "res/single_vertical_axis.db";
            path_diag = "res/single_vertical_diag.db";
        }else if(standard_v && block_number > 1){
            char _epsa[10];
            char _epsb[10];
            gcvt(tempelec[0], 5, _epsa);
            gcvt(tempelec[1], 5, _epsb);
            std::string epsa(_epsa);
            std::string epsb(_epsb);
            path = "res/vertical" + epsa + "_" + epsb + ".db" ;
            path_axis = "res/vertical_axis" + epsa + "_" + epsb + ".db" ;
            path_diag = "res/vertical_diag" + epsa + "_" + epsb + ".db" ;
        }else if(standard_h && block_number == 1){
            path = "res/single_horizontal.db";
            path_axis = "res/single_horizontal_axis.db";
            path_diag = "res/single_horizontal_diag.db";
        }else if(standard_h && block_number > 1){
            char _epsa[10];
            char _epsb[10];
            gcvt(tempelec[0], 5, _epsa);
            gcvt(tempelec[1], 5, _epsb);
            std::string epsa(_epsa);
            std::string epsb(_epsb);
            path = "res/horizontal" + epsa + "_" + epsb + ".db" ;
            path_axis = "res/horizontal_axis" + epsa + "_" + epsb + ".db" ;
            path_diag = "res/horizontal_diag" + epsa + "_" + epsb + ".db" ;
        }
        std::cout<<path << std::endl;			 
        std::cout<<path_axis << std::endl;			 
        std::cout<<path_diag << std::endl;			 
        
        ofstream fid(path_axis.c_str(),ios::binary);
        if(!fid){
            exit(1);
        }
        for(int j=0;j<static_cast<int>(_two_axis.size());j++){
                fid.write((char*)(&_two_axis[j]),sizeof(_two_axis[j]));
        }
        fid.close();
        ofstream fid1(path_diag.c_str(),ios::binary);
        for(int i = 0 ; i <  static_cast<int>(_two_diag.size()); i++){
            fid1.write((char*)(&_two_diag[i]),sizeof(_two_diag[i]));
        }
        fid1.close();
        ofstream fid2(path.c_str(),ios::binary);
        for(int k=0;k<static_cast<int>(_pdf.size());k++)
            fid2.write((char*)(&_pdf[k]),sizeof(_pdf[k]));
        fid2.close();
        }			
    }
/*	vector<int>().swap(ele_x);
    vector<int>().swap(ele_y);
    vector<int>().swap(ele_z);
    vector<vector<double>>().swap(segments_x);
    vector<vector<double>>().swap(segments_y);
    vector<vector<double>>().swap(segments_z);
    boundary_position.release();
    matrixbcm.release();
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
    vector<int>().swap(BdElementInterSize);
    vector<BLOCKINFO>().swap(blocks);
    vector<CONDUCTORINFO>().swap(conductors);
*/
}
void write_res(){
    if(multiCal ==false){
    FILE *fp=fopen("res/trans_vec.txt","w+");
    FILE *fp_x=fopen("res/cfg-x.txt","w+");
    FILE *fp_y=fopen("res/cfg-y.txt","w+");
    FILE *fp_z=fopen("res/cfg-z.txt","w+");
    FILE *fp_p=fopen("res/cfg-p.txt","w+");
    FILE *fp_xyz1=fopen("res/cfg-xyz1.db","w+");
    if(fp==NULL || fp_x==NULL || fp_y==NULL || fp_z==NULL || fp_p==NULL || fp_xyz1==NULL){
        printf("open file trans_vec.txt/cfg-x/y/z/p/xyz1.txt error\n");
        exit(1);
    }
                                                    
    //write to file
    double tmp_sum=0.0, tmp_sum1=0.0;
    for(int j=0;j<cur_ele_id[0];j++){
        if(vector_g[0][j]>=0)				
            tmp_sum += vector_g[0][j];
        else{
            tmp_sum1 += vector_g[0][j];
            vector_g[0][j] = 0.0;
        }
        fprintf(fp,"id=%d p=%e\n",j,vector_g[0][j]); 
        fprintf(fp_x, "%e\n", gBdElements[0][j].cfg.x);
        fprintf(fp_y, "%e\n", gBdElements[0][j].cfg.y);
        fprintf(fp_z, "%e\n", gBdElements[0][j].cfg.z);
        fprintf(fp_p, "%e\n", vector_g[0][j]);
        /*fprintf(fp_xyz1, "%e %e %e 1.0\n", gBdElements[0][j].cfg.x, gBdElements[0][j].cfg.y, gBdElements[0][j].cfg.z);
    */
        fwrite(&gBdElements[0][j].cfg.x,sizeof(double),1,fp_xyz1);
        fwrite(&gBdElements[0][j].cfg.y,sizeof(double),1,fp_xyz1);
        fwrite(&gBdElements[0][j].cfg.z,sizeof(double),1,fp_xyz1);
    }
    //close file
    fclose(fp);
    fclose(fp_x);
    fclose(fp_y);
    fclose(fp_p);
    fclose(fp_xyz1);	
    //write range 
    FILE *fp1=fopen("res/range.txt","w+");
    if(fp1==NULL){
        printf("open file range.txt error\n");
        exit(1);
    }
    for(int i=0; i<=range_id[0]; i++){
        fprintf(fp1, "id=%d begin=%d end=%d\n sum=%d\n", i, prange[0][i].begin, prange[0][i].end, prange[0][i].end-prange[0][i].begin);
    }
    fprintf(fp1, "total of probability:[+]=%e    [-]=%e\n", tmp_sum, tmp_sum1);
    fclose(fp1);
    }
    else{
    char fname1[200];
sprintf(fname1,"%s%f%s%f%s","res/0.4p",xglobal,"x",yglobal,"y31.db");
FILE *fp=fopen(fname1,"w+");
if (fp==NULL){
    printf("open 0.4p%fx%fy31.db failed\n",xglobal,yglobal);
    exit(1);
}
    double tmp_sum=0.0, tmp_sum1=0.0;
for(int j=0;j<cur_ele_id[0];j++){
        if(vector_g[0][j]>=0)				
            tmp_sum += vector_g[0][j];
        else{
            tmp_sum1 += vector_g[0][j];
            vector_g[0][j] = 0.0;
        }
    fwrite(&vector_g[0][j],sizeof(double),1,fp);
    /*fprintf(fp,"id=%d p=%e\n",j,vector_g[0][j]);*/ 
}
fclose(fp);
}

}
void write_res2(){
    FILE *fp=fopen("res/trans_vec.txt","w+");
    FILE *fp_x=fopen("res/cfg-x.txt","w+");
    FILE *fp_y=fopen("res/cfg-y.txt","w+");
    FILE *fp_z=fopen("res/cfg-z.txt","w+");
    FILE *fp_p=fopen("res/cfg-p.txt","w+");
    FILE *fp_xyz1=fopen("res/cfg-xyz1.txt","w+");
    if(fp==NULL || fp_x==NULL || fp_y==NULL || fp_z==NULL || fp_p==NULL || fp_xyz1==NULL){
        printf("open file trans_vec.txt/cfg-x/y/z/p/xyz1.txt error\n");
        exit(1);
    }
    //write to file
    double tmp_sum=0.0, tmp_sum1=0.0;
    for(int i=0;i<static_cast<int>(Curmed);i++){
        int tlen;
        if(i==0)
            tlen=0;
        else
            tlen=interface_start_id[0];
        for(int j=0;j<interface_start_id[i];j++){
            if(final_res[j+tlen] >= 0)
                tmp_sum += final_res[j+tlen];
            else{
                tmp_sum1 += final_res[j+tlen];
                final_res[j+tlen] = 0.0;
            }			
            fprintf(fp, "id=%d    p=%e\n", j+tlen, final_res[j+tlen]);
            fprintf(fp_x, "%e\n", gBdElements[i][j].cfg.x);
            fprintf(fp_y, "%e\n", gBdElements[i][j].cfg.y);
            fprintf(fp_z, "%e\n", gBdElements[i][j].cfg.z);
            fprintf(fp_p, "%e\n", final_res[j+tlen]);
            fprintf(fp_xyz1, "%e %e %e 1.0\n", gBdElements[i][j].cfg.x, gBdElements[i][j].cfg.y, gBdElements[i][j].cfg.z);
        }
    }
    //close file
    fclose(fp);
    fclose(fp_x);
    fclose(fp_y);
    fclose(fp_p);
    fclose(fp_xyz1);	
    //write range 
    FILE *fp1=fopen("res/range.txt","w+");
    if(fp1==NULL){
        printf("open file range.txt error\n");
        exit(1);
    }
    int tlen=0;
    for(int i=0;i<static_cast<int>(Curmed);i++){
        for(int j=0;j<=range_id[i];j++){
            fprintf(fp1, "id=%d begin=%d(%d) end=%d(%d)\n sum=%d\n", j, prange[i][j].begin+tlen, prange[i][j].begin, prange[i][j].end+tlen, prange[i][j].end, prange[i][j].end-prange[i][j].begin);
        }
        tlen += interface_start_id[i];
        fprintf(fp1, "---------------\n");
    }
    fprintf(fp1, "total of probability:[+]=%e    [-]=%e\n", tmp_sum, tmp_sum1);
    fclose(fp1);
}
void write_matrix_res(){
    //write to file
    FILE* afp[4];
    afp[0]=fopen("res/G1.txt","w+");
    afp[1]=fopen("res/H1.txt","w+");
    if(Curblk==2){
        afp[2]=fopen("res/G2.txt","w+");
        afp[3]=fopen("res/H2.txt","w+");
    }
    FILE* fp_g=fopen("res/gg.txt","w+");
    FILE* fp_h=fopen("res/hh.txt","w+");
    if(afp[0]==NULL || afp[1]==NULL ||fp_g==NULL||fp_h==NULL){
        printf("open file G.txt/H.txt/g.txt/h.txt error\n");
        exit(1);
    }
    if(Curmed==2&&(afp[2]==NULL||afp[3]==NULL)){
        printf("open file G2.txt/H2.txt error,exit...\n");
        exit(1);
    }
    for(int k=0;k<Curblk;k++){
        for(int i=0;i<cur_ele_id[k];i++){
            for(int j=0;j<cur_ele_id[k];j++){
                fprintf(afp[k*2], "%e  ", matrix_G[k][i*cur_ele_id[k]+j]);
                fprintf(afp[2*k+1], "%e  ", matrix_H[k][i*cur_ele_id[k]+j]);
            }
            fprintf(afp[k*2],"\n\n");
            fprintf(afp[2*k+1],"\n\n");
            if(k==1)
                continue;
            fprintf(fp_g, "%e\n", vector_g[k][i]);
            fprintf(fp_h, "%e\n", vector_h[k][i]);
        }
    }
    fclose(afp[0]);
    fclose(afp[1]);
    if(Curblk==2){
        fclose(afp[2]);
        fclose(afp[3]);
    }
    fclose(fp_g);
    fclose(fp_h);
}
void save_ele(int bid){
    //output to debug
    char fname[100];
    sprintf(fname, "%s%d%s", "res/elements", bid+1, ".txt");
    FILE* fp=fopen(fname, "w+");
    if(fp==NULL){
        printf("open elements%d.txt error\n", bid+1);
        exit(1);
    }
    for(int i=0;i<cur_ele_id[bid];i++){
        fprintf(fp, "id=%d  type=%d  ld(%lf, %lf, %lf)  rd(%lf, %lf, %lf)  ru(%lf, %lf, %lf)  lu(%lf, %lf, %lf) cfg(%lf, %lf, %lf)\n", 
                i, gBdElements[bid][i].type, 
                    gBdElements[bid][i].leftdown.x, gBdElements[bid][i].leftdown.y, gBdElements[bid][i].leftdown.z, 
                        gBdElements[bid][i].rightdown.x, gBdElements[bid][i].rightdown.y, gBdElements[bid][i].rightdown.z,
                            gBdElements[bid][i].rightup.x, gBdElements[bid][i].rightup.y, gBdElements[bid][i].rightup.z,
                                gBdElements[bid][i].leftup.x, gBdElements[bid][i].leftup.y, gBdElements[bid][i].leftup.z,
                                    gBdElements[bid][i].cfg.x, gBdElements[bid][i].cfg.y, gBdElements[bid][i].cfg.z);
    }
    if(Curmed==1 || (Curmed == 2 && bid == pointInMediumId))
        fprintf(fp, "cfg(%lf, %lf, %lf)\n", 
                    gBdElements[bid][cur_ele_id[bid]].cfg.x, gBdElements[bid][cur_ele_id[bid]].cfg.y, gBdElements[bid][cur_ele_id[bid]].cfg.z);
    fclose(fp);
}
#endif
