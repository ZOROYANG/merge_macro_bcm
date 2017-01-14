#ifndef _BOUNDARYTYPE_H
#define _BOUNDARYTYPE_H
#include "frwbcm.h"

/*
* element type
*/
int Element_Type_new(TPOINT* p1, TPOINT* p2, TPOINT* p3, TPOINT* p4, int pnum)
{
    double X[4], Y[4], Z[4];
    X[0]=p1->x; 	Y[0]=p1->y; 	Z[0]=p1->z;
    X[1]=p2->x; 	Y[1]=p2->y; 	Z[1]=p2->z;
    X[2]=p3->x; 	Y[2]=p3->y; 	Z[2]=p3->z;
    if(pnum>3){
        X[3]=p4->x; 	Y[3]=p4->y; 	Z[3]=p4->z;
    }
    
    if(pnum==3)     // Triangle element  
    {
        if(fabs(X[1]-X[0])<LIMIT&&fabs(X[2]-X[0])<LIMIT)
        {
            if(fabs(Y[1]-Y[0])<LIMIT||fabs(Y[1]-Y[2])<LIMIT||fabs(Y[2]-Y[0])<LIMIT)
            {
                return ZOY_TRIANGLE; 
            }
            else if(fabs(Z[1]-Z[0])<LIMIT||fabs(Z[1]-Z[2])<LIMIT||fabs(Z[2]-Z[0])<LIMIT)
            {
                return YOZ_TRIANGLE; 
            }
            else
            {
                return ARBITARY_TRIANGLE; 
            }
        }
        else if(fabs(Y[1]-Y[0])<LIMIT&&fabs(Y[2]-Y[0])<LIMIT)
        {
            if(fabs(Z[1]-Z[0])<LIMIT||fabs(Z[1]-Z[2])<LIMIT||fabs(Z[2]-Z[0])<LIMIT)
            {
                return XOZ_TRIANGLE; 
            }
            else if(fabs(X[1]-X[0])<LIMIT||fabs(X[1]-X[2])<LIMIT||fabs(X[2]-X[0])<LIMIT)
            {
                return ZOX_TRIANGLE; 
            }
            else
            {
                return ARBITARY_TRIANGLE; 
            }
        }
        else if(fabs(Z[1]-Z[0])<LIMIT&&fabs(Z[2]-Z[0])<LIMIT)
        {
            if(fabs(X[1]-X[0])<LIMIT||fabs(X[1]-X[2])<LIMIT||fabs(X[2]-X[0])<LIMIT)
            {
                return YOX_TRIANGLE; 
            }
            else if(fabs(Y[1]-Y[0])<LIMIT||fabs(Y[1]-Y[2])<LIMIT||fabs(Y[2]-Y[0])<LIMIT)
            {
                return XOY_TRIANGLE; 
            }
            else
            {
                return ARBITARY_TRIANGLE; 
            }
        }
        else
        {
            return ARBITARY_TRIANGLE; 
        }
    }
    else 	// Quadrilateral elements 
    {
        if(fabs(Z[1]-Z[0])<LIMIT&&fabs(Z[3]-Z[0])<LIMIT)
        {
            if(fabs(X[1]-X[0])<LIMIT&&fabs(X[3]-X[2])<LIMIT)
            {
                if(fabs(Y[3]-Y[0])<LIMIT&&fabs(Y[2]-Y[1])<LIMIT)
                {
                    return XOY_RECTANGLE; 
                }
                else if(fabs(Y[3]-Y[0]-Y[2]+Y[1])<LIMIT)
                {
                    return YOX_PARALLELOGRAM;
                }
                else
                {
                    return YOX_TRAPEZOID; 
                }
            }
            else if(fabs(X[3]-X[0])<LIMIT&&fabs(X[1]-X[2])<LIMIT)
            {
                if(fabs(Y[1]-Y[0])<LIMIT&&fabs(Y[2]-Y[3])<LIMIT)
                {
                    return XOY_RECTANGLE; 
                }
                else if(fabs(Y[3]-Y[0]-Y[2]+Y[1])<LIMIT)
                {
                    return YOX_PARALLELOGRAM;
                }
                else
                {
                    return YOX_TRAPEZOID; 
                }
            }
            else if(fabs(Y[1]-Y[0])<LIMIT&&fabs(Y[3]-Y[2])<LIMIT)
            {
                if(fabs(X[3]-X[0]-X[2]+X[1])<LIMIT)
                {
                    return XOY_PARALLELOGRAM;
                }
                else
                {
                    return XOY_TRAPEZOID; 
                }
            }
            else if(fabs(Y[3]-Y[0])<LIMIT&&fabs(Y[1]-Y[2])<LIMIT)
            {
                if(fabs(X[3]-X[0]-X[2]+X[1])<LIMIT)
                {
                    return XOY_PARALLELOGRAM;
                }
                else
                {
                    return XOY_TRAPEZOID; 
                }
            }
            else
            {
                return ARBITARY_QUADRILATERAL; 
            }
        }
        else if(fabs(X[1]-X[0])<LIMIT&&fabs(X[3]-X[0])<LIMIT)
        {
            if(fabs(Y[1]-Y[0])<LIMIT&&fabs(Y[3]-Y[2])<LIMIT)
            {
                if(fabs(Z[3]-Z[0])<LIMIT&&fabs(Z[2]-Z[1])<LIMIT)
                {
                    return YOZ_RECTANGLE; 
                }
                else if(fabs(Z[3]-Z[0]-Z[2]+Z[1])<LIMIT)
                {
                    return ZOY_PARALLELOGRAM;
                }
                else
                {
                    return ZOY_TRAPEZOID; 
                }
            }
            else if(fabs(Y[3]-Y[0])<LIMIT&&fabs(Y[1]-Y[2])<LIMIT)
            {
                if(fabs(Z[1]-Z[0])<LIMIT&&fabs(Z[2]-Z[3])<LIMIT)
                {
                    return YOZ_RECTANGLE; 
                }
                else if(fabs(Y[3]-Y[0]-Y[2]+Y[1])<LIMIT)
                {
                    return ZOY_PARALLELOGRAM;
                }
                else
                {
                    return ZOY_TRAPEZOID; 
                }
            }
            else if(fabs(Z[1]-Z[0])<LIMIT&&fabs(Z[3]-Z[2])<LIMIT)
            {
                if(fabs(Y[3]-Y[0]-Y[2]+Y[1])<LIMIT)
                {
                    return YOZ_PARALLELOGRAM;
                }
                else
                {
                    return YOZ_TRAPEZOID; 
                }
            }
            else if(fabs(Z[3]-Z[0])<LIMIT&&fabs(Z[1]-Z[2])<LIMIT)
            {
                if(fabs(Y[3]-Y[0]-Y[2]+Y[1])<LIMIT)
                {
                    return YOZ_PARALLELOGRAM;
                }
                else
                {
                    return YOZ_TRAPEZOID; 
                }
            }
            else
            {
                return ARBITARY_QUADRILATERAL; 
            }
        }
        else if(fabs(Y[1]-Y[0])<LIMIT&&fabs(Y[3]-Y[0])<LIMIT)
        {
            if(fabs(Z[1]-Z[0])<LIMIT&&fabs(Z[3]-Z[2])<LIMIT)
            {
                if(fabs(X[3]-X[0])<LIMIT&&fabs(X[2]-X[1])<LIMIT)
                {
                    return ZOX_RECTANGLE; 
                }
                else if(fabs(X[3]-X[0]-X[2]+X[1])<LIMIT)
                {
                    return XOZ_PARALLELOGRAM;
                }
                else
                {
                    return XOZ_TRAPEZOID; 
                }
            }
            else if(fabs(Z[3]-Z[0])<LIMIT&&fabs(Z[1]-Z[2])<LIMIT)
            {
                if(fabs(X[1]-X[0])<LIMIT&&fabs(X[2]-X[3])<LIMIT)
                {
                    return ZOX_RECTANGLE; 
                }
                else if(fabs(Z[3]-Z[0]-Z[2]+Z[1])<LIMIT)
                {
                    return XOZ_PARALLELOGRAM;
                }
                else
                {
                    return XOZ_TRAPEZOID; 
                }
            }
            else if(fabs(X[1]-X[0])<LIMIT&&fabs(X[3]-X[2])<LIMIT)
            {
                if(fabs(Z[3]-Z[0]-Z[2]+Z[1])<LIMIT)
                {
                    return ZOX_PARALLELOGRAM;
                }
                else
                {
                    return ZOX_TRAPEZOID; 
                }
            }
            else if(fabs(X[3]-X[0])<LIMIT&&fabs(X[1]-X[2])<LIMIT)
            {
                if(fabs(Z[3]-Z[0]-Z[2]+Z[1])<LIMIT)
                {
                    return ZOX_PARALLELOGRAM;
                }
                else
                {
                    return ZOX_TRAPEZOID; 
                }
            }
            else
            {
                return ARBITARY_QUADRILATERAL; 
            }
        }
        else 	// Inclined quadrilateral elements 
        {
            if(fabs(Y[1]-Y[0])<LIMIT&&fabs(Z[1]-Z[0])<LIMIT&&fabs(Y[3]-Y[2])<LIMIT&&fabs(Z[3]-Z[2])<LIMIT)
                if(fabs(X[2]-X[1])<LIMIT&&fabs(X[3]-X[0])<LIMIT)
                {
                    return XOY_INCLINED_RECTANGLE; 
                }
                else
                {
                    return XOY_INCLINED_TRAPEZOID; 
                }
            else if(fabs(Y[3]-Y[0])<LIMIT&&fabs(Z[3]-Z[0])<LIMIT&&fabs(Y[1]-Y[2])<LIMIT&&fabs(Z[1]-Z[2])<LIMIT)
                if(fabs(X[0]-X[1])<LIMIT&&fabs(X[2]-X[3])<LIMIT)
                {
                    return XOY_INCLINED_RECTANGLE; 
                }
                else
                {
                    return XOY_INCLINED_TRAPEZOID; 
                }
            else if(fabs(Z[1]-Z[0])<LIMIT&&fabs(X[1]-X[0])<LIMIT&&fabs(Z[3]-Z[2])<LIMIT&&fabs(X[3]-X[2])<LIMIT)
                if(fabs(Y[2]-Y[1])<LIMIT&&fabs(Y[3]-Y[0])<LIMIT)
                {
                    return YOZ_INCLINED_RECTANGLE; 
                }
                else
                {
                    return YOZ_INCLINED_TRAPEZOID; 
                }
            else if(fabs(Z[3]-Z[0])<LIMIT&&fabs(X[3]-X[0])<LIMIT&&fabs(Z[1]-Z[2])<LIMIT&&fabs(X[1]-X[2])<LIMIT)
                if(fabs(Y[0]-Y[1])<LIMIT&&fabs(Y[2]-Y[3])<LIMIT)
                {
                    return YOZ_INCLINED_RECTANGLE; 
                }
                else
                {
                    return YOZ_INCLINED_TRAPEZOID; 
                }
            else if(fabs(X[1]-X[0])<LIMIT&&fabs(Y[1]-Y[0])<LIMIT&&fabs(X[3]-X[2])<LIMIT&&fabs(Y[3]-Y[2])<LIMIT)
                if(fabs(Z[2]-Z[1])<LIMIT&&fabs(Z[3]-Z[0])<LIMIT)
                {
                    return ZOX_INCLINED_RECTANGLE; 
                }
                else
                {
                    return ZOX_INCLINED_TRAPEZOID; 
                }
            else if(fabs(X[3]-X[0])<LIMIT&&fabs(Y[3]-Y[0])<LIMIT&&fabs(X[1]-X[2])<LIMIT&&fabs(Y[1]-Y[2])<LIMIT)
                if(fabs(Z[0]-Z[1])<LIMIT&&fabs(Z[2]-Z[3])<LIMIT)
                {
                    return ZOX_INCLINED_RECTANGLE; 
                }
                else
                {
                    return ZOX_INCLINED_TRAPEZOID; 
                }
            else
            {
                return ARBITARY_QUADRILATERAL; 
            }
        }
    }
}
#endif
