#ifndef _CSWEEPLINE_H_
#define _CSWEEPLINE_H_

#include "sweepline.h"
#include "frwbcm.h"
#include "sdef.h"
#include "boundarytype.h"
#include "quadrangle.h"

SweepLine::SweepLine(double dPosition,PolyEdge * opPolyEdge)
{
    mdPosition=dPosition;
    mStartEdgesHead.mpNext=opPolyEdge;
    if(opPolyEdge!=NULL)
        opPolyEdge->mpNext=NULL;
}

PolyEdge:: PolyEdge(ushort usPolyNum, CPnt3d  oNode1, CPnt3d 
    oNode2)
{
    musPolyNum= usPolyNum;
    mNode1= oNode1;
    mNode2= oNode2;
}

void SweepLine:: mvInsertEdge(PolyEdge * opEdge)// insert into the sweepline head
{
    if(opEdge != NULL)
    {
        opEdge->mpNext= mStartEdgesHead.mpNext;
        mStartEdgesHead.mpNext= opEdge;
    }
}

void SweepLineLst:: mvDelSweepLineLst() 
{
    SweepLine * opCurrSweepLine;

    opCurrSweepLine= mSweepLineHead.mpNext;
    while(opCurrSweepLine != NULL)
    {
        mSweepLineHead.mpNext= opCurrSweepLine->mpNext;
        delete opCurrSweepLine;
        opCurrSweepLine= mSweepLineHead.mpNext;
    }
}

void SweepLineLst:: mvInsertSweepLine(double dPosition, PolyEdge * opPolyEdge)
{
    SweepLine * opPrevSweepLine, * opCurrSweepLine; // PrevSweepLine and CurrSweepLine

    opPrevSweepLine= &mSweepLineHead;
    opCurrSweepLine= opPrevSweepLine->mpNext;

    while(opCurrSweepLine != NULL && MbFltGT(dPosition, opCurrSweepLine->mdPosition))
    {//dPosition is bigger than Y Coordinate of CurrSweepLine 
        opPrevSweepLine= opCurrSweepLine;
        opCurrSweepLine= opCurrSweepLine->mpNext;
    }//find opCurrSweepLine->mdPostion >= dPostion
    if(opCurrSweepLine == NULL)		//dPosition bigger than each one of list, append to the tailor
    {
        opCurrSweepLine= new SweepLine(dPosition, opPolyEdge);
        opCurrSweepLine->mpNext= NULL;
        opPrevSweepLine->mpNext= opCurrSweepLine;
    }
    else if(MbFltEq(dPosition, opCurrSweepLine->mdPosition))
        opCurrSweepLine->mvInsertEdge(opPolyEdge);	// insert to the head of opCurSweepLine
    else
    //dPosition < opCurrSweepLine, 
    //PrevSweep---->opPolyEdge---------->former Cur, insert before opCurrSweep
    {
        opCurrSweepLine= new SweepLine(dPosition, opPolyEdge);
        opCurrSweepLine->mpNext= opPrevSweepLine->mpNext;
        opPrevSweepLine->mpNext= opCurrSweepLine;
    }

}

int guiInterEleNum;
SlabElementLst *gopSlabElementLst;

void Slab:: mvDelEdgeLst() 
{
    PolyEdge * opCurrEdge;

    opCurrEdge= mEdgeHead.mpNext;
    while(opCurrEdge != NULL)
    {
        mEdgeHead.mpNext= opCurrEdge->mpNext;
        delete opCurrEdge;
        opCurrEdge= mEdgeHead.mpNext;
    }
}

void SlabElementLst::mvInsert(SlabElement *se)
{
    mpEnd->mpNext=se;
    mpEnd=se;
}

void SlabElementLst::mvInsert(PolyEdge &left,PolyEdge &right,double s,double e)
{
    if(left.IsSameEdge(mpEnd->moLeft) && right.IsSameEdge(mpEnd->moRight) && (MbFltEQ(s,mpEnd->mdEndPosition)))
    {
        mpEnd->mdEndPosition=e;
    }
    else
    {
        mvInsert(new SlabElement(left,right,s,e));
    }
}

void Slab:: mvProcess(int iAxis1, int iAxis2, SweepLine * opSweepLine)
{
    double dBottCoor1, dTopCoor1, dBottCoor2, dTopCoor2, dPosition;
    PolyEdge * opPrevEdge, * opCurrEdge, * opNextEdge, * opPrevStartEdge, * opCurrStartEdge;
    // Pre Cur, Next PrevS CurS

    //The slab is between this sweepline and the next one.
    mdStartPosition= opSweepLine->mdPosition; // Start Position oPSweepLine 
    mdEndPosition= opSweepLine->mpNext->mdPosition; // 
    mdMidPosition= (mdStartPosition +mdEndPosition) / 2.0;

    opPrevEdge= &mEdgeHead; // Pre
    opCurrEdge= opPrevEdge->mpNext; // Curr

    //modified the edges remained from the previous slab
    while(opCurrEdge != NULL)
    {
        if(MbFltEq(mdStartPosition, opCurrEdge->mNode2.mdGetCoordinate(iAxis1)))
        {
            opPrevEdge->mpNext= opCurrEdge->mpNext;
            delete opCurrEdge;
            opCurrEdge= opPrevEdge->mpNext;
        }
        else
        {
            dBottCoor1= opCurrEdge->mNode1.mdGetCoordinate(iAxis1);
            dTopCoor1= opCurrEdge->mNode2.mdGetCoordinate(iAxis1);
            dBottCoor2= opCurrEdge->mNode1.mdGetCoordinate(iAxis2);
            dTopCoor2= opCurrEdge->mNode2.mdGetCoordinate(iAxis2);
            dPosition= dBottCoor2 +(dTopCoor2 -dBottCoor2) / (dTopCoor1 -dBottCoor1) *(mdMidPosition -dBottCoor1);
            opCurrEdge->mdPosition= dPosition; // CurrPosition 

            opPrevEdge= opCurrEdge;
            opCurrEdge= opCurrEdge->mpNext;
        }
    }
    // after this circle opCurrEdge is NULL

    //get new edges from the sweepline
    opPrevStartEdge= &(opSweepLine->mStartEdgesHead); 
    opCurrStartEdge= opPrevStartEdge->mpNext;
    while(opCurrStartEdge != NULL)
    {
        dBottCoor1= opCurrStartEdge->mNode1.mdGetCoordinate(iAxis1);
        dTopCoor1= opCurrStartEdge->mNode2.mdGetCoordinate(iAxis1);
        dBottCoor2= opCurrStartEdge->mNode1.mdGetCoordinate(iAxis2);
        dTopCoor2= opCurrStartEdge->mNode2.mdGetCoordinate(iAxis2);
        dPosition= dBottCoor2 +(dTopCoor2 -dBottCoor2) / (dTopCoor1 -dBottCoor1) *(mdMidPosition -dBottCoor1);
        opCurrStartEdge->mdPosition= dPosition;

        opPrevEdge= &mEdgeHead;
        opCurrEdge= opPrevEdge->mpNext;

        while(opCurrEdge != NULL && MbFltGT(dPosition , opCurrEdge->mdPosition))
        {
            opPrevEdge= opCurrEdge;
            opCurrEdge= opCurrEdge->mpNext;
        } // opPrevEdge is Null or dPosition and opCurrEdge is equal.

        opPrevStartEdge->mpNext= opCurrStartEdge->mpNext;

        opCurrStartEdge->mpNext= opCurrEdge;
        opPrevEdge->mpNext= opCurrStartEdge;
        opCurrStartEdge= opPrevStartEdge->mpNext;
    }
    opSweepLine->mStartEdgesHead.mpNext= NULL;

    //Now, scan the edges in the current slab, merge the small Slabs which can be merged

    opNextEdge=mEdgeHead.mpNext;
    bool interfaceslab=true;
    while((opCurrEdge= opNextEdge) != NULL && (opNextEdge= opCurrEdge->mpNext) != NULL)
    {
        if(!MbFltEq(opCurrEdge->mdPosition, opNextEdge->mdPosition))
        {
            //two conductors are not touching each other
            int pn;
            pn=opNextEdge->musPolyNum;
            if(interfaceslab)
                gopSlabElementLst[pn].mvInsert(*opCurrEdge,*opNextEdge,mdStartPosition,mdEndPosition);
            
        }
        interfaceslab=!interfaceslab;
    }

}

double myinterpolate(double y1,double y2,double y3,double x1,double x3)
{
    return x1+((x1-x3)/(y1-y3))*(y2-y1);
}
void mothervertex(DPLANE &plane, double xlu, double xld,double xru,double xrd, double s, double e, double z,int x, int y);

void printpoint1(const TPOINT a){
    std::cout << a.x << " " << a.y << " " << a.z << std::endl;
}
void printplane1(const TPOINT *a,int n){
    for(int i = 0 ; i< n; i++){
        printpoint(a[i]);
    }
}
void printmotherele(const DPLANE &motherele){
//	std::cout << " Mother  ele ..................i" << std::endl;
//	printplane1(motherele.windows.vertex,4);
//	std::cout << motherele.windows.xEle << " " << motherele.windows.yEle << std::endl;
    for(int i = 0 ; i <motherele.pores.size();i++){
//		printplane(motherele.pores[i].vertex,4); 
    }
    //std::cout << " Mother  ele .......*****......i" << std::endl;
}
void boundaryInterface(DPLANE &plane,int index)
{	
//	std::cout << "interface " << std::endl;
    //printmotherele(	plane);
    
    gopSlabElementLst=new SlabElementLst[plane.pores.size()+1];

    guiInterEleNum=0;
    
    CPnt3d opnt1,opnt2;
    double pa1,pa2,pb1,pb2;//pa1=Point A's coordinate1
    const int iAxis1=1;		//iAxis1 is the axis which is vertical to the sweepline
    const int iAxis2=0;
    
    double windowssizex;
    double windowssizey;
    //Every Interface will have a SweepLineLst
    SweepLineLst opSweepLineLst; // Every Interface will have a SweepLineLst
    
    PolyEdge *pPolyEdge;

    //now generate the SweepLineLsts
    //add window polygon's edges to the SweepLineLst
    int vn  = 4;
    windowssizex = plane.windows.vertex[1].x-plane.windows.vertex[0].x;
    windowssizey = plane.windows.vertex[3].y-plane.windows.vertex[0].y;
    for(int i=0;i<vn;i++) // here we only consider the ploy with 4 vertex point
    {
        opnt1.x=plane.windows.vertex[i%vn].x;
        opnt1.y=plane.windows.vertex[i%vn].y;
        opnt2.x=plane.windows.vertex[(i+1)%vn].x;
        opnt2.y=plane.windows.vertex[(i+1)%vn].y;
        opnt1.z=plane.windows.vertex[0].z;                        //ThunderZ i+1?
        opnt2.z=opnt1.z;
        /*if ( i == 0){
            windowssizex = opnt2.x-opnt1.x;
            windowssizey = opnt2.y-opnt1.y;
        }*/
        pa1=opnt1.mdGetCoordinate(iAxis1);
        pb1=opnt2.mdGetCoordinate(iAxis1);
        if(MbFltLT(pa1,pb1))
        {
            pPolyEdge=new PolyEdge(plane.pores.size(),opnt1,opnt2);
            opSweepLineLst.mvInsertSweepLine(pa1,pPolyEdge);
            opSweepLineLst.mvInsertSweepLine(pb1,NULL);
        }	
        else if(MbFltGT(pa1,pb1))
        {
            pPolyEdge=new PolyEdge(plane.pores.size(),opnt2,opnt1);
            opSweepLineLst.mvInsertSweepLine(pb1,pPolyEdge);
            opSweepLineLst.mvInsertSweepLine(pa1,NULL);
        }
            
    }

    //add the conductor's polygon's edges to the SweepLineLst
    for(int i=0;i<plane.pores.size();i++)
    {
        for(int j=0;j<vn;j++)
        {

            opnt1.x=plane.pores[i].vertex[j%vn].x;
            opnt1.y=plane.pores[i].vertex[j%vn].y;
            opnt2.x=plane.pores[i].vertex[(j+1)%vn].x;
            opnt2.y=plane.pores[i].vertex[(j+1)%vn].y;
            opnt1.z=plane.pores[i].vertex[0].z;
        //	std::cout << i << " point " << opnt1.z << std::endl;
            opnt2.z=opnt1.z;

            pa1=opnt1.mdGetCoordinate(iAxis1);
            pb1=opnt2.mdGetCoordinate(iAxis1);

            if(MbFltLT(pa1,pb1))
            {
                pPolyEdge=new PolyEdge(i,opnt1,opnt2);
                opSweepLineLst.mvInsertSweepLine(pa1,pPolyEdge);
                opSweepLineLst.mvInsertSweepLine(pb1,NULL);
            }	
            else if(MbFltGT(pa1,pb1))
            {
                pPolyEdge=new PolyEdge(i,opnt2,opnt1);
                opSweepLineLst.mvInsertSweepLine(pb1,pPolyEdge);
                opSweepLineLst.mvInsertSweepLine(pa1,NULL);
            }
            
        }
    }

    //generate mother elements, mainly in the Slab::Process function
    SweepLine * opSweepLine;
    Slab oSlab;

    opSweepLine= opSweepLineLst.mSweepLineHead.mpNext;
    while(opSweepLine->mpNext != NULL)
    {
        oSlab.mvProcess(iAxis1, iAxis2, opSweepLine);
        opSweepLine= opSweepLine->mpNext;
    }
    std::vector<DPLANE> MotherElement; 
    oSlab.mvDelEdgeLst();
    for(int i = 0 ; i <= plane.pores.size() ; i++){
        SlabElementLst *opgopSlabElementLst = &gopSlabElementLst[i];
        SlabElement *p1 = opgopSlabElementLst->moHead.mpNext;
        while (p1!=NULL){
            
            DPLANE tempMotherElement;

            double xleftup = myinterpolate(p1->moLeft.mNode1.y,p1->mdEndPosition,
                            p1->moLeft.mNode2.y,p1->moLeft.mNode1.x,p1->moLeft.mNode2.x);
            double xleftdown = myinterpolate(p1->moLeft.mNode1.y,p1->mdStartPosition,
                            p1->moLeft.mNode2.y,p1->moLeft.mNode1.x,p1->moLeft.mNode2.x);
            double xrightup = myinterpolate(p1->moRight.mNode1.y,p1->mdEndPosition,
                p1->moRight.mNode2.y,p1->moRight.mNode1.x,p1->moRight.mNode2.x);
            double xrightdown = myinterpolate(p1->moRight.mNode1.y,p1->mdStartPosition,
                            p1->moRight.mNode2.y,p1->moRight.mNode1.x,p1->moRight.mNode2.x);
                
            int xEle = int(((xrightdown - xleftdown)/windowssizex)*plane.windows.xEle + 0.5);
            int yEle = int(((p1->mdEndPosition - p1->mdStartPosition)/windowssizey)* plane.windows.yEle + 0.5);
            if (xEle == 0){
                xEle = 1;
            }
            if (yEle == 0){
                yEle = 1;
            }
        //	std::cout << ":;;;;;;;;;;;;;;;;;;;;;;" << std::endl;
        //	std::cout << xEle << " " << yEle << std::endl;
            mothervertex(tempMotherElement, xleftup, xleftdown, xrightup, xrightdown, 
                                    p1->mdStartPosition,p1->mdEndPosition, p1->moLeft.mNode1.z, xEle,yEle);
            MotherElement.push_back(tempMotherElement);
            p1 = p1->mpNext;
        }	
    }
    //divide MotherElements;
    for(int i = 0 ; i < MotherElement.size(); i++){
        //printmotherele(MotherElement[i]);
        TPOINT p1 = MotherElement[i].windows.vertex[0];
        TPOINT p2 = MotherElement[i].windows.vertex[1];
        TPOINT p3 = MotherElement[i].windows.vertex[2];
        TPOINT p4 = MotherElement[i].windows.vertex[3];
        int xEle = MotherElement[i].windows.xEle;
        int yEle = MotherElement[i].windows.yEle;
        divide_quadrangle(&p1, &p2, &p3, &p4, xEle, yEle, plane.inverse, index,true);
    }
            
    delete []gopSlabElementLst;
    //delete []gopSlabElementLst2;
}

void mothervertex(DPLANE &plane, double xlu, double xld,double xru,double xrd, double s, double e, double z,int x, int y)
{
    
    TPOINT v1{xld, s, z};
    TPOINT v2{xrd, s, z};
    TPOINT v3{xru, e, z};
    TPOINT v4{xlu, e, z};
    
    plane.windows.vertex[0] = v1;
    plane.windows.vertex[1] = v2;
    plane.windows.vertex[2] = v3;
    plane.windows.vertex[3] = v4;

    plane.windows.xEle = x;
    plane.windows.yEle = y;

}
#endif
