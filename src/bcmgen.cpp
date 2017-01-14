#include "frwbcm.h"
#include "mymath.h"
#include "boundarytype.h"
#include "quadrangle.h"
#include "cube.h"
#include "matrixbcm.h"
#include "release.h"
//main
void bcmgen(){
    
    printf("Generating Boundary elements...\n");
    cube_point_gen(0);
    EleGen_cube(0);
    MatrixGenBcm();
    release_mem();
    cout<<"FRW-BCM END!"<<endl;
}
