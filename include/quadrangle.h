#ifndef _QUADRANGLE_H
#define _QUADRANGLE_H

#include "frwbcm.h"
#include "mymath.h"
////////////////////////////
/*
* divide trapezoid element unit
*	 4 _________ 3
*	  /         \ 
*     1 /___________\ 2
* p1,p2,p3,p4 is the 4 vertexs of trapezoid, edge from 1 to 2 is the longer edge
* nx ny is the number to be divided along x,y
*/

extern bool frw;
extern bool bcm;
void printpoint(TPOINT *p){
    std::cout << p->x << " " << p->y << " " << p->z << std::endl;
}
void divide_quadrangle(TPOINT *p1, TPOINT *p2, TPOINT *p3, TPOINT *p4, int nx, int ny, bool reverse, int bid, bool isInterface){
    if(!parallel(p1, p2, p3, p4)){
        cout<<"divide quad:4 points not parallel..exit..\n";
        exit(1);
    }
    if(nx<=0 || ny<=0){
        printf("invalid divide info: nx or ny is negative\n");
        printf("nx=%d, ny=%d\n", nx, ny);
        exit(1);
    }
    int i,j;
    double longer_x;
    TVECTOR vec14, vec23, vec_x1, vec_x2;
    TPOINT start1, start2;
    TPOINT end1, end2;
    std::cout << "--------------------" << std::endl;	
    printpoint(p1);	
    printpoint(p2);	
    printpoint(p3);	
    printpoint(p4);	
    std::cout<< " nx " << nx << " " << "ny " << ny << std::endl;	
    longer_x = euclid_distance(p1, p2);
    sub_point(p4, p1, &vec14);
    sub_point(p3, p2, &vec23);
    down_point(&vec14, ny, &vec14);
    down_point(&vec23, ny, &vec23);	
    
    for(j=0; j<ny; j++){
        TPOINT tmp;
        up_point(&vec14, j, &tmp);
        add_point(p1, &tmp, &start1);
        up_point(&vec14, j+1, &tmp);
        add_point(p1, &tmp, &start2);
        up_point(&vec23, j, &tmp);
        add_point(p2, &tmp, &end1);		
        up_point(&vec23, j+1, &tmp);
        add_point(p2, &tmp, &end2);		
        sub_point(&end1, &start1, &vec_x1);
        sub_point(&end2, &start2, &vec_x2);	
        
        double tmp_dis = euclid_distance(&start1, &end1);
        int rnx;
        TPOINT cache;
        rnx = myround((tmp_dis / longer_x) * nx);

        down_point(&vec_x1, rnx, &vec_x1);
        down_point(&vec_x2, rnx, &vec_x2);
        TBDELEINFO tBdElement;	
        for(i=0; i<rnx; i++){
            up_point(&vec_x1, i, &tmp);
            add_point(&start1, &tmp, &cache);
            tBdElement.leftdown = cache;
            up_point(&vec_x1, i+1, &tmp);
            add_point(&start1, &tmp, &cache);
            tBdElement.rightdown = cache;
            up_point(&vec_x2, i, &tmp);
            add_point(&start2, &tmp, &cache);
            tBdElement.leftup = cache;
            up_point(&vec_x2, i+1, &tmp);
            add_point(&start2, &tmp, &cache);
            tBdElement.rightup = cache;
            cache.x=(tBdElement.leftdown.x + tBdElement.rightdown.x + tBdElement.rightup.x + tBdElement.leftup.x)/4;
            cache.y=(tBdElement.leftdown.y + tBdElement.rightdown.y + tBdElement.rightup.y + tBdElement.leftup.y)/4;
            cache.z=(tBdElement.leftdown.z + tBdElement.rightdown.z + tBdElement.rightup.z + tBdElement.leftup.z)/4;
            tBdElement.cfg = cache;
            
            if(i == 0 && j == 0){
                std::cout << cache.x << " " << cache.y << " " << cache.z << std::endl;
            }
            if(reverse){
                cache = tBdElement.leftdown;
                tBdElement.leftdown = tBdElement.rightdown;
                tBdElement.rightdown = cache;
                cache = tBdElement.leftup;
                tBdElement.leftup = tBdElement.rightup;
                tBdElement.rightup = cache;				
            }
            tBdElement.type = Element_Type_new(&tBdElement.leftdown, 
                    &tBdElement.rightdown, 
                    &tBdElement.rightup, 
                    &tBdElement.leftup, 4);		
            if(frw){
                gBdElements[bid][cur_ele_id[bid]] = tBdElement;
            }if(bcm){
                if(isInterface){
                    BdElementInter.push_back(tBdElement);
                    BdElementInterSize[bid]++;
                }else{
                    boundaryposition[0].push_back(tBdElement.cfg.x);
                    boundaryposition[1].push_back(tBdElement.cfg.y);
                    boundaryposition[2].push_back(tBdElement.cfg.z);
                }
                BdElementsbcm.push_back(tBdElement);
            }	
            cur_ele_id[bid] ++;
        }
    }
}

#endif
