/*
* Configure.h
*
*  Created on: 8 Jul, 2016
*      Author: ming
*/

#ifndef CONFIGURE_H_
#define CONFIGURE_H_

#include "Debugger.h"
#include <stddef.h>
#include <vector>

class Configure {
    int num_cond = 0;                             // the number of conductors
    
    double diel;	        // dielectric value

    Debugger debg;
    
public:
    Configure();
    Configure(double* ori, double* len, int* sa, double d);
    Configure(double* ori, double* len, int* sa, int nc, double** co, double** cl, int** cs, double d);
    
    // we copy to the call for get*, so don't forget to delete them
    int getNumCond();
    
    double origin[3];       // Macro origin, origin[0]->axis1, origin[1]->axis2, origin[2]->axis3
    double length[3];   	// Macro size, default for cubic, length[0]-> axis1, length[1]-> axis2, length[2]-> axis3
    int seg_axis[3];        // FDM segments in direction axis1, axis2, axis3(corresponding to axis[0], axis[1], axis3)
    
    double** cond_ori = NULL;        // the conductors' origin points
    double** cond_len = NULL;        // the conductors' 3-D size
    int** cond_seg = NULL;           // the conductors' segmentation
    
    ~Configure();
    void Debug();
};

#endif /* CONFIGURE_H_ */
