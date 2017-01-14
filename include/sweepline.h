#ifndef _SWEEPLINE_H_
#define _SWEEPLINE_H_

#include "frwbcm.h"
#include "sdef.h"
typedef unsigned short ushort;

class CPnt3d //three dimension point
{
public:
    double x, y, z; // coordinate of the point x, y ,z;
    CPnt3d(){} //constuction funtion
    ~CPnt3d(){}//release function
    void operator=(const CPnt3d &oPnt) // overloading =
    {
        x=oPnt.x;
        y=oPnt.y;
        z=oPnt.z;
    }
    void mvSetX(const double d) //set the value of coordinate of x
    {
        x=d;
    }	void mvSetY(const double d) //set the value of coordinate of y
    {
        y=d;
    }	void mvSetZ(const double d) //set the value of coordinate of z
    {
        z=d;
    }
    double mdGetX() // get the value of coordinate of x
    {
        return x;
    }
    double mdGetY()//get the value of coordinate of y
    {
        return y;
    }
    double mdGetZ()// get the value of coordinate of z
    {
        return z;
    }
    bool operator==(const CPnt3d &o) // overload the operation == 
    {
        return (MbFltEq(x,o.x) && MbFltEq(y,o.y) && MbFltEq(z,o.z));
    }
    double mdGetCoordinate(int iAxis)	// get the value of coordinate  through  index
    {
        switch(iAxis)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: return 0;
        }
    }
};

typedef class CPnt3d * PPnt3d; // point of Cpnt3d

class PolyEdge // Edge of Poly
{
    public:
    ushort  musPolyNum; 
    double  mdPosition;
    CPnt3d  mNode1,  mNode2; // two points on the Edge.
    PolyEdge * mpNext; // Next point 

    PolyEdge() {}
    PolyEdge(ushort ,CPnt3d ,CPnt3d );
    bool IsSameEdge(PolyEdge& pe)
    {
    return (mNode1==pe.mNode1 && mNode2==pe.mNode2);
    }
    PolyEdge& operator= (PolyEdge& pe)
    {
    mNode1=pe.mNode1;
    mNode2=pe.mNode2;
    musPolyNum=pe.musPolyNum;
    mdPosition=pe.mdPosition;
    mpNext=pe.mpNext;
    }


};

class SweepLine 	
{
    public:
        double	mdPosition; // Position 
        PolyEdge   mStartEdgesHead;
        SweepLine  * mpNext; //next point

        SweepLine()
        {
            mStartEdgesHead.mpNext=NULL;
        }

        SweepLine(double dPosition,PolyEdge * opPolyEdge); // insert opPloyEdge ..
        void mvInsertEdge(PolyEdge * ); // insert into sweepline head
};

class SweepLineLst 	
{
    public:
        SweepLine mSweepLineHead; // definition SweepLst

        SweepLineLst()
        {
            mSweepLineHead.mpNext=NULL;
        }

        ~SweepLineLst()
        {
            mvDelSweepLineLst();
        }

        void mvDelSweepLineLst();
        void mvInsertSweepLine(double,PolyEdge * );
};


class Slab 	
{
    public:
        double	mdStartPosition, mdMidPosition, mdEndPosition;
        PolyEdge  mEdgeHead; // the first edge of the slab chain 

        Slab()
        {
            mEdgeHead.mpNext=NULL;
        }	
    
        ~Slab()
        {
            mvDelEdgeLst();
        }

        void mvDelEdgeLst();
        void mvProcess(int ,int ,SweepLine*);
    
};
                                                    
class SlabElement	//slab Element			//node of SlabElementLst 
{				
    public:
    PolyEdge moLeft,moRight;
    double mdStartPosition,mdEndPosition;
    SlabElement *mpNext;
    SlabElement(){mpNext=NULL;}
    SlabElement(PolyEdge &left,PolyEdge &right,double s,double e)
    {
        moLeft=left;
        moRight=right;
        mdStartPosition=s;
        mdEndPosition=e;
        mpNext=NULL;
    }
};

extern unsigned int guiSlabElementCount;

class SlabElementLst
{
    public:
    SlabElement moHead; //Head element of the SlabElementLst
    SlabElement *mpEnd; // Last element of the Slab ElementList
    void mvInsert(SlabElement *);
    void mvInsert(PolyEdge &,PolyEdge &, double,double);
    SlabElementLst(){mpEnd=&moHead;}
    ~SlabElementLst()
    {
        SlabElement *p1;
        p1=moHead.mpNext;
        while(p1!=NULL)
        {
            moHead.mpNext=p1->mpNext;
            delete p1;
            p1=moHead.mpNext;
        }
    }
};
#endif

