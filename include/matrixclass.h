#ifndef _MATRIXCLASS_H_
#define _MATRIXCLASS_H_

#include <stdio.h>
#include <cmath>
#include "shape.h"
#include "lapacke.h"
#include "cblas.h"

extern void RangeSet(RANGE &range, size_t start, size_t end, size_t insert, int layerid, int conductorid);

class Matrix
{
public:
    double ** data;
    int row, col;

public:

    Matrix ()
    {
        row=col=0;
        data=NULL;
    }

    Matrix (const Matrix& a) // assign with class Matrix
    {
        int i, j;
        row= a.rows();
        col= a.cols();
        data= new double * [row];
        for(i=0; i<row; i++)
        {
            data[i]= new double [col];
            for(j=0; j<col; j++)
                data[i][j]= a(i,j);
        }
    }

    Matrix (int r, int c) // assign new element
    {
        int i;
        row= r;
        col= c;
        data= new double * [r];
        for(i=0; i<r; i++){
            data[i]= new double [c];
            for(int j = 0 ; j < c ; j++){
                data[i][j] = 0 ;
            }
        }
    }

    Matrix (int rc)	// unit matrix;
    {
        int i,j;
        row= rc;
        col= rc;
        data= new double * [rc];
        for(i=0; i<rc; i++)
        {
            data[i]= new double [rc];
            for(j=0; j<rc; j++)
                if(i == j)
                    data[i][j] = 1;
                else
                    data[i][j] = 0;
        }
    }

    Matrix (int dimension , int type, std::vector<std::vector<RANGE> > &range){
        
        row = dimension;
        col = dimension;
        data = new double * [row];
    //	std::cout << " -------------" << std::endl;
        for(int i = 0 ; i < row ; i++){
            data[i] = new double [col];
            for ( int j = 0; j < col ; j++){
                if( i == j ) {
                    data[i][j] = 1 ;
                }else{
                    data[i][j] = 0 ;
                }
    //			std::cout << data[i][j] << " " ;
            }
    //		std::cout << std::endl;
        }
        for( int i = 1 ; i < static_cast<int>(range.size()) ; i++){
            int sign = MAXNUM;
            for(int j = 0 ; j < static_cast<int>(range[i].size()); j++){
                int insert = range[i][j].insert;
                int start = range[i][j].start;
                int end = range[i][j].end;
                if(type ==0 || type == 1){
                    if(range[i][j].layerid == i && range[i][j].conductorid != MAXNUM){
                        adjust(insert,start, end);
                    }else{
                        continue;
                    }
                }else if (type == -1){
                    if(range[i][j].layerid == i+1 && range[i][j].conductorid != MAXNUM){
                        adjust(insert,start, end);
                    }else{
                        continue;
                    }
                }
                for(int k = 0 ; k < static_cast<int>(range[i-1].size());k++){
                    if(range[i-1][k].conductorid == range[i][j].conductorid &&
                    range[i-1][k].layerid == range[i][j].layerid){
                        range[i-1][k].end += end - start + 1;
                        sign = k;
                    }
                    if(k > sign ){
                        range[i-1][k].end += range[i][j].end - range[i][j].start + 1;
                        range[i-1][k].start += range[i][j].end - range[i][j].start + 1;
                
                    }
                }
                for (int k = 0 ; k < j ; k++){
                    range[i][k].end += range[i][j].end - range[i][j].start + 1;
                    range[i][k].start += range[i][j].end - range[i][j].start + 1;
                }
                RangeSet(range[i][j],MAXNUM,MAXNUM,MAXNUM,MAXNUM,MAXNUM);
            }
        }
    }			
                    
    void adjust(const int insert, const int start, const int end){
        int range = end - start;
        for ( int j = 0; j < col ; j++){
            if (j == insert -1){ 
                for(int k = start - 1; k < end ;k++){
                    data[k][j] = 1;
                    data[j][j] = 0;
                    j = j + 1;
                }
                j = j -1;
            }else if( j == start -1){
                for(int k = insert -1 ; k < insert + range; k++){
                    data[k][j] = 1;
                    data[j][j] = 0;
                    j = j + 1;
                }
                j = j -1;
            }
        }
    
    }
    Matrix ( int dimension, int insert, int start, int end){
        
        row = dimension;
        col = dimension;
        int range = end - start;
        data = new double * [row];
        for(int i = 0 ; i < row ; i++){
            data[i] = new double [col];
            for ( int j = 0; j < col ; j++){
                data[i][j] = 0 ;
            }
        }
        for ( int j = 0; j < col ; j++){
            if (j == insert -1){ 
                for(int k = start - 1; k < end ;k++){
                    data[k][j] = 1;
                    j = j + 1;
                }
                j = j -1;
            }else if( j == start -1){
                for(int k = insert -1 ; k < insert + range; k++){
                    data[k][j] = 1;
                    j = j + 1;
                }
                j = j -1;
            }else{
                data[j][j] = 1 ;
            }
        }
    
    }

    Matrix ( int dimension,int start, int end){
        row = dimension;
        col = dimension - end + start;
        data = new double *[row];
        for ( int i = 0 ; i < row ; i++){
            data[i] = new double[col];
            for(int j = 0 ; j < col ; j++){
                if( j < start -1 ){
                    if( i == j ){
                        data[i][j] = 1;
                    }else{
                        data[i][j] = 0 ;
                    }
                }else if(j == start -1){
                    if(i >= start -1  && i <= end -1){
                        data[i][j] = 1;
                    }else{
                        data[i][j] = 0 ;
                    }
                }else{
                    if( i == j + end -start){
                        data[i][j] = 1;
                    }else{
                        data[i][j] = 0;
                    }
                }
            }
        }
    }
    void cohesion(int dimension, const std::vector<std::vector<RANGE> > &range, int type ,std::vector<int> &ccp){
        
        //std::cout << " cohesion " << std::endl;	
        row = dimension;
        col = dimension;
    //	std::cout << row <<  " " << std::endl;	
        for(int i = 0 ; i< static_cast<int>(range.size()); i++){
            for(int j = 0 ; j < static_cast<int>(range[i].size());j++){
                if(type == 0 || type == 1){
                    if(range[i][j].layerid ==  i + 1 && 
                    range[i][j].conductorid != MAXNUM){
                        col = col - (range[i][j].end - range[i][j].start);
                    }
                }else if(type == -1){
                    if((range[i][j].layerid ==  i + 1 || range[i][j].layerid == i+2) &&  
                    range[i][j].conductorid != MAXNUM){
                        col = col - (range[i][j].end - range[i][j].start);
                    }
                }
            }
        }
    //	std::cout << col <<  " " << std::endl;	
        data = new double *[row];
        for ( int i = 0 ; i < row ; i++){
            data[i] = new double[col];
            for(int j = 0 ; j < col ; j++){
                data[i][j] = 0;
            }
        }
        int shiftcol = 0 ,shift = 0, sign = 0, pre = 0, shiftcl = 0, shiftcll = 0;
        for(int i = 0 ; i < static_cast<int>(range.size()); i++){
            for(int j = 0 ; j < static_cast<int>(range[i].size()) ; j ++){
                if(type == 0 || type == 1){
                    if(range[i][j].layerid == i + 1 && range[i][j].conductorid == MAXNUM){
                        sign = range[i][j].start;
                        shift =  range[i][j].end - range[i][j].start;
                    } 
                    if(range[i][j].layerid ==  i + 1 && 
                    range[i][j].conductorid != MAXNUM){
                        int start = range[i][j].start;
                        int end = range[i][j].end;
                        shiftcl = end; 
                        if(shift == 0 && j==0 ){
                            shiftcol = 0;
                        }else if(pre == sign){
                            shiftcol = shiftcol + 1;
                        }else{
                            shiftcol = shiftcol + shift + 1;
                        }
                        for(int k = start - 1 ; k < end ; k++){
                            data[k][shiftcol] = 1;
                        }
                        shiftcll = shiftcol + 1;
                        ccp.push_back(shiftcol);
                        pre = sign;
                    }
                }else if(type == -1){
                    if((range[i][j].layerid == i + 2 ||range[i][j].layerid ==  i + 1) && 
                    range[i][j].conductorid != MAXNUM){
                        int start = range[i][j].start;
                        int end = range[i][j].end;
                        for(int k = start - 1 ; k < end ; k++){
                            data[k][shiftcol] = 1;
                        }
                        shiftcol = shiftcol + 1;
                    }
                }
                if(range[i][j].layerid ==  i + 1 && 
                range[i][j].conductorid == MAXNUM){
                    int start = range[i][j].start;
                    int end = range[i][j].end;
                    for(int k = start - 1 ; k < end ; k++){
                        data[shiftcl][shiftcll]= 1;
                        shiftcl = shiftcl + 1;
                        shiftcll = shiftcll + 1;
                    }
                    
                }
            }
        } 
    }

    int rows() const { return row;}
    int cols() const { return col;}

    void print(FILE *fp)
    {
        for(int i=0; i<row; i++)
        {
            for(int j=0; j<col; j++)
                fprintf(fp, " %lf", data[i][j]);
            fprintf(fp,"\n");
        }
    }
    
    double& operator () (int i, int j)
    {
        return data[i][j];
    }

    double operator () (int i, int j) const
    {
        return data[i][j];
    }

    double& elem (int i, int j) const
    {
        return data[i][j];
    }

    void release()
    {
        if(data && row > 0 && col > 0)
        {
            for(int i=0; i<row; i++)
                delete[] data[i];
            delete[] data;
        }
        row=col=0;
        data=NULL;
    }
    
    int resize (int r, int c, double val)
    {
        if(r<=0 || c<=0)
        {
//		  	printf("can't resize to negative dimension");
            release();
            return -1;
        }
        if((r== row) && (c== col))
            return 0;

        release();
        row = r;
        col = c;
        data= new double * [r];
        for(int i=0; i<r; i++)
        {
            data[i]= new double [c];
            for(int j=0; j<c; j++)
                data[i][j]= val;
        }
        return 0;
    }

    int resize (int r, int c)
    {
        if(r<=0 || c<=0)
        {
//		  	printf("can't resize to negative dimension");
            release();
            return -1;
        }
        if((r== row) && (c== col))
            return 0;

        int old_r= row, i, j;
        int old_c= col;
        double ** old_data = data;
        data= new double * [r];
        for(i=0; i<r; i++)
            data[i]= new double [c];
        row= r;
        col= c;
        if (old_data && old_r >0 && old_c >0)
        {
            int min_r= old_r<r? old_r: r;
            int min_c= old_c<c? old_c: c;
            for(i= 0; i<min_r; i++)
                for(j=0; j<min_c; j++)
                    data[i][j]= old_data[i][j];
            for(i= 0; i<old_r; i++)
                delete [] old_data[i];
            delete [] old_data;
        }
        return 0;
    }

    Matrix& fill (double val)
    {
        if(row >0 && col >0)
            for (int i=0; i<row; i++)
                for(int j=0; j<col; j++)
                    data[i][j]= val;
        return *this;
    }

    Matrix extract (int r1, int c1, int r2, int c2) const
    {
        if (r1 > r2) { int tmp = r1; r1 = r2; r2 = tmp; }
        if (c1 > c2) { int tmp = c1; c1 = c2; c2 = tmp; }

        int new_r = r2 - r1 + 1;
        int new_c = c2 - c1 + 1;

        Matrix result (new_r, new_c);

        for (int j = 0; j < new_c; j++)
            for (int i = 0; i < new_r; i++)
                result.elem (i, j) = elem (r1+i, c1+j);
        return  result;
    }

    Matrix& insert (const Matrix& a, int r, int c)
    {
        int a_rows = a.rows ();
        int a_cols = a.cols ();

        if (r < 0 || r + a_rows > rows () || c < 0 || c + a_cols > cols ())
        {
            printf("range error for insert");
            return *this;
        }

        for (int j = 0; j < a_cols; j++)
        for (int i = 0; i < a_rows; i++)
            elem (r+i, c+j) = a.elem (i, j);

        return *this;
    }

    Matrix operator - ()
    {
        row= rows();
        col= cols();
        Matrix result (row,col);
        int i, j;
        for(i=0; i<row; i++)
            for(j=0; j<col; j++)
                result(i,j) = - data[i][j];
        return result;
    }

    void operator = (const Matrix& a)
    {
        if(this != &a)
        {
            if(a.row>0 && a.col>0)
            {
                this->release();
                row= a.rows();
                col= a.cols();
                data= new double * [row];
                int i, j;
                for(i=0; i<row; i++)
                {
                    data[i]= new double [col];
                    for(j=0; j<col; j++)
                        data[i][j]= a(i, j);
                }
            }
        }
    }

    Matrix operator + (const Matrix& a)
    {
        int nr = rows ();
        int nc = cols ();

        int a_nr = a.rows ();
        int a_nc = a.cols ();

        if (nr != a_nr || nc != a_nc)
        {
            printf("Error in Matrix operator +: %d, %d, %d, %d\n", nr, nc, a_nr, a_nc);
            return Matrix ();
        }

        if (nr == 0 || nc == 0)
            return Matrix (nr, nc);

        Matrix result (nr,nc);
        int j;
        for (int i = 0; i < nr ; i++)
            for (j = 0; j < nc ; j++)
                result.elem (i, j) = data[i][j] + a(i,j);

        return result;
    }

    Matrix operator - (const Matrix& a)
    {
        int nr = rows ();
        int nc = cols ();

        int a_nr = a.rows ();
        int a_nc = a.cols ();

        if (nr != a_nr || nc != a_nc)
        {
            printf("Error in Matrix operator -: %d, %d, %d, %d\n", nr, nc, a_nr, a_nc);
            return Matrix ();
        }

        if (nr == 0 || nc == 0)
            return Matrix (nr, nc);

        Matrix result (nr,nc);
        int j;
        for (int i = 0; i < nr ; i++)
            for (j = 0; j < nc ; j++)
                result.elem (i, j) = data[i][j] - a.elem (i, j);

        return result;
    }

    Matrix operator * (const double& a)
    {
        int nr = rows ();
        int nc = cols ();

        if (nr == 0 || nc == 0)
            return Matrix (nr, nc);

        Matrix result (nr,nc);
        int j;
        for (int i = 0; i < nr ; i++)
            for (j = 0; j < nc ; j++)
                result.elem (i, j) = data[i][j] * a;

        return result;
    }
    
    Matrix operator * (const Matrix &a){
    
        //std::cout << " &&&&&&&&&&&&&& " <<std::endl;	
        //std::cout << row << " " << col << " " << a.row << " " << a.col<<std::endl;	
        lapack_int M = (lapack_int)row;
        lapack_int N = (lapack_int)(a.col);
        lapack_int K = (lapack_int)col;
        double *A;
        double *B;
        A = new double[row * col];
        for(int i = 0 ; i < row ;i++){
            for(int j = 0 ; j < col; j++){
                A[i * col + j] = data[i][j];
            }
        }
        B = new double[a.row * a.col];
        for(int i = 0 ; i < a.row ;i++){
            for(int j = 0 ; j < a.col; j++){
                B[i * a.col + j] =a.data[i][j];
            }
        }
        double *C;
        C = new double [M * N];
        for(int i = 0 ; i < M * N ;i++){
            C[i] = 0 ;
        }
        cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,M,N,K,1,A,K,B,N,1,C,N);
        Matrix result(M,N);
        for(int i = 0 ; i < M ; i++){
            for(int j = 0 ; j < N ; j++){
                result.data[i][j] = C[i*N + j];
            }
        }
        delete [] A;
        delete [] B;
        delete [] C;
        return result;
    }
    /*Matrix operator * (const Matrix& a)

    {
        Matrix retval;
        double val;

        int nr = rows ();
        int nc = cols ();

        int a_nr = a.rows ();
        int a_nc = a.cols ();

        if (nc != a_nr)
            printf ("operator * error: deminsion do not match\n");
        else if (nr == 0 || nc == 0 || a_nc == 0)
            printf ("operator * error: deminsion zero of input\n");
        else
        {
            int ld  = nr;
            int lda = a_nr;
            int i,j,k;

            retval.resize (nr, a_nc);
            for(i=0; i<nr; i++)
                for(j=0; j<a_nc; j++)
                {
                val=0;
                for(k=0; k<nc; k++)
                    val += data[i][k] * a(k, j);
                retval(i, j)= val;
                }
        }

        return retval;
    }*/
    void transpose(){
        
        Matrix temp(*this);
        this->release();
        row = temp.col;
        col = temp.row;
        data = new double *[row];
        for(int i = 0 ; i < row ;i++){
            data[i] = new double [col];
            for(int j = 0 ; j < col ; j++){
                data[i][j] = temp.data[j][i];
            }
        }
    //	std::cout << "transpose " << row << "  " << col << std::endl;
        temp.release();
    } 
    
    void matrixrowmajor(const Matrix &a){
    
        row = 1;
        col = a.row * a.col;
        data = new double * [row];	
        data[0] = new double [col];
        for(int i = 0 ; i < row ; i++){
            for(int j = 0 ; j < a.col ; j++){
                data[0][i * col + j] = a.data[i][j];
            }
        }
    }
    void vectortomatrix(std::vector<std::vector<double> > a){
        row = a.size();
        col = a[0].size();
        data = new double *[row];
        for(int i = 0 ; i < row ; i++){
            data[i] = new double [col];
            for(int j = 0 ; j < col ; j++){
                data[i][j] = a[i][j];
            }
        }
    }	
    
    /*~Matrix()
    {
        release();
    }*/

};
#endif
