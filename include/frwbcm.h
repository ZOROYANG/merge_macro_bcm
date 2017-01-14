#ifndef _FRWBCM_H
#define _FRWBCM_H

#include <vector>
#include <cmath>
#include <string.h>	//strcat
#include <stdlib.h>
#include <stdio.h>
#include "shape.h"
#include <sys/times.h>
#include "matrixclass.h"
#include "Matrix_Sparse.h"
#include "Configure.h"
#include "FDMSolver.h"

using namespace std;

//transfer area type
#define NULL_TRANS_AREA			0
#define CUBE_TRANS_AREA 		1
#define PRISM_TRANS_AREA  		2	//yuwj. 3-prism, assumed normal prism
#define PRISM_TSV_TRANS_AREA 	3	//yuwj. for TSV prism, norm approx. circle
#define FRUSTUM_TRANS_AREA		4	//yuwj. for side tangent

#define MAX_BLOCK_NUM 10
#define MAX_COND_NUM 50
#define MAX_CHIP_NUM 50
#define MAX_RANGE_NUM 50
#define MAX_AXIS_NUM 3

struct range_t{//  begin<= id <end
    int begin;
    int end;
};
struct range_t *prange[MAX_BLOCK_NUM]={NULL};
int range_id[MAX_BLOCK_NUM]={0};
//cache plane info: rough element unit info
//for multi medium
struct PlaneInfo{
    TPOINT p1, p2, p3, p4;
    int nx, ny, rv, pnum;	
};
struct PlaneInfo *poly_in_interface;
int cur_poly_id=0;

vector<double> diag;
vector<double> _diag;
vector<vector<double>> segments;
vector<vector<double>> segments_x;
vector<vector<double>> segments_y;
vector<vector<double>> segments_z;
vector<double> high;
vector<int> ele_x;
vector<int> ele_y;
vector<int> ele_z;
size_t sindex;
vector<double> _pdf;
vector<double> _two_axis;
vector<double> _two_diag;
vector<double> _length;

TBDELEINFO **gBdElements;	//[block_id][id]
vector<vector<TBDELEINFO> > gBdElementsbcm;
vector<TBDELEINFO> BdElementsbcm;
vector<TBDELEINFO> BdElementInter;
vector<vector<double> > boundaryposition(3);

TGEOMETRYINFO *tempblock2;	//to save every block axis info
vector<int> BdElementInterSize(MAX_BLOCK_NUM,0);
vector<BLOCKINFO> blocks;
extern vector<CONDUCTORINFO> conductors;	//to save every block axis info

vector<vector<DPLANE> > dblocks; 
vector<vector<DPLANE> > dconductors;
vector<DPLANE> dinterfaces;

//double ***matrix_G;
double **matrix_G;
double **matrix_H;
double **vector_g;
double **vector_h;
double **matrix_w;
double ***matrix_A;
double **matrix_D;
double ***matrix_AC;
//double **matrix_X;
double *matrix_X;
double *final_res;
Matrix matrixbcm;
Matrix boundary_position;
std::vector<std::vector<double>> Areas;
int trans_area_type[MAX_BLOCK_NUM] = {NULL_TRANS_AREA};	//transfer region type mode
int cur_ele_id[MAX_BLOCK_NUM]={0};
TPOINT *vertexBuf[MAX_BLOCK_NUM]={NULL};		//bottom 3d point
TPOINT *vertexTopBuf[MAX_BLOCK_NUM]={NULL};	//top 3d point
TPOINT *vertexBufCon[MAX_COND_NUM]={NULL};		//bottom 3d point
TPOINT *vertexTopBufCon[MAX_COND_NUM]={NULL};	//top 3d point
int vertexNum[MAX_BLOCK_NUM] = {0};
int pointInMediumId = 0;
//for multi medium  to solve equation
//int *g_indx[MAX_BLOCK_NUM+1]={NULL};	//the third for X_T
lapack_int *g_indx[MAX_BLOCK_NUM+1]={NULL};	//the third for X_T
//interface start id in gBdElements[i]
int interface_start_id[MAX_BLOCK_NUM]={0};
//vector vertical interface 
TVECTOR gvNormal;
double normalLen=0.0;
double xglobal;
double yglobal;
vector<int> condp;

//to compute wvt
bool needWeight = true;
extern bool multiCal;
double *vector_wg[MAX_AXIS_NUM];
double *vector_tg[MAX_AXIS_NUM*2];
double *vector_wh[MAX_AXIS_NUM];
double *vector_th[MAX_AXIS_NUM*2];
TPOINT pointForW[MAX_AXIS_NUM*2];
double *final_wvt[MAX_AXIS_NUM];

//to compute using FDM
bool useFDM = false;

#define	LIMIT 			1e-8
#define	LIMIT2 			1e-12
#define TINY 			1.0e-20
#define STEP_H 			0.005

#define START_RANGE(bid) 	range_id[bid]=0; \
                            prange[bid]=new range_t[MAX_RANGE_NUM]; \
                            prange[bid][range_id[bid]].begin=cur_ele_id[bid];
                
#define UPDATE_RANGE(bid) 	prange[bid][range_id[bid]].end=cur_ele_id[bid]; \
                            prange[bid][++range_id[bid]].begin=cur_ele_id[bid];
                
#define END_RANGE(bid)		prange[bid][range_id[bid]].end=cur_ele_id[bid];
#define END_RANGE2(bid)		range_id[bid] --;
                
#define EXAMINE_INTERFACE(ptr1, ptr2, ptr3, ptr4, inx, iny, irv, ibid, upd)	\
        if(Curmed == 2 && in_interface(ptr1, ptr2, ptr3, ptr4)){			\
            if(bid == 0){													\
                poly_in_interface[cur_poly_id].p1=*ptr1;					\
                poly_in_interface[cur_poly_id].p2=*ptr2;					\
                poly_in_interface[cur_poly_id].p3=*ptr3;					\
                poly_in_interface[cur_poly_id].p4=*ptr4;					\
                poly_in_interface[cur_poly_id].nx=inx;						\
                poly_in_interface[cur_poly_id].ny=iny;						\
                poly_in_interface[cur_poly_id].rv=irv;						\
                poly_in_interface[cur_poly_id].pnum=4;						\
                cur_poly_id ++;												\
            }																\
        }																	\
        else{																\
            divide_quadrangle(ptr1, ptr2, ptr3, ptr4, inx, iny, irv, ibid,false); \
            if(upd>0){														\
                UPDATE_RANGE(ibid)											\
            }																\
        }
#define EXAMINE_INTERFACE2(ptr1, ptr2, ptr3, inx, iny, irv, ibid, upd)	\
        if(Curmed == 2 && in_interface(ptr1, ptr2, ptr3, NULL)){	\
            if(bid == 0){											\
                poly_in_interface[cur_poly_id].p1=*ptr1;			\
                poly_in_interface[cur_poly_id].p2=*ptr2;			\
                poly_in_interface[cur_poly_id].p3=*ptr3;			\
                poly_in_interface[cur_poly_id].nx=inx;				\
                poly_in_interface[cur_poly_id].ny=iny;				\
                poly_in_interface[cur_poly_id].rv=irv;				\
                poly_in_interface[cur_poly_id].pnum=3;				\
                cur_poly_id ++;										\
            } 														\
        }															\
        else{														\
            divide_triangle(ptr1, ptr2, ptr3, inx, iny, irv, ibid); \
            if(upd>0){ 											\
                UPDATE_RANGE(ibid)									\
            }														\
        }

extern int Curblk;
extern uint Curmed;
extern ushort ** usTempEle;
extern ushort ** usTempEleCon;
extern TPOINT observePoint;
extern int block_number;
extern int cond_number;
extern bool cohesion;
extern std::string filename;
extern double sum0, sum1;
extern TPOINT2D *pddlPoint2DBuf;
extern WORD pddlPoint2DNum;
extern TPOINT *pddlOutLoopPoint3DBuf;
extern WORD   pddlOutLoopPoint3DNum;
//interface
extern TPOINT *pddlInterfacePoint3DBuf;
extern TPOINT **pddlInterfacePoint3DBufMul;
extern WORD   pddlInterfacePoint3DNum;
//diel
extern double *tempelec;

extern void New_Integration(int type,double	P[3],double X[4],double Y[4],double Z[4]);
extern double tri(double X[3],double Y[3],double Z[3]);
extern double quadbak(double P[3],double X[4],double Y[4],double Z[4]);
extern void gauss();
extern void MatrixGen();
extern void MatrixGen_More();

extern double distance(double x,double y,double z,double X[4],double Y[4],double Z[4]);

#endif
