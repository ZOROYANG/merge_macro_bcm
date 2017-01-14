#ifndef _LINEAREQUATION_H
#define _LINEAREQUATION_H

#include "frwbcm.h"

void nrerror(const char *error_text)
/* Numerical Recipes standard error handler */
{
    fprintf(stderr,"Numerical Recipes run-time error...\n");
    fprintf(stderr,"%s\n",error_text);
    fprintf(stderr,"...now exiting to system...\n");
    exit(1);
}

void ludcmp(double **a, int n, int *indx, double *d)
{
    int i,imax,j,k;
    double big,dum,sum,temp;
    double *vv;

    //vv=vector(n);
    vv=new double[n];
    *d=1.0;
    for (i=0;i<n;i++) {
        big=0.0;
        for (j=0;j<n;j++)
            if ((temp=fabs(a[i][j])) > big) big=temp;
        if (big == 0.0) nrerror("Singular matrix in routine ludcmp");
        vv[i]=1.0/big;
    }
    for (j=0;j<n;j++) {
        for (i=0;i<j;i++) {
            sum=a[i][j];
            for (k=0;k<i;k++) sum -= a[i][k]*a[k][j];
            a[i][j]=sum;
        }
        big=0.0;
        for (i=j;i<n;i++) {
            sum=a[i][j];
            for (k=0;k<j;k++)
                sum -= a[i][k]*a[k][j];
            a[i][j]=sum;
            if ( (dum=vv[i]*fabs(sum)) >= big) {
                big=dum;
                imax=i;
            }
        }
        if (j != imax) {
            for (k=0;k<n;k++) {
                dum=a[imax][k];
                a[imax][k]=a[j][k];
                a[j][k]=dum;
            }
            *d = -(*d);
            vv[imax]=vv[j];
        }
        indx[j]=imax;
        if (a[j][j] == 0.0) a[j][j]=TINY;
        if (j != n-1) {
            dum=1.0/(a[j][j]);
            for (i=j+1;i<n;i++) a[i][j] *= dum;
        }
    }
    //free_vector(vv);
    delete[] vv;
}
//ax=b 
void lubksb(double **a, int n, int *indx, double b[])
{
    int i,ii=-1,ip,j;
    double sum;

    for (i=0;i<n;i++) {
        ip=indx[i];
        sum=b[ip];
        b[ip]=b[i];
        if (ii>-1)
            for (j=ii;j<i;j++) sum -= a[i][j]*b[j];
        else if (sum) ii=i;
        b[i]=sum;
    }
    for (i=n-1;i>=0;i--) {
        sum=b[i];
        for (j=i+1;j<n;j++) sum -= a[i][j]*b[j];
        b[i]=sum/a[i][i];
    }
}
//
/*void linear_equation_solve(int n, double **a, double *b, int bid=0){
    double p;
    g_indx[bid] = new lapack_int[n];	//1->n
    ludcmp(a, n, g_indx[bid], &p);
    lubksb(a, n, g_indx[bid], b);
}*/
//use lapacke
void linear_equation_solve(int num, double *a, double *b, int bid=0){
    lapack_int m, n;
    m = (lapack_int)num;
    n = (lapack_int)num;
    g_indx[bid] = new lapack_int[num];
    float seconds,seconds2;
    struct tms  ti1,ti2,ti3;
    times(&ti1);
    LAPACKE_dgetrf(LAPACK_ROW_MAJOR, m, n, a, m, g_indx[bid]);
    (void) times(&ti2);
    LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'N', n, 1, a, m, g_indx[bid], b, 1);
    (void) times(&ti3);
    seconds = (float)(ti2.tms_utime - ti1.tms_utime)/60.0;	
    seconds2 = (float)(ti3.tms_utime - ti2.tms_utime)/60.0;
    cout<<"LU time="<<seconds<<endl;
    cout<<"back time="<<seconds2<<endl;
    cout<<"total time="<<seconds2+seconds<<endl;
}
//use lapacke
void linear_equation_test(){
    int num=3;
    lapack_int m, n;
    lapack_int *indx;
    m = (lapack_int)num;
    n = (lapack_int)num;
    indx = new lapack_int[num];
    double a[9]={1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0};
    //double b[6]={3.0, 2.0, 1.0, 7.0, 6.0, 2.0};
    double b[6]={3.0, 7.0, 2.0, 6.0, 1.0, 2.0};
    LAPACKE_dgetrf(LAPACK_ROW_MAJOR, m, n, a, m, indx);
    LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'N', n, 2, a, m, indx, b, 2);
    cout<<"---------"<<endl;
    for(int i=0;i<2;i++){
        for(int j=0;j<n;j++){
            cout<<b[i*n+j]<<" ";
        }
        cout<<endl;
    }
    cout<<"---------"<<endl;
}
#endif
