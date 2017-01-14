#ifndef _FRUSTUM_H
#define _FRUSTUM_H

#include "frwbcm.h"

// frustum element generate
// just handle the frustum containing 4 vertexs in bottom/top
// bid: block id
void EleGen_frustum(int bid){
    if(vertexNum[bid]!=8){
        printf("sorry! this program can just handle the frustum containing 8 vertexs,exit...\n");
        printf("<><>vertexNum=%d\n", vertexNum[bid]);
        exit(1);
    }
    //prepare array
    int nx, ny, nz;
    nx = usTempEle[bid][0];
    ny = usTempEle[bid][1];
    nz = usTempEle[bid][2];
    double len_z=vertexTopBuf[bid][0].z-vertexBuf[bid][0].z;
    double len1=len_ptol(&vertexTopBuf[bid][0], &vertexBuf[bid][0], &vertexBuf[bid][1]);
    double len2=len_ptol(&vertexTopBuf[bid][1], &vertexBuf[bid][1], &vertexBuf[bid][2]);
    double len3=len_ptol(&vertexTopBuf[bid][2], &vertexBuf[bid][2], &vertexBuf[bid][3]);
    double len4=len_ptol(&vertexTopBuf[bid][3], &vertexBuf[bid][3], &vertexBuf[bid][0]);
    double max_len=bigger(len1, bigger(len2, bigger(len3, len4)));
    int nzz=myround((max_len/len_z)*nz);
    int ntotal = 2*(nx*ny + ny*nzz + nx*nzz);//
    gBdElements[bid] = new TBDELEINFO[ntotal+1];
    cur_ele_id[bid] = 0;
    //begin to generate
    int rnx, rny;
    START_RANGE(bid)
    rnx=nx; rny=ny;
    //bottom and top
    EXAMINE_INTERFACE(&vertexBuf[bid][0], &vertexBuf[bid][1], &vertexBuf[bid][2], &vertexBuf[bid][3], rnx, rny, 1, bid, 1)	
    EXAMINE_INTERFACE(&vertexTopBuf[bid][0], &vertexTopBuf[bid][1], &vertexTopBuf[bid][2], &vertexTopBuf[bid][3], rnx, rny, 0, bid, 1)
    //sides
    rny=myround((len1/len_z)*nz);
    EXAMINE_INTERFACE(&vertexBuf[bid][0], &vertexBuf[bid][1], &vertexTopBuf[bid][1], &vertexTopBuf[bid][0], rnx, rny, 0, bid, 1)
    rnx=ny; rny=myround((len2/len_z)*nz);
    EXAMINE_INTERFACE(&vertexBuf[bid][1], &vertexBuf[bid][2], &vertexTopBuf[bid][2], &vertexTopBuf[bid][1], rnx, rny, 0, bid, 1)
    rnx=nx; rny=myround((len3/len_z)*nz);
    EXAMINE_INTERFACE(&vertexBuf[bid][2], &vertexBuf[bid][3], &vertexTopBuf[bid][3], &vertexTopBuf[bid][2], rnx, rny, 0, bid, 1)
    rnx=ny; rny=myround((len4/len_z)*nz);
    EXAMINE_INTERFACE(&vertexBuf[bid][3], &vertexBuf[bid][0], &vertexTopBuf[bid][0], &vertexTopBuf[bid][3], rnx, rny, 0, bid, 1)
    END_RANGE2(bid)	
}
#endif
