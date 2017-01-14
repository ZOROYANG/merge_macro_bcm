#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "frwbcm.h"

//make the interface divide the same
void unify_interface_divide(){
    //cout<<"interface num="<<pddlInterfacePoint3DNum<<endl;
    if(cur_poly_id <= 0){
        cout <<"error: two mediums, but no interface, exit...\n";
        exit(1);
    }
    for(int i=0;i<Curmed;i++)
        interface_start_id[i]=cur_ele_id[i];
    //for 0
    for(int i=0; i<cur_poly_id; i++){
        if(poly_in_interface[i].pnum == 4)
            divide_quadrangle(&poly_in_interface[i].p1, 
                            &poly_in_interface[i].p2,
                            &poly_in_interface[i].p3,
                            &poly_in_interface[i].p4,
                            poly_in_interface[i].nx,
                            poly_in_interface[i].ny,
                            poly_in_interface[i].rv, 0,false);
        else
            divide_triangle(&poly_in_interface[i].p1, 
                            &poly_in_interface[i].p2,
                            &poly_in_interface[i].p3,
                            poly_in_interface[i].nx,
                            poly_in_interface[i].ny,
                            poly_in_interface[i].rv, 0);
    }
    /*for observe point*/
    if(pointInMediumId == 0)
        gBdElements[0][cur_ele_id[0]].cfg=observePoint;
    save_ele(0);
    //for other
    for(int i=1;i<Curmed;i++){
        for(int j=interface_start_id[0];j<cur_ele_id[0];j++){
            gBdElements[i][cur_ele_id[i]] = gBdElements[0][j];
            reverse_points(&gBdElements[i][cur_ele_id[i]]);
            cur_ele_id[i] ++;
        }		
        if(pointInMediumId == i)
            gBdElements[i][cur_ele_id[i]].cfg=observePoint;
        save_ele(i);
    }
}

void mulCross(TVECTOR *v1, TVECTOR *v2, TVECTOR *res){
    if(v1 == res || v2 == res){
        cout<<"cross mul result cannot save in v1 or v2, exit...\n";
        exit(1);
    }
    res->x = v1->y*v2->z - v2->y*v1->z;
    res->y = v1->z*v2->x - v2->z*v1->x;
    res->z = v1->x*v2->y - v2->x*v1->y;
}
double lenVec(TVECTOR *v){
    return sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
}
void tg_init(){
    if(Curmed == 2){
        poly_in_interface = new PlaneInfo[MAX_CHIP_NUM];
        cur_poly_id=0;
        pointInMediumId = pointInMediumId - 1;
        if(pddlInterfacePoint3DNum <3){
            cout<<"point offered in interface so less, at least 3,exit...\n";
            exit(1);
        }
        TVECTOR v1,v2;
        sub_point(&pddlInterfacePoint3DBuf[1], &pddlInterfacePoint3DBuf[0], &v1);
        sub_point(&pddlInterfacePoint3DBuf[2], &pddlInterfacePoint3DBuf[0], &v2);
        mulCross(&v1, &v2, &gvNormal);
        normalLen = lenVec(&gvNormal);
    }
}
#endif
