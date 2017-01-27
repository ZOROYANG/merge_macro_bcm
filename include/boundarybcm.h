#ifndef _BOUNDARYBCM_H_
#define _BOUNDARYBCM_H_

#include "frwbcm.h"
#include "plane.h"
#include "csweepline.h"
#include<vector>

using namespace std;

extern int type;
extern std::vector<DPLANE> dinterfaces;
void boundaryInterface(DPLANE &plane,int index);

void inversePlane(TBDELEINFO &tBdElement){
    TPOINT cache;
    cache = tBdElement.leftdown;
    tBdElement.leftdown = tBdElement.rightdown;
    tBdElement.rightdown = cache;
    cache = tBdElement.leftup;
    tBdElement.leftup = tBdElement.rightup;
    tBdElement.rightup = cache;				
}

void boundarybcm(const bool standard, const double h, const size_t low, const size_t seg){
    planeExtract(standard, h, low, seg);	
    if(dblocks.size() != dconductors.size()){
        exit(1);
    }
    /*for(int i = 0 ; i < dblocks.size() ; i++){
        std::cout << "size " << i+1  << " " << dblocks[i].size() << std::endl;
    }
    std::cout << "dinterfaces size " << " " <<dinterfaces.size() << std::endl;*/
    if(type == 0 || type == 1){	
        for ( int i = 0 ; i < static_cast<int>(dblocks.size());i++){
            START_RANGE(i)
            if(dconductors[i][0].isempty == false){
                for ( int j = 0 ; j < static_cast<int>(dconductors[i].size()); j++){
                    divide_quadrangle(&dconductors[i][j].windows.vertex[0],
                                    &dconductors[i][j].windows.vertex[1],&dconductors[i][j].windows.vertex[2],
                                    &dconductors[i][j].windows.vertex[3],dconductors[i][j].windows.xEle,
                                    dconductors[i][j].windows.yEle,dconductors[i][j].inverse,i,false);
                }
            }
            for(int j = 0 ; j < static_cast<int>(dblocks[i].size()) ; j++){
                divide_quadrangle(&dblocks[i][j].windows.vertex[0],
                                &dblocks[i][j].windows.vertex[1],&dblocks[i][j].windows.vertex[2],
                                &dblocks[i][j].windows.vertex[3],dblocks[i][j].windows.xEle,
                                dblocks[i][j].windows.yEle,dblocks[i][j].inverse,i,false);
            }
        /*	for(int j = 0 ; j < dblocks[i].size() ; j++){
            std::cout << " boundary  " << dblocks[i][j].windows.xEle << " " <<  
                dblocks[i][j].windows.yEle << std::endl;}*/
            if(blocks.size() > 1){
                if( i == 0){
                    //	std::cout << i << "boundary " << std::endl;
                    dinterfaces[0].inverse = false;
                    if(dinterfaces[0].isinterface){
                        boundaryInterface(dinterfaces[0], i); 
                        
                    }else{
                        divide_quadrangle(&dinterfaces[0].windows.vertex[0],
                                        &dinterfaces[0].windows.vertex[1],&dinterfaces[i].windows.vertex[2],
                                        &dinterfaces[0].windows.vertex[3],dinterfaces[i].windows.xEle,
                                        dinterfaces[i].windows.yEle,false,i,true);
                        
                    }	
                    
                }else if ( i < static_cast<int>(dblocks.size())-1 && i > 0){
                    //	std::cout << i << "boundary " << std::endl;
                    for(int k = 0 ; k < static_cast<int>(BdElementInter.size());k++){
                        inversePlane(BdElementInter[k]);
                        BdElementsbcm.push_back(BdElementInter[k]);
                        cur_ele_id[i]++;
                        
                    }
                    std::vector<TBDELEINFO>().swap(BdElementInter);
                    dinterfaces[i].inverse = false;	
                    if(dinterfaces[i].isinterface){
                        boundaryInterface(dinterfaces[i], i) ;
                        
                    }else{
                        divide_quadrangle(&dinterfaces[i].windows.vertex[0],
                                        &dinterfaces[i].windows.vertex[1],&dinterfaces[i].windows.vertex[2],
                                        &dinterfaces[i].windows.vertex[3],dinterfaces[i].windows.xEle,
                                        dinterfaces[i].windows.yEle,false,i,true);
                        
                    }	
                    
                }else {
                    for(int k = 0 ; k < static_cast<int>(BdElementInter.size());k++){
                        inversePlane(BdElementInter[k]);
                        BdElementsbcm.push_back(BdElementInter[k]);
                        cur_ele_id[i]++;
                        
                    }
                    
                }
                
            }
            gBdElementsbcm.push_back(BdElementsbcm);
            std::vector<TBDELEINFO>().swap(BdElementsbcm);
            END_RANGE2(i)
            
        }
        
    }else if(type == -1){
        int blockssize = blocks.size();	
        for ( int i = blocks.size()-1 ; i >= 0 ;i--){
            int inverse_i = abs(i-blockssize+1);
            /*std::cout << "inverse _ i " << i << " " << inverse_i << std::endl;*/
            START_RANGE(inverse_i)
            if(dconductors[i][0].isempty == false){
                for ( int j = 0 ; j < static_cast<int>(dconductors[i].size()); j++){
                    divide_quadrangle(&dconductors[i][j].windows.vertex[0],
                                    &dconductors[i][j].windows.vertex[1],&dconductors[i][j].windows.vertex[2],
                                    &dconductors[i][j].windows.vertex[3],dconductors[i][j].windows.xEle,
                                    dconductors[i][j].windows.yEle,dconductors[i][j].inverse,inverse_i,false);
                    
                }
                
            }
            for(int j = 0 ; j < static_cast<int>(dblocks[i].size()) ; j++){
                divide_quadrangle(&dblocks[i][j].windows.vertex[0],&dblocks[i][j].windows.vertex[1],
                                &dblocks[i][j].windows.vertex[2],&dblocks[i][j].windows.vertex[3],
                                dblocks[i][j].windows.xEle,dblocks[i][j].windows.yEle,dblocks[i][j].inverse,inverse_i,false);
                
            }
            if( i == 0){
                for(int k = 0 ; k < static_cast<int>(BdElementInter.size());k++){
                    inversePlane(BdElementInter[k]);
                    BdElementsbcm.push_back(BdElementInter[k]);
                    cur_ele_id[inverse_i]++;
                    
                }
                
            }else if ( i < static_cast<int>(dblocks.size())-1 && i > 0){
                std::vector<TBDELEINFO> TempBdElementInter;
                for(int k = 0 ; k < static_cast<int>(BdElementInter.size());k++){
                    inversePlane(BdElementInter[k]);
                    TempBdElementInter.push_back(BdElementInter[k]);
                    
                }
                std::vector<TBDELEINFO>().swap(BdElementInter);	
                if(dinterfaces[i-1].isinterface){
                    boundaryInterface(dinterfaces[i-1], inverse_i); 
                    
                }else{
                    divide_quadrangle(&dinterfaces[i].windows.vertex[0],
                                    &dinterfaces[i].windows.vertex[1],&dinterfaces[i].windows.vertex[2],
                                    &dinterfaces[i].windows.vertex[3],dinterfaces[i].windows.xEle,
                                    dinterfaces[i].windows.yEle,false,inverse_i,true);
                    
                }	
                for(int k = 0 ; k < static_cast<int>(BdElementInter.size());k++){
                    inversePlane(BdElementInter[k]);
                    BdElementsbcm.push_back(BdElementInter[k]);
                    cur_ele_id[inverse_i]++;
                    
                }
                
            }else {
                dinterfaces[i-1].inverse = false;
                if( dinterfaces[i-1].isinterface){
                    boundaryInterface(dinterfaces[i-1], inverse_i);	
                    
                }else{
                    divide_quadrangle(&dinterfaces[i-1].windows.vertex[0],
                                    &dinterfaces[i-1].windows.vertex[1],&dinterfaces[i-1].windows.vertex[2],
                                    &dinterfaces[i-1].windows.vertex[3],dinterfaces[i-1].windows.xEle,
                                    dinterfaces[i-1].windows.yEle,false,inverse_i,true);
                    
                }
                
            }
            gBdElementsbcm.push_back(BdElementsbcm);
            std::vector<TBDELEINFO>().swap(BdElementsbcm);
            END_RANGE2(inverse_i)
            
        }
        
    }
}
#endif
