#ifndef _BOUNDARYFRW_H_
#define _BOUNDARYFRW_H_
#include "frwbcm.h"
void cube_point_gen(int bid){
    
    vertexBuf[bid] = new TPOINT[4];
    vertexTopBuf[bid] = new TPOINT[4];
    vertexNum[bid] = 4;
    vertexBuf[bid][0] = tempblock2[bid].basePoint;
    TPOINT temp;
    add_point(&tempblock2[bid].basePoint, &tempblock2[bid].xAxis, &temp);
    vertexBuf[bid][1] = temp;
    add_point(&tempblock2[bid].basePoint, &tempblock2[bid].yAxis, &temp);
    vertexBuf[bid][3] = temp;
    add_point(&vertexBuf[bid][1], &vertexBuf[bid][3], &temp);
    vertexBuf[bid][2].x = temp.x-vertexBuf[bid][0].x;
    vertexBuf[bid][2].y = temp.y-vertexBuf[bid][0].y;
    vertexBuf[bid][2].z = temp.z-vertexBuf[bid][0].z;
    add_point(&tempblock2[bid].basePoint, &tempblock2[bid].zAxis, &temp);	
    for(int i=0;i<4;i++){
        vertexTopBuf[bid][i] = vertexBuf[bid][i];
        vertexTopBuf[bid][i].z = temp.z;
    }
}
////////////////////////////
/* 
* extract all elements info
*/
void EleGen_cube(int bid){
    int nx, ny, nz;
    //int cnx[2];
    //int cny[2];
    //int cnz[2];
    nx = usTempEle[bid][0];
    ny = usTempEle[bid][1];
    nz = usTempEle[bid][2];
    int ntotal = 2*(nx*ny + ny*nz + nx*nz);
    gBdElements[bid] = new TBDELEINFO[ntotal+1];	
    cur_ele_id[bid] = 0;
    START_RANGE(bid)
    EXAMINE_INTERFACE(&vertexBuf[bid][0], &vertexBuf[bid][1], &vertexBuf[bid][2], &vertexBuf[bid][3], nx, ny, 1, bid, 1)		
    EXAMINE_INTERFACE(&vertexTopBuf[bid][0], &vertexTopBuf[bid][1], &vertexTopBuf[bid][2], &vertexTopBuf[bid][3], nx, ny, 0, bid, 1)		
    EXAMINE_INTERFACE(&vertexBuf[bid][0], &vertexBuf[bid][1], &vertexTopBuf[bid][1], &vertexTopBuf[bid][0], nx, nz, 0, bid, 1)		
    EXAMINE_INTERFACE(&vertexBuf[bid][1], &vertexBuf[bid][2], &vertexTopBuf[bid][2], &vertexTopBuf[bid][1], ny, nz, 0, bid, 1)		
    EXAMINE_INTERFACE(&vertexBuf[bid][2], &vertexBuf[bid][3], &vertexTopBuf[bid][3], &vertexTopBuf[bid][2], nx, nz, 0, bid, 1)		
    EXAMINE_INTERFACE(&vertexBuf[bid][3], &vertexBuf[bid][0], &vertexTopBuf[bid][0], &vertexTopBuf[bid][3], ny, nz, 0, bid, 1)
    END_RANGE2(bid)
}
#endif
