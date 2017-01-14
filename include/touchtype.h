#ifndef _TOUCHTYPE_
#define _TOUCHTYPE_

#include "frwbcm.h"
#include "shape.h"

// return 1 for up touch, return -1 for down touch , return 0 for no touch
int touchtype(const vector<BLOCKINFO> &block, const vector<CONDUCTORINFO> &conductor){
    
    int typeup = 0;
    int typedown = 0;
    for(int i = 0 ; i < block.size(); i++){
        for(int j = 0 ; j < conductor.size();j++){
            if(conductor[j].layerNum == i+1){
                if(isInInterface(conductor[j].topP, block[i].topP)){
                    typeup = 1 ;
                    break;
                }
            }
        }
        if(typeup == 1){break;}
    }
    for(int i = 0 ; i < block.size(); i++){
        for(int j = 0 ; j < conductor.size();j++){
            if(conductor[j].layerNum == i+1){
                if(isInInterface(conductor[j].botP, block[i].botP)){
                    typedown = -1 ;
                    break;
                }
            }
        }
        if(typedown == -1){break;}
    }
//	std::cout << "typeup " << typeup << "   typedown " << typedown << std::endl; 
    if(typeup == 1 && typedown == -1 ){
        std::cout << " Only one touch type is allow at one time" << std::endl;		 
        exit(1);
    }
    if(typeup == 1 ){
        return 1;
    }if(typedown == -1){
        return -1;
    }if(typeup == typedown && typeup == 0){
        return 0;
    }
        
}

#endif
