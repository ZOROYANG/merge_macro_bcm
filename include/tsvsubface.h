#ifndef _TSVSURFACE_H
#define _TSVSURFACE_H

#include "frwbcm.h"
//divide the boundary elements of tsv
void tsv_subface_solve(TPOINT* vertexBufIn, TPOINT* vertexIn, int vertexInNum, int nx, int ny, int isbot, int bid){
    reorder(vertexIn, vertexInNum);
    //
    double leny, lenx;
    lenx = euclid_distance(&vertexBufIn[0], &vertexBufIn[1]);
    leny = euclid_distance(&vertexBufIn[0], &vertexBufIn[3]);	
    TVECTOR vec_y;
    sub_point(&vertexBufIn[3], &vertexBufIn[0], &vec_y);
    TPOINT p1, p2, p3, p4;
    double h1, h2;
    int num = vertexInNum/2 - 1;
    int rnx, rny, tnum, reverse;
    tnum=vertexInNum/4-1;
    //bot-down-rec
    h2=len_ptol(&vertexIn[0], &vertexBufIn[0], &vertexBufIn[1]);
    p1=vertexBufIn[0]; p2=vertexBufIn[1];
    zoom_point(&vec_y, h2/leny, &p3);
    zoom_point(&vec_y, h2/leny, &p4);
    add_point(&p3, &vertexBufIn[1], &p3);
    add_point(&p4, &vertexBufIn[0], &p4);
    rny=myround((h2/leny)*ny);
    reverse=isbot;
    EXAMINE_INTERFACE(&p1, &p2, &p3, &p4, nx, rny, reverse, bid, 0)
    //bot-middle-trapezoid
    for(int i=0; i<num; i++){
        h1=len_ptol(&vertexIn[(0-i+vertexInNum)%vertexInNum], &vertexBufIn[0], &vertexBufIn[1]);
        h2=len_ptol(&vertexIn[(0-i-1+vertexInNum)%vertexInNum], &vertexBufIn[0], &vertexBufIn[1]);
        //left
        zoom_point(&vec_y, h1/leny, &p1);
        zoom_point(&vec_y, h2/leny, &p4);
        add_point(&vertexBufIn[0], &p1, &p1);
        add_point(&vertexBufIn[0], &p4, &p4);
        p2=vertexIn[(0-i+vertexInNum)%vertexInNum];
        p3=vertexIn[(0-i-1+vertexInNum)%vertexInNum];
        reverse=isbot;
        if(i>tnum){//p4 p3 p2 p1
            TPOINT tp;
            tp=p1; p1=p4; p4=tp;
            tp=p2; p2=p3; p3=tp;
            reverse=1-reverse;
        }
        rnx=myround((euclid_distance(&p1, &p2)/lenx)*nx);
        rny=myround((len_ptol(&p1, &p3, &p4)/leny)*ny);
        EXAMINE_INTERFACE(&p1, &p2, &p3, &p4, rnx, rny, reverse, bid, 0)
        //right	
        zoom_point(&vec_y, h1/leny, &p2);
        zoom_point(&vec_y, h2/leny, &p3);
        add_point(&vertexBufIn[1], &p2, &p2);
        add_point(&vertexBufIn[1], &p3, &p3);
        p1=vertexIn[(1+i+vertexInNum)%vertexInNum];
        p4=vertexIn[(1+i+1+vertexInNum)%vertexInNum];
        reverse=isbot;
        if(i>tnum){//p4 p3 p2 p1
            TPOINT tp;
            tp=p1; p1=p4; p4=tp;
            tp=p2; p2=p3; p3=tp;
            reverse=1-reverse;
        }
        rnx=myround((euclid_distance(&p1, &p2)/lenx)*nx);
        rny=myround((len_ptol(&p1, &p3, &p4)/leny)*ny);
        EXAMINE_INTERFACE(&p1, &p2, &p3, &p4, rnx, rny, reverse, bid, 0)	
    }
    //bot-up-rec	
    h2=len_ptol(&vertexIn[(vertexNum[bid]-4)/2], &vertexBufIn[0], &vertexBufIn[1]);
    zoom_point(&vec_y, h2/leny, &p1);
    zoom_point(&vec_y, h2/leny, &p2);
    add_point(&p1, &vertexBufIn[0], &p1);
    add_point(&p2, &vertexBufIn[1], &p2);
    p3=vertexBufIn[2]; p4=vertexBufIn[3];
    rny=myround(((leny-h2)/leny)*ny);	
    reverse=isbot;
    EXAMINE_INTERFACE(&p1, &p2, &p3, &p4, nx, rny, reverse, bid, 1)
}
#endif
