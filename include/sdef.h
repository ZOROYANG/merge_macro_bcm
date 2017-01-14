/*---------------------------------------------------------
    Sdef.hxx defines the user types, global macros,
    and some macros and functions specially for
    debugging. For using COW, you needn't have to 
    explicitly include it in your program. Because
    it will has been included when you include any
    header file, which is the interface of COW's class.
---------------------------------------------------------*/

#ifndef _SDEF_H_
#define _SDEF_H_

#include <stdlib.h>

#if defined (MHostMachine_Intelx86) && defined (MHostSystem_MS_DOS)
#include <process.h>
#endif

#include <math.h> 
#include <iostream>

#define PERM_AIR 8.854187818
#define MAXNUM 99999999

#define MINCOOR -1e32
#define MAXCOOR  1e32

#define	MED_MED 	0 //Big_medium and Big_medium
#define MED_CON		1
#define	CON_CON		2
#define MED_LAYER	3
#define SMED_MED        4 
#define SMED_SMED       5
#define SMED_LAYER      6

#define MATCH		0
#define SURF1_IN_SURF2	1
#define SURF2_IN_SURF1	2
#define INTERSECTION	3
#define NOT_INTERSECT	4

#define ON_BORDER 	0
#define IN_POLYGON	1
#define NOT_IN_POLYGON -1

#define OUT  0  
#define IN   1
#define ON   2

#define CONDUCTOR_SURFACE -2
#define B3D_INVALID            0
#define LAYER_SURFACE      1
#define CON_AT_MED_INTERFACE 2
#define CON_CON_INTERFACE    3
#define MEDIUM_SURFACE       -1
#define MED_MED_INTERFACE    1
#define MED_HAS_HOLE         2
#define MED_HAS_MED_HOLE         6
#define MED_INTERFACE_HAS_CON_HOLE 3 
#define MED_INTERFACE_HAS_MED_HOLE 5 
#define LAYER_HAS_HOLE 4 
#define CON_PART_INVALID	7
#define B3D_INVALID_UNREAL	8	//added by lutt on 8/24/00

// added by houjs 
#define MAXPDDLBLOCKNUM 200
#define MAXELENO 32767

#define FALSE 0
#define true 1
typedef unsigned char uchar;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned short word;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef word ClassId;

typedef unsigned char BOOL;

#define CONDUCTOR_P 0
#define BOUNDARY  1
#define INTERFACE 2
#define NITER 100
#define TOL 0.001
const double cdEpsilon = 1e-10;
const double cdPie = 3.1415926535;

#define MbFltEq(f1,f2) (fabs((f1) - (f2)) < cdEpsilon)
#define MbFltGT(f1,f2) (((f1) - (f2) ) > cdEpsilon)
#define MbFltLT(f1,f2) (((f2) - (f1) ) > cdEpsilon)
#define MbFltGE(f1,f2) (((f1) - (f2) ) > - cdEpsilon)
#define MbFltLE(f1,f2) (((f2) - (f1) ) > - cdEpsilon)
#define MbFltEQ(f1,f2) (fabs((f1) - (f2)) < cdEpsilon)


#define MfLess(f1,f2) ((f1<f2)?f1:f2)
#define MfGreater(f1,f2) ((f1>f2)?f1:f2)
    //duyu added them
double dAcos ( double dCosVal );

// constants of the ralationships among geometry objects.

#if defined(CC) || defined(_AIX) || ( defined(__SUNPRO_CC_COMPAT) && (__SUNPRO_CC_COMPAT < 5) )
#if !defined(__LP64__)
#endif
#endif

const int ciParallel = 0;
const int ciIntersect = 1;
const int ciCoincide = 2;
const int ciDifferent = 3;
const int ciCoincidePartly = 4;
const int ciSuperset = 5;
const int ciSubset = 6;
const int ciinside = 7;
const int cionboundary = 8;
const int cioutside = 9;

#if defined (MHostMachine_Intelx86) && defined (MHostSystem_MS_DOS)
#define MInlineOn
#define MInlineOff
#else
#define MInlineOn
#define MInlineOff
#endif

/*  In release version, inline functions are allowed, and the
    diagnotis features even do NOT generate any machine code.
*/

#if defined (MHostMachine_Intelx86) && defined (MHostSystem_MS_DOS)
#define MInline
#else
#define MInline
#endif

#define MPassedHere NULL

void inline vDump ( const char*  /* ccpOutputFormat */, ... ) { }
#define MbAssertObjValid(oObj,cpPrompt) true
#define MbAssertObjValidExit(oObj,cpPrompt) true
#define MbAssert(zExp,cpPrompt) true
#define MbAssertExit(zExp,cpPrompt) true

// MShaneDebug

#endif  
// M_SDEF_H
