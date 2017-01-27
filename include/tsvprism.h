#ifndef _TSVPRISM_H
#define _TSVPRISM_H
#include "frwbcm.h"

//tsv prism
void EleGen_tsv_prism(int bid){//just handle 4 vertex
    if(!((vertexNum[bid]-4)>0 && (vertexNum[bid]-4)%4==0)){
        printf("this program cannot handle your transfer region, exit...\n");
        printf("<><>vertexNum[bid]=%d\n", vertexNum[bid]);
        exit(1);
    }
    int nx, ny, nz;
    nx = usTempEle[bid][0];
    ny = usTempEle[bid][1];
    nz = usTempEle[bid][2]; 
    int ntotal = 2*(nx*ny) + 4*(ny*nz) + 4*(nx*nz);
    gBdElements[bid] = new TBDELEINFO[ntotal+1];
    cur_ele_id[bid] = 0;
    //up and down subface
    START_RANGE(bid)
    tsv_subface_solve(vertexBuf[bid], vertexBuf[bid]+4, vertexNum[bid]-4, nx, ny, 1, bid);
    tsv_subface_solve(vertexTopBuf[bid], vertexTopBuf[bid]+4, vertexNum[bid]-4, nx, ny, 0, bid);
    //side surface
    TPOINT *start, *start1;
    int loop_num;
    int rnx;//, rny;
    //in side surface
    start=vertexBuf[bid]+4;
    start1=vertexTopBuf[bid]+4;
    loop_num=vertexNum[bid]-4;
    double lenx;
    lenx = euclid_distance(&vertexBuf[bid][0], &vertexBuf[bid][1]);
    rnx=myround((euclid_distance(&start[0], &start[1])/lenx)*nx);
    for(int i=0; i<loop_num; i++){
        EXAMINE_INTERFACE(&start[i], &start[(i+1)%loop_num], &start1[(i+1)%loop_num], &start1[i], rnx, nz, 1, bid, 1)
    }
    //out side surface	
    for(int i=0; i<4; i++){
        if(i%2==0)
            rnx=nx;
        else
            rnx=ny;
        EXAMINE_INTERFACE(&vertexBuf[bid][i], &vertexBuf[bid][(i+1)%4], &vertexTopBuf[bid][(i+1)%4], &vertexTopBuf[bid][i], rnx, nz, 0, bid, 1)
    }
    END_RANGE2(bid)
}
#endif
