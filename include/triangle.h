#ifndef _TRIANGLE_H
#define _TRIANGLE_H
#include "frwbcm.h"

void divide_triangle(TPOINT *p1, TPOINT *p2, TPOINT *p3, int nx, int ny, bool reverse, int bid){	
    if(nx<=0 || ny<=0){
        printf("invalid divide info: nx or ny is negative\n");
        printf("nx=%d, ny=%d\n", nx, ny);
        exit(1);		
    }
    int i;
    if(ny == 1){
        TVECTOR vec_x;
        sub_point(p2, p1, &vec_x);
        down_point(&vec_x, nx, &vec_x);
        TPOINT cache;
        for(i=0; i<nx; i++){
            up_point(&vec_x, i, &cache);
            add_point(p1, &cache, &cache);
            gBdElements[bid][cur_ele_id[bid]].leftdown = cache;			
            up_point(&vec_x, i+1, &cache);
            add_point(p1, &cache, &cache);
            gBdElements[bid][cur_ele_id[bid]].rightdown = cache;
            gBdElements[bid][cur_ele_id[bid]].rightup = *p3;
            //usefulless
            gBdElements[bid][cur_ele_id[bid]].leftup = *p3;
            cache.x = (gBdElements[bid][cur_ele_id[bid]].leftdown.x + gBdElements[bid][cur_ele_id[bid]].rightdown.x + gBdElements[bid][cur_ele_id[bid]].rightup.x)/3;
            cache.y = (gBdElements[bid][cur_ele_id[bid]].leftdown.y + gBdElements[bid][cur_ele_id[bid]].rightdown.y + gBdElements[bid][cur_ele_id[bid]].rightup.y)/3;
            cache.z = (gBdElements[bid][cur_ele_id[bid]].leftdown.z + gBdElements[bid][cur_ele_id[bid]].rightdown.z + gBdElements[bid][cur_ele_id[bid]].rightup.z)/3;
            
            gBdElements[bid][cur_ele_id[bid]].cfg = cache;
            if(reverse){
                cache = gBdElements[bid][cur_ele_id[bid]].leftdown;
                gBdElements[bid][cur_ele_id[bid]].leftdown = gBdElements[bid][cur_ele_id[bid]].rightdown;
                gBdElements[bid][cur_ele_id[bid]].rightdown = cache;
            }
            //gBdElements[bid][cur_ele_id[bid]].type = XOY_TRIANGLE;
            
            gBdElements[bid][cur_ele_id[bid]].type = Element_Type_new(&gBdElements[bid][cur_ele_id[bid]].leftdown, 
                    &gBdElements[bid][cur_ele_id[bid]].rightdown, 
                    &gBdElements[bid][cur_ele_id[bid]].rightup, 
                    &gBdElements[bid][cur_ele_id[bid]].leftup, 3);
            
            cur_ele_id[bid] ++;
        }
    }else{	// >1
        TVECTOR vec31, vec32;
        sub_point(p1, p3, &vec31);
        sub_point(p2, p3, &vec32);
        down_point(&vec31, ny, &vec31);
        down_point(&vec32, ny, &vec32);
        add_point(p3, &vec31, &vec31);
        add_point(p3, &vec32, &vec32);
        int rnx;
        rnx = myround((euclid_distance(&vec31, &vec32)/euclid_distance(p1, p2))*nx);
        divide_quadrangle(p1, p2, &vec32, &vec31, nx, ny-1, reverse, bid,false);	//too limited, please fix it to fit all situations
        divide_triangle(&vec31, &vec32, p3, rnx, 1, reverse, bid);
    }
}
#endif
