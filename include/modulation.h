#ifndef _MODULATION_H_
#define _MODULATION_H_

#include "position.h"
#include "matrixclass.h"
extern int type;
std::vector<int> cpos;
void printmatrix(const Matrix &example){
    for(int i = 0 ; i < example.row ; i++){
        for(int j = 0 ; j< example.col;j++){
            std::cout << example.data[i][j] << " " ;
        }
        std::cout << std::endl;
    }
}	

void adjustposition(Matrix &m, const std::vector<std::vector<RANGE>> &range){
    
    int dimension = mrange.back().back().end;
    assert(m.col == dimension);
    Matrix am(m.row, m.col);
    size_t p = 0;
    for(int i = 1; i< static_cast<int>(range.size());i++){
        for(int j = 0 ; j < static_cast<int>(range[i].size()); j++){
            
            if(range[i][j].layerid == i && range[i][j].conductorid != MAXNUM){
                
                for(int k = 0 ; k < m.row; k ++){
                    for( size_t h = p ; h < static_cast<size_t>(m.col); h++){
                        if( h < range[i][j].insert - 1){
                            am.data[k][h] = m.data[k][h];
                        }else if( h >= range[i][j].insert - 1 && 
                            h < range[i][j].insert + range[i][j].end - range[i][j].start){
                            size_t kk = range[i][j].start - 1;
                            am.data[k][h] = m.data[k][kk];
                            kk = kk + 1;
                        }else if( h > range[i][j].end - 1){
                            am.data[k][h] = m.data[k][h];
                        }else if (h > range[i][j].insert + range[i][j].end - range[i][j].start - 1){
                            am.data[k][h] = m.data[k][h - range[i][j].end  + range[i][j].start - 1];
                        }
                    }
                }
                m = am;
                Matrix temp(m.row,m.col);
                am = temp;
                temp.release();
            }
        }
    }
    am.release();
}				
void MatrixModulate(){
    printf("!!!Begin Matrixmodulate!!!\n");
    PositionRange();
    printf("!!!Finish PositionRange!!!\n");
    
/*	std::cout << "---------------" << std::endl;
    std::cout<< mrange.size()<< std::endl;
    std::cout << mrange[0].size()<<std::endl;
    std::cout << "---------------" << std::endl;
    std::cout << "mexample------------" << std::endl;
    Matrix mexample(20, 5, 13, 18);
    printmatrix(mexample);	
*/	
    int dimension = mrange.back().back().end;
    
    std::cout << "mrange.size before modulation" << mrange.size() <<std::endl;;
    for(int i = 0 ; i< static_cast<int>(mrange.size());i++){
        for(int j = 0 ; j < static_cast<int>(mrange[i].size());j++){
            std::cout << "mrange[i].size " << i << mrange[i].size()<<std::endl;;
            std::cout << "---------******------" << std::endl;
            printrange(mrange[i][j]);
            std::cout << "-------------------" << std::endl;
        }
    }

    std::cout << boundaryposition[0][780] << std::endl;;	
    std::cout << boundaryposition[1][780] << std::endl;;	
    std::cout << boundaryposition[2][780] << std::endl;;	
    
//	Matrix tempbcm,temposition;
/*	std::cout << " ++++++++++++++++++++++++++" << std::endl;
    std::cout<<" boundaryposition " <<  boundaryposition.size() << " " << boundaryposition[0].size() << " " << std::endl;
    std::cout<< dimension <<std::endl;
    
    std::cout << " ++++++++++++++++++++++++++" << std::endl;
*/	boundary_position.vectortomatrix(boundaryposition);
    
    std::cout << boundary_position.data[0][780] << std::endl;;	
    std::cout << boundary_position.data[1][780] << std::endl;;	
    std::cout << boundary_position.data[2][780] << std::endl;;	
    adjustposition(boundary_position, mrange);
//	tempbcm = matrixbcm * adjust;
    std::cout << matrixbcm.data[0][780] << std::endl;;	
    std::cout << matrixbcm.data[1][780] << std::endl;;	
    std::cout << matrixbcm.data[2][780] << std::endl;;	
    
    adjustposition(matrixbcm, mrange);	
    std::cout << " ++++++++++++++++++++++++++" << std::endl;
//	temposition = boundary_position * adjust;
//	matrixbcm = tempbcm;
//	boundary_position = temposition;
//	adjust.release();
//	tempbcm.release();
//	temposition.release();	
    Matrix adjust(dimension,type,mrange);
    
    std::cout << boundary_position.data[0][920] << std::endl;;	
    std::cout << boundary_position.data[1][920] << std::endl;;	
    std::cout << boundary_position.data[2][920] << std::endl;;	

    std::cout << matrixbcm.data[0][920] << std::endl;;	
    std::cout << matrixbcm.data[1][920] << std::endl;;	
    std::cout << matrixbcm.data[2][920] << std::endl;;	
        
    std::cout << "mrange.size after modulation" << mrange.size() <<std::endl;;
    for(int i = 0 ; i< static_cast<int>(mrange.size());i++){
        for(int j = 0 ; j < static_cast<int>(mrange[i].size());j++){
            std::cout << "mrange[i].size " << i << mrange[i].size()<<std::endl;;
            std::cout << "---------******------" << std::endl;
            printrange(mrange[i][j]);
            std::cout << "-------------------" << std::endl;
        }
    }

}
    
void MatrixCohesion(){

    int dimension = mrange.back().back().end;
    Matrix tempbcm_r;
//	Matrix tempbcm_l;
    
    Matrix tempbcm_rr;
    Matrix a;
    a.cohesion(dimension, mrange, type,cpos);
    tempbcm_rr = boundary_position * a;
    boundary_position.release();
    boundary_position = tempbcm_rr;
    tempbcm_rr.release();

    tempbcm_r = matrixbcm * a;
    a.transpose();
    matrixbcm.release();
    matrixbcm = a * tempbcm_r;
    tempbcm_r.release();
    a.release();
}

void ModulatePosition(){
    
    for(int i = 0 ; i< static_cast<int>(cpos.size()); i++){
        boundary_position.data[0][cpos[i]] = 1e10;
        boundary_position.data[1][cpos[i]] = 1e10;
        boundary_position.data[2][cpos[i]] = 1e10;
        condp.push_back(cpos[i] + 1);
        std::cout << cpos[i] + 1 <<  " index " << std::endl;
    }

/*	int shift = 0,position = 0;
//	std::cout<< "boundary_position " << boundary_position.row << " " << boundary_position.col << std::endl;
    for(int i = 0 ; i < mrange.size(); i++){
        for(int j = 0 ; j < mrange[i].size(); j++){
            if(mrange[i][j].conductorid != MAXNUM && mrange[i][j].layerid == i + 1){
                position = mrange[i][j].start - shift;	
                condp.push_back(position);
                boundary_position.data[0][position - 1 ] = 1e10;
                boundary_position.data[1][position - 1 ] = 1e10;
                boundary_position.data[2][position - 1 ] = 1e10;
                shift = shift + mrange[i][j].end - mrange[i][j].start;
            }else{
                shift = shift + mrange[i][j].end - mrange[i][j].start + 1;
            }
        }
    }
    */
    if(static_cast<int>(condp.size()) != cond_number){
    exit(1);}

}
void ModulateBCM(){

    vector<double> row_sum(matrixbcm.row,0);
    vector<double> row_sum_positive(matrixbcm.row,0);
    vector<double> row_sum_negative(matrixbcm.row,0);
    std::cout << "orignal " << std::endl;
    for(size_t i = 0 ;  i < static_cast<size_t>(matrixbcm.col) ; i++){
        diag.push_back(matrixbcm.data[i][i]);
        std::cout << matrixbcm.data[i][i] << std::endl;
        double sum_row = 0, sum_row_p = 0, sum_row_n = 0;
        for(size_t j = 0 ; j < static_cast<size_t>(matrixbcm.row); j++){
            sum_row += matrixbcm.data[i][j];
            if(matrixbcm.data[i][j] >= 0 && i!=j){
                sum_row_p += matrixbcm.data[i][j];
                matrixbcm.data[i][j] = 0;
            }else{
                sum_row_n += fabs(matrixbcm.data[i][j]);
            }
                
        }
        row_sum[i] = sum_row;
        row_sum_positive[i] = sum_row_p;
        row_sum_negative[i] = sum_row_n;
    }
    
    for(size_t i = 0 ;  i < static_cast<size_t>(matrixbcm.row) ; i++){
        double increase = row_sum_positive[i] - row_sum[i];
        for(size_t j = 0 ; j < static_cast<size_t>(matrixbcm.col) ; j++){
            if(matrixbcm.data[i][j] != 0){
                double modulate = (fabs(matrixbcm.data[i][j])/row_sum_negative[i]) * increase;
                matrixbcm.data[i][j] += modulate;
            }
        }
    }
    for(size_t i = 0 ; i < static_cast<size_t>(matrixbcm.row) ; i++){
        for(size_t j = 0 ; j < static_cast<size_t>(matrixbcm.col) ; j++){
            if(matrixbcm.data[i][j] >= 0 && i != j){
                matrixbcm.data[i][j] = 0;
            }
        }
    }
    for(size_t i = 0 ; i < static_cast<size_t>(matrixbcm.row) ; i++){
        for(size_t j = 0 ; j < static_cast<size_t>(matrixbcm.col) ; j++){
            if(j > i){
                matrixbcm.data[i][j] =( matrixbcm.data[i][j] + matrixbcm.data[j][i])/ 2;
                matrixbcm.data[j][i] = matrixbcm.data[i][j];
            }
        }
    }
    for(size_t i = 0 ; i < static_cast<size_t>(matrixbcm.col) ; i++){
        double sum_row = 0;
        for(size_t j = 0 ; j < static_cast<size_t>(matrixbcm.row); j++){
            sum_row += matrixbcm.data[i][j];
            if(i != j ){
                matrixbcm.data[i][j] = fabs(matrixbcm.data[i][j]);
            }
        }
        matrixbcm.data[i][i] -= sum_row;
        
        
        for(size_t j = 0 ; j < static_cast<size_t>(matrixbcm.row); j++){
            if(i != j){
                matrixbcm.data[i][j] = matrixbcm.data[i][j] / matrixbcm.data[i][i];
            }
        }
        
        _diag.push_back(matrixbcm.data[i][i]);
        matrixbcm.data[i][i] = 0;
    }
    std::cout << "error " << std::endl;
    std::cout << _diag[0] << " " << diag[0] << std::endl;
    for(int i = 0 ; i < static_cast<int>(diag.size()); i++){
        std::cout << diag[i] - _diag[i] << std::endl;
    }	
}

#endif	
