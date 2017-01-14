#ifndef _SHAPE_H__
#define _SHAPE_H__

#include <stdio.h>
#include <fstream>
#include <memory.h>
#include <malloc.h>
#include <math.h>
#include "sdef.h"
#include "elenode.h"
#include <vector>
using namespace std;
typedef unsigned char BYTE;
typedef unsigned int  WORD;

const double ARUCARY = 0.5E-8;
#define Equal(x,y)	(fabs((x)-(y))<ARUCARY)

typedef struct tagPOINT2D {
    double x,y;
} TPOINT2D,TVECTOR2D;

typedef TPOINT2D *  TPPOINT2D;

typedef struct tagPOINT {
    double x,y,z;
} TPOINT,TVECTOR;
typedef TVECTOR * TPVECTOR;
typedef TPOINT  * TPPOINT;

typedef struct tagBOUNDELEINFO {
    WORD mask;
    long xAxis;
    long yAxis;
    long zAxis;
} TBOUNDELEINFO,* TPBOUNDELEINFO;

typedef struct tagBOUNDELEINFO2 {
    WORD mask;
    long Axis1;
    long Axis2;
}TBOUNDELEINFO2,* TPBOUNDELEINFO2;


typedef struct tagCOORDINATION {
    TPOINT basePoint;
    TVECTOR xAxis;
    TVECTOR yAxis;
    TVECTOR zAxis;
} TCOORDINATION,* TPCOORDINATION;

typedef TCOORDINATION TGEOMETRYINFO;
typedef TGEOMETRYINFO * TPGEOMETRYINFO;

class CONDUCTORINFO { 
public:
    int layerNum = MAXNUM; //add by [zhangbl]
    int blockNum = MAXNUM;
    TGEOMETRYINFO geo;
    WORD xEle;
    WORD yEle;
    WORD zEle;
    TPOINT botP[4];
    TPOINT topP[4];
};	
typedef CONDUCTORINFO BLOCKINFO;

typedef struct tagPORE {
    TPOINT vertex[4];
    WORD xEle;
    WORD yEle;
    int layerid = MAXNUM; // 
    int conductorid = MAXNUM;
}PORE,SURFACE;

typedef struct tagDPLANE{
    SURFACE windows; // for interface 
    vector<PORE> pores; // conductor.
    bool inverse = false;
    bool isempty = false;
    bool isinterface = false;;
}DPLANE;

typedef struct tagTPGEOMETRYINFO2 {
    TPOINT basePoint;
    TVECTOR xAxis;
    TVECTOR yAxis;
} TGEOMETRYINFO2 ,* TPGEOMETRYINFO2;

typedef struct tagELECTRICITYINFO {
    WORD mask;
    double voltVal;
    double dielVal;
    double chrgVal;
} TELECTRICITYINFO,* TPELECTRICITYINFO;

#define ENABLE_VOLT 1
#define ENABLE_DIEL 2
#define ENABLE_CHRG 4
#define ENABLE_DIEL_AND_CHRG 6
#define ENABLE_NONE_VALUE    0

#define ENABLE_NONE 0
#define ENABLE_2D 1
#define ENABLE_3D 2
#define ENABLE_4P_EX 4

typedef struct tagBLOCK {
    TGEOMETRYINFO 		blkGeomInfo;
    TELECTRICITYINFO 	blkElecInfo;
    TBOUNDELEINFO 		blkBoundEleInfo;
} TBLOCK,* TPBLOCK;

typedef struct tagSPHERE {
    TGEOMETRYINFO 		sphGeomInfo;
    TELECTRICITYINFO 	sphElecInfo;
    TBOUNDELEINFO 		sphBoundEleInfo;
} TSPHERE,* TPSPHERE;

typedef struct tagCYLINDER {
    TGEOMETRYINFO 		cylGeomInfo;
    TELECTRICITYINFO 	cylElecInfo;
    TBOUNDELEINFO 		cylBoundEleInfo;
    double                  cylScale;
} TCYLINDER,* TPCYLINDER;


#define ENABLE_XAXIS 1
#define ENABLE_YAXIS 2
#define ENABLE_ZAXIS 4
#define ENABLE_ID_4  8

typedef struct tagBOUNDELEINFOEX {
    WORD  mask;
    long xAxis;
    long yAxis;
    long zAxis;
    int  nPointID[4];
} TBOUNDELEINFOEX,* TPBOUNDELEINFOEX;


typedef enum tagLOOPTYPE {
    IN_LOOP,
    OUT_LOOP
} LOOPTYPE;
typedef LOOPTYPE loopType;

typedef struct tagPOLY {
    TGEOMETRYINFO 		plyGeomInfo;
    TELECTRICITYINFO 	plyElecInfo;
    double                  plyScale;
    int                     plyPointNum;
    TPPOINT2D		plyFloorPoints;
    int 			plyBoundEleInfoNum;
    TPBOUNDELEINFOEX        plyBoundEleInfoEx;
    TBOUNDELEINFO 		plyBoundEleInfo;
} TPOLY,* TPPOLY;

typedef struct tagPLANE {

    TGEOMETRYINFO2            plnGeomInfo; 
    TELECTRICITYINFO          plnElecInfo;
    TBOUNDELEINFO             plnBoundEleInfo;
    WORD                      plnInLoopNum;
    int                       plnOutLoopPointNum;
    TPPOINT2D                 plnOutLoopPoints;
    TPOINT2D                  **plnInLoopPoints;
    int                       *plnInLoopPointNum;


} TPLANE,* TPPLANE;

typedef struct tagCIRPLANE {
    TGEOMETRYINFO2            crpGeomInfo; 
    TELECTRICITYINFO          crpElecInfo;
    TBOUNDELEINFO             crpBoundEleInfo;

} TCIRPLANE, *TPCIRPLANE; 

typedef struct tagROTPLANE {
    TGEOMETRYINFO            rtpGeomInfo; 
    TELECTRICITYINFO          rtpElecInfo;
    TBOUNDELEINFO             rtpBoundEleInfo;

} TROTPLANE, *TPROTPLANE; 

#ifndef FALSE
#define FALSE (int)0
#endif
#ifndef TRUE
#define TRUE  (int)1  
#endif

typedef unsigned int SHAPEID;

typedef union tagHSHAPE {
    TPBLOCK pBlock;
    TPSPHERE pSphere;
    TPCYLINDER pCylinder;
    TPPOLY    pPoly;
    TPPLANE   pPlane;
    TPCIRPLANE pCirplane;
    TPROTPLANE pRotplane;
} HSHAPE;

typedef enum tagSHAPETYPE  {
    PDDL_BLOCK,
    PDDL_SPHERE,
    PDDL_CYLINDER,
    PDDL_POLY, 
    PDDL_PLANE,
    PDDL_CIRPLANE,
    PDDL_ROTPLANE,
    PDDL_SPRPLANE,
    PDDL_LAYER,     
    PDDL_CONDUCTOR, 
    PDDL_MEDIUM, 
    PDDL_MAX=PDDL_MEDIUM,   
    PDDL_NONE=0xffff     
} SHAPETYPE;

typedef struct tagSHAPE {
    SHAPETYPE shpType;
    SHAPEID   shpID;
    HSHAPE    hShape;
} TSHAPE,* TPSHAPE;

typedef struct tagTELECOBJECT {
        SHAPETYPE objType;
        WORD objShapeNums;
        WORD objShapeArray[1];
} TELECOBJECT,* TPELECOBJECT;

//add by [whquan]
typedef struct tagBdEleInfo{
    TPOINT leftdown;
    TPOINT rightdown;
    TPOINT rightup;
    TPOINT leftup;
    TPOINT cfg;	
    int type;	//XOY_RECTANGLE ? OR other
}TBDELEINFO,* TPBDELEINFO;

struct Var
{
char name[32];
int type;
int tno;
int blkno;
char layer;
}; 
typedef struct tagRange{
    size_t start = MAXNUM;
    size_t end = MAXNUM;
    size_t insert = MAXNUM;
    int layerid = MAXNUM;
    int conductorid = MAXNUM;
}RANGE;
#endif 
//end of _H_SHAPE__
