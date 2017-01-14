/***********************************************************************
*        Name:             CONSTSYSTEM.c                               *
*        Functions:   						       *
*            1.   ElementInstall()                                     *
*               The main task of the block is to read the element      *
*               information from specified file ,then install them     *
*               into the array gopMasterElement[] ,and use the array to*
*               form the arry  ConstNode[].                            *
*        Author:  Hou JinSong                                          *
*        Starting time:  4/23/1997                                     *
***********************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include "sdef.h"
#include "elenode.h"
#include <sys/times.h>


int NUM, triNUM, Not_0_Volt, gauss_count[33];
int * eletype;
double triW[17][17],triT[17][17][2];
double W[33][32],T[33][32];
double sum0,sum1;

/***************************************************************
*
***************************************************************/
void gauss()
{
    double a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16;
    double w1,w2,w3,w4,w5,w6,w7,w8;
/*  Initialization of the the data area    */
    T[1][0]=0;			W[1][0]=2.0;
    T[2][1]=0.577350269189626;      W[2][1]=1.0;
    T[2][0]=-T[2][1];               W[2][0]=1.0;
    T[3][1]=0.774596669241483;      W[3][1]=0.555555555555556;
    T[3][2]=0.0;                    W[3][2]=0.888888888888889;
    T[3][0]=-T[3][1];               W[3][0]=W[3][1];
    T[4][1]=0.861136311594053;      W[4][1]=0.347854845137454;
    T[4][2]=-T[4][1];               W[4][2]=W[4][1];
    T[4][3]=0.339981043584856;      W[4][3]=0.652145154862546;
    T[4][0]=-T[4][3];               W[4][0]=W[4][3];
    T[5][1]=0.906179845938664;      W[5][1]=0.236926885056189;
    T[5][2]=-T[5][1];               W[5][2]=W[5][1];
    T[5][3]=0.538469310105683;      W[5][3]=0.478628670499366;
    T[5][4]=-T[5][3];               W[5][4]=W[5][3];
    T[5][0]=0;                      W[5][0]=0.568888888888889;
    T[6][1]=0.932469514203152;      W[6][1]=0.171324492379170;
    T[6][2]=-T[6][1];               W[6][2]=W[6][1];
    T[6][3]=0.661209386466265;      W[6][3]=0.360761573048139;
    T[6][4]=-T[6][3];               W[6][4]=W[6][3];
    T[6][5]=0.238619186083197;      W[6][5]=0.467913934572691;
    T[6][0]=-T[6][5];               W[6][0]=W[6][5];
    T[7][1]=0.949107912342759;      W[7][1]=0.129484966168870;
    T[7][2]=-T[7][1];               W[7][2]=W[7][1];
    T[7][3]=0.741531185599394;      W[7][3]=0.279705391489277;
    T[7][4]=-T[7][3];               W[7][4]=W[7][3];
    T[7][5]=0.405845151377397;      W[7][5]=0.381830050505119;
    T[7][6]=-T[7][5];               W[7][6]=W[7][5];
    T[7][0]=0.0;                    W[7][0]=0.417959183673469;
    T[8][1]=0.960289856497536;      W[8][1]=0.101228536290376;
    T[8][2]=-T[8][1];               W[8][2]=W[8][1];
    T[8][3]=0.796666477413627;      W[8][3]=0.222381034453374;
    T[8][4]=-T[8][3];               W[8][4]=W[8][3];
    T[8][5]=0.525532409916329;      W[8][5]=0.313706645877887;
    T[8][6]=-T[8][5];               W[8][6]=W[8][5];
    T[8][7]=0.183434642495650;      W[8][7]=0.362683783378362;
    T[8][0]=-T[8][7];               W[8][0]=W[8][7];
    T[12][0]=0.981560634246719;     W[12][0]=0.047175336386512;
    T[12][1]=-T[12][0];             W[12][1]=W[12][0];
    T[12][2]=0.904117256370475;     W[12][2]=0.106939325995318;
    T[12][3]=-T[12][2];             W[12][3]=W[12][2];
    T[12][4]=0.769902674194305;     W[12][4]=0.160078328543346;
    T[12][5]=-T[12][4];             W[12][5]=W[12][4];
    T[12][6]=0.587317954286617;     W[12][6]=0.203167426723066;
    T[12][7]=-T[12][6];             W[12][7]=W[12][6];
    T[12][8]=0.367831498998180;     W[12][8]=0.233492536538355;
    T[12][9]=-T[12][8];             W[12][9]=W[12][8];
    T[12][10]=0.125233408511469;    W[12][10]=0.249147045813403;
    T[12][11]=-T[12][10];           W[12][11]=W[12][10];
    T[16][0]=0.989400934991649932596;  W[16][0]=0.027152459411754094852;
    T[16][1]=-T[16][0];             W[16][1]=W[16][0];
    T[16][2]=0.944575023073232576078;  W[16][2]=0.062253523938647892863;
    T[16][3]=-T[16][2];             W[16][3]=W[16][2];
    T[16][4]=0.865631202387831743880;  W[16][4]=0.095158511682492784810;
    T[16][5]=-T[16][4];             W[16][5]=W[16][4];
    T[16][6]=0.755404408355003033895;  W[16][6]=0.124628971255533872052;
    T[16][7]=-T[16][6];             W[16][7]=W[16][6];
    T[16][8]=0.617876244402643748447;  W[16][8]=0.149595988816576732081;
    T[16][9]=-T[16][8];             W[16][9]=W[16][8];
    T[16][10]=0.458016777657227386342; W[16][10]=0.169156519395002538189;
    T[16][11]=-T[16][10];           W[16][11]=W[16][10];
    T[16][12]=0.281603550779258913230; W[16][12]=0.182603415044923588867;
    T[16][13]=-T[16][12];           W[16][13]=W[16][12];
    T[16][14]=0.095012509837637440185; W[16][14]=0.189450610455068496285;
    T[16][15]=-T[16][14];           W[16][15]=W[16][14];
    T[20][0]=0.993128599185094924786;  W[20][0]=0.017614007139152118312;
    T[20][1]=-T[20][0];             W[20][1]=W[20][0];
    T[20][2]=0.963971927277913791268;  W[20][2]=0.040601429800386941331;
    T[20][3]=-T[20][2];             W[20][3]=W[20][2];
    T[20][4]=0.912234428251325905868;  W[20][4]=0.062672048334109063570;
    T[20][5]=-T[20][4];             W[20][5]=W[20][4];
    T[20][6]=0.839116971822218823395;  W[20][6]=0.083276741576704748725;
    T[20][7]=-T[20][6];             W[20][7]=W[20][6];
    T[20][8]=0.746331906460150792614;  W[20][8]=0.101930119817240435037;
    T[20][9]=-T[20][8];             W[20][9]=W[20][8];
    T[20][10]=0.636053680726515025453; W[20][10]=0.118194531961518417312;
    T[20][11]=-T[20][10];           W[20][11]=W[20][10];
    T[20][12]=0.510867001950827098004; W[20][12]=0.131688638449176626898;
    T[20][13]=-T[20][12];           W[20][13]=W[20][12];
    T[20][14]=0.373706088715419560673; W[20][14]=0.142096109318382051329;
    T[20][15]=-T[20][14];           W[20][15]=W[20][14];
    T[20][16]=0.227785851141645078080; W[20][16]=0.149172986472603746788;
    T[20][17]=-T[20][16];           W[20][17]=W[20][16];
    T[20][18]=0.076526521133497333755; W[20][18]=0.152753387130725850698;
    T[20][19]=-T[20][18];           W[20][19]=W[20][18];
    T[24][0]=0.995187219997021360180;  W[24][0]=0.012341229799987199547;
    T[24][1]=-T[24][0];             W[24][1]=W[24][0];
    T[24][2]=0.974728555971309498198;  W[24][2]=0.028531388628933663181;
    T[24][3]=-T[24][2];             W[24][3]=W[24][2];
    T[24][4]=0.938274552002732758524;  W[24][4]=0.044277438817419806169;
    T[24][5]=-T[24][4];             W[24][5]=W[24][4];
    T[24][6]=0.886415527004401034213;  W[24][6]=0.059298584915436780746;
    T[24][7]=-T[24][6];             W[24][7]=W[24][6];
    T[24][8]=0.820001985973902921954;  W[24][8]=0.073346481411080305734;
    T[24][9]=-T[24][8];             W[24][9]=W[24][8];
    T[24][10]=0.740124191578554364244; W[24][10]=0.086190161531953275917;
    T[24][11]=-T[24][10];           W[24][11]=W[24][10];
    T[24][12]=0.648093651936975569252; W[24][12]=0.097618652104113888270;
    T[24][13]=-T[24][12];           W[24][13]=W[24][12];
    T[24][14]=0.545421471388839535656; W[24][14]=0.107444270115965634783;
    T[24][15]=-T[24][14];           W[24][15]=W[24][14];
    T[24][16]=0.433793507626045138487; W[24][16]=0.115505668053725601353;
    T[24][17]=-T[24][16];           W[24][17]=W[24][16];
    T[24][18]=0.315042679696163374387; W[24][18]=0.121670472927803391204;
    T[24][19]=-T[24][18];           W[24][19]=W[24][18];
    T[24][20]=0.191118867473616309159; W[24][20]=0.125837456346828296121;
    T[24][21]=-T[24][20];           W[24][21]=W[24][20];
    T[24][22]=0.064056892862605626805; W[24][22]=0.127938195346752156974;
    T[24][23]=-T[24][22];           W[24][23]=W[24][22];
    T[32][0]=0.997263861849481563545;  W[32][0]=0.007018610009470096600;
    T[32][1]=-T[32][0];             W[32][1]=W[32][0];
    T[32][2]=0.985611511545268335400;  W[32][2]=0.016274394730905670605;
    T[32][3]=-T[32][2];             W[32][3]=W[32][2];
    T[32][4]=0.964762255587506430774;  W[32][4]=0.025392065309262059456;
    T[32][5]=-T[32][4];             W[32][5]=W[32][4];
    T[32][6]=0.934906075937739689171;  W[32][6]=0.034273862913021433103;
    T[32][7]=-T[32][6];             W[32][7]=W[32][6];
    T[32][8]=0.896321155766052123965;  W[32][8]=0.042835898022226680657;
    T[32][9]=-T[32][8];             W[32][9]=W[32][8];
    T[32][10]=0.849367613732569970134; W[32][10]=0.050998059262376176196;
    T[32][11]=-T[32][10];           W[32][11]=W[32][10];
    T[32][12]=0.794483795967942406963; W[32][12]=0.058684093478535547145;
    T[32][13]=-T[32][12];           W[32][13]=W[32][12];
    T[32][14]=0.732182118740289680387; W[32][14]=0.065822222776361846838;
    T[32][15]=-T[32][14];           W[32][15]=W[32][14];
    T[32][16]=0.663044266930215200975; W[32][16]=0.072345794108848506225;
    T[32][17]=-T[32][16];           W[32][17]=W[32][16];
    T[32][18]=0.587715757240762329041; W[32][18]=0.078193895787070306472;
    T[32][19]=-T[32][18];           W[32][19]=W[32][18];
    T[32][20]=0.506899908932229390024; W[32][20]=0.083311924226946755222;
    T[32][21]=-T[32][20];           W[32][21]=W[32][20];
    T[32][22]=0.421351276130635345364; W[32][22]=0.087652093004403811143;
    T[32][23]=-T[32][22];           W[32][23]=W[32][22];
    T[32][24]=0.331868602282127649780; W[32][24]=0.091173878695763884713;
    T[32][25]=-T[32][24];           W[32][25]=W[32][24];
    T[32][26]=0.239287362252137074545; W[32][26]=0.093844399080804565639;
    T[32][27]=-T[32][26];           W[32][27]=W[32][26];
    T[32][28]=0.144471961582796493485; W[32][28]=0.095638720079274859419;
    T[32][29]=-T[32][28];           W[32][29]=W[32][28];
    T[32][30]=0.048307665687738316235; W[32][30]=0.096540088514727800567;
    T[32][31]=-T[32][30];           W[32][31]=W[32][30];

    triT[1][1][0]=0.33333333; triT[1][1][1]=0.33333333; triW[1][1]=1.0/2;
    triT[3][1][0]=0.5;        triT[3][1][1]=0.5;        triW[3][1]=0.33333333/2;
    triT[3][2][0]=0;          triT[3][2][1]=0.5;        triW[3][2]=0.33333333/2;
    triT[3][3][0]=0.5;        triT[3][3][1]=0;          triW[3][3]=0.33333333/2;
    triT[4][1][0]=0.33333333; triT[4][1][1]=0.33333333; triW[4][1]=-9.0/16.0/2;
    triT[4][2][0]=0.6;        triT[4][2][1]=0.2;        triW[4][2]=25.0/48.0/2;
    triT[4][3][0]=0.2;        triT[4][3][1]=0.6;        triW[4][3]=25.0/48.0/2;
    triT[4][4][0]=0.2;        triT[4][4][1]=0.2;        triW[4][4]=25.0/48.0/2;
    triT[7][1][0]=0.33333333; triT[7][1][1]=0.33333333; triW[7][1]=0.22500000/2;
    triT[7][2][0]=0.79742699; triT[7][2][1]=0.10128651; triW[7][2]=0.12593918/2;
    triT[7][3][0]=0.10128651; triT[7][3][1]=0.79742699; triW[7][3]=triW[7][2];
    triT[7][4][0]=0.10128651; triT[7][4][1]=0.10128651; triW[7][4]=0.12593918/2;
    triT[7][5][0]=0.05971587; triT[7][5][1]=0.47014206; triW[7][5]=0.13239415/2;
    triT[7][6][0]=0.47014206; triT[7][6][1]=0.05971587; triW[7][6]=0.13239415/2;
    triT[7][7][0]=0.47014206; triT[7][7][1]=0.47014206; triW[7][7]=0.13239415/2;

    triT[13][1][0]=0.0651301029 ; triT[13][1][1]=0.0651301029 ;
    triW[13][1] = 0.0533472356/2;
    triT[13][2][0]=0.8697297941 ; triT[13][2][1]=0.0651301029 ;
    triW[13][2] = 0.0533472356/2;
    triT[13][3][0]=0.0651301029 ; triT[13][3][1]=0.8697297941 ;
    triW[13][3] = 0.0533472356/2;
    triT[13][4][0]=0.3128654960 ; triT[13][4][1]=0.0486903154 ;
    triW[13][4] = 0.0771137608/2;
    triT[13][5][0]=0.6384441885 ; triT[13][5][1]=0.3128654960 ;
    triW[13][5] = 0.0771137608/2;
    triT[13][6][0]=0.0486903154 ; triT[13][6][1]=0.6384441885 ;
    triW[13][6] = 0.0771137608/2;
    triT[13][7][0]=0.6384441885;  triT[13][7][1]=0.0486903154 ;
    triW[13][7] = 0.0771137608/2;
    triT[13][8][0]=0.3128654960;  triT[13][8][1]=0.6384441885 ;
    triW[13][8] = 0.0771137608/2;
    triT[13][9][0]=0.0486903154;  triT[13][9][1]=0.3128654960 ;
    triW[13][9] = 0.0771137608/2;
    triT[13][10][0]=0.2603459660; triT[13][10][1]=0.2603459660;
    triW[13][10]= 0.1756152574/2;  
    triT[13][11][0]=0.4793080678; triT[13][11][1]=0.2603459600;
    triW[13][11]= 0.1756152574/2;
    triT[13][12][0]=0.2603459660; triT[13][12][1]=0.4793080678;
    triW[13][12]= 0.1756152574/2;
    triT[13][13][0]=0.3333333333; triT[13][13][1]=0.3333333333;
    triW[13][13]= -0.1495700444/2;

    a1 = 0.00970378512 ;  a2 = 0.02891208422;   a3 = 0.04612207989;
    a4 = 0.05021012321 ;  a5 = 0.06546699455;   a6 = 0.09363778441;
    a7 = 0.13005607918 ;  a8 = 0.13741910412;   a9 = 0.23864865974;
    a10= 0.27899046348 ;  a11= 0.31116455224;   a12= 0.38749748338;
    a13= 0.48450832666 ;  a14= 0.63173125166;   a15= 0.67294686319;
    a16= 0.87742880935 ;
    w1 = 0.005423225910;  w2 = 0.010167259651;  w3 = 0.022584049287;
    w4 = 0.023568368199;  w5 = 0.035388067900;  w6 = 0.042339725518;
    w7 = 0.044185088522;  w8 = 0.066344216093;
    
    triT[16][1][0] = a7;  triT[16][1][1] = a1;  triW[16][1] = w1;
    triT[16][2][0] = a12;  triT[16][2][1] = a2;  triW[16][2] = w3;
    triT[16][3][0] = a15;  triT[16][3][1] = a4;  triW[16][3] = w5;
    triT[16][4][0] = a16;  triT[16][4][1] = a5;  triW[16][4] = w4;
    triT[16][5][0] = a6;  triT[16][5][1] = a3;  triW[16][5] = w2;
    triT[16][6][0] = a10;  triT[16][6][1] = a8;  triW[16][6] = w6;
    triT[16][7][0] = a13;  triT[16][7][1] = a9;  triW[16][7] = w8;
    triT[16][8][0] = a14;  triT[16][8][1] = a11;  triW[16][8] = w7;
    triT[16][9][0] = a3;  triT[16][9][1] = a6;  triW[16][9] = w2;
    triT[16][10][0] = a8;  triT[16][10][1] = a10;  triW[16][10] = w6;
    triT[16][11][0] = a9;  triT[16][11][1] = a13;  triW[16][11] = w8;
    triT[16][12][0] = a11;  triT[16][12][1] = a14;  triW[16][12] = w7;
    triT[16][13][0] = a1;  triT[16][13][1] = a7;  triW[16][13] = w1;
    triT[16][14][0] = a2;  triT[16][14][1] = a12;  triW[16][14] = w3;
    triT[16][15][0] = a4;  triT[16][15][1] = a15;  triW[16][15] = w5;
    triT[16][16][0] = a5;  triT[16][16][1] = a16;  triW[16][16] = w4;


/*    The end of initialization   */
}/* end of gauss() */

double distance(double x,double y,double z,double X[4],double Y[4],double Z[4])
{
    double a,b,c,a1,b1,c1,d1,e1,f1,tmp;
    a1 = Y[2]-Y[1];
    b1 = Z[3]-Z[1];
    c1 = Y[3]-Y[1];
    d1 = Z[2]-Z[1];
    e1 = X[3]-X[1];
    f1 = X[2]-X[1];
    a=a1*b1-c1*d1;
    b=e1*d1-f1*b1;
    c=f1*c1-e1*a1;
    
    tmp=a*(X[1]-x)+b*(Y[1]-y)+c*(Z[1]-z);
    return(tmp/sqrt(a*a+b*b+c*c));
}


inline double New_distance(double p,double p1,double p2)
//double	p,p1,p2;
{
    double R;

    if(p1<p2)
        if(p<p1)
        R=fabs(p1-p);
        else if(p>p2)
        R=fabs(p2-p);
        else
        R=0;
    else
        if(p<p2)
        R=fabs(p2-p);
        else if(p>p1)
        R=fabs(p1-p);
        else
        R=0;
    return(R);
}

inline void Gauss_points(double t)
{
    if(t>=5.0)
        NUM=1;
    else if(t>=1.2)
        NUM=2;
    else if(t>=1.0)
        NUM=4;
    else if(t>=0.8)
        NUM=6;
    else if(t>=0.55)
        NUM=8;
    else if(t>=0.17)
        NUM=12;
    else
        NUM=32;

    gauss_count[NUM]++;
}

inline int Signbit(double value)
{
    if(value<0)
        return (1);
    else
        return (0);
}

inline double my_asin(double para)
{
    if(para>1.0)
        return(asin(1.0));
    else if(para<-1.0)
        return(asin(-1.0));
    else
        return(asin(para));

}

inline double my_log(double para)
{
    if(para<LIMIT)
        return(0.0);
    else
        return(log(para));
}

void	Semi_Analytical(double x,double y,double z,double X[4],double Y[4],double Z[4])
//double	x,y,z,X[4],Y[4],Z[4];
{
    int i;
    double a,b,c,d,e,f,g,h,p,q,dist;

    dist=Z[0]-z; c=dist*dist;

    if(Signbit((X[1]-X[0])*(Y[3]-Y[0])-(X[3]-X[0])*(Y[1]-Y[0])))
        dist*=-1;

    if(X[2]>X[0])
    {
        b=X[2]-x; a=X[0]-x;
    }
    else
    {
        b=X[0]-x; a=X[2]-x;
    }
    g=a*a;
    h=b*b;
    d=(Y[2]+Y[0])/2;
    e=fabs(Y[2]-Y[0])/2;

    if(Not_0_Volt&&fabs(Z[0]-z)>LIMIT)
        for(i=0;i<NUM;i++)
        {
        f=d+e*T[NUM][i]-y;	f=f*f;
        p=sqrt(g+c+f);	q=sqrt(h+c+f);
        sum0+=log((b+q)/(a+p))*W[NUM][i];
        sum1-=(b/q-a/p)*W[NUM][i]/(f+c);
        }
    else
        for(i=0;i<NUM;i++)
        {
        f=d+e*T[NUM][i]-y;	f=f*f;
        p=sqrt(g+c+f);	q=sqrt(h+c+f);
        if(fabs(a+p)>LIMIT)
            sum0+=my_log((b+q)/(a+p))*W[NUM][i];
        }

    sum0*=e;
    sum1*=e*dist;
}

void	Analytical(double x,double y,double z,double X[4],double Y[4],double Z[4])
{
    double  a1,a2,b1,b2,c,c2,d1,d2,d3,d4,e1,e2,g,h,p,q,dist;
    c=Z[0]-z;       c2=c*c;

    if(Signbit((X[1]-X[0])*(Y[3]-Y[0])-(X[3]-X[0])*(Y[1]-Y[0])))
        dist=-c;
    else
        dist=c;

    if(X[2]>X[0])
    {
        a2=X[2]-x; a1=X[0]-x;
    }
    else
    {
        a1=X[2]-x; a2=X[0]-x;
    }

    if(Y[2]>Y[0])
    {
        b2=Y[2]-y; b1=Y[0]-y;
    }
    else
    {
        b1=Y[2]-y; b2=Y[0]-y;
    }

    g=a2*a2; h=a1*a1;
    p=b2*b2; q=b1*b1;
    d1=sqrt(p+g+c2); d2=sqrt(q+h+c2);
    d3=sqrt(p+h+c2); d4=sqrt(q+g+c2);
    e1=sqrt(g+c2);   e2=sqrt(h+c2);

    if(fabs(c)>LIMIT)
    {
        sum0+=b2*log((a2+d1)/(a1+d3));
        sum0+=b1*log((a1+d2)/(a2+d4));
        if(b1>=0&&b2>=0)
        {
            sum0+=a2*log((fabs(b2)+d1)/(fabs(b1)+d4));
            sum0+=a1*log((fabs(b1)+d2)/(fabs(b2)+d3));

            sum0+=fabs(c)*(-my_asin((a2*d1+g+c2)/((a2+d1)*e1))
            -my_asin((a1*d2+h+c2)/((a1+d2)*e2))
            +my_asin((a1*d3+h+c2)/((a1+d3)*e2))
            +my_asin((a2*d4+g+c2)/((a2+d4)*e1)));
        }
        else if(b1<0&&b2>=0)
        {
            sum0+=a2*log((fabs(b2)+d1)*(fabs(b1)+d4)/(e1*e1));
            sum0-=a1*log((fabs(b1)+d2)*(fabs(b2)+d3)/(e2*e2));

            sum0+=fabs(c)*(-my_asin((a2*d1+g+c2)/((a2+d1)*e1))
            +my_asin((a1*d2+h+c2)/((a1+d2)*e2))
            +my_asin((a1*d3+h+c2)/((a1+d3)*e2))
            -my_asin((a2*d4+g+c2)/((a2+d4)*e1))
            +2*my_asin((a2*e1+g+c2)/((a2+e1)*e1))
            -2*my_asin((a1*e2+h+c2)/((a1+e2)*e2)));
        }
        else if(b1<0&&b2<0)
        {
            sum0+=a2*log((fabs(b1)+d4)/(fabs(b2)+d1));
            sum0+=a1*log((fabs(b2)+d3)/(fabs(b1)+d2));

            sum0+=fabs(c)*(+my_asin((a2*d1+g+c2)/((a2+d1)*e1))
            +my_asin((a1*d2+h+c2)/((a1+d2)*e2))
            -my_asin((a1*d3+h+c2)/((a1+d3)*e2))
            -my_asin((a2*d4+g+c2)/((a2+d4)*e1)));
        }

        if(Not_0_Volt)
        {
            sum1-=atan(a2*b2/(c*d1));
            sum1-=atan(a1*b1/(c*d2));
            sum1+=atan(a1*b2/(c*d3));
            sum1+=atan(a2*b1/(c*d4));
            sum1/=c;
        }
    }
    else
    {
        if(b1>=0&&b2>=0)
        {
            if(fabs(a2)>LIMIT)
            {
                if(fabs(a1)>LIMIT)
                {
            if(fabs(a1+d3)>LIMIT)
                        sum0+=b2*my_log((a2+d1)/(a1+d3));
            if(fabs(a2+d4)>LIMIT)
                        sum0+=b1*my_log((a1+d2)/(a2+d4));
                    sum0+=a2*log((fabs(b2)+d1)/(fabs(b1)+d4));
                    sum0+=a1*log((fabs(b1)+d2)/(fabs(b2)+d3));
                }
                else
                {
            if(fabs(b2)>LIMIT)
                        sum0+=b2*my_log((a2+d1)/fabs(b2));
            if(fabs(a2+d4)>LIMIT)
                        sum0+=b1*my_log(fabs(b1)/(a2+d4));
                    sum0+=a2*log((fabs(b2)+d1)/(fabs(b1)+d4));
                }
            }
            else
            {
                if(fabs(a1)>LIMIT)
                {
            if(fabs(a1+d3)>LIMIT)
                        sum0+=b2*my_log(fabs(b2)/(a1+d3));
            if(fabs(b1)>LIMIT)
                        sum0+=b1*my_log((a1+d2)/fabs(b1));
                    sum0+=a1*log((fabs(b1)+d2)/(fabs(b2)+d3));
                }
            }
        }
        else if(b1<0&&b2>=0)
        {
            if(fabs(a2)>LIMIT)
            {
                if(fabs(a1)>LIMIT)
                {
            if(fabs(a1+d3)>LIMIT)
                        sum0+=b2*my_log((a2+d1)/(a1+d3));
            if(fabs(a2+d4)>LIMIT)
                        sum0+=b1*my_log((a1+d2)/(a2+d4));
                    sum0+=a2*log((fabs(b2)+d1)*(fabs(b1)+d4)/g);
                    sum0-=a1*log((fabs(b1)+d2)*(fabs(b2)+d3)/h);
                }
                else
                {
            if(fabs(b2)>LIMIT)
                        sum0+=b2*my_log((a2+d1)/fabs(b2));
            if(fabs(a2+d4)>LIMIT)
                        sum0+=b1*my_log(fabs(b1)/(a2+d4));
                    sum0+=a2*log((fabs(b2)+d1)*(fabs(b1)+d4)/g);
                }
            }
            else
            {
                if(fabs(a1)>LIMIT)
                {
            if(fabs(a1+d3)>LIMIT)
                        sum0+=b2*my_log(fabs(b2)/(a1+d3));
            if(fabs(b1)>LIMIT)
                        sum0+=b1*my_log((a1+d2)/fabs(b1));
                    sum0-=a1*log((fabs(b1)+d2)*(fabs(b2)+d3)/h);
                }
            }
        }
        else if(b1<0&&b2<0)
        {
            if(fabs(a2)>LIMIT)
            {
                if(fabs(a1)>LIMIT)
                {
            if(fabs(a1+d3)>LIMIT)
                        sum0+=b2*my_log((a2+d1)/(a1+d3));
            if(fabs(a2+d4)>LIMIT)
                        sum0+=b1*my_log((a1+d2)/(a2+d4));
                    sum0+=a2*log((fabs(b1)+d4)/(fabs(b2)+d1));
                    sum0+=a1*log((fabs(b2)+d3)/(fabs(b1)+d2));
                }
                else
                {
            if(fabs(b2)>LIMIT)
                        sum0+=b2*my_log((a2+d1)/fabs(b2));
            if(fabs(a2+d4)>LIMIT)
                        sum0+=b1*my_log(fabs(b1)/(a2+d4));
                    sum0+=a2*log((fabs(b1)+d4)/(fabs(b2)+d1));
                }
            }
            else
            {
                if(fabs(a1)>LIMIT)
                {
            if(fabs(a1+d3)>LIMIT)
                        sum0+=b2*my_log(fabs(b2)/(a1+d3));
            if(fabs(b1)>LIMIT)
                        sum0+=b1*my_log((a1+d2)/fabs(b1));
                    sum0+=a1*log((fabs(b2)+d3)/(fabs(b1)+d2));
                }
            }
        }
    }
    sum1*=dist;
}


void	Sum_Parallelogram(double x,double y,double z,double X[4],double Y[4],double Z[4])
//double	x,y,z,X[4],Y[4],Z[4];
{
    int	i;
    double	a,b,c,d,e,f,g,g1,h,h1,p,q,R,t,k,dist;

    dist=Z[0]-z; c=dist*dist;

    if(Signbit((X[1]-X[0])*(Y[3]-Y[0])-(X[3]-X[0])*(Y[1]-Y[0])))
        dist*=-1;

    if(fabs(Y[1]-Y[0])<LIMIT)
    {
        k=(X[3]-X[0])/(Y[3]-Y[0]);
        if(X[0]<X[1])
        {
        a=X[0]-k*Y[0]-x; b=X[1]-k*Y[1]-x;
        }
        else
        {
        a=X[1]-k*Y[1]-x; b=X[0]-k*Y[0]-x;
        }
    }
    else
    {
        k=(X[1]-X[0])/(Y[1]-Y[0]);
        if(X[0]<X[3])
        {
        a=X[0]-k*Y[0]-x; b=X[3]-k*Y[3]-x;
        }
        else
        {
        a=X[3]-k*Y[3]-x; b=X[0]-k*Y[0]-x;
        }
    }

    d=(Y[2]+Y[0])/2;
    e=fabs(Y[2]-Y[0])/2;

    R=New_distance(y,Y[0],Y[2]);
    t=sqrt(R*R+c)/fabs(Y[2]-Y[0]);

    Gauss_points(t);

    if(Not_0_Volt&&fabs(Z[0]-z)>LIMIT)
        for(i=0;i<NUM;i++)
        {
            f=d+e*T[NUM][i];	t=k*f;
            f=f-y;			f=f*f;
            g=t+a; 			g1=g*g;
            h=t+b;			h1=h*h;
            p=sqrt(g1+c+f);	q=sqrt(h1+c+f);
            sum0+=log((h+q)/(g+p))*W[NUM][i];
            sum1-=(h/q-g/p)*W[NUM][i]/(f+c);
        }
    else
        for(i=0;i<NUM;i++)
        {
            f=d+e*T[NUM][i];	t=k*f;
            f=f-y;			f=f*f;
            g=t+a; 			g1=g*g;
            h=t+b;			h1=h*h;
            p=sqrt(g1+c+f);	q=sqrt(h1+c+f);
            if(fabs(g+p)>LIMIT)
                sum0+=my_log((h+q)/(g+p))*W[NUM][i];
        }

    sum0*=e;
    sum1*=e*dist;
}

void	Sum_Triangle(double x,double y,double z,double X[4],double Y[4],double Z[4])
//double	x,y,z,X[4],Y[4],Z[4];
{
    int i,flag;
    double a,b,c,d,e,f,g,g1,h,h1,p,q,R,t,t1,t2,k1,k2,dist;

    dist=Z[0]-z; c=dist*dist;

    if(Signbit((X[1]-X[0])*(Y[2]-Y[0])-(X[2]-X[0])*(Y[1]-Y[0])))
        dist*=-1;

    if(fabs(Y[1]-Y[0])<LIMIT)
    {
        if(X[0]<X[1])
        {
            if(fabs(X[2]-X[0])<LIMIT)
            {
                k2=(X[1]-X[2])/(Y[1]-Y[2]);
                a=X[0]-x; b=X[1]-k2*Y[1]-x; flag=-1;
            }
            else if(fabs(X[2]-X[1])<LIMIT)
            {
                k1=(X[2]-X[0])/(Y[2]-Y[0]);
                a=X[0]-k1*Y[0]-x; b=X[1]-x; flag=1;
            }
        else
        {
                k1=(X[2]-X[0])/(Y[2]-Y[0]);
                k2=(X[1]-X[2])/(Y[1]-Y[2]);
                a=X[0]-k1*Y[0]-x; b=X[1]-k2*Y[1]-x; flag=0;
        }
        }
        else
        {
            if(fabs(X[2]-X[0])<LIMIT)
            {
                k1=(X[1]-X[2])/(Y[1]-Y[2]);
                a=X[1]-k1*Y[1]-x; b=X[0]-x; flag=1;
            }
            else if(fabs(X[2]-X[1])<LIMIT)
            {
                k2=(X[2]-X[0])/(Y[2]-Y[0]);
                a=X[1]-x; b=X[0]-k2*Y[0]-x; flag=-1;
            }
            else
            {
                k1=(X[1]-X[2])/(Y[1]-Y[2]);
                k2=(X[2]-X[0])/(Y[2]-Y[0]);
                a=X[1]-k1*Y[1]-x; b=X[0]-k2*Y[0]-x; flag=0;
            }
        }
        d=(Y[2]+Y[0])/2;
        e=fabs(Y[2]-Y[0])/2;
        R=New_distance(y,Y[0],Y[2]);
        t=sqrt(R*R+c)/fabs(Y[2]-Y[0]);
    }
    else if(fabs(Y[1]-Y[2])<LIMIT)
    {
        if(X[1]<X[2])
        {
            if(fabs(X[0]-X[1])<LIMIT)
            {
                k2=(X[2]-X[0])/(Y[2]-Y[0]);
                a=X[1]-x; b=X[2]-k2*Y[2]-x; flag=-1;
            }
            else if(fabs(X[0]-X[2])<LIMIT)
            {
                k1=(X[1]-X[0])/(Y[1]-Y[0]);
                a=X[1]-k1*Y[1]-x; b=X[2]-x; flag=1;
            }
            else
            {
                k1=(X[1]-X[0])/(Y[1]-Y[0]);
                k2=(X[0]-X[2])/(Y[0]-Y[2]);
                a=X[1]-k1*Y[1]-x; b=X[2]-k2*Y[2]-x; flag=0;
            }
        }
        else
        {
            if(fabs(X[1]-X[0])<LIMIT)
            {
                k1=(X[0]-X[2])/(Y[0]-Y[2]);
                a=X[2]-k1*Y[2]-x; b=X[1]-x; flag=1;
            }
            else if(fabs(X[2]-X[0])<LIMIT)
            {
                k2=(X[1]-X[0])/(Y[1]-Y[0]);
                a=X[2]-x; b=X[1]-k2*Y[1]-x; flag=-1;
            }
            else
            {
                k1=(X[0]-X[2])/(Y[0]-Y[2]);
                k2=(X[1]-X[0])/(Y[1]-Y[0]);
                a=X[2]-k1*Y[2]-x; b=X[1]-k2*Y[1]-x; flag=0;
            }
        }
        d=(Y[2]+Y[0])/2;
        e=fabs(Y[2]-Y[0])/2;
        R=New_distance(y,Y[0],Y[2]);
        t=sqrt(R*R+c)/fabs(Y[2]-Y[0]);
    }
    else 	/* fabs(Y[0]-Y[2])<LIMIT */
    {
        if(X[0]<X[2])
        {
            if(fabs(X[0]-X[1])<LIMIT)
            {
                k2=(X[2]-X[1])/(Y[2]-Y[1]);
                a=X[0]-x; b=X[2]-k2*Y[2]-x; flag=-1;
            }
            else if(fabs(X[1]-X[2])<LIMIT)
            {
                k1=(X[1]-X[0])/(Y[1]-Y[0]);
                a=X[0]-k1*Y[0]-x; b=X[2]-x; flag=1;
            }
            else
            {
                k1=(X[1]-X[0])/(Y[1]-Y[0]);
                k2=(X[1]-X[2])/(Y[1]-Y[2]);
                a=X[0]-k1*Y[0]-x; b=X[2]-k2*Y[2]-x; flag=0;
            }
        }
        else 	
        {
            if(fabs(X[1]-X[0])<LIMIT)
            {
                k1=(X[1]-X[2])/(Y[1]-Y[2]);
                a=X[2]-k1*Y[2]-x; b=X[0]-x; flag=1;
            }
            else if(fabs(X[2]-X[1])<LIMIT)
            {
                k2=(X[1]-X[0])/(Y[1]-Y[0]);
                a=X[2]-x; b=X[0]-k2*Y[0]-x; flag=-1;
            }
            else
            {
                k1=(X[1]-X[2])/(Y[1]-Y[2]);
                k2=(X[1]-X[0])/(Y[1]-Y[0]);
                a=X[2]-k1*Y[2]-x; b=X[0]-k2*Y[0]-x; flag=0;
            }
        }
        d=(Y[1]+Y[0])/2;
        e=fabs(Y[1]-Y[0])/2;
        R=New_distance(y,Y[0],Y[1]);
        t=sqrt(R*R+c)/fabs(Y[1]-Y[0]);
    }

    Gauss_points(t);

    switch(flag)
    {
    case -1:	
        if(Not_0_Volt&&fabs(Z[0]-z)>LIMIT)
        {
            for(i=0;i<NUM;i++)
            {
                f=d+e*T[NUM][i];	t2=k2*f;
                f=f-y;		f=f*f;
                g=a*a; 		
                h=t2+b;		h1=h*h;
                p=sqrt(g+c+f);	q=sqrt(h1+c+f);
                sum0+=log((h+q)/(a+p))*W[NUM][i];
                sum1-=(h/q-a/p)*W[NUM][i]/(f+c);
            }
        }
        else
        {
            for(i=0;i<NUM;i++)
            {
                f=d+e*T[NUM][i];	t2=k2*f;
                f=f-y;		f=f*f;
                g=a*a; 		
                h=t2+b;		h1=h*h;
                p=sqrt(g+c+f);	q=sqrt(h1+c+f);
                if(fabs(a+p)>LIMIT)
                    sum0+=my_log((h+q)/(a+p))*W[NUM][i];
            }
        }
        break;
    case 0:	
        if(Not_0_Volt&&fabs(Z[0]-z)>LIMIT)
        {
            for(i=0;i<NUM;i++)
            {
                f=d+e*T[NUM][i];	t1=k1*f;	t2=k2*f;
                f=f-y;		f=f*f;
                g=t1+a; 		g1=g*g;
                h=t2+b;		h1=h*h;	
                p=sqrt(g1+c+f);	q=sqrt(h1+c+f);
                sum0+=log((h+q)/(g+p))*W[NUM][i];
                sum1-=(h/q-g/p)*W[NUM][i]/(f+c);
            }
        }
        else
        {
            for(i=0;i<NUM;i++)
            {
                f=d+e*T[NUM][i];	t1=k1*f;	t2=k2*f;
                f=f-y;		f=f*f;
                g=t1+a; 		g1=g*g;
                h=t2+b;		h1=h*h;	
                p=sqrt(g1+c+f);	q=sqrt(h1+c+f);
                if(fabs(g+p)>LIMIT)
                    sum0+=my_log((h+q)/(g+p))*W[NUM][i];
            }
        }
        break;
    case 1:
        if(Not_0_Volt&&fabs(Z[0]-z)>LIMIT)
        {
            for(i=0;i<NUM;i++)
            {
                f=d+e*T[NUM][i];	t1=k1*f;
                f=f-y;		f=f*f;
                g=t1+a; 		g1=g*g;
                h=b*b;	
                p=sqrt(g1+c+f);	q=sqrt(h+c+f);
                sum0+=log((b+q)/(g+p))*W[NUM][i];
                sum1-=(b/q-g/p)*W[NUM][i]/(f+c);
            }
        }
        else
        {
            for(i=0;i<NUM;i++)
            {
                f=d+e*T[NUM][i];	t1=k1*f;
                f=f-y;		f=f*f;
                g=t1+a; 		g1=g*g;
                h=b*b;	
                p=sqrt(g1+c+f);	q=sqrt(h+c+f);
                if(fabs(g+p)>LIMIT)
                    sum0+=my_log((b+q)/(g+p))*W[NUM][i];
            }
        }
        break;
    }

    sum0*=e;
    sum1*=e*dist;
}

void	Sum_Trapezoid(double x,double y,double z,double X[4],double Y[4],double Z[4])
//double	x,y,z,X[4],Y[4],Z[4];
{
    int	i,flag;
    double	a,b,c,d,e,f,g,g1,h,h1,p,q,R,t,t1,t2,k1,k2,dist;

    dist=Z[0]-z; c=dist*dist;

    if(Signbit((X[1]-X[0])*(Y[2]-Y[0])-(X[2]-X[0])*(Y[1]-Y[0])))
        dist*=-1;

    if(fabs(Y[1]-Y[0])<LIMIT&&fabs(Y[3]-Y[2])<LIMIT)
    {
        if(X[0]<X[1])
        {
            if(fabs(X[3]-X[0])<LIMIT)
            {
                k2=(X[1]-X[2])/(Y[1]-Y[2]);
                a=X[0]-x; b=X[1]-k2*Y[1]-x; flag=-1;
            }
            else if(fabs(X[2]-X[1])<LIMIT)
            {
                k1=(X[3]-X[0])/(Y[3]-Y[0]);
                a=X[0]-k1*Y[0]-x; b=X[1]-x; flag=1;
            }
            else
            {
                k1=(X[3]-X[0])/(Y[3]-Y[0]);
                k2=(X[1]-X[2])/(Y[1]-Y[2]);
                a=X[0]-k1*Y[0]-x; b=X[1]-k2*Y[1]-x; flag=0;
            }
        }
        else
        {
            if(fabs(X[3]-X[0])<LIMIT)
            {
                k1=(X[1]-X[2])/(Y[1]-Y[2]);
                a=X[1]-k1*Y[1]-x; b=X[0]-x; flag=1;
            }
            else if(fabs(X[2]-X[1])<LIMIT)
            {
                k2=(X[3]-X[0])/(Y[3]-Y[0]);
                a=X[1]-x; b=X[0]-k2*Y[0]-x; flag=-1;
            }
            else
            {
                k1=(X[1]-X[2])/(Y[1]-Y[2]);
                k2=(X[3]-X[0])/(Y[3]-Y[0]);
                a=X[1]-k1*Y[1]-x; b=X[0]-k2*Y[0]-x; flag=0;
            }
        }
    }
    else if(fabs(Y[1]-Y[2])<LIMIT&&fabs(Y[3]-Y[0])<LIMIT)
    {
        if(X[1]<X[2])
        {
            if(fabs(X[0]-X[1])<LIMIT)
            {
                k2=(X[2]-X[3])/(Y[2]-Y[3]);
                a=X[1]-x; b=X[2]-k2*Y[2]-x; flag=-1;
            }
            else if(fabs(X[3]-X[2])<LIMIT)
            {
                k1=(X[1]-X[0])/(Y[1]-Y[0]);
                a=X[1]-k1*Y[1]-x; b=X[2]-x; flag=1;
            }
            else
            {
                k1=(X[1]-X[0])/(Y[1]-Y[0]);
                k2=(X[3]-X[2])/(Y[3]-Y[2]);
                a=X[1]-k1*Y[1]-x; b=X[2]-k2*Y[2]-x; flag=0;
            }
        }
        else
        {
            if(fabs(X[1]-X[0])<LIMIT)
            {
                k1=(X[3]-X[2])/(Y[3]-Y[2]);
                a=X[2]-k1*Y[2]-x; b=X[1]-x; flag=1;
            }
            else if(fabs(X[2]-X[3])<LIMIT)
            {
                k2=(X[1]-X[0])/(Y[1]-Y[0]);
                a=X[2]-x; b=X[1]-k2*Y[1]-x; flag=-1;
            }
            else
            {
                k1=(X[3]-X[2])/(Y[3]-Y[2]);
                k2=(X[1]-X[0])/(Y[1]-Y[0]);
                a=X[2]-k1*Y[2]-x; b=X[1]-k2*Y[1]-x; flag=0;
            }
        }
    }

    d=(Y[2]+Y[0])/2;
    e=fabs(Y[2]-Y[0])/2;
    R=New_distance(y,Y[0],Y[2]);
    t=sqrt(R*R+c)/fabs(Y[2]-Y[0]);

    Gauss_points(t);

    switch(flag)
    {
    case -1:	
        if(Not_0_Volt&&fabs(Z[0]-z)>LIMIT)
        {
            for(i=0;i<NUM;i++)
            {
                f=d+e*T[NUM][i];	t2=k2*f;
                f=f-y;		f=f*f;
                g=a*a; 		
                h=t2+b;		h1=h*h;
                p=sqrt(g+c+f);	q=sqrt(h1+c+f);
                sum0+=log((h+q)/(a+p))*W[NUM][i];
                sum1-=(h/q-a/p)*W[NUM][i]/(f+c);
            }
        }
        else
        {
            for(i=0;i<NUM;i++)
            {
                f=d+e*T[NUM][i];	t2=k2*f;
                f=f-y;		f=f*f;
                g=a*a; 		
                h=t2+b;		h1=h*h;
                p=sqrt(g+c+f);	q=sqrt(h1+c+f);
                if(fabs(a+p)>LIMIT)
                    sum0+=my_log((h+q)/(a+p))*W[NUM][i];
            }
        }
        break;
    case 0:	
        if(Not_0_Volt&&fabs(Z[0]-z)>LIMIT)
        {
            for(i=0;i<NUM;i++)
            {
                f=d+e*T[NUM][i];	t1=k1*f;	t2=k2*f;
                f=f-y;		f=f*f;
                g=t1+a; 		g1=g*g;
                h=t2+b;		h1=h*h;	
                p=sqrt(g1+c+f);	q=sqrt(h1+c+f);
                sum0+=log((h+q)/(g+p))*W[NUM][i];
                sum1-=(h/q-g/p)*W[NUM][i]/(f+c);
            }
        }
        else
        {
            for(i=0;i<NUM;i++)
            {
                f=d+e*T[NUM][i];	t1=k1*f;	t2=k2*f;
                f=f-y;		f=f*f;
                g=t1+a; 		g1=g*g;
                h=t2+b;		h1=h*h;	
                p=sqrt(g1+c+f);	q=sqrt(h1+c+f);
                if(fabs(g+p)>LIMIT)
                    sum0+=my_log((h+q)/(g+p))*W[NUM][i];
            }
        }
        break;
    case 1:
        if(Not_0_Volt&&fabs(Z[0]-z)>LIMIT)
        {
            for(i=0;i<NUM;i++)
            {
                f=d+e*T[NUM][i];	t1=k1*f;
                f=f-y;		f=f*f;
                g=t1+a; 		g1=g*g;
                h=b*b;	
                p=sqrt(g1+c+f);	q=sqrt(h+c+f);
                sum0+=my_log((b+q)/(g+p))*W[NUM][i];
                sum1-=(b/q-g/p)*W[NUM][i]/(f+c);
            }
        }
        else
        {
            for(i=0;i<NUM;i++)
            {
                f=d+e*T[NUM][i];	t1=k1*f;
                f=f-y;		f=f*f;
                g=t1+a; 		g1=g*g;
                h=b*b;	
                p=sqrt(g1+c+f);	q=sqrt(h+c+f);
                if(fabs(g+p)>LIMIT)
                    sum0+=my_log((b+q)/(g+p))*W[NUM][i];
            }
        }
        break;
    }

    sum0*=e;
    sum1*=e*dist;
}

void	Sum_Inc_Rectangle(double x,double y,double z,double X[4],double Y[4],double Z[4])
//double	x,y,z,X[4],Y[4],Z[4];
{
    int	i;
    double	a,b,c,c1,d,e,f,g,h,p,q,R,t,dist,k;

    dist=distance(x,y,z,X,Y,Z);

    if(X[2]>X[0])
    {
        b=X[2]-x; a=X[0]-x; 
    }
    else
    {
        b=X[0]-x; a=X[2]-x;
    }
    k=(Z[2]-Z[0])/(Y[2]-Y[0]);	
    c=Z[0]-k*Y[0]-z;
    g=a*a; h=b*b;
    d=(Y[2]+Y[0])/2;
    e=fabs(Y[2]-Y[0])/2;

    R=New_distance(y,Y[0],Y[2]);
    t=R/fabs(Y[2]-Y[0]);

    Gauss_points(t);

    if(Not_0_Volt)
    {
        for(i=0;i<NUM;i++)
        {
            f=d+e*T[NUM][i];	t=k*f;
            f=f-y;			f=f*f;
            c1=c+t;			c1=c1*c1;
            p=sqrt(g+c1+f);	q=sqrt(h+c1+f);
            if(fabs(a+p)>LIMIT)
                sum0+=my_log((b+q)/(a+p))*W[NUM][i];
            if(p>LIMIT&&q>LIMIT&&(f+c1)>LIMIT)
                sum1-=(b/q-a/p)*W[NUM][i]/(f+c1);
        }
    }
    else
    {
        for(i=0;i<NUM;i++)
        {
            f=d+e*T[NUM][i];	t=k*f;
            f=f-y;			f=f*f;
            c1=c+t;			c1=c1*c1;
            p=sqrt(g+c1+f);	q=sqrt(h+c1+f);
            if(fabs(a+p)>LIMIT)
                sum0+=my_log((b+q)/(a+p))*W[NUM][i];
        }
    }

    k=sqrt(1+k*k);
    sum0*=e*k;
    sum1*=e*dist*k;
}

void	Sum_Inc_Trapezoid(double x,double y,double z,double X[4],double Y[4],double Z[4])
//double	x,y,z,X[4],Y[4],Z[4];
{
    int	i;
    double	a,b,c,c1,d,e,f,g,g1,h,h1,p,q,R,t,t1,t2,dist,k,k1,k2;

    dist=distance(x,y,z,X,Y,Z);

    if(fabs(Y[1]-Y[0])<LIMIT&&fabs(Y[3]-Y[2])<LIMIT)
    {
        if(X[0]<X[1])
        {
            k1=(X[3]-X[0])/(Y[3]-Y[0]);
            k2=(X[1]-X[2])/(Y[1]-Y[2]);
            a=X[0]-k1*Y[0]-x; b=X[1]-k2*Y[1]-x;
        }
        else
        {
            k1=(X[1]-X[2])/(Y[1]-Y[2]);
            k2=(X[3]-X[0])/(Y[3]-Y[0]);
            a=X[1]-k1*Y[1]-x; b=X[0]-k2*Y[0]-x;
        }
    }
    else if(fabs(Y[1]-Y[2])<LIMIT&&fabs(Y[3]-Y[0])<LIMIT)
    {
        if(X[1]<X[2])
        {
            k1=(X[1]-X[0])/(Y[1]-Y[0]);
            k2=(X[3]-X[2])/(Y[3]-Y[2]);
            a=X[1]-k1*Y[1]-x; b=X[2]-k2*Y[2]-x;
        }
        else
        {
            k1=(X[3]-X[2])/(Y[3]-Y[2]);
            k2=(X[1]-X[0])/(Y[1]-Y[0]);
            a=X[2]-k1*Y[2]-x; b=X[1]-k2*Y[1]-x;
        }
    }

    d=(Y[2]+Y[0])/2;
    e=fabs(Y[2]-Y[0])/2;
    k=(Z[2]-Z[0])/(Y[2]-Y[0]);	
    c=Z[0]-k*Y[0]-z;

    R=New_distance(y,Y[0],Y[2]);
    t=R/fabs(Y[2]-Y[0]);

    Gauss_points(t);

    if(Not_0_Volt)
    {
        for(i=0;i<NUM;i++)
        {
        f=d+e*T[NUM][i];	t=k*f;
        t1=k1*f;        	t2=k2*f;
        f=f-y;			f=f*f;
        g=t1+a;             	g1=g*g;
        h=t2+b;             	h1=h*h;
        c1=c+t;			c1=c1*c1;
        p=sqrt(g1+c1+f);	q=sqrt(h1+c1+f);
        if(fabs(g+p)>LIMIT)
            sum0+=my_log((h+q)/(g+p))*W[NUM][i];
        if(p>LIMIT&&q>LIMIT&&(f+c1)>LIMIT)
            sum1-=(h/q-g/p)*W[NUM][i]/(f+c1);
        }
    }
    else
    {
        for(i=0;i<NUM;i++)
        {
        f=d+e*T[NUM][i];	t=k*f;
        t1=k1*f;        	t2=k2*f;
        f=f-y;			f=f*f;
        g=t1+a;             	g1=g*g;
        h=t2+b;             	h1=h*h;
        c1=c+t;			c1=c1*c1;
        p=sqrt(g1+c1+f);	q=sqrt(h1+c1+f);
        if(fabs(g+p)>LIMIT)
            sum0+=my_log((h+q)/(g+p))*W[NUM][i];
        }
    }

    k=sqrt(1+k*k);
    sum0*=e*k;
    sum1*=e*dist*k;
}

void 	New_Integration(int type,double	P[3],double X[4],double Y[4],double Z[4])
//double	P[3],X[4],Y[4],Z[4];
{
    double 	t1,t2,R1,R2,c2;

    sum0 = sum1 =0.0;
    Not_0_Volt = 1;

    switch(type)
    {
        case XOY_RECTANGLE:
            R1=New_distance(P[0],X[0],X[2]);
            R2=New_distance(P[1],Y[0],Y[2]);
            c2=(Z[0]-P[2])*(Z[0]-P[2]);
            t1=sqrt(R1*R1+c2)/fabs(X[2]-X[0]);
            t2=sqrt(R2*R2+c2)/fabs(Y[2]-Y[0]);
            if(t1<t2)
            {
                Gauss_points(t2);
                if(MbFltEq(fabs(Z[0]-P[2]),0.0))
                if(NUM>2&&!MbFltEq(t2,0.0))
                        Analytical(P[0],P[1],P[2],X,Y,Z);
                else
                        Semi_Analytical(P[0],P[1],P[2],X,Y,Z);
                else
                if(NUM>6&&!MbFltEq(t2,0.0))
                        Analytical(P[0],P[1],P[2],X,Y,Z);
                else
                        Semi_Analytical(P[0],P[1],P[2],X,Y,Z);
            }
            else
            {
                Gauss_points(t1);
                if(MbFltEq(fabs(Z[0]-P[2]),0.0))
                if(NUM>2&&!MbFltEq(t1,0.0))
                        Analytical(P[1],P[0],P[2],Y,X,Z);
                else
                        Semi_Analytical(P[1],P[0],P[2],Y,X,Z);
                else
                if(NUM>6&&!MbFltEq(t1,0.0))
                        Analytical(P[1],P[0],P[2],Y,X,Z);
                else
                        Semi_Analytical(P[1],P[0],P[2],Y,X,Z);
                sum1*=-1;
            }
            break;
        case ZOX_RECTANGLE:
            R1=New_distance(P[2],Z[0],Z[2]);
            R2=New_distance(P[0],X[0],X[2]);
            c2=(Y[0]-P[1])*(Y[0]-P[1]);
            t1=sqrt(R1*R1+c2)/fabs(Z[2]-Z[0]);
            t2=sqrt(R2*R2+c2)/fabs(X[2]-X[0]);
            if(t1<t2)
            {
                Gauss_points(t2);
                if(MbFltEq(fabs(Y[0]-P[1]),0.0))
                if(NUM>2&&!MbFltEq(t2,0.0))
                        Analytical(P[2],P[0],P[1],Z,X,Y);
                else
                        Semi_Analytical(P[2],P[0],P[1],Z,X,Y);
                else
                if(NUM>6&&!MbFltEq(t2,0.0))
                        Analytical(P[2],P[0],P[1],Z,X,Y);
                else
                        Semi_Analytical(P[2],P[0],P[1],Z,X,Y);
            }
            else
            {
                Gauss_points(t1);
                if(MbFltEq(fabs(Y[0]-P[1]),0.0))
                if(NUM>2&&!MbFltEq(t1,0.0))
                        Analytical(P[0],P[2],P[1],X,Z,Y);
                else
                        Semi_Analytical(P[0],P[2],P[1],X,Z,Y);
                else
                if(NUM>6&&!MbFltEq(t1,0.0))
                        Analytical(P[0],P[2],P[1],X,Z,Y);
                else
                        Semi_Analytical(P[0],P[2],P[1],X,Z,Y);
                sum1*=-1;
            }
            break;
        case YOZ_RECTANGLE:
            R1=New_distance(P[1],Y[0],Y[2]);
            R2=New_distance(P[2],Z[0],Z[2]);
            c2=(X[0]-P[0])*(X[0]-P[0]);
            t1=sqrt(R1*R1+c2)/fabs(Y[2]-Y[0]);
            t2=sqrt(R2*R2+c2)/fabs(Z[2]-Z[0]);
            if(t1<t2)
            {
                Gauss_points(t2);
                if(MbFltEq(fabs(X[0]-P[0]),0.0))
                if(NUM>2&&!MbFltEq(t2,0.0))
                        Analytical(P[1],P[2],P[0],Y,Z,X);
                else
                        Semi_Analytical(P[1],P[2],P[0],Y,Z,X);
                else
                if(NUM>6&&!MbFltEq(t2,0.0))
                        Analytical(P[1],P[2],P[0],Y,Z,X);
                else
                        Semi_Analytical(P[1],P[2],P[0],Y,Z,X);
            }
            else
            {
                Gauss_points(t1);
                if(MbFltEq(fabs(X[0]-P[0]),0.0))
                if(NUM>2&&!MbFltEq(t1,0.0))
                        Analytical(P[2],P[1],P[0],Z,Y,X);
                else
                        Semi_Analytical(P[2],P[1],P[0],Z,Y,X);
                else
                if(NUM>6&&!MbFltEq(t1,0.0))
                        Analytical(P[2],P[1],P[0],Z,Y,X);
                else
                        Semi_Analytical(P[2],P[1],P[0],Z,Y,X);
                sum1*=-1;
            }
            break;
        case XOY_PARALLELOGRAM:
            Sum_Parallelogram(P[0],P[1],P[2],X,Y,Z);
                        break;
        case YOX_PARALLELOGRAM:
            Sum_Parallelogram(P[1],P[0],P[2],Y,X,Z);
            sum1*=-1;
                        break;
        case YOZ_PARALLELOGRAM:
            Sum_Parallelogram(P[1],P[2],P[0],Y,Z,X);
                        break;
        case ZOY_PARALLELOGRAM:
            Sum_Parallelogram(P[2],P[1],P[0],Z,Y,X);
            sum1*=-1;
                        break;
        case ZOX_PARALLELOGRAM:
            Sum_Parallelogram(P[2],P[0],P[1],Z,X,Y);
                        break;
        case XOZ_PARALLELOGRAM:
            Sum_Parallelogram(P[0],P[2],P[1],X,Z,Y);
            sum1*=-1;
                        break;
        case XOY_TRIANGLE:
            Sum_Triangle(P[0],P[1],P[2],X,Y,Z);
                        break;
        case YOX_TRIANGLE:
            Sum_Triangle(P[1],P[0],P[2],Y,X,Z);
            sum1*=-1;
                        break;
        case YOZ_TRIANGLE:
            Sum_Triangle(P[1],P[2],P[0],Y,Z,X);
                        break;
        case ZOY_TRIANGLE:
            Sum_Triangle(P[2],P[1],P[0],Z,Y,X);
            sum1*=-1;
                        break;
        case ZOX_TRIANGLE:
            Sum_Triangle(P[2],P[0],P[1],Z,X,Y);
                        break;
        case XOZ_TRIANGLE:
            Sum_Triangle(P[0],P[2],P[1],X,Z,Y);
            sum1*=-1;
                        break;
        case XOY_TRAPEZOID:
            Sum_Trapezoid(P[0],P[1],P[2],X,Y,Z);
                        break;
        case YOX_TRAPEZOID:
            Sum_Trapezoid(P[1],P[0],P[2],Y,X,Z);
            sum1*=-1;
                        break;
        case YOZ_TRAPEZOID:
            Sum_Trapezoid(P[1],P[2],P[0],Y,Z,X);
                        break;
        case ZOY_TRAPEZOID:
            Sum_Trapezoid(P[2],P[1],P[0],Z,Y,X);
            sum1*=-1;
                        break;
        case ZOX_TRAPEZOID:
            Sum_Trapezoid(P[2],P[0],P[1],Z,X,Y);
                        break;
        case XOZ_TRAPEZOID:
            Sum_Trapezoid(P[0],P[2],P[1],X,Z,Y);
            sum1*=-1;
                        break;
        case XOY_INCLINED_RECTANGLE:
            Sum_Inc_Rectangle(P[0],P[1],P[2],X,Y,Z);
                        break;
        case YOZ_INCLINED_RECTANGLE:
            Sum_Inc_Rectangle(P[1],P[2],P[0],Y,Z,X);
                        break;
        case ZOX_INCLINED_RECTANGLE:
            Sum_Inc_Rectangle(P[2],P[0],P[1],Z,X,Y);
                        break;
        case XOY_INCLINED_TRAPEZOID:
            Sum_Inc_Trapezoid(P[0],P[1],P[2],X,Y,Z);
                        break;
        case YOZ_INCLINED_TRAPEZOID:
            Sum_Inc_Trapezoid(P[1],P[2],P[0],Y,Z,X);
                        break;
        case ZOX_INCLINED_TRAPEZOID:
            Sum_Inc_Trapezoid(P[2],P[0],P[1],Z,X,Y);
                        break;
    }
}

/********************************************************************
*
*********************************************************************/
double integration(double X[3],double Y[3],double Z[3])
//double X[3],Y[3],Z[3];

{
    double inte,dis,sinm,sinp;//tgp;
    double dis1,dis2,cosp,cosm;

    dis=sqrt((X[0]-X[1])*(X[0]-X[1])+(Y[0]-Y[1])*(Y[0]-Y[1])
                +(Z[0]-Z[1])*(Z[0]-Z[1]));
    dis1=sqrt((X[2]-X[1])*(X[2]-X[1])+(Y[2]-Y[1])*(Y[2]-Y[1])
                +(Z[2]-Z[1])*(Z[2]-Z[1]));
    dis2=sqrt((X[0]-X[2])*(X[0]-X[2])+(Y[0]-Y[2])*(Y[0]-Y[2])
                +(Z[0]-Z[2])*(Z[0]-Z[2]));

    if ( fabs(dis) < 1e-6 || fabs(dis2) < 1e-6 )
    return 0;
    cosp=(dis*dis+dis2*dis2-dis1*dis1)/(2.0*dis*dis2);
    sinp=sqrt(fabs(1.0-cosp*cosp));
    //tgp=sinp/((1.0+cosp));
    cosm=(dis1*dis1+dis*dis-dis2*dis2)/(2.0*dis1*dis);

    if ( fabs(sinp) < 1e-6 )
    return 0;
    if (fabs(cosm)>=0.000001)  
    {
    sinm=sqrt((1.00-cosm*cosm));
    double temp1=cosp*cosm;
    double temp2=sinp*sinm;
    double temp3=(1.0+cosm)/(1.0-cosm)*(1.0-temp1+temp2)/(1.0+temp1-temp2);
    inte=dis*sinm*0.5*log(temp3);
    }
    else
        inte = dis*0.5*log((1.0+sinp)/(1.0-sinp));


    return(inte);
}

/**********************************************************************
*
***********************************************************************/
double quadbak(double P[3],double X[4],double Y[4],double Z[4])
{
    int i;
    double sum=0;
    double A[3],B[3],C[3];

    A[0]=P[0];
    B[0]=P[1];
    C[0]=P[2];

    for (i=0;i<3;i++)
    {
        A[1]=X[i];
        A[2]=X[i+1];
        B[1]=Y[i];
        B[2]=Y[i+1];
        C[1]=Z[i];
        C[2]=Z[i+1];
        sum=sum+ integration(A,B,C);
    }

    A[1]=X[0];
    A[2]=X[3];
    B[1]=Y[0];
    B[2]=Y[3];
    C[1]=Z[0];
    C[2]=Z[3];

    sum=sum+ integration(A,B,C);

    return(sum);
    
}

    /*********************************************************************
    *
    **********************************************************************/
    double tri(double X[3],double Y[3],double Z[3])
    //double X[3],Y[3],Z[3];
    {

        int i;
        double sum=0,x0=0,y0=0,z0=0;
        double A[3],B[3],C[3];

        for (i=0;i<3;i++)
        {
            x0=x0+X[i];
            y0=y0+Y[i];
            z0=z0+Z[i];
        }

        x0=x0/3;
        y0=y0/3;
        z0=z0/3;

        A[0]=x0;
        B[0]=y0;
        C[0]=z0;

        for (i=0;i<2;i++)
        {
            A[1]=X[i];
            A[2]=X[i+1];
            B[1]=Y[i];
            B[2]=Y[i+1];
            C[1]=Z[i];
            C[2]=Z[i+1];
            sum=sum+ integration(A,B,C);
        }

        A[1]=X[0];
        B[1]=Y[0];
        C[1]=Z[0];

        sum=sum+ integration(A,B,C);

        return(sum);
    }

