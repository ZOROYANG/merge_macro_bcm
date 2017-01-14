/*
    The header file containing the definition of the shareing structure.
    Any code inserted into this file must be completely C style.
*/
#ifndef _ELENODE_H_
#define _ELENODE_H_

#define ConstEle 0
#define max_src_pnt 10
#define max_poly 1
#define max_level 7 

extern ushort ** usElements,** usTempEle,**usTempEleCon;
extern int Conblk, Curblk,iDivideDef,iRefineLevel,iErrorFlag;

#define	XOY_RECTANGLE		1
#define	YOZ_RECTANGLE		2
#define	ZOX_RECTANGLE		3
#define XOY_PARALLELOGRAM	4 
#define YOX_PARALLELOGRAM	5 
#define XOZ_PARALLELOGRAM	6 
#define ZOX_PARALLELOGRAM	7 
#define YOZ_PARALLELOGRAM	8 
#define ZOY_PARALLELOGRAM	9 
#define	XOY_TRIANGLE		10
#define	YOX_TRIANGLE		11
#define	YOZ_TRIANGLE		12
#define	ZOY_TRIANGLE		13
#define	ZOX_TRIANGLE		14
#define	XOZ_TRIANGLE		15
#define	XOY_TRAPEZOID		16
#define	YOX_TRAPEZOID		17
#define	YOZ_TRAPEZOID		18
#define	ZOY_TRAPEZOID		19
#define	ZOX_TRAPEZOID		20
#define	XOZ_TRAPEZOID		21
#define	XOY_INCLINED_RECTANGLE	22
#define	YOZ_INCLINED_RECTANGLE	23
#define	ZOX_INCLINED_RECTANGLE	24
#define	XOY_INCLINED_TRAPEZOID	25
#define	YOZ_INCLINED_TRAPEZOID	26
#define	ZOX_INCLINED_TRAPEZOID	27
#define	ARBITARY_TRIANGLE	28
#define	ARBITARY_QUADRILATERAL	29

#define	LIMIT 			1e-8

#endif
// _ELENODE_HXX_
