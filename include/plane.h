#ifndef _PLANE_H_
#define _PLANE_H_

#include "frwbcm.h"
#include "cvertex.h"
#include <vector>
#include "touchtype.h"
#include <cassert>
using namespace std;

extern vector<CONDUCTORINFO> conductors;
extern vector<BLOCKINFO> blocks;
extern vector<vector<DPLANE> > dblocks;
extern vector<vector<DPLANE> > dconductors;
extern vector<DPLANE> dinterfaces;

int type;

void reset(DPLANE &plane){

    std::vector<PORE>().swap(plane.pores);
    plane.windows.vertex[0] = TPOINT {0,0,0};	
    plane.windows.vertex[1] = TPOINT {0,0,0};	
    plane.windows.vertex[2] = TPOINT {0,0,0};	
    plane.windows.vertex[3] = TPOINT {0,0,0};
    plane.windows.layerid = MAXNUM;
    plane.windows.conductorid = MAXNUM;
    plane.windows.xEle = 0 ; 
    plane.windows.yEle = 0 ;
    plane.inverse =false;
    plane.isinterface =false;
    plane.isempty = false;
}

void dplaneAssign(DPLANE &plane, int layerid, int conductorid, TPOINT ld,TPOINT rd, TPOINT ru, TPOINT lu, WORD x, WORD y, bool ispore){
    
    if(ispore){
        PORE pore;
        pore.vertex[0] = ld;
        pore.vertex[1] = rd;
        pore.vertex[2] = ru;
        pore.vertex[3] = lu;
        pore.xEle = x ;
        pore.yEle = y ;
        pore.layerid = layerid;
        pore.conductorid = conductorid;
        plane.isinterface = ispore;
        plane.pores.push_back(pore);
    }else{
        plane.windows.vertex[0] = ld;
        plane.windows.vertex[1] = rd;
        plane.windows.vertex[2] = ru;
        plane.windows.vertex[3] = lu;
        plane.windows.xEle = x ;
        plane.windows.yEle = y ;
        plane.windows.layerid = layerid;
        plane.windows.conductorid = conductorid;
    
    }
}

void casePlane(bool conductor, DPLANE &plane, int layerid, int conductorsid, int surfaceid, bool ispore){

    CONDUCTORINFO *tempblock;
    if(conductor){
        tempblock = &conductors[conductorsid-1];
    }else{
        tempblock = &blocks[layerid-1];
    }
    switch(surfaceid){
        case 0:dplaneAssign(plane,layerid, conductorsid,tempblock->topP[0],tempblock->topP[1],
            tempblock->topP[2],tempblock->topP[3],tempblock->xEle,tempblock->yEle, ispore);
            break;
        case 1:dplaneAssign(plane,layerid, conductorsid, tempblock->botP[3],tempblock->botP[0],
                tempblock->topP[0],tempblock->topP[3],tempblock->yEle,tempblock->zEle, ispore);
            break;
        case 2:dplaneAssign(plane,layerid, conductorsid, tempblock->botP[0],tempblock->botP[1],
                    tempblock->topP[1],tempblock->topP[0],tempblock->xEle,tempblock->zEle, ispore);
            break;
        case 3:dplaneAssign(plane,layerid, conductorsid, tempblock->botP[1],tempblock->botP[2],
                    tempblock->topP[2],tempblock->topP[1],tempblock->yEle,tempblock->zEle, ispore);
            break;
        case 4:dplaneAssign(plane,layerid, conductorsid, tempblock->botP[2],tempblock->botP[3],
                    tempblock->topP[3],tempblock->topP[2],tempblock->xEle,tempblock->zEle, ispore);
            break;
        case 5:dplaneAssign(plane,layerid, conductorsid, tempblock->botP[0],tempblock->botP[1],
                    tempblock->botP[2],tempblock->botP[3],tempblock->xEle,tempblock->yEle, ispore);
            break;
        default: break;
    }
}

void printpoint(const TPOINT a){
    std::cout << a.x << " " << a.y << " " << a.z << std::endl;
}
void printplane(const TPOINT *a,int n){
    for(int i = 0 ; i< n; i++){
        printpoint(a[i]);
    }
}
void printinterface(const DPLANE &interfacess){
    printplane(interfacess.windows.vertex,4);
    for(int i = 0 ; i < static_cast<int>(interfacess.pores.size());i++){
        printplane(interfacess.pores[i].vertex,4); 
    }
}

double areaBound(TPOINT *p1, TPOINT *p2, unsigned int n){

    double x, y, z;
    x = (p2->x - p1->x)/ n;
    y = (p2->y - p1->y)/ n;
    z = (p2->z - p1->z)/ n;
    return sqrt(x * x + y * y +  z * z);
    //return sqrt(res1->x*res1->x + res1->y *res1->y + res1->z*res1->z);
}

void segExtract(){

    assert(block_number == static_cast<int>(blocks.size()));

    segments_x.resize(block_number);	
    segments_y.resize(block_number);	
    segments_z.resize(block_number);
    high.push_back(blocks[0].botP[0].z);	
    segments.resize(3);
    for(int j = 0 ; j < block_number; j++){
        
        double lengthx = areaBound(&blocks[j].botP[0],&blocks[j].botP[1],blocks[j].xEle);
        double lengthy = areaBound(&blocks[j].botP[0],&blocks[j].botP[3],blocks[j].yEle);
        double lengthz = areaBound(&blocks[j].botP[0],&blocks[j].topP[0],blocks[j].zEle);
        ele_x.push_back(blocks[j].xEle);
        ele_y.push_back(blocks[j].yEle);
        ele_z.push_back(blocks[j].zEle);
        if(block_number == 1){
            _length.push_back(blocks[0].botP[1].x - blocks[0].botP[0].x);
            _length.push_back(blocks[0].botP[3].y - blocks[0].botP[0].y);
            _length.push_back(blocks[0].topP[0].z - blocks[0].botP[0].z);
        }else{
            if( j == 0){
                _length.push_back(0.0);
                _length.push_back(0.0);
                _length.push_back(0.0);
            }
        }	
        high.push_back(blocks[j].topP[0].z);
        for(size_t i = 0 ; i < blocks[j].xEle ; i++){
            segments_x[j].push_back(blocks[j].botP[0].x + (i * lengthx + (i + 1) * lengthx)/2);
        }
        for(size_t i = 0 ; i < blocks[j].yEle ; i++){
            segments_y[j].push_back(blocks[j].botP[0].y+ (i * lengthy + (i + 1) * lengthy)/2);
        }
        for(size_t i = 0 ; i < blocks[j].zEle ; i++){
            segments_z[j].push_back(blocks[j].botP[0].z + (i * lengthz + (i + 1) * lengthz)/2);
        }
    }
}

void areaExtract(){
    
    Areas.resize(2);
    double lengthx, lengthy;
    for(size_t i = 0 ; i < 2; i++){
        Areas[i].resize(blocks.size()+1);
        for(size_t j = 0 ; j < static_cast<size_t>(blocks.size()) + 1 ; j++){
            if( i == 0){
                if(j == 0){
        
                    lengthx = areaBound(&blocks[j].botP[0],&blocks[j].botP[1],blocks[j].xEle);
                    lengthy = areaBound(&blocks[j].botP[0],&blocks[j].botP[3],blocks[j].yEle);
                    Areas[i][0] = lengthx * lengthy;
                }else{
                    lengthx = areaBound(&blocks[j-1].botP[0],&blocks[j-1].botP[3],blocks[j-1].yEle);
                    lengthy = areaBound(&blocks[j-1].botP[0],&blocks[j-1].topP[0],blocks[j-1].zEle);
                    Areas[i][j] = lengthx * lengthy;
                }
            }
            if(i == 1){
                if(j == 0){
                    lengthx = areaBound(&blocks[j].topP[0],&blocks[j].topP[1],blocks[j].xEle);
                    lengthy = areaBound(&blocks[j].topP[0],&blocks[j].topP[3],blocks[j].yEle);
                    Areas[i][0] = lengthx * lengthy;
                }else{
                    lengthx = areaBound(&blocks[j-1].botP[0],&blocks[j-1].botP[1],blocks[j-1].xEle);
                    lengthy = areaBound(&blocks[j-1].botP[0],&blocks[j-1].topP[0],blocks[j-1].zEle);
                    Areas[i][j] = lengthx * lengthy;
                }
            }
        }
    }
}
            
            
void planeExtract(const bool standard, const double h, const size_t low, const size_t seg){
    
    for(int i = 0 ; i < Curblk ; i++){
        cube_point_gen(i);
        BLOCKINFO btemp;
        btemp.blockNum = i ;
        for (int j = 0 ; j < 4 ; j++){
                btemp.botP[j]=vertexBuf[i][j];
                btemp.topP[j]=vertexTopBuf[i][j];
            if(standard){
                if(  i == 0){
                    btemp.topP[j].z = h;
                }else{
                    btemp.botP[j].z= h;
                    if(standard_h){
                        btemp.topP[j].z = 0.5;
                    }if(standard_v){
                        btemp.topP[j].z = 1.0;
                    }
                }
            }
        }
        btemp.xEle = usTempEle[i][0];
        btemp.yEle = usTempEle[i][1];
        if (!standard){
            btemp.zEle = usTempEle[i][2];
        }else{
            if( i == 0){
                btemp.zEle = low;
            }else{
                btemp.zEle = seg - low;
            }
        }
        blocks.push_back(btemp);
    }
/*	for(int i = 0 ; i < blocks.size();i++){
        std::cout << "print block " << std::endl;
        printplane(blocks[i].botP,4);
        printplane(blocks[i].topP,4);
        std::cout << blocks[i].xEle << " " << blocks[i].yEle << " " << blocks[i].zEle << std::endl;
    }*/
    areaExtract();
    segExtract();	
    condVertexPoint();
/*	std::cout << "print conductor " << std::endl;
    for(int i = 0 ; i < conductors.size();i++){
        printplane(conductors[i].botP,4);
        printplane(conductors[i].topP,4);
        std::cout << conductors[i].xEle << " " << conductors[i].yEle << " " << conductors[i].zEle << std::endl;
    }
*/
    //bool up = false;
    //bool down = false;

    type = touchtype(blocks,conductors);
    
    DPLANE interface;
    //std::cout << " type = " << type << std::endl;
    if ( type == 0 ){
    //conductors do not touch interface
        for(int i = 0 ; i < static_cast<int>(blocks.size()); i++){
            DPLANE ptemp[6];
            std::vector<DPLANE> dcond;
            std::vector<DPLANE> dbloc;
            int conductorid = 0 ;
            for(int j = 0 ; j < static_cast<int>(conductors.size());j++){
                if(conductors[j].layerNum == i + 1){
                    conductorid = j + 1;				
                    
                    casePlane(true, ptemp[5], i+1, conductorid, 5, false);
                    dcond.push_back(ptemp[5]);
                    reset(ptemp[5]);
                    for(int k = 0; k < 5; k++){
                        casePlane(true, ptemp[k], i+1, conductorid, k, false);
                        if(k < 5){
                            ptemp[k].inverse = true;
                        }
                        if(k > 0){
                            dcond.push_back(ptemp[k]);
                            reset(ptemp[k]);
                        }
                    }
                    dcond.push_back(ptemp[0]);
                    reset(ptemp[0]);
                }
            }
            if(dcond.size() == 0){
                DPLANE empty;
                empty.isempty = true ;
                dcond.push_back(empty);
            }
            dconductors.push_back(dcond);	
            for(int k = 0 ; k < 6 ; k++){
                casePlane(false, ptemp[k], i+1, MAXNUM, k, false);
    //			std::cout << " ********************************* " << std::endl;
            }
            ptemp[5].inverse = true;
            if(i == 0){
                if(blocks.size() == 1){
                    dbloc.push_back(ptemp[5]);
                    for(int k = 1 ; k < 5 ; k++){
                        dbloc.push_back(ptemp[k]);
                    }
                    dbloc.push_back(ptemp[0]);
                }else{
                    dbloc.push_back(ptemp[5]);
                    for(int k = 1 ; k < 5 ; k++){
                        dbloc.push_back(ptemp[k]);
                    }
                }
            }else if(i > 0 && i == static_cast<int>(blocks.size()) -1 ){
                for(int k = 1 ; k < 5; k++){
                    dbloc.push_back(ptemp[k]);
                }
                dbloc.push_back(ptemp[0]);
            }else{
                for(int k = 1 ; k< 5; k++){
                    dbloc.push_back(ptemp[k]);
                }
            }
            if(blocks.size() > 1 && i < static_cast<int>(blocks.size())-1){
                dinterfaces.push_back(ptemp[0]);
            }
            dblocks.push_back(dbloc);
        }	
    }else if( type == 1){				
        for(int i = 0 ; i < static_cast<int>(blocks.size()) ; i++){
            DPLANE surface;
            DPLANE ptemp[6];
            vector<DPLANE> dcond;
            vector<DPLANE> dbloc;	
            int conductorid = 0;
            if(interface.pores.size()>0){
            //	std::cout << " the " << i << "th" <<" time!" << std::endl;
                for ( int ii = 0 ; ii < static_cast<int>(interface.pores.size());ii++){
                    surface.windows = interface.pores[ii];
                    surface.inverse = interface.inverse;
                    dcond.push_back(surface);
                }
            }
            reset(interface);
            for(int j = 0 ; j < static_cast<int>(conductors.size()); j++){
                
                if(conductors[j].layerNum == i + 1){
                
                    conductorid = j + 1;	
                    for(int k = 1; k < 6; k++){
                        casePlane(true, ptemp[k], i+1, conductorid, k, false);
                        if(k < 5){
                            ptemp[k].inverse = true;
                        }
                    }
                    if(isInInterface(conductors[j].topP,blocks[i].topP)){
                        casePlane(true,ptemp[0],i+1, conductorid, 0, true);
                        ptemp[0].inverse = true;
                        dcond.push_back(ptemp[5]);
                        reset(ptemp[5]);
                        for(int k = 1 ; k< 5 ; k++){
                            dcond.push_back(ptemp[k]);
                            reset(ptemp[k]);
                        }
                    }else{
                        DPLANE temptop;
                        casePlane(true, temptop,i+1, conductorid, 0, false);
                        temptop.inverse = true;
                        dcond.push_back(ptemp[5]);
                        reset(ptemp[5]);
                        for(int k = 1; k < 5 ;k++){
                            dcond.push_back(ptemp[k]);
                            reset(ptemp[k]);
                        }
                        dcond.push_back(temptop);
                    }	
                }
            }
            if(dcond.size() == 0){
                DPLANE empty;
                empty.isempty = true ;
                dcond.push_back(empty);
            }
            dconductors.push_back(dcond);
            std::vector<DPLANE>().swap(dcond);
            for(int k = 0 ; k < 6 ; k++)
                casePlane(false, ptemp[k],i + 1 , MAXNUM, k , false);
            ptemp[5].inverse = true;
            if(blocks.size() > 1 && i < static_cast<int>(blocks.size())-1){
                interface = ptemp[0];
                dinterfaces.push_back(interface);
            }
            ptemp[0].inverse = false;
            if(i == 0){
                if(blocks.size() == 1){
                    dbloc.push_back(ptemp[5]);
                    for(int k = 1 ; k < 5 ; k++){
                        dbloc.push_back(ptemp[k]);
                    }
                    dbloc.push_back(ptemp[0]);
                }else{
                    dbloc.push_back(ptemp[5]);
                    for(int k = 1 ; k < 5 ; k++){
                        dbloc.push_back(ptemp[k]);
                    }
                }
            }else if( i > 0 && i == static_cast<int>(blocks.size())-1){
                for ( int k = 1 ; k < 5; k++){
                    dbloc.push_back(ptemp[k]);
                }
                dbloc.push_back(ptemp[0]);
            }else{
                for(int k = 1 ; k < 5 ; k ++){
                    dbloc.push_back(ptemp[k]);
                }
            }
            dblocks.push_back(dbloc);
        }
    }else if( type == -1){	
        for(int i = blocks.size()-1 ; i >=0  ; i--){
            /*std::cout << " inversr plane " << i << std::endl;*/
            DPLANE surface;
            DPLANE ptemp[6];
            vector<DPLANE> dcond;
            vector<DPLANE> dbloc;	
            int conductorid = 0;
            if(interface.pores.size()>0){
                for ( int k = 0 ; k < static_cast<int>(interface.pores.size());k++){
                    surface.windows = interface.pores[k];
                    surface.inverse = interface.inverse;
                    dcond.push_back(surface);
                }
            }
            reset(interface);
            for(int j = 0 ; j < static_cast<int>(conductors.size()); j++){
                
                if(conductors[j].layerNum == i + 1){
                
                    conductorid = j + 1;	
                    for(int k = 0; k < 5; k++){
                        casePlane(true, ptemp[k], i+1, conductorid, k, false);
                        ptemp[i].inverse = true;
                    }
                    if(isInInterface(conductors[j].botP,blocks[i].botP)){
                        casePlane(true,ptemp[5],i+1, conductorid, 5, true);
                        for(int k = 1 ; k < 5 ; k++){
                            dcond.push_back(ptemp[k]);
                            reset(ptemp[k]);
                        }
                        dcond.push_back(ptemp[0]);
                        reset(ptemp[0]);
                            
                    }else{
                        DPLANE tempbot;
                        casePlane(true, tempbot,i+1, conductorid, 5, false);
                        for(int k = 1; k < 5 ;k++){
                            dcond.push_back(ptemp[k]);
                            reset(ptemp[k]);
                        }
                        dcond.push_back(tempbot);
                        dcond.push_back(ptemp[0]);
                        reset(ptemp[0]);
                    }	
                }
            }
            if(dcond.size() == 0){
                DPLANE empty;
                empty.isempty = true ;
                dcond.push_back(empty);
            }
            dconductors.push_back(dcond);
            std::vector<DPLANE>().swap(dcond);
            for(int k = 0 ; k < 6 ; k++)
                casePlane(false, ptemp[k],i + 1 , MAXNUM, k , false);
            if(i > 0 && blocks.size() > 1){
                interface = ptemp[5];
                dinterfaces.push_back(interface);
            }
            ptemp[5].inverse = true;
            if(i == 0){
                if(blocks.size() == 1){
                    dbloc.push_back(ptemp[5]);

                    for(int k = 1 ; k < 5 ; k++){
                        dbloc.push_back(ptemp[k]);
                    }
                    dbloc.push_back(ptemp[0]);
                }else{
                    dbloc.push_back(ptemp[5]);
                    for(int k = 1 ; k < 5 ; k++){
                        dbloc.push_back(ptemp[k]);
                    }
                }
            }else if(i > 0 && i == static_cast<int>(blocks.size())-1){
                for ( int k = 1 ; k < 5; k++){
                    dbloc.push_back(ptemp[k]);
                }
                dbloc.push_back(ptemp[0]);
            }else{
                for(int k = 1 ; k < 5 ; k ++){
                    dbloc.push_back(ptemp[k]);
                }
            }
            dblocks.push_back(dbloc);
        }
    }else{
        cout << "unexpected type! Program end!" << endl;
    }
}	
#endif
