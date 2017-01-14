#ifndef _CONDUCTORPOINT_H
#define _CONDUCTORPOINT_H

#include "frwbcm.h"
#include "mymath.h"
#include <vector>
#include "shape.h"

using namespace std;

extern vector<CONDUCTORINFO> conductors; 

void condVertexPoint(){

    if(conductors.size() > 0){

        for(size_t i = 0 ; i < conductors.size(); i++){
            conductors[i].botP[0] = conductors[i].geo.basePoint;
            TPOINT tempcond;
            add_point(&conductors[i].geo.basePoint, &conductors[i].geo.xAxis, &tempcond);
            conductors[i].botP[1] = tempcond;
            add_point(&conductors[i].geo.basePoint, &conductors[i].geo.yAxis, &tempcond);
            conductors[i].botP[3] = tempcond;
            add_point(&conductors[i].botP[1], &conductors[i].botP[3], &tempcond);
            conductors[i].botP[2].x = tempcond.x-conductors[i].botP[0].x;
            conductors[i].botP[2].y = tempcond.y-conductors[i].botP[0].y;
            conductors[i].botP[2].z = tempcond.z-conductors[i].botP[0].z;
            add_point(&conductors[i].geo.basePoint, &conductors[i].geo.zAxis, &tempcond);	
            for(int j = 0 ; j < 4 ; j++){
                conductors[i].topP[j] = conductors[i].botP[j];
                conductors[i].topP[j].z = tempcond.z;
            }
            /*		condCoordinate(conductors[i]ors[i]);
            }*/
        }
    }else{
        return;
    //	std::cout << " The Model does not contain conductors" << std::endl;
    }
} 
#endif
