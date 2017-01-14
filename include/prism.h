#ifndef _PRISM_H
#define _PRISM_H

#include "frwbcm.h"

//prism
void EleGen_prism(int bid){	
    //vertexNum=3,4,6£¬>=8
    //you'd better implement 3 first before midddle report
    int nx, ny, nz;
    nx = usTempEle[bid][0];
    ny = usTempEle[bid][1];
    nz = usTempEle[bid][2];
    int ntotal = 2*(nx*ny + ny*nz + nx*nz);
    gBdElements[bid] = new TBDELEINFO[ntotal+1];	
    cur_ele_id[bid] = 0;
    if(vertexNum[bid] == 3){
        START_RANGE(bid)
        EXAMINE_INTERFACE2(&vertexBuf[bid][0], &vertexBuf[bid][1], &vertexBuf[bid][2], nx, ny, 1, bid, 1)
        EXAMINE_INTERFACE2(&vertexTopBuf[bid][0], &vertexTopBuf[bid][1], &vertexTopBuf[bid][2], nx, ny, 0, bid, 1)
        EXAMINE_INTERFACE(&vertexBuf[bid][0], &vertexBuf[bid][1], &vertexTopBuf[bid][1], &vertexTopBuf[bid][0], nx, nz, 0, bid, 1)
        double hypotenuse = euclid_distance(&vertexBuf[bid][1], &vertexBuf[bid][2]);
        double len01 = euclid_distance(&vertexBuf[bid][0], &vertexBuf[bid][1]);
        int rnx = myround((hypotenuse/len01)*nx);		
        EXAMINE_INTERFACE(&vertexBuf[bid][1], &vertexBuf[bid][2], &vertexTopBuf[bid][2], &vertexTopBuf[bid][1], rnx, nz, 0, bid, 1)
        EXAMINE_INTERFACE(&vertexBuf[bid][2], &vertexBuf[bid][0], &vertexTopBuf[bid][0], &vertexTopBuf[bid][2], ny, nz, 0, bid, 1)
        END_RANGE2(bid)
    }
    else if(vertexNum[bid] == 4){//we assume len_01 > len_23
        START_RANGE(bid)
        EXAMINE_INTERFACE(&vertexBuf[bid][0], &vertexBuf[bid][1], &vertexBuf[bid][2], &vertexBuf[bid][3], nx, ny, 1, bid, 1)		
        EXAMINE_INTERFACE(&vertexTopBuf[bid][0], &vertexTopBuf[bid][1], &vertexTopBuf[bid][2], &vertexTopBuf[bid][3], nx, ny, 0, bid, 1)		
        EXAMINE_INTERFACE(&vertexBuf[bid][0], &vertexBuf[bid][1], &vertexTopBuf[bid][1], &vertexTopBuf[bid][0], nx, nz, 0, bid, 1)		
        double leny = len_ptol(&vertexBuf[bid][0], &vertexBuf[bid][3], &vertexBuf[bid][2]);
        int rny = myround((euclid_distance(&vertexBuf[bid][1], &vertexBuf[bid][2]) / leny) * ny);
        EXAMINE_INTERFACE(&vertexBuf[bid][1], &vertexBuf[bid][2], &vertexTopBuf[bid][2], &vertexTopBuf[bid][1], rny, nz, 0, bid, 1)		
        double lenx = euclid_distance(&vertexBuf[bid][0], &vertexBuf[bid][1]);
        int rnx = myround((euclid_distance(&vertexBuf[bid][2], &vertexBuf[bid][3]) / lenx) * nx);
        EXAMINE_INTERFACE(&vertexBuf[bid][2], &vertexBuf[bid][3], &vertexTopBuf[bid][3], &vertexTopBuf[bid][2], rnx, nz, 0, bid, 1)		
        rny = myround((euclid_distance(&vertexBuf[bid][3], &vertexBuf[bid][0]) / leny) * ny);
        EXAMINE_INTERFACE(&vertexBuf[bid][3], &vertexBuf[bid][0], &vertexTopBuf[bid][0], &vertexTopBuf[bid][3], rny, nz, 0, bid, 1)
        END_RANGE2(bid)
    }
    else if(vertexNum[bid] == 6){//default:
        reorder(vertexBuf[bid], vertexNum[bid]);
        reorder(vertexTopBuf[bid], vertexNum[bid]);
        double lenx = euclid_distance(&vertexBuf[bid][3], &vertexBuf[bid][6]);
        double len12 = euclid_distance(&vertexBuf[bid][1], &vertexBuf[bid][2]);
        int rnx = myround((len12/lenx)*nx);
        START_RANGE(bid)
        EXAMINE_INTERFACE(&vertexBuf[bid][5], &vertexBuf[bid][2], &vertexBuf[bid][1], &vertexBuf[bid][0], nx, ny/2, 0, bid, 0)	// 0? because here offer points clockwise
        EXAMINE_INTERFACE(&vertexBuf[bid][5], &vertexBuf[bid][2], &vertexBuf[bid][3], &vertexBuf[bid][4], nx, ny/2, 1, bid, 1)		
        EXAMINE_INTERFACE(&vertexTopBuf[bid][5], &vertexTopBuf[bid][2], &vertexTopBuf[bid][1], &vertexTopBuf[bid][0], nx, ny/2, 1, bid, 0)// the same as up
        EXAMINE_INTERFACE(&vertexTopBuf[bid][5], &vertexTopBuf[bid][2], &vertexTopBuf[bid][3], &vertexTopBuf[bid][4], nx, ny/2, 0, bid, 1)
        //
        for(int i=0; i<6; i++){			
            EXAMINE_INTERFACE(&vertexBuf[bid][i], &vertexBuf[bid][(i+1)%6], &vertexTopBuf[bid][(i+1)%6], &vertexTopBuf[bid][i], rnx, nz, 0, bid, 1)			
        }
        END_RANGE2(bid)
    }
    else if(vertexNum[bid] >= 8 && vertexNum[bid]%4 == 0){ //possibly assume normal prism. yuwj
        reorder(vertexBuf[bid], vertexNum[bid]);
        reorder(vertexTopBuf[bid], vertexNum[bid]);
        int num = vertexNum[bid]/2 - 1;
        int rnx, rny, reverse;
        int tnum = vertexNum[bid]/4 -1;
        double lenxy = euclid_distance(&vertexBuf[bid][0-tnum+vertexNum[bid]], &vertexBuf[bid][1+tnum]);
        //bottom
        START_RANGE(bid)
        for(int i=0; i<num; i++){
            TPOINT p1, p2, p3, p4;
            p1=vertexBuf[bid][(0-i+vertexNum[bid])%vertexNum[bid]]; 
            p2=vertexBuf[bid][(1+i+vertexNum[bid])%vertexNum[bid]]; 
            p3=vertexBuf[bid][(1+i+1+vertexNum[bid])%vertexNum[bid]];			
            p4=vertexBuf[bid][(0-i-1+vertexNum[bid])%vertexNum[bid]];
            reverse=1;
            if(i<tnum){//p4 p3 p2 p1
                TPOINT tp;
                tp=p1; p1=p4; p4=tp;
                tp=p2; p2=p3; p3=tp;
                reverse=0;
            }
            rnx=myround((euclid_distance(&p1, &p2)/lenxy)*nx);
            double verlen=len_ptol(&p1, &p3, &p4);
            rny=myround((verlen/lenxy)*ny);
            EXAMINE_INTERFACE(&p1, &p2, &p3, &p4, rnx, rny, reverse, bid, i-num+2)
        }
        //top		
        for(int i=0; i<num; i++){
            TPOINT p1, p2, p3, p4;
            p1=vertexTopBuf[bid][(0-i+vertexNum[bid])%vertexNum[bid]]; 
            p2=vertexTopBuf[bid][(1+i+vertexNum[bid])%vertexNum[bid]]; 
            p3=vertexTopBuf[bid][(1+i+1+vertexNum[bid])%vertexNum[bid]];			
            p4=vertexTopBuf[bid][(0-i-1+vertexNum[bid])%vertexNum[bid]];
            reverse=0;
            if(i<tnum){//p4 p3 p2 p1
                TPOINT tp;
                tp=p1; p1=p4; p4=tp;
                tp=p2; p2=p3; p3=tp;
                reverse=1;
            }
            rnx=myround((euclid_distance(&p1, &p2)/lenxy)*nx);
            double verlen=len_ptol(&p1, &p3, &p4);
            rny=myround((verlen/lenxy)*ny);
            EXAMINE_INTERFACE(&p1, &p2, &p3, &p4, rnx, rny, reverse, bid, i-num+2)
        }
        //side
        rnx=myround((euclid_distance(&vertexBuf[bid][0], &vertexBuf[bid][1])/lenxy)*nx);
        for(int i=0; i<vertexNum[bid]; i++){			
            EXAMINE_INTERFACE(&vertexBuf[bid][i], &vertexBuf[bid][(i+1)%vertexNum[bid]], &vertexTopBuf[bid][(i+1)%vertexNum[bid]], &vertexTopBuf[bid][i], rnx, nz, 0, bid, 1)
        }
        END_RANGE2(bid)
    }
    else{
        printf("!!!vertexNum[bid]=%d\n", vertexNum[bid]);
        printf("this program cannot handle your transfer region, exit...\n");
        exit(1);
    }
}

#endif
