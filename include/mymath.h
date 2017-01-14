#ifndef _MYMATH_H
#define _MYMATH_H
#include "frwbcm.h"

double mul2(int n,double *vec1,double *vec2)
{
    double re=0;
    for(int i=0;i<n;i++)
        re +=vec1[i]*vec2[i];
    return re;
}

//euclid distance
double euclid_distance(TPOINT *p1, TPOINT *p2){
    return sqrt(  ((p1->x-p2->x)*(p1->x-p2->x))
                + ((p1->y-p2->y)*(p1->y-p2->y))
                + ((p1->z-p2->z)*(p1->z-p2->z)));
}
double bigger(double a, double b){
    return a>b?a:b;
}
/* * mat:n*n	vec:n*1	 re:n*1	re=mat*vec
* because we can use mat[i] to read one line,so we use matrix*vec instead
*/
void mul_mat_vec(int n, double **mat, double *vec, double *re){
    int i;
    for(i=0;i<n;i++){
        re[i] = mul2(n, mat[i], vec);
    }
}
void mul_mat_vec(int n, double *mat, double *vec, double *re){
    int i;
    for(i=0;i<n;i++){
        re[i] = mul2(n, &mat[i*n], vec);
    }
}
void mul_vec_num(int n, double *vec, int num){
    for(int i=0;i<n;i++)
        vec[i]*=num;
}
//matrix transpose
void matrix_trans(int n, double **mat){
    int i,j;
    double tmpttt;
    for(i=0;i<n;i++){
        for(j=i+1;j<n;j++){
            tmpttt=mat[i][j];
            mat[i][j]=mat[j][i];
            mat[j][i]=tmpttt;
        }
    }
}
//matrix transpose overload
void matrix_trans(int n, double *mat){
    int i,j;
    double tmpttt;
    for(i=0;i<n;i++){
        for(j=i+1;j<n;j++){
            tmpttt=mat[i*n+j];
            mat[i*n+j]=mat[j*n+i];
            mat[j*n+i]=tmpttt;
        }
    }
}
//
void sub_point(TPOINT *p1, TPOINT *p2, TPOINT *res){
    res->x = p1->x - p2->x;
    res->y = p1->y - p2->y;
    res->z = p1->z - p2->z;
}
void down_point(TPOINT *p, int num, TPOINT *res){
    res->x = p->x / num;
    res->y = p->y / num;
    res->z = p->z / num;
}
void up_point(TPOINT* p, int num, TPOINT* res){
    res->x = p->x * num;
    res->y = p->y * num;
    res->z = p->z * num;
}
void zoom_point(TPOINT* p, double fac, TPOINT* res){
    res->x = p->x*fac;
    res->y = p->y*fac;
    res->z = p->z*fac;	
}
void add_point(TPOINT *p1, TPOINT *p2, TPOINT *res){
    res->x = p1->x + p2->x;
    res->y = p1->y + p2->y;
    res->z = p1->z + p2->z;
}
double mul_point(TPOINT *p1, TPOINT *p2){
    return p1->x*p2->x + p1->y*p2->y + p1->z*p2->z;
}
void sub_vec(int n,double *vec1,double *vec2,double *re)
{
    for (int i=0;i<n;i++){
        re[i] = vec1[i] - vec2[i];
    }
}
//length: p1 to line p2p3
double len_ptol(TPOINT *p1, TPOINT *p2, TPOINT *p3){
    double len21 = euclid_distance(p2, p1);
    double len23 = euclid_distance(p2, p3);
    TPOINT vec21, vec23;
    sub_point(p1, p2, &vec21);
    sub_point(p3, p2, &vec23);
    double cossit = mul_point(&vec21, &vec23) / (len21*len23);
    double sinsit = sqrt(1-cossit*cossit);
    //printf("^^^^%lf\n", len21*sinsit);
    return len21*sinsit;
}
//4 to 0, 5 to 1, we assume a>0
int myround(double a){
    int b=(int)(a+0.5);
    if(b==0)
        return 1;
    return b;
}
//
void coutPoint(TPOINT *p, const char *end=""){
    std::cout<<"("<<p->x<<", "<<p->y<<", "<<p->z<<") "<<std::endl;
}

//2D->3D
void point2Dto3D(int bid){
    vertexBuf[bid] = new TPOINT[pddlPoint2DNum];
    vertexTopBuf[bid] = new TPOINT[pddlPoint2DNum];
    vertexNum[bid] = pddlPoint2DNum;	//Poly's all bottm vertex. yuwj
    for(int i=0; i<vertexNum[bid]; i++){
        vertexBuf[bid][i].x = vertexTopBuf[bid][i].x = pddlPoint2DBuf[i].x + tempblock2[bid].basePoint.x;
        vertexBuf[bid][i].y = vertexTopBuf[bid][i].y = pddlPoint2DBuf[i].y + tempblock2[bid].basePoint.y;
        vertexBuf[bid][i].z = tempblock2[bid].basePoint.z;
        vertexTopBuf[bid][i].z = tempblock2[bid].zAxis.z + tempblock2[bid].basePoint.z;
    }
}
//save 3D
//bid: block id
void point3D_save(int bid){
    vertexNum[bid] = pddlOutLoopPoint3DNum;
    vertexBuf[bid] = new TPOINT[vertexNum[bid]];
    vertexTopBuf[bid] = new TPOINT[vertexNum[bid]];
    for(int i=0; i<vertexNum[bid]; i++){
        if(i<vertexNum[bid]/2){
            vertexBuf[bid][i] = pddlOutLoopPoint3DBuf[i];
        }else{
            vertexTopBuf[bid][i-vertexNum[bid]/2] = pddlOutLoopPoint3DBuf[i];
        }
    }
}

bool parallel(TPOINT* p1, TPOINT* p2, TPOINT* p3, TPOINT* p4){
    TVECTOR v1,v2;
    v1.x=p2->x-p1->x;
    v1.y=p2->y-p1->y;
    v1.z=p2->z-p1->z;
    v2.x=p4->x-p3->x;
    v2.y=p4->y-p3->y;
    v2.z=p4->z-p3->z;
    if(v1.y*v2.z-v1.z*v2.y < LIMIT
            && v1.z*v2.x-v1.x*v2.z < LIMIT
            && v1.x*v2.y-v1.y*v2.x < LIMIT)
        return true;
    return false;
}

void reorder(TPOINT* buf, int num){
    TPOINT* copy;
    copy = new TPOINT[num];
    int i;
    for(i=0; i<num; i++){
        copy[i] = buf[i];
    }
    double cx, cy;
    int cid = 0;
    cx = buf[0].x; cy = buf[0].y;
    for(i=1; i<num; i++){
        if(buf[i].y < cy){
            cid = i;
            cy=buf[i].y;
            cx=buf[i].x;
        }
        else if(buf[i].y == cy && buf[i].x < cx){
            cid = i;
            cy=buf[i].y;
            cx=buf[i].x;
        }
    }
    for(i=0; i<num; i++){
        buf[i] = copy[(cid+i)%num];
    }
}
bool equal_point(TPOINT *p1, TPOINT *p2){
    if(p1==NULL || p2==NULL)
        return false;
    return p1->x==p2->x && p1->y==p2->y && p1->z==p2->z;
}
bool ptInIntf(TPOINT *p1){
    TVECTOR v;
    v.x=p1->x-pddlInterfacePoint3DBuf[0].x;
    v.y=p1->y-pddlInterfacePoint3DBuf[0].y;
    v.z=p1->z-pddlInterfacePoint3DBuf[0].z;
    double len;
    len = mul_point(&v, &gvNormal)/normalLen;
    //cout<<"**********point to surface len="<<len<<endl;
    if(fabs(len) < LIMIT)
        return true;
    return false;
}
bool in_interface(TPOINT *p1, TPOINT *p2, TPOINT *p3, TPOINT *p4=NULL){
    int tsum=0;
    if(ptInIntf(p1))
        tsum++;	
    if(ptInIntf(p2))
        tsum++;
    if(ptInIntf(p3))
        tsum++;
    if(tsum == 3)
        return true;
    else
        return false;
}
bool isInInterface(const TPOINT p1[], const TPOINT p[]){
    bool isSameZ = true;
    for( int i= 0 ; i < 3; i++){
        if(!MbFltEQ(p1[i].z,p1[i+1].z) &&  !MbFltEQ(p[i].z,p[i+1].z)){
            isSameZ = false;
        }
    }
    if(isSameZ){
        if(MbFltEQ(p1[0].z, p[0].z)){
            return true;
        }else {
            return false;
        }
    }else{
        std::cout << "The bottom plane of conductor or block is not parallel with X plane! " << std :: endl;
        exit(1);
    }
    
}
//mian ji: triangle
double tri_cover(TPOINT* p1, TPOINT* p2, TPOINT* p3){
    double base=euclid_distance(p2, p3);
    double height=len_ptol(p1, p2, p3);
    return 0.5*base*height;
}
//mian ji: quadrangle
double quad_cover(TPOINT* p1, TPOINT* p2, TPOINT* p3, TPOINT* p4){
    TPOINT *tp1, *tp2, *tp3, *tp4;
    if(parallel(p1, p2, p3, p4)){
        tp1=p1; tp2=p2; tp3=p3; tp4=p4;
    }
    else if(parallel(p2, p3, p4, p1)){
        tp1=p2; tp2=p3; tp3=p4; tp4=p1;
    }
    else{
        cout << "quad's points order is not good,I cannot handle it,exit...\n";
        coutPoint(p1);
        coutPoint(p2);
        coutPoint(p3);
        coutPoint(p4);
        cout<<endl;
        exit(1);
    }
    double base1=euclid_distance(tp1, tp2);
    double base2=euclid_distance(tp3, tp4);
    double height=len_ptol(tp3, tp1, tp2);
    return 0.5*(base1+base2)*height;
}
//return eS;	//diel*S
double ele_cover(int bid, int id){
    if(gBdElements[bid][id].type > 9 && gBdElements[bid][id].type < 16){ //triangle
        return tri_cover(&gBdElements[bid][id].leftdown, &gBdElements[bid][id].rightdown, &gBdElements[bid][id].rightup);
    }
    else{
        return quad_cover(&gBdElements[bid][id].leftdown, &gBdElements[bid][id].rightdown, &gBdElements[bid][id].rightup, &gBdElements[bid][id].leftup);
    }
}
double ele_coverbcm(int bid, int id){
    if(gBdElementsbcm[bid][id].type > 9 && gBdElementsbcm[bid][id].type < 16){ //triangle
        return tri_cover(&gBdElementsbcm[bid][id].leftdown, &gBdElementsbcm[bid][id].rightdown, &gBdElementsbcm[bid][id].rightup);
    }
    else{
        return quad_cover(&gBdElementsbcm[bid][id].leftdown, &gBdElementsbcm[bid][id].rightdown, &gBdElementsbcm[bid][id].rightup, &gBdElementsbcm[bid][id].leftup);
    }
}
//reverse points order
void reverse_points(TBDELEINFO *pe){
    if(pe->type > 9 && pe->type < 16){ //triangle
        TPOINT cache=pe->leftdown;
        pe->leftdown = pe->rightdown;
        pe->rightdown = cache;		
    }else{
        TPOINT cache=pe->leftdown;
        pe->leftdown = pe->rightdown;
        pe->rightdown = cache;
        cache=pe->leftup;
        pe->leftup = pe->rightup;
        pe->rightup = cache;
    }
}
#endif
