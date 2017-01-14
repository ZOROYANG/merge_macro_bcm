/*
* FDMSolver.cpp
*
*  Created on: 8 Jul, 2016
*      Author: ming
*/

#include "FDMSolver.h"
FDM_Solver::FDM_Solver() : inum(0), bnum(0){
    // TODO Auto-generated constructor stub

}

FDM_Solver::~FDM_Solver() {
    // TODO Auto-generated destructor stub
    printf("~~~In ~FDMSolver~~~\n");
    if(macro_conf->getNumCond() == 0){
        int sa[3];
        for(int i = 0; i < 3; ++ i){
            sa[i] = macro_conf->seg_axis[i];
        }
        
        for(int i = 0; i < 3; ++ i){
            sa[i] += 2;
        }

        for(int i = 0; i < sa[0]; ++ i){
            for(int j = 0; j < sa[1]; ++ j){
                delete[] points[i][j];
            }
            delete[] points[i];
        }
        delete[] points;


        this->A11.matrixFree();
        this->A12.matrixFree();
        this->A21.matrixFree();
        this->A22.matrixFree();
    }else{
        printf("~~~In ~FDMSolver MultiConductors Branch~~~\n");
        /*
        this->A11.matrixFree();
        this->A12.matrixFree();
        this->A13.matrixFree();
        this->A21.matrixFree();
        this->A22.matrixFree();
        this->A23.matrixFree();
        this->A31.matrixFree();
        this->A32.matrixFree();
        this->A33.matrixFree();
        */
    } 
}

void FDM_Solver::Construct_Matrix_A11(){
    int sa[3];
    for(int i = 0; i < 3; ++ i){
        sa[i] = macro_conf->seg_axis[i];
    }
        
    for(int i = 0; i < 3; ++ i){
        sa[i] += 2;
    }
    A11.n_row = inum;
    A11.n_col = inum;

    for(int i = 0; i < sa[0]; ++ i){
        for(int j = 0; j < sa[1]; ++ j){
            for(int k = 0; k < sa[2]; ++ k){
                if(points[i][j][k] < 0){
                    continue;
                }
                
                // boundary points
                if(points[i][j][k] >= inum){
                    
                    continue;
                }
                
                // inner points

                double center = 0.;
                if((i + 1) == (sa[0] - 1)){
                    if(i == 1){
                        center += -2. * this->uscale2[0];
                    }else{
                        center += -1. * this->uscale2[0];
                        
                        A11.i.push_back(points[i][j][k]);
                        A11.j.push_back(points[i - 1][j][k]);
                        A11.x.push_back(this->uscale2[0] / 3.);
                    }
                }else{
                    if(i == 1){
                        center += -1. * this->uscale2[0];
                        
                        A11.i.push_back(points[i][j][k]);
                        A11.j.push_back(points[i + 1][j][k]);
                        A11.x.push_back(this->uscale2[0] / 3.);
                    }else{
                        center += this->uscale2[0] / -2.;
                    
                        A11.i.push_back(points[i][j][k]);
                        A11.j.push_back(points[i + 1][j][k]);
                        A11.x.push_back(this->uscale2[0] / 4.);
                    
                        A11.i.push_back(points[i][j][k]);
                        A11.j.push_back(points[i - 1][j][k]);
                        A11.x.push_back(this->uscale2[0] / 4.);
                    }
                }

                if((j + 1) == (sa[1] - 1)){
                    if(j == 1){
                        center += -2. * this->uscale2[1];
                    }else{
                        center += -1. * this->uscale2[1];
                        
                        A11.i.push_back(points[i][j][k]);
                        A11.j.push_back(points[i][j - 1][k]);
                        A11.x.push_back(this->uscale2[1] / 3.);
                    }
                }else{
                    if(j == 1){
                        center += -1. * this->uscale2[1];
                        
                        A11.i.push_back(points[i][j][k]);
                        A11.j.push_back(points[i][j + 1][k]);
                        A11.x.push_back(this->uscale2[1] / 3.);
                    }else{
                        center += this->uscale2[1] / -2.;
                    
                        A11.i.push_back(points[i][j][k]);
                        A11.j.push_back(points[i][j + 1][k]);
                        A11.x.push_back(this->uscale2[1] / 4.);
                    
                        A11.i.push_back(points[i][j][k]);
                        A11.j.push_back(points[i][j - 1][k]);
                        A11.x.push_back(this->uscale2[1] / 4.);
                    }
                }
                
                if((k + 1) == (sa[2] - 1)){
                    if(k == 1){
                        center += -2. * this->uscale2[2];
                    }else{
                        center += -1. * this->uscale2[2];
                        
                        A11.i.push_back(points[i][j][k]);
                        A11.j.push_back(points[i][j][k - 1]);
                        A11.x.push_back(this->uscale2[2] / 3.);
                    }
                }else{
                    if(k == 1){
                        center += -1. * this->uscale2[2];
                        
                        A11.i.push_back(points[i][j][k]);
                        A11.j.push_back(points[i][j][k + 1]);
                        A11.x.push_back(this->uscale2[2] / 3.);
                    }else{
                        center += this->uscale2[2] / -2.;
                    
                        A11.i.push_back(points[i][j][k]);
                        A11.j.push_back(points[i][j][k + 1]);
                        A11.x.push_back(this->uscale2[2] / 4.);
                    
                        A11.i.push_back(points[i][j][k]);
                        A11.j.push_back(points[i][j][k - 1]);
                        A11.x.push_back(this->uscale2[2] / 4.);
                    }
                }

                A11.i.push_back(points[i][j][k]);
                A11.j.push_back(points[i][j][k]);
                A11.x.push_back(center);

            }
        }
    }


    int status;
    A11.nz = A11.i.size();
    A11.Ap.resize(A11.n_col + 1, 0);
    A11.Ai.resize(A11.nz, 0);
    A11.Ax.resize(A11.nz, 0);
    status = umfpack_di_triplet_to_col(A11.n_row, A11.n_col, A11.nz, &A11.i[0], &A11.j[0], &A11.x[0], &A11.Ap[0], &A11.Ai[0], &A11.Ax[0], NULL);
    if(status != 0){
        printf("A11: triplet_to_col goes wrong!\n");
    }

    return;
}

void FDM_Solver::Construct_Matrix_A12(){
    int sa[3];
    for(int i = 0; i < 3; ++ i){
        sa[i] = macro_conf->seg_axis[i];
    }
    
    for(int i = 0; i < 3; ++ i){
        sa[i] += 2;
    }
    A12.n_row = inum;
    A12.n_col = bnum;

    for(int i = 0; i < sa[0]; ++ i){
        for(int j = 0; j < sa[1]; ++ j){
            for(int k = 0; k < sa[2]; ++ k){
                if(points[i][j][k] < 0){
                    continue;
                }

                // boundary points
                if(points[i][j][k] >= inum){
                    continue;
                }

                // inner points
                if((i + 1) == (sa[0] - 1)){
                    if(i == 1){
                        A12.i.push_back(points[i][j][k]);
                        A12.j.push_back(points[i + 1][j][k] - inum);
                        A12.x.push_back(this->uscale2[0]);

                        A12.i.push_back(points[i][j][k]);
                        A12.j.push_back(points[i - 1][j][k] - inum);
                        A12.x.push_back(this->uscale2[0]);
                    }else{
                        A12.i.push_back(points[i][j][k]);
                        A12.j.push_back(points[i + 1][j][k] - inum);
                        A12.x.push_back(2. * this->uscale2[0] / 3.);
                    }
                }else{
                    if(i == 1){
                        A12.i.push_back(points[i][j][k]);
                        A12.j.push_back(points[i - 1][j][k] - inum);
                        A12.x.push_back(2. * this->uscale2[0] / 3.);
                    }else{
                    }
                }

                if((j + 1) == (sa[1] - 1)){
                    if(j == 1){
                        A12.i.push_back(points[i][j][k]);
                        A12.j.push_back(points[i][j + 1][k] - inum);
                        A12.x.push_back(this->uscale2[1]);

                        A12.i.push_back(points[i][j][k]);
                        A12.j.push_back(points[i][j - 1][k] - inum);
                        A12.x.push_back(this->uscale2[1]);
                    }else{
                        A12.i.push_back(points[i][j][k]);
                        A12.j.push_back(points[i][j + 1][k] - inum);
                        A12.x.push_back(2. * this->uscale2[1] / 3.);
                    }
                }else{
                    if(j == 1){
                        A12.i.push_back(points[i][j][k]);
                        A12.j.push_back(points[i][j - 1][k] - inum);
                        A12.x.push_back(2. * this->uscale2[1] / 3.);
                    }else{
                    }
                }

                if((k + 1) == (sa[2] - 1)){
                    if(k == 1){
                        A12.i.push_back(points[i][j][k]);
                        A12.j.push_back(points[i][j][k + 1] - inum);
                        A12.x.push_back(this->uscale2[2]);

                        A12.i.push_back(points[i][j][k]);
                        A12.j.push_back(points[i][j][k - 1] - inum);
                        A12.x.push_back(this->uscale2[2]);
                    }else{
                        A12.i.push_back(points[i][j][k]);
                        A12.j.push_back(points[i][j][k + 1] - inum);
                        A12.x.push_back(2. * this->uscale2[2] / 3.);
                    }
                }else{
                    if(k == 1){
                        A12.i.push_back(points[i][j][k]);
                        A12.j.push_back(points[i][j][k - 1] - inum);
                        A12.x.push_back(2. * this->uscale2[2] / 3.);
                    }else{
                    }
                }
            }
        }
    }

    int status;
    A12.nz = A12.i.size();
    A12.Ap.resize(A12.n_col + 1, 0);
    A12.Ai.resize(A12.nz, 0);
    A12.Ax.resize(A12.nz, 0);
    status = umfpack_di_triplet_to_col(A12.n_row, A12.n_col, A12.nz, &A12.i[0], &A12.j[0], &A12.x[0], &A12.Ap[0], &A12.Ai[0], &A12.Ax[0], NULL);
    if(status != 0){
        printf("A12: triplet_to_col goes wrong!\n");
    }

    return;
}

// 2nd-order
void FDM_Solver::Construct_Matrix_A21(){
    int sa[3];
    for(int i = 0; i < 3; ++ i){
        sa[i] = macro_conf->seg_axis[i];
    }
    
    for(int i = 0; i < 3; ++ i){
        sa[i] += 2;
    }
    A21.n_row = bnum;
    A21.n_col = inum;

    for(int i = 0; i < sa[0]; ++ i){
        for(int j = 0; j < sa[1]; ++ j){
            for(int k = 0; k < sa[2]; ++ k){
                if(points[i][j][k] < 0){
                    continue;
                }

                // boundary points, take care du/dx = -E
                if(points[i][j][k] >= inum){
                    if(i == 0){
                        A21.i.push_back(points[i][j][k] - inum);
                        A21.j.push_back(points[i + 1][j][k]);
                        A21.x.push_back(-3. / 2. * this->uscale[0]);
                                                
                        A21.i.push_back(points[i][j][k] - inum);
                        A21.j.push_back(points[i + 2][j][k]);
                        A21.x.push_back(1. / 6. * this->uscale[0]);
                        continue;
                    }

                    if(i == (sa[0] - 1)){
                        A21.i.push_back(points[i][j][k] - inum);
                        A21.j.push_back(points[i - 1][j][k]);
                        A21.x.push_back(-3. / 2. * this->uscale[0]);

                        A21.i.push_back(points[i][j][k] - inum);
                        A21.j.push_back(points[i - 2][j][k]);
                        A21.x.push_back(1. / 6. * this->uscale[0]);
                        continue;
                    }

                    if(j == 0){
                        A21.i.push_back(points[i][j][k] - inum);
                        A21.j.push_back(points[i][j + 1][k]);
                        A21.x.push_back(-3. / 2. * this->uscale[1]);

                        A21.i.push_back(points[i][j][k] - inum);
                        A21.j.push_back(points[i][j + 2][k]);
                        A21.x.push_back(1. / 6. * this->uscale[1]);
                        continue;
                    }

                    if(j == (sa[1] - 1)){
                        A21.i.push_back(points[i][j][k] - inum);
                        A21.j.push_back(points[i][j - 1][k]);
                        A21.x.push_back(-3. / 2. * this->uscale[1]);

                        A21.i.push_back(points[i][j][k] - inum);
                        A21.j.push_back(points[i][j - 2][k]);
                        A21.x.push_back(1. / 6. * this->uscale[1]);
                        continue;
                    }

                    if(k == 0){
                        A21.i.push_back(points[i][j][k] - inum);
                        A21.j.push_back(points[i][j][k + 1]);
                        A21.x.push_back(-3. / 2. * this->uscale[2]);

                        A21.i.push_back(points[i][j][k] - inum);
                        A21.j.push_back(points[i][j][k + 2]);
                        A21.x.push_back(1. / 6. * this->uscale[2]);
                        continue;
                    }

                    if(k == (sa[2] - 1)){
                        A21.i.push_back(points[i][j][k] - inum);
                        A21.j.push_back(points[i][j][k - 1]);
                        A21.x.push_back(-3. / 2. * this->uscale[2]);

                        A21.i.push_back(points[i][j][k] - inum);
                        A21.j.push_back(points[i][j][k - 2]);
                        A21.x.push_back(1. / 6. * this->uscale[2]);
                        continue;
                    }
                }
            }
        }
    }

    int status;
    A21.nz = A21.i.size();
    A21.Ap.resize(A21.n_col + 1, 0);
    A21.Ai.resize(A21.nz, 0);
    A21.Ax.resize(A21.nz, 0);
    status = umfpack_di_triplet_to_col(A21.n_row, A21.n_col, A21.nz, &A21.i[0], &A21.j[0], &A21.x[0], &A21.Ap[0], &A21.Ai[0], &A21.Ax[0], NULL);
    if(status != 0){
        printf("A21: triplet_to_col goes wrong!\n");
    }

    return;
}

void FDM_Solver::Construct_Matrix_A22(){
    int sa[3];
    for(int i = 0; i < 3; ++ i){
        sa[i] = macro_conf->seg_axis[i];
    }
    
    for(int i = 0; i < 3; ++ i){
        sa[i] += 2;
    }
    A22.n_row = bnum;
    A22.n_col = bnum;

    for(int i = 0; i < sa[0]; ++ i){
        for(int j = 0; j < sa[1]; ++ j){
            for(int k = 0; k < sa[2]; ++ k){
                if(points[i][j][k] < 0){
                    continue;
                }

                // boundary points, take care du/dx = -E
                if(points[i][j][k] >= inum){
                    A22.i.push_back(points[i][j][k] - inum);
                    A22.j.push_back(points[i][j][k] - inum);
                    if(i == 0){
                        A22.x.push_back(4. /3. * this->uscale[0]);
                        continue;
                    }

                    if(i == (sa[0] - 1)){
                        A22.x.push_back(4. /3. * this->uscale[0]);
                        continue;
                    }

                    if(j == 0){
                        A22.x.push_back(4. /3. * this->uscale[1]);
                        continue;
                    }

                    if(j == (sa[1] - 1)){
                        A22.x.push_back(4. /3. * this->uscale[1]);
                        continue;
                    }

                    if(k == 0){
                        A22.x.push_back(4. /3. * this->uscale[2]);
                        continue;
                    }

                    if(k == (sa[2] - 1)){
                        A22.x.push_back(4. /3. * this->uscale[2]);
                        continue;
                    }
                }
            }
        }
    }

    int status;
    A22.nz = A22.i.size();
    A22.Ap.resize(A22.n_col + 1, 0);
    A22.Ai.resize(A22.nz, 0);
    A22.Ax.resize(A22.nz, 0);
    status = umfpack_di_triplet_to_col(A22.n_row, A22.n_col, A22.nz, &A22.i[0], &A22.j[0], &A22.x[0], &A22.Ap[0], &A22.Ai[0], &A22.Ax[0], NULL);
    if(status != 0){
        printf("A22: triplet_to_col goes wrong!\n");
    }
    
    return;
}

namespace ssss
{
//Matrix_Sparse x;
    //vector<int> i;
}

// dielectric value is diel, S may be a matrix
void FDM_Solver::solve(){
    printf("in solve()\n");
    //for (auto x : this->A11.i) printf("%d ", x); puts("");
    if(macro_conf->getNumCond() == 0){
        Cap = convert2Matrix(compute_Formula(A11, A12, A21, A22, inum, bnum));
        return;
    }else{
        /*printf("in solve() else\n%d %d\n", inrnk, crnk);
        for (auto x : A11.i) printf("%d ", x); puts("");
        this->A11;
        //this->A11.Debug("debug_A11.txt");
        puts("this->A11");
        
        for (auto x : A11.i) printf("%d ", x); puts("");
        //ssss::x.n_row = this->A11.n_row;
        printf("%d\n", this->A11.i.size());
        //ssss::x.i.clear();
        //printf("%d\n", ssss::x.i.size());
        
        for (auto x : A11.i)
        {
            int y = x;
            ssss::x.i.push_back(1);
        }
        */
        printf("just Push_back...\n");
        vector<double> ii;
        for (int i=0; i<1024; ++i) {
            printf("%u\n", ii.size());
            ii.push_back(0);
        }
        //ssss::x.i.push_back(1);
        //ssss::x.i = this->A11.i;
        //Matrix_Sparse x = this->A11;
        printf("A11 OK\n");
        this->A13.Debug("debug_A13.txt");
        printf("A13 OK\n");
        this->A31.Debug("debug_A31.txt");
        printf("A31 OK\n");
        this->A33.Debug("debug_A33.txt");
        printf("A33 OK\n");
        
        cs_di tempc11 = compute_Formula(A33, A31, A13, A11, inrnk, crnk);
        printf("compute_Formula over\n");
        std::vector<std::vector<double> > c11 = convert2Matrix(tempc11);
        printf("c11 over\n");
        std::vector<std::vector<double> > c22 = convert2Matrix(compute_Formula(A33, A32, A23, A22, inrnk, brnk));
        printf("c22 over\n");
        std::vector<std::vector<double> > c12 = convert2Matrix(compute_Formula(A33, A32, A13, A12, inrnk, brnk));
        printf("c12 over\n");
        std::vector<std::vector<double> > c21 = convert2Matrix(compute_Formula(A33, A31, A23, A21, inrnk, crnk));
        printf("c21 over\n");
        
        this->debg.Vec_Mtx_print(c11, "C11.txt");
        this->debg.Vec_Mtx_print(c12, "C12.txt");
        this->debg.Vec_Mtx_print(c21, "C21.txt");
        this->debg.Vec_Mtx_print(c22, "C22.txt");
        
        for(int j = 0; j < c11.size(); ++ j){
            std::vector<double> rvec;
            for(int i = 0; i < c11[j].size(); ++ i){
                rvec.push_back(c11[j][i]);
            }
            for(int i = 0; i < c12[j].size(); ++ i){
                rvec.push_back(c12[j][i]);
            }
            Cap.push_back(rvec);
        }
        
        for(int j = 0; j < c21.size(); ++ j){
            std::vector<double> rvec;
            for(int i = 0; i < c21[j].size(); ++ i){
                rvec.push_back(c21[j][i]);
            }
            for(int i = 0; i < c22[j].size(); ++ i){
                rvec.push_back(c22[j][i]);
            }
            Cap.push_back(rvec);
        }
        
        this->debg.Vec_Mtx_print(Cap, "uncompressed.txt");
        
        this->cmprsBoundary();
        printf("compressed size: %lu\n", Cap.size());
        FILE* output;
	output = fopen(ans_path.c_str(), "w");
	if(output == NULL){
		printf("Cann't open %s\n", ans_path.c_str());
		exit(1);
	}
	
	for(int u = 0; u < Cap.size(); ++ u){
		for(int v = 0; v < Cap[u].size(); ++ v){
			fprintf(output, "%10f ", Cap[u][v]);
		}
		fprintf(output, ";\n");
	}
	
	fclose(output);
        return;
    }
}

cs_di FDM_Solver::compute_Formula(Matrix_Sparse iA11, Matrix_Sparse iA12, Matrix_Sparse iA21, Matrix_Sparse iA22, int p_row, int p_col){
    printf("in compute_Formula()\n");
    // calculate iA22 - iA21 * inv(iA11) * iA12
    cs_di rCap;
    // construct a22
    cs_di a22;
    a22.m = iA22.n_row;
    a22.n = iA22.n_col;
    a22.p = &(iA22.Ap[0]);
    a22.i = &(iA22.Ai[0]);
    a22.x = &(iA22.Ax[0]);
    a22.nzmax = iA22.nz;
    a22.nz = -1;
    
    // construct a21
    cs_di a21;
    a21.m = iA21.n_row;
    a21.n = iA21.n_col;
    a21.p = &(iA21.Ap[0]);
    a21.i = &(iA21.Ai[0]);
    a21.x = &(iA21.Ax[0]);
    a21.nzmax = iA21.nz;
    a21.nz = -1;
    // construct a11
    cs_di a11;
    a11.m = iA11.n_row;
    a11.n = iA11.n_col;
    a11.p = &(iA11.Ap[0]);
    a11.i = &(iA11.Ai[0]);
    a11.x = &(iA11.Ax[0]);
    a11.nzmax = iA11.nz;
    a11.nz = -1;
    // construct a12
    cs_di a12;
    a12.m = iA12.n_row;
    a12.n = iA12.n_col;
    a12.p = &(iA12.Ap[0]);
    a12.i = &(iA12.Ai[0]);
    a12.x = &(iA12.Ax[0]);
    a12.nzmax = iA12.nz;
    a12.nz = -1;
    
    cs_di pCap; // part Capacitency, inv(a11) * a12
    pCap.nz = -1;
    pCap.m = p_row;
    pCap.n = p_col;
    int countnz = 0;

    //pCap.p = new int[pCap.n + 1];
    std::vector<int> p_temp;
    std::vector<int> row_idx;
    std::vector<double> value;

    /* Here is supposed to apply UMFPACK_A, because
            * Int sys ;		Input argument, not modified.

        Defines which system to solve.  (') is the linear algebraic transpose
        (complex conjugate if A is complex), and (.') is the array transpose.

            sys value	    system solved
            UMFPACK_A       Ax=b
            UMFPACK_At      A'x=b
            UMFPACK_Aat     A.'x=b
            UMFPACK_Pt_L    P'Lx=b
            UMFPACK_L       Lx=b
            UMFPACK_Lt_P    L'Px=b
            UMFPACK_Lat_P   L.'Px=b
            UMFPACK_Lt      L'x=b
            UMFPACK_U_Qt    UQ'x=b
            UMFPACK_U       Ux=b
            UMFPACK_Q_Ut    QU'x=b
            UMFPACK_Q_Uat   QU.'x=b
            UMFPACK_Ut      U'x=b
            UMFPACK_Uat     U.'x=b
    */
    for(int k = 0; k < a12.n; ++ k){
        // x is as long as the width of a11
        std::vector<double> x(pCap.m, 0);

        std::vector<double> b(pCap.m, 0);
        for(int j = a12.p[k]; j < a12.p[k + 1]; ++ j){
            b[a12.i[j]] = a12.x[j];
        }

        void *Symbolic , *Numeric;
        double *null = (double *) NULL ;
        umfpack_di_symbolic(a11.m, a11.n, a11.p, a11.i, a11.x, &Symbolic, null, null);
        umfpack_di_numeric(a11.p, a11.i, a11.x, Symbolic, &Numeric, null, null);
        umfpack_di_free_symbolic(&Symbolic);
        // reference to above
        umfpack_di_solve(UMFPACK_A, a11.p, a11.i, a11.x, &x[0], &b[0], Numeric, null, null);
        umfpack_di_free_numeric(&Numeric);

        //pCap.p[k] = countnz;
        p_temp.push_back(countnz);
        for(int u = 0; u < pCap.m; ++ u){
            if(x[u] != 0){
                ++ countnz;
                row_idx.push_back(u);
                value.push_back(x[u]);
            }
        }
    }
        
    //pCap.p[pCap.n] = countnz;
    p_temp.push_back(countnz);
    // assure it's valid
    assert(row_idx.size() == value.size());
        
    //pCap.i = new int[row_idx.size()];
    //pCap.x = new double[value.size()];
    //int vsize = value.size();
    //for(int k = 0; k < vsize; ++ k){
        //pCap.i[k] = row_idx[k];
        //pCap.x[k] = value[k];
    //}
    pCap.i = &(row_idx[0]);
    pCap.p = &(p_temp[0]);
    pCap.x = &(value[0]);

    pCap.nzmax = countnz;
        
    cs_di* res1 = cs_di_multiply(&a21, &pCap);
    cs_di* res2 = cs_di_add(&a22, res1, 1, -1);

    // need to delete the mem of rCap
    printf("Hello1!\n");
    
    rCap.m = res2->m;
    rCap.n = res2->n;
    rCap.nzmax = res2->nzmax;
    printf("m: %d, n: %d, nzmax: %d\n", rCap.n, rCap.n, rCap.nzmax);
    rCap.p = new int[rCap.n + 1];
    for(int k = 0; k <= rCap.n; ++ k){
        rCap.p[k] = res2->p[k];
    }
    printf("Hello2!\n");
    
    rCap.i = new int[rCap.nzmax];
    for(int k = 0; k < rCap.nzmax; ++ k){
        rCap.i[k] = res2->i[k];
    }
    printf("Hello3!\n");
    
    rCap.x = new double[rCap.nzmax];
    for(int k = 0; k < rCap.nzmax; ++ k){
        rCap.x[k] = res2->x[k];
    }
    printf("Hello4!\n");
    
    rCap.nz = -1;
        
    cs_di_spfree(res1);
    cs_di_spfree(res2);
    
    return rCap;
}

void FDM_Solver::Debug(){
    if(macro_conf->getNumCond() == 0){
        int sa[3];
        for(int i = 0; i < 3; ++ i){
            sa[i] = macro_conf->seg_axis[i];
        }
        
        for(int i = 0; i < 3; ++ i){
            sa[i] += 2;
        }
        this->debg.Order_print(this->points, sa[0], sa[1], sa[2], this->inum, this->bnum, this->uscale, this->uscale2);
    }else{
        this->debg.Cut_print(gridX, gridY, gridZ, eleX, eleY, eleZ, crnk, brnk, inrnk);
    }
    
    return;
}

std::vector<std::vector<double> > FDM_Solver::getCap(){
        return this->Cap;
}

FDM_Solver::FDM_Solver(Configure* mconf) : macro_conf(mconf){
    if(macro_conf->getNumCond() == 0){
        this->uniform();
    }else{
        printf("do nothing!\n");
        //this->nonuniform();
    }
}

void FDM_Solver::uniform(){
    // TODO Auto-generated constructor stub
    int sa[3];
    for(int i = 0; i < 3; ++ i){
        sa[i] = macro_conf->seg_axis[i];
    }
    double len[3];
    for(int i = 0; i < 3; ++ i){
        len[i] = macro_conf->length[i];
    }
    
    inum = sa[0] * sa[1] * sa[2];
    bnum = 2 * (sa[0] * sa[1] + sa[0] * sa[2] + sa[2] * sa[1]);

    for(int i = 0; i < 3; ++ i){
                // 1/h, h = (length / segments) / 2
        uscale[i] = double(sa[i]) * 2. / len[i];
                // 1/h^2
        uscale2[i] = uscale[i] * uscale[i];
    }

    for(int i = 0; i < 3; ++ i){
            sa[i] += 2;
    }


    points = new int**[sa[0]];
    for(int i = 0; i < sa[0]; ++ i){
        points[i] = new int*[sa[1]];
        for(int j = 0; j < sa[1]; ++ j){
            points[i][j] = new int[sa[2]];
        }
    }
    
    for(int i = 0; i < sa[0]; ++ i){
        for(int j = 0; j < sa[1]; ++ j){
            for(int k = 0; k < sa[2]; ++ k){
                points[i][j][k] = -1;
            }
        }
    }

    int idx = 0;
    for(int i = 1; i < sa[0] - 1; ++ i){
        for(int j = 1; j < sa[1] - 1; ++ j){
            for(int k = 1; k < sa[2] - 1; ++ k){
                points[i][j][k] = idx;
                ++ idx;
            }
        }
    }
        /* view the cubic this way, your left is the "left"!
    *   ______
    *  /     /|
    * /_____/ |
    * |     | |
    * |     | /
    * |_____|/
    */

    // bottom
    idx = inum;
    for(int j = 1; j < sa[1] - 1; ++ j){
        for(int i = 1; i < sa[0] - 1; ++ i){
            points[i][j][0] = idx;
            ++ idx;
        }
    }

    // left
    for(int k = 1; k < sa[2] - 1; ++ k){
        for(int j = sa[1] - 2; j > 0; -- j){
            points[0][j][k] = idx;
            ++ idx;
        }
    }

    // front
    for(int k = 1; k < sa[2] - 1; ++ k){
        for(int i = 1; i < sa[0] - 1; ++ i){
            points[i][0][k] = idx;
            ++ idx;
        }
    }

    // right
    for(int k = 1; k < sa[2] - 1; ++ k){
        for(int j = 1; j < sa[1] - 1; ++ j){
            points[sa[0] - 1][j][k] = idx;
            ++ idx;
        }
    }

    // back
    for(int k = 1; k < sa[2] - 1; ++ k){
        for(int i = sa[0] - 2; i > 0; -- i){
            points[i][sa[1] - 1][k] = idx;
            ++ idx;
        }
    }

    // top
    for(int j = 1; j < sa[1] - 1; ++ j){
        for(int i = 1; i < sa[0] - 1; ++ i){
            points[i][j][sa[2] - 1] = idx;
            ++ idx;
        }
    }


    this->Construct_Matrix_A11();
    this->A11.Debug("A11.txt");

    this->Construct_Matrix_A12();
    this->A12.Debug("A12.txt");

    this->Construct_Matrix_A21();
    this->A21.Debug("A21.txt");

    this->Construct_Matrix_A22();
    this->A22.Debug("A22.txt");
    
}

void FDM_Solver::nonuniform(){
    // the amount of conductor surface elements
    crnk = 0;
    
    // total grid number of each conductor
    int *grid_numc = new int[macro_conf->getNumCond()];
    for(int i = 0; i < macro_conf->getNumCond(); ++ i){
        grid_numc[i] = (macro_conf->cond_seg[i][0] * macro_conf->cond_seg[i][1]
            + macro_conf->cond_seg[i][0] * macro_conf->cond_seg[i][2]
            + macro_conf->cond_seg[i][1] * macro_conf->cond_seg[i][2]) * 2;
            crnk += grid_numc[i];
    }
    
    // the amount of boundary elements
    std::vector<double> rawGridX;
    stepX = macro_conf->length[0] / macro_conf->seg_axis[0];
    double upperX = macro_conf->length[0] + macro_conf->origin[0];
    double lowerX = macro_conf->origin[0];
    for(double i = lowerX; i <= upperX;i += stepX){
        rawGridX.push_back(i);
    }
    for(int i = 0; i < macro_conf->getNumCond(); ++ i){
        rawGridX.push_back(macro_conf->cond_ori[i][0]);
        rawGridX.push_back(macro_conf->cond_ori[i][0] + macro_conf->cond_len[i][0]);
    }
    sort(rawGridX.begin(), rawGridX.end());
    for(int i = 0; i < rawGridX.size() - 1; ++ i){
        if(fabs(rawGridX[i] - rawGridX[i + 1]) > (stepX * precision)){
            gridX.push_back(rawGridX[i]);
        }
    }
    gridX.push_back(rawGridX[rawGridX.size() - 1]);
    
    
    std::vector<double> rawGridY;
    stepY = macro_conf->length[1] / macro_conf->seg_axis[1];
    double upperY = macro_conf->length[1] + macro_conf->origin[1];
    double lowerY = macro_conf->origin[1];
    for(double i = lowerY; i <= upperY;i += stepY){
        rawGridY.push_back(i);
    }
    for(int i = 0; i < macro_conf->getNumCond(); ++ i){
        rawGridY.push_back(macro_conf->cond_ori[i][1]);
        rawGridY.push_back(macro_conf->cond_ori[i][1] + macro_conf->cond_len[i][1]);
    }
    sort(rawGridY.begin(), rawGridY.end());
    for(int i = 0; i < rawGridY.size() - 1; ++ i){
        if(fabs(rawGridY[i] - rawGridY[i + 1]) > (stepY * precision)){
            gridY.push_back(rawGridY[i]);
        }
    }
    gridY.push_back(rawGridY[rawGridY.size() - 1]);
    
    
    std::vector<double> rawGridZ;
    stepZ = macro_conf->length[2] / macro_conf->seg_axis[2];
    double upperZ = macro_conf->length[2] + macro_conf->origin[2];
    double lowerZ = macro_conf->origin[2];
    for(double i = lowerZ; i <= upperZ;i += stepZ){
        rawGridZ.push_back(i);
    }
    for(int i = 0; i < macro_conf->getNumCond(); ++ i){
        rawGridZ.push_back(macro_conf->cond_ori[i][2]);
        rawGridZ.push_back(macro_conf->cond_ori[i][2] + macro_conf->cond_len[i][2]);
    }
    sort(rawGridZ.begin(), rawGridZ.end());
    for(int i = 0; i < rawGridZ.size() - 1; ++ i){
        if(fabs(rawGridZ[i] - rawGridZ[i + 1]) > (stepZ * precision)){
            gridZ.push_back(rawGridZ[i]);
        }
    }
    gridZ.push_back(rawGridZ[rawGridZ.size() - 1]);
    
    // total macro boudary grid ?
    brnk = 2 * ((gridX.size() - 1) * (gridY.size() - 1)
                    + (gridX.size() - 1) * (gridZ.size() - 1)
                    + (gridY.size() - 1) * (gridZ.size() - 1));
    
    // record map relationship 

    printf(">>Begin MAP\n");
    int idx  = 0;
    for(int i = 0; i < macro_conf->getNumCond(); ++ i){
        int itr = 0;
        while(itr < gridX.size() && gridX[itr] < macro_conf->cond_ori[i][0]){
            ++ itr;
        }
        int startX = itr;
        // take care '<' instead of '<='
        while(itr < gridX.size() && gridX[itr] < macro_conf->cond_ori[i][0] + macro_conf->cond_len[i][0]){
            ++ itr;
        }
        int endX = itr;
        
        itr = 0;
        while(itr < gridY.size() && gridY[itr] < macro_conf->cond_ori[i][1]){
            ++ itr;
        }
        int startY = itr;
        // take care '<' instead of '<='
        while(itr < gridY.size() && gridY[itr] < macro_conf->cond_ori[i][1] + macro_conf->cond_len[i][1]){
            ++ itr;
        }
        int endY = itr;
        
        itr = 0;
        while(itr < gridZ.size() && gridZ[itr] < macro_conf->cond_ori[i][2]){
            ++ itr;
        }
        int startZ = itr;
        // take care '<' instead of '<='
        while(itr < gridZ.size() && gridZ[itr] < macro_conf->cond_ori[i][2] + macro_conf->cond_len[i][2]){
            ++ itr;
        }
        int endZ = itr;
        
        // order those on the surface of conductors
        // bottom
        for(int j = startY; j < endY; ++ j){
            for(int i = startX; i < endX; ++ i){
                TPOINT tp;
                tp.x = (gridX[i] + gridX[i + 1]) / 2;
                tp.y = (gridY[j] + gridY[j + 1]) / 2;
                tp.z = gridZ[startZ];
                i2p.insert(std::make_pair(idx, tp));
                p2i.insert(std::make_pair(tp, idx));
                ++ idx;
            }
        }

        // left
        for(int k = startZ; k < endZ; ++ k){
            for(int j = endY - 1; j >= startY; -- j){
                TPOINT tp;
                tp.x = gridX[startX];
                tp.y = (gridY[j] + gridY[j + 1]) / 2;
                tp.z = (gridZ[k] + gridZ[k + 1]) / 2;
                i2p.insert(std::make_pair(idx, tp));
                p2i.insert(std::make_pair(tp, idx));
                ++ idx;
            }
        }

        // front
        for(int k = startZ; k < endZ; ++ k){
            for(int i = startX; i < endX; ++ i){
                TPOINT tp;
                tp.x = (gridX[i] + gridX[i + 1]) / 2;
                tp.y = gridY[startY];
                tp.z = (gridZ[k] + gridZ[k + 1]) / 2;
                i2p.insert(std::make_pair(idx, tp));
                p2i.insert(std::make_pair(tp, idx));
                ++ idx;
            }
        }

        // right
        for(int k = startZ; k < endZ; ++ k){
            for(int j = startY; j < endY; ++ j){
                TPOINT tp;
                tp.x = gridX[endX];
                tp.y = (gridY[j] + gridY[j + 1]) / 2;
                tp.z = (gridZ[k] + gridZ[k + 1]) / 2;
                i2p.insert(std::make_pair(idx, tp));
                p2i.insert(std::make_pair(tp, idx));
                ++ idx;
            }
        }

        // back
        for(int k = startZ; k < endZ; ++ k){
            for(int i = endX - 1; i >= startX; -- i){
                TPOINT tp;
                tp.x = (gridX[i] + gridX[i + 1]) / 2;
                tp.y = gridY[endY];
                tp.z = (gridZ[k] + gridZ[k + 1]) / 2;
                i2p.insert(std::make_pair(idx, tp));
                p2i.insert(std::make_pair(tp, idx));
                ++ idx;
            }
        }

        // top
        for(int j = startY; j < endY; ++ j){
            for(int i = startX; i < endX; ++ i){
                TPOINT tp;
                tp.x = (gridX[i] + gridX[i + 1]) / 2;
                tp.y = (gridY[j] + gridY[j + 1]) / 2;
                tp.z = gridZ[endZ];
                i2p.insert(std::make_pair(idx, tp));
                p2i.insert(std::make_pair(tp, idx));
                ++ idx;
            }
        }
    }
    
    // order those on the surface of macro
    // bottom
    for(int j = 0; j < gridY.size() - 1; ++ j){
        for(int i = 0; i < gridX.size() - 1; ++ i){
            TPOINT tp;
            tp.x = (gridX[i] + gridX[i + 1]) / 2;
            tp.y = (gridY[j] + gridY[j + 1]) / 2;
            tp.z = gridZ[0];
            i2p.insert(std::make_pair(idx, tp));
            p2i.insert(std::make_pair(tp, idx));
            ++ idx;
        }
    }

    // left
    for(int k = 0; k < gridZ.size() - 1; ++ k){
        for(int j = gridY.size() - 2; j >= 0; -- j){
            TPOINT tp;
            tp.x = gridX[0];
            tp.y = (gridY[j] + gridY[j + 1]) / 2;
            tp.z = (gridZ[k] + gridZ[k + 1]) / 2;
            i2p.insert(std::make_pair(idx, tp));
            p2i.insert(std::make_pair(tp, idx));
            ++ idx;
        }
    }

    // front
    for(int k = 0; k < gridZ.size() - 1; ++ k){
        for(int i = 0; i < gridX.size() - 1; ++ i){
            TPOINT tp;
            tp.x = (gridX[i] + gridX[i + 1]) / 2;
            tp.y = gridY[0];
            tp.z = (gridZ[k] + gridZ[k + 1]) / 2;
            i2p.insert(std::make_pair(idx, tp));
            p2i.insert(std::make_pair(tp, idx));
            ++ idx;
        }
    }

    // right
    for(int k = 0; k < gridZ.size() - 1; ++ k){
        for(int j = 0; j < gridY.size() - 1; ++ j){
            TPOINT tp;
            tp.x = gridX[gridX.size() - 1];
            tp.y = (gridY[j] + gridY[j + 1]) / 2;
            tp.z = (gridZ[k] + gridZ[k + 1]) / 2;
            i2p.insert(std::make_pair(idx, tp));
            p2i.insert(std::make_pair(tp, idx));
            ++ idx;
        }
    }

    // back
    for(int k = 0; k < gridZ.size() - 1; ++ k){
        for(int i = gridX.size() - 2; i >= 0; -- i){
            TPOINT tp;
            tp.x = (gridX[i] + gridX[i + 1]) / 2;
            tp.y = gridY[gridY.size() - 1];
            tp.z = (gridZ[k] + gridZ[k + 1]) / 2;
            i2p.insert(std::make_pair(idx, tp));
            p2i.insert(std::make_pair(tp, idx));
            ++ idx;
        }
    }

    // top
    for(int j = 0; j < gridY.size() - 1; ++ j){
        for(int i = 0; i < gridX.size() - 1; ++ i){
            TPOINT tp;
            tp.x = (gridX[i] + gridX[i + 1]) / 2;
            tp.y = (gridY[j] + gridY[j + 1]) / 2;
            tp.z = gridZ[gridZ.size() - 1];
            i2p.insert(std::make_pair(idx, tp));
            p2i.insert(std::make_pair(tp, idx));
            ++ idx;
        }
    }
    
    for(int k = 0; k < gridZ.size() - 1; ++ k){
        for(int j = 0; j < gridY.size() - 1; ++ j){
            for(int i = 0; i < gridX.size() - 1; ++ i){
                TPOINT tp;
                tp.x = (gridX[i] + gridX[i + 1]) / 2;
                tp.y = (gridY[j] + gridY[j + 1]) / 2;
                tp.z = (gridZ[k] + gridZ[k + 1]) / 2;
                
                if(!isInsideCond(tp.x, tp.y, tp.z)){
                    i2p.insert(std::make_pair(idx, tp));
                    p2i.insert(std::make_pair(tp, idx));
                    ++ idx;
                }
            }
        }
    }
    
    this->debg.Order_print(i2p);
    printf("i2p: %lu, p2i: %lu\n>>End MAP\n", i2p.size(), p2i.size());
    
    // the amount of inner elements
    inrnk = i2p.size() - crnk - brnk;
    
    // the elements position in 3 Dimension
    eleX.push_back(gridX[0]);
    for(int i = 1; i < gridX.size(); ++ i){
        eleX.push_back((gridX[i] + gridX[i - 1]) / 2);
        
        if(i + 1 == gridX.size()){
            break;
        }
        for(int j = 0; j < macro_conf->getNumCond(); ++ j){
            if(fabs(gridX[i] - macro_conf->cond_ori[j][0]) < stepX * precision || fabs(gridX[i] - macro_conf->cond_ori[j][0] - macro_conf->cond_len[j][0]) < stepX * precision){
                eleX.push_back(gridX[i]);
                break;
            }
        }
    }
    eleX.push_back(gridX[gridX.size() - 1]);
    
    eleY.push_back(gridY[0]);
    for(int i = 1; i < gridY.size(); ++ i){
        eleY.push_back((gridY[i] + gridY[i - 1]) / 2);
        
        if(i + 1 == gridY.size()){
            break;
        }
        for(int j = 0; j < macro_conf->getNumCond(); ++ j){
            if(fabs(gridY[i] - macro_conf->cond_ori[j][1]) < stepY * precision || fabs(gridY[i] - macro_conf->cond_ori[j][1] - macro_conf->cond_len[j][1]) < stepY * precision){
                eleY.push_back(gridY[i]);
                break;
            }
        }
    }
    eleY.push_back(gridY[gridY.size() - 1]);
    
    eleZ.push_back(gridZ[0]);
    for(int i = 1; i < gridZ.size(); ++ i){
        eleZ.push_back((gridZ[i] + gridZ[i - 1]) / 2);
        
        if(i + 1 == gridZ.size()){
            break;
        }
        for(int j = 0; j < macro_conf->getNumCond(); ++ j){
            if(fabs(gridZ[i] - macro_conf->cond_ori[j][2]) < stepZ * precision || fabs(gridZ[i] - macro_conf->cond_ori[j][2] - macro_conf->cond_len[j][2]) < stepZ * precision){
                eleZ.push_back(gridZ[i]);
                break;
            }
        }
    }
    eleZ.push_back(gridZ[gridZ.size() - 1]);
    
    this->Construct_Matrix_A11_A12_A13_nuf();
    this->A11.Debug("A11.txt");
    this->A12.Debug("A12.txt");
    this->A13.Debug("A13.txt");
    
    this->Construct_Matrix_A21_A22_A23_nuf();
    this->A21.Debug("A21.txt");
    this->A22.Debug("A22.txt");
    this->A23.Debug("A23.txt");
    
    
    this->Construct_Matrix_A31_A32_A33_nuf();
    this->A31.Debug("A31.txt");
    this->A32.Debug("A32.txt");
    this->A33.Debug("A33.txt");
    
    printf("have a try\n");
    
    printf("%d\n", A11.i.size());
    //solve();
    printf("Well\n");
    
    delete grid_numc;
}

bool FDM_Solver::isInsideCond(double x, double y, double z){
    bool inside = false;
    for(int cid = 0; cid < macro_conf->getNumCond(); ++ cid){
        if((x > macro_conf->cond_ori[cid][0]) && (x < (macro_conf->cond_ori[cid][0] + macro_conf->cond_len[cid][0]))){
            if((y > macro_conf->cond_ori[cid][1]) && (y < (macro_conf->cond_ori[cid][1] + macro_conf->cond_len[cid][1]))){
                if((z > macro_conf->cond_ori[cid][2]) && (z < (macro_conf->cond_ori[cid][2] + macro_conf->cond_len[cid][2]))){
                    //printf("z ");
                    inside = true;
                    break;
                }
            }
        }
    }
    return inside;
}

void FDM_Solver::Construct_Matrix_A11_A12_A13_nuf(){
    A11.n_row = crnk;
    A11.n_col = crnk;
    
    A12.n_row = crnk;
    A12.n_col = brnk;
    
    A13.n_row = crnk;
    A13.n_col = inrnk;
    
    for(int j = 0; j < crnk; ++ j){
        // center Point
        double dcenter = 0;
        
        TPOINT tp = i2p[j];
        
        // X dimension
        for(int i = 0; i < eleX.size(); ++ i){
            if(fabs(tp.x - eleX[i]) < (stepX * precision)){
                if(i + 2 < eleX.size() && i > 1){
                    if(isInsideCond(eleX[i - 1], tp.y, tp.z)){
                        TPOINT tp1 = tp;
                        int m = i;
                        do{
                            ++ m;
                            tp1.x = eleX[m];
                        }while(p2i.count(tp1) <= 0);
                        int rnk1 = p2i[tp1];
                        
                        TPOINT tp2 = tp;
                        int k = m;
                        do{
                            ++ k;
                            tp2.x = eleX[k];
                        }while(p2i.count(tp2) <= 0);
                        int rnk2 = p2i[tp2];
                        
                        double h1 = fabs(tp.x - tp1.x);
                        double h2 = fabs(tp1.x - tp2.x);
                        
                        if(rnk1 >= crnk){
                            rnk1 -= crnk;
                            if(rnk1 >= brnk){
                                rnk1 -= brnk;
                                A13.i.push_back(j);
                                A13.j.push_back(rnk1);
                                A13.x.push_back(-(h1 + h2)/h1/h2);
                            }else{
                                A12.i.push_back(j);
                                A12.j.push_back(rnk1);
                                A12.x.push_back(-(h1 + h2)/h1/h2);
                            }
                        }else{
                            A11.i.push_back(j);
                            A11.j.push_back(rnk1);
                            A11.x.push_back(-(h1 + h2)/h1/h2);
                        }
                        
                        if(rnk2 >= crnk){
                            rnk2 -= crnk;
                            if(rnk2 >= brnk){
                                rnk2 -= brnk;
                                A13.i.push_back(j);
                                A13.j.push_back(rnk2);
                                A13.x.push_back(h1/(h1 + h2)/h2);
                            }else{
                                A12.i.push_back(j);
                                A12.j.push_back(rnk2);
                                A12.x.push_back(h1/(h1 + h2)/h2);
                            }
                        }else{
                            A11.i.push_back(j);
                            A11.j.push_back(rnk2);
                            A11.x.push_back(h1/(h1 + h2)/h2);
                        }
                        
                        
                        dcenter = (2 * h1 + h2)/h1/(h1 + h2);
                    }else if(isInsideCond(eleX[i + 1], tp.y, tp.z)){
                        TPOINT tp1 = tp;
                        int m = i;
                        do{
                            -- m;
                            tp1.x = eleX[m];
                        }while(p2i.count(tp1) <= 0);
                        int rnk1 = p2i[tp1];
                        
                        TPOINT tp2 = tp;
                        int k = m;
                        do{
                            -- k;
                            tp2.x = eleX[k];
                        }while(p2i.count(tp2) <= 0);
                        int rnk2 = p2i[tp2];
                        
                        double h1 = fabs(tp.x - tp1.x);
                        double h2 = fabs(tp1.x - tp2.x);
                        
                        if(rnk1 >= crnk){
                            rnk1 -= crnk;
                            if(rnk1 >= brnk){
                                rnk1 -= brnk;
                                A13.i.push_back(j);
                                A13.j.push_back(rnk1);
                                A13.x.push_back(-(h1 + h2)/h1/h2);
                            }else{
                                A12.i.push_back(j);
                                A12.j.push_back(rnk1);
                                A12.x.push_back(-(h1 + h2)/h1/h2);
                            }
                        }else{
                            A11.i.push_back(j);
                            A11.j.push_back(rnk1);
                            A11.x.push_back(-(h1 + h2)/h1/h2);
                        }
                        
                        if(rnk2 >= crnk){
                            rnk2 -= crnk;
                            if(rnk2 >= brnk){
                                rnk2 -= brnk;
                                A13.i.push_back(j);
                                A13.j.push_back(rnk2);
                                A13.x.push_back(h1/(h1 + h2)/h2);
                            }else{
                                A12.i.push_back(j);
                                A12.j.push_back(rnk2);
                                A12.x.push_back(h1/(h1 + h2)/h2);
                            }
                        }else{
                            A11.i.push_back(j);
                            A11.j.push_back(rnk2);
                            A11.x.push_back(h1/(h1 + h2)/h2);
                        }
                        
                        dcenter = (2 * h1 + h2)/h1/(h1 + h2);
                    }
                }
                
                break;
            }
        }
        
        
        
        // Y dimension
        for(int i = 0; i < eleY.size(); ++ i){
            if(fabs(tp.y - eleY[i]) < (stepY * precision)){
                if(i + 2 < eleY.size() && i > 1){
                    if(isInsideCond(tp.x, eleY[i - 1], tp.z)){
                        TPOINT tp1 = tp;
                        int m = i;
                        do{
                            ++ m;
                            tp1.y = eleY[m];
                        }while(p2i.count(tp1) <= 0);
                        int rnk1 = p2i[tp1];
                        
                        TPOINT tp2 = tp;
                        int k = m;
                        do{
                            ++ k;
                            tp2.y = eleY[k];
                        }while(p2i.count(tp2) <= 0);
                        int rnk2 = p2i[tp2];
                        
                        double h1 = fabs(tp.y - tp1.y);
                        double h2 = fabs(tp1.y - tp2.y);
                        
                        if(rnk1 >= crnk){
                            rnk1 -= crnk;
                            if(rnk1 >= brnk){
                                rnk1 -= brnk;
                                A13.i.push_back(j);
                                A13.j.push_back(rnk1);
                                A13.x.push_back(-(h1 + h2)/h1/h2);
                            }else{
                                A12.i.push_back(j);
                                A12.j.push_back(rnk1);
                                A12.x.push_back(-(h1 + h2)/h1/h2);
                            }
                        }else{
                            A11.i.push_back(j);
                            A11.j.push_back(rnk1);
                            A11.x.push_back(-(h1 + h2)/h1/h2);
                        }
                        
                        if(rnk2 >= crnk){
                            rnk2 -= crnk;
                            if(rnk2 >= brnk){
                                rnk2 -= brnk;
                                A13.i.push_back(j);
                                A13.j.push_back(rnk2);
                                A13.x.push_back(h1/(h1 + h2)/h2);
                            }else{
                                A12.i.push_back(j);
                                A12.j.push_back(rnk2);
                                A12.x.push_back(h1/(h1 + h2)/h2);
                            }
                        }else{
                            A11.i.push_back(j);
                            A11.j.push_back(rnk2);
                            A11.x.push_back(h1/(h1 + h2)/h2);
                        }
                        
                        dcenter = (2 * h1 + h2)/h1/(h1 + h2);
                    }else if(isInsideCond(tp.x, eleY[i + 1], tp.z)){
                        TPOINT tp1 = tp;
                        int m = i;
                        do{
                            -- m;
                            tp1.y = eleY[m];
                        }while(p2i.count(tp1) <= 0);
                        int rnk1 = p2i[tp1];
                        
                        TPOINT tp2 = tp;
                        int k = m;
                        do{
                            -- k;
                            tp2.y = eleY[k];
                        }while(p2i.count(tp2) <= 0);
                        int rnk2 = p2i[tp2];
                        
                        double h1 = fabs(tp.y - tp1.y);
                        double h2 = fabs(tp1.y - tp2.y);
                        
                        if(rnk1 >= crnk){
                            rnk1 -= crnk;
                            if(rnk1 >= brnk){
                                rnk1 -= brnk;
                                A13.i.push_back(j);
                                A13.j.push_back(rnk1);
                                A13.x.push_back(-(h1 + h2)/h1/h2);
                            }else{
                                A12.i.push_back(j);
                                A12.j.push_back(rnk1);
                                A12.x.push_back(-(h1 + h2)/h1/h2);
                            }
                        }else{
                            A11.i.push_back(j);
                            A11.j.push_back(rnk1);
                            A11.x.push_back(-(h1 + h2)/h1/h2);
                        }
                        
                        if(rnk2 >= crnk){
                            rnk2 -= crnk;
                            if(rnk2 >= brnk){
                                rnk2 -= brnk;
                                A13.i.push_back(j);
                                A13.j.push_back(rnk2);
                                A13.x.push_back(h1/(h1 + h2)/h2);
                            }else{
                                A12.i.push_back(j);
                                A12.j.push_back(rnk2);
                                A12.x.push_back(h1/(h1 + h2)/h2);
                            }
                        }else{
                            A11.i.push_back(j);
                            A11.j.push_back(rnk2);
                            A11.x.push_back(h1/(h1 + h2)/h2);
                        }
                        
                        dcenter = (2 * h1 + h2)/h1/(h1 + h2);
                    }
                }
                
                break;
            }
        }
   
        
   
        // Z dimension
        for(int i = 0; i < eleZ.size(); ++ i){
            if(fabs(tp.z - eleZ[i]) < (stepZ * precision)){
                if(i + 2 < eleZ.size() && i > 1){
                    if(isInsideCond(tp.x, tp.y, eleZ[i - 1])){
                        TPOINT tp1 = tp;
                        int m = i;
                        do{
                            ++ m;
                            tp1.z = eleZ[m];
                        }while(p2i.count(tp1) <= 0);
                        int rnk1 = p2i[tp1];
                        
                        TPOINT tp2 = tp;
                        int k = m;
                        do{
                            ++ k;
                            tp2.z = eleZ[k];
                        }while(p2i.count(tp2) <= 0);
                        int rnk2 = p2i[tp2];
                        
                        double h1 = fabs(tp.z - tp1.z);
                        double h2 = fabs(tp1.z - tp2.z);
                        
                        if(rnk1 >= crnk){
                            rnk1 -= crnk;
                            if(rnk1 >= brnk){
                                rnk1 -= brnk;
                                A13.i.push_back(j);
                                A13.j.push_back(rnk1);
                                A13.x.push_back(-(h1 + h2)/h1/h2);
                            }else{
                                A12.i.push_back(j);
                                A12.j.push_back(rnk1);
                                A12.x.push_back(-(h1 + h2)/h1/h2);
                            }
                        }else{
                            A11.i.push_back(j);
                            A11.j.push_back(rnk1);
                            A11.x.push_back(-(h1 + h2)/h1/h2);
                        }
                        
                        if(rnk2 >= crnk){
                            rnk2 -= crnk;
                            if(rnk2 >= brnk){
                                rnk2 -= brnk;
                                A13.i.push_back(j);
                                A13.j.push_back(rnk2);
                                A13.x.push_back(h1/(h1 + h2)/h2);
                            }else{
                                A12.i.push_back(j);
                                A12.j.push_back(rnk2);
                                A12.x.push_back(h1/(h1 + h2)/h2);
                            }
                        }else{
                            A11.i.push_back(j);
                            A11.j.push_back(rnk2);
                            A11.x.push_back(h1/(h1 + h2)/h2);
                        }
                        
                        dcenter = (2 * h1 + h2)/h1/(h1 + h2);
                    }else if(isInsideCond(tp.x, tp.y, eleZ[i + 1])){
                        TPOINT tp1 = tp;
                        int m = i;
                        do{
                            --m;
                            tp1.z = eleZ[m];
                        }while(p2i.count(tp1) <= 0);
                        int rnk1 = p2i[tp1];
                        
                        TPOINT tp2 = tp;
                        int k = m;
                        do{
                            -- k;
                            tp2.z = eleZ[k];
                        }while(p2i.count(tp2) <= 0);
                        int rnk2 = p2i[tp2];
                        
                        double h1 = fabs(tp.z - tp1.z);
                        double h2 = fabs(tp1.z - tp2.z);
                        
                        if(rnk1 >= crnk){
                            rnk1 -= crnk;
                            if(rnk1 >= brnk){
                                rnk1 -= brnk;
                                A13.i.push_back(j);
                                A13.j.push_back(rnk1);
                                A13.x.push_back(-(h1 + h2)/h1/h2);
                            }else{
                                A12.i.push_back(j);
                                A12.j.push_back(rnk1);
                                A12.x.push_back(-(h1 + h2)/h1/h2);
                            }
                        }else{
                            A11.i.push_back(j);
                            A11.j.push_back(rnk1);
                            A11.x.push_back(-(h1 + h2)/h1/h2);
                        }
                        
                        if(rnk2 >= crnk){
                            rnk2 -= crnk;
                            if(rnk2 >= brnk){
                                rnk2 -= brnk;
                                A13.i.push_back(j);
                                A13.j.push_back(rnk2);
                                A13.x.push_back(h1/(h1 + h2)/h2);
                            }else{
                                A12.i.push_back(j);
                                A12.j.push_back(rnk2);
                                A12.x.push_back(h1/(h1 + h2)/h2);
                            }
                        }else{
                            A11.i.push_back(j);
                            A11.j.push_back(rnk2);
                            A11.x.push_back(h1/(h1 + h2)/h2);
                        }
                        
                        dcenter = (2 * h1 + h2)/h1/(h1 + h2);
                    }
                }
                
                break;
            }
        }
        
        
        
        A11.i.push_back(j);
        A11.j.push_back(j);
        A11.x.push_back(dcenter);
    }
    
    int status;
    A11.nz = A11.i.size();
    A11.Ap.resize(A11.n_col + 1, 0);
    A11.Ai.resize(A11.nz, 0);
    A11.Ax.resize(A11.nz, 0);
    
    status = umfpack_di_triplet_to_col(A11.n_row, A11.n_col, A11.nz, A11.i.data(), A11.j.data(), A11.x.data(), A11.Ap.data(), A11.Ai.data(), A11.Ax.data(), NULL);
    
    /*
    int *A11i = new int[A11.nz](); for (int i=0; i<A11.nz; ++i) A11i[i] = A11.i[i];
    int *A11j = new int[A11.nz](); for (int i=0; i<A11.nz; ++i) A11j[i] = A11.j[i];
    int *A11x = new int[A11.nz](); for (int i=0; i<A11.nz; ++i) A11x[i] = A11.x[i];
    int *A11Ap = new int[A11.nz](); for (int i=0; i<A11.nz; ++i) A11Ap[i] = A11.Ap[i];
    int *A11Ai = new int[A11.nz](); for (int i=0; i<A11.nz; ++i) A11Ai[i] = A11.Ai[i];
    int *A11Ax = new int[A11.nz](); for (int i=0; i<A11.nz; ++i) A11Ax[i] = A11.Ax[i];
    
    
    status = umfpack_di_triplet_to_col(A11.n_row, A11.n_col, A11.nz, A11i, A11j, A11x, A11Ap, A11Ai, A11Ax, NULL);
    
    for(int i=0; i<A11.nz; ++i)
    
    delete[] A11i;
    delete[] A11j;
    delete[] A11x;
    delete[] A11Ap;
    delete[] A11Ai;
    delete[] A11Ax;
    */
    
    if(status != 0){
        printf("A11: triplet_to_col goes wrong!\n");
    }
    
    //int status;
    if(A12.nz > 0){
        A12.nz = A12.i.size();
        A12.Ap.resize(A12.n_col + 1, 0);
        A12.Ai.resize(A12.nz, 0);
        A12.Ax.resize(A12.nz, 0);
        
        /*
        int *A12i = new int[A12.nz](); for (int i=0; i<A12.nz; ++i) A12i[i] = A12.i[i];
        int *A12j = new int[A12.nz](); for (int i=0; i<A12.nz; ++i) A12j[i] = A12.j[i];
        int *A12x = new int[A12.nz](); for (int i=0; i<A12.nz; ++i) A12x[i] = A12.x[i];
        int *A12Ap = new int[A12.nz](); for (int i=0; i<A12.nz; ++i) A12Ap[i] = A12.Ap[i];
        int *A12Ai = new int[A12.nz](); for (int i=0; i<A12.nz; ++i) A12Ai[i] = A12.Ai[i];
        int *A12Ax = new int[A12.nz](); for (int i=0; i<A12.nz; ++i) A12Ax[i] = A12.Ax[i];
        */
        
        
        status = umfpack_di_triplet_to_col(A12.n_row, A12.n_col, A12.nz, &A12.i[0], &A12.j[0], &A12.x[0], &A12.Ap[0], &A12.Ai[0], &A12.Ax[0], NULL);
        
        /*
        delete[] A12i;
        delete[] A12j;
        delete[] A12x;
        delete[] A12Ap;
        delete[] A12Ai;
        delete[] A12Ax;
        */
        
        if(status != 0){
            printf("A12: triplet_to_col goes wrong!\n");
        }
    }else if(A12.nz == 0){
        A12.nz = A12.i.size();
        A12.Ap.resize(A12.n_col + 1, 0);
        A12.Ai.resize(A12.nz + 1, 0);
        A12.Ax.resize(A12.nz + 1, 0);
        A12.i.resize(A12.nz + 1, 0);
        A12.j.resize(A12.nz + 1, 0);
        A12.x.resize(A12.nz + 1, 0);
        status = umfpack_di_triplet_to_col(A12.n_row, A12.n_col, A12.nz, &A12.i[0], &A12.j[0], &A12.x[0], &A12.Ap[0], &A12.Ai[0], &A12.Ax[0], NULL);
        if(status != 0){
            printf("A12: triplet_to_col goes wrong! status:%d\n", status);
        }
    }
    
    //int status;
    A13.nz = A13.i.size();
    A13.Ap.resize(A13.n_col + 1, 0);
    A13.Ai.resize(A13.nz, 0);
    A13.Ax.resize(A13.nz, 0);
    status = umfpack_di_triplet_to_col(A13.n_row, A13.n_col, A13.nz, A13.i.data(), A13.j.data(), A13.x.data(), A13.Ap.data(), A13.Ai.data(), A13.Ax.data(), NULL);
    if(status != 0){
        printf("A13: triplet_to_col goes wrong!\n");
    }
    
    return;
}

void FDM_Solver::Construct_Matrix_A21_A22_A23_nuf(){
    A21.n_row = brnk;
    A21.n_col = crnk;
    
    A22.n_row = brnk;
    A22.n_col = brnk;
    
    A23.n_row = brnk;
    A23.n_col = inrnk;
    
    int sum = crnk + brnk;
    for(int j = crnk; j < sum; ++ j){
        double dcenter = 0;
        TPOINT tp = i2p[j];
        
        // X dimension
        if(fabs(tp.x - eleX[0]) < (stepX * precision)){
            TPOINT tp1 = tp;
            int m = 0;
            do{
                ++ m;
                tp1.x = eleX[m];
            }while(p2i.count(tp1) <= 0);
            int rnk1 = p2i[tp1];
            
            TPOINT tp2 = tp;
            int k = m;
            do{
                ++ k;
                tp2.x = eleX[k];
            }while(p2i.count(tp2) <= 0);
            int rnk2 = p2i[tp2];
            
            double h1 = fabs(tp.x - tp1.x);
            double h2 = fabs(tp1.x - tp2.x);
            
            if(rnk1 >= crnk){
                rnk1 -= crnk;
                if(rnk1 >= brnk){
                    rnk1 -= brnk;
                    A23.i.push_back(j - crnk);
                    A23.j.push_back(rnk1);
                    A23.x.push_back(-(h1 + h2)/h1/h2);
                }else{
                    A22.i.push_back(j - crnk);
                    A22.j.push_back(rnk1);
                    A22.x.push_back(-(h1 + h2)/h1/h2);
                }
            }else{
                A21.i.push_back(j - crnk);
                A21.j.push_back(rnk1);
                A21.x.push_back(-(h1 + h2)/h1/h2);
            }
            
            if(rnk2 >= crnk){
                rnk2 -= crnk;
                if(rnk2 >= brnk){
                    rnk2 -= brnk;
                    A23.i.push_back(j - crnk);
                    A23.j.push_back(rnk2);
                    A23.x.push_back(h1/(h1 + h2)/h2);
                }else{
                    A22.i.push_back(j - crnk);
                    A22.j.push_back(rnk2);
                    A22.x.push_back(h1/(h1 + h2)/h2);
                }
            }else{
                A21.i.push_back(j - crnk);
                A21.j.push_back(rnk2);
                A21.x.push_back(h1/(h1 + h2)/h2);
            }
            
            
            dcenter = (2 * h1 + h2)/h1/(h1 + h2);
        }else if(fabs(tp.x - eleX[eleX.size() - 1]) < (stepX * precision)){
            TPOINT tp1 = tp;
            int m = eleX.size() - 1;
            do{
                -- m;
                tp1.x = eleX[m];
            }while(p2i.count(tp1) <= 0);
            int rnk1 = p2i[tp1];
            
            TPOINT tp2 = tp;
            int k = m;
            do{
                -- k;
                tp2.x = eleX[k];
            }while(p2i.count(tp2) <= 0);
            int rnk2 = p2i[tp2];
            
            double h1 = fabs(tp.x - tp1.x);
            double h2 = fabs(tp1.x - tp2.x);
            
            if(rnk1 >= crnk){
                rnk1 -= crnk;
                if(rnk1 >= brnk){
                    rnk1 -= brnk;
                    A23.i.push_back(j - crnk);
                    A23.j.push_back(rnk1);
                    A23.x.push_back(-(h1 + h2)/h1/h2);
                }else{
                    A22.i.push_back(j - crnk);
                    A22.j.push_back(rnk1);
                    A22.x.push_back(-(h1 + h2)/h1/h2);
                }
            }else{
                A21.i.push_back(j - crnk);
                A21.j.push_back(rnk1);
                A21.x.push_back(-(h1 + h2)/h1/h2);
            }
            
            if(rnk2 >= crnk){
                rnk2 -= crnk;
                if(rnk2 >= brnk){
                    rnk2 -= brnk;
                    A23.i.push_back(j);
                    A23.j.push_back(rnk2);
                    A23.x.push_back(h1/(h1 + h2)/h2);
                }else{
                    A22.i.push_back(j);
                    A22.j.push_back(rnk2);
                    A22.x.push_back(h1/(h1 + h2)/h2);
                }
            }else{
                A21.i.push_back(j);
                A21.j.push_back(rnk2);
                A21.x.push_back(h1/(h1 + h2)/h2);
            }
            
            
            dcenter = (2 * h1 + h2)/h1/(h1 + h2);
        }
        
        
        // Y dimension
        if(fabs(tp.y - eleY[0]) < (stepY * precision)){
            TPOINT tp1 = tp;
            int m = 0;
            do{
                ++ m;
                tp1.y = eleY[m];
            }while(p2i.count(tp1) <= 0);
            int rnk1 = p2i[tp1];
            
            TPOINT tp2 = tp;
            int k = m;
            do{
                ++ k;
                tp2.y = eleY[k];
            }while(p2i.count(tp2) <= 0);
            int rnk2 = p2i[tp2];
            
            double h1 = fabs(tp.y - tp1.y);
            double h2 = fabs(tp1.y - tp2.y);
            
            if(rnk1 >= crnk){
                rnk1 -= crnk;
                if(rnk1 >= brnk){
                    rnk1 -= brnk;
                    A23.i.push_back(j - crnk);
                    A23.j.push_back(rnk1);
                    A23.x.push_back(-(h1 + h2)/h1/h2);
                }else{
                    A22.i.push_back(j - crnk);
                    A22.j.push_back(rnk1);
                    A22.x.push_back(-(h1 + h2)/h1/h2);
                }
            }else{
                A21.i.push_back(j - crnk);
                A21.j.push_back(rnk1);
                A21.x.push_back(-(h1 + h2)/h1/h2);
            }
            
            if(rnk2 >= crnk){
                rnk2 -= crnk;
                if(rnk2 >= brnk){
                    rnk2 -= brnk;
                    A23.i.push_back(j - crnk);
                    A23.j.push_back(rnk2);
                    A23.x.push_back(h1/(h1 + h2)/h2);
                }else{
                    A22.i.push_back(j - crnk);
                    A22.j.push_back(rnk2);
                    A22.x.push_back(h1/(h1 + h2)/h2);
                }
            }else{
                A21.i.push_back(j - crnk);
                A21.j.push_back(rnk2);
                A21.x.push_back(h1/(h1 + h2)/h2);
            }
            
            
            dcenter = (2 * h1 + h2)/h1/(h1 + h2);
        }else if(fabs(tp.y - eleY[eleY.size() - 1]) < (stepY * precision)){
            TPOINT tp1 = tp;
            int m = eleY.size() - 1;
            do{
                -- m;
                tp1.y = eleY[m];
            }while(p2i.count(tp1) <= 0);
            int rnk1 = p2i[tp1];
            
            TPOINT tp2 = tp;
            int k = m;
            do{
                -- k;
                tp2.y = eleY[k];
            }while(p2i.count(tp2) <= 0);
            int rnk2 = p2i[tp2];
            
            double h1 = fabs(tp.y - tp1.y);
            double h2 = fabs(tp1.y - tp2.y);
            
            if(rnk1 >= crnk){
                rnk1 -= crnk;
                if(rnk1 >= brnk){
                    rnk1 -= brnk;
                    A23.i.push_back(j - crnk);
                    A23.j.push_back(rnk1);
                    A23.x.push_back(-(h1 + h2)/h1/h2);
                }else{
                    A22.i.push_back(j - crnk);
                    A22.j.push_back(rnk1);
                    A22.x.push_back(-(h1 + h2)/h1/h2);
                }
            }else{
                A21.i.push_back(j - crnk);
                A21.j.push_back(rnk1);
                A21.x.push_back(-(h1 + h2)/h1/h2);
            }
            
            if(rnk2 >= crnk){
                rnk2 -= crnk;A12.Ai.resize(A12.nz + 1, 0);
                if(rnk2 >= brnk){
                    rnk2 -= brnk;
                    A23.i.push_back(j - crnk);
                    A23.j.push_back(rnk2);
                    A23.x.push_back(h1/(h1 + h2)/h2);
                }else{
                    A22.i.push_back(j - crnk);
                    A22.j.push_back(rnk2);
                    A22.x.push_back(h1/(h1 + h2)/h2);
                }
            }else{
                A21.i.push_back(j - crnk);
                A21.j.push_back(rnk2);
                A21.x.push_back(h1/(h1 + h2)/h2);
            }
            
            
            dcenter = (2 * h1 + h2)/h1/(h1 + h2);
        }

        
        // Z dimension
        if(fabs(tp.z - eleZ[0]) < (stepZ * precision)){
            TPOINT tp1 = tp;
            int m = 0;
            do{
                ++ m;
                tp1.z = eleZ[m];
            }while(p2i.count(tp1) <= 0);
            int rnk1 = p2i[tp1];
            
            TPOINT tp2 = tp;
            int k = m;
            do{
                ++ k;
                tp2.z = eleZ[k];
            }while(p2i.count(tp2) <= 0);
            int rnk2 = p2i[tp2];
            
            double h1 = fabs(tp.z - tp1.z);
            double h2 = fabs(tp1.z - tp2.z);
            
            if(rnk1 >= crnk){
                rnk1 -= crnk;
                if(rnk1 >= brnk){
                    rnk1 -= brnk;
                    A23.i.push_back(j - crnk);
                    A23.j.push_back(rnk1);
                    A23.x.push_back(-(h1 + h2)/h1/h2);
                }else{
                    A22.i.push_back(j - crnk);
                    A22.j.push_back(rnk1);
                    A22.x.push_back(-(h1 + h2)/h1/h2);
                }
            }else{
                A21.i.push_back(j - crnk);
                A21.j.push_back(rnk1);
                A21.x.push_back(-(h1 + h2)/h1/h2);
            }
            
            if(rnk2 >= crnk){
                rnk2 -= crnk;
                if(rnk2 >= brnk){
                    rnk2 -= brnk;
                    A23.i.push_back(j - crnk);
                    A23.j.push_back(rnk2);
                    A23.x.push_back(h1/(h1 + h2)/h2);
                }else{
                    A22.i.push_back(j - crnk);
                    A22.j.push_back(rnk2);
                    A22.x.push_back(h1/(h1 + h2)/h2);
                }
            }else{
                A21.i.push_back(j - crnk);
                A21.j.push_back(rnk2);
                A21.x.push_back(h1/(h1 + h2)/h2);
            }
            
            
            dcenter = (2 * h1 + h2)/h1/(h1 + h2);
        }else if(fabs(tp.z - eleZ[eleZ.size() - 1]) < (stepZ * precision)){
            TPOINT tp1 = tp;
            int m = eleZ.size() - 1;
            do{
                -- m;
                tp1.z = eleZ[m];
            }while(p2i.count(tp1) <= 0);
            int rnk1 = p2i[tp1];
            
            TPOINT tp2 = tp;
            int k = m;
            do{
                -- k;
                tp2.z = eleZ[k];
            }while(p2i.count(tp2) <= 0);
            int rnk2 = p2i[tp2];
            
            double h1 = fabs(tp.z - tp1.z);
            double h2 = fabs(tp1.z - tp2.z);
            
            if(rnk1 >= crnk){
                rnk1 -= crnk;
                if(rnk1 >= brnk){
                    rnk1 -= brnk;
                    A23.i.push_back(j - crnk);
                    A23.j.push_back(rnk1);
                    A23.x.push_back(-(h1 + h2)/h1/h2);
                }else{
                    A22.i.push_back(j - crnk);
                    A22.j.push_back(rnk1);
                    A22.x.push_back(-(h1 + h2)/h1/h2);
                }
            }else{
                A21.i.push_back(j - crnk);
                A21.j.push_back(rnk1);
                A21.x.push_back(-(h1 + h2)/h1/h2);
            }
            
            if(rnk2 >= crnk){
                rnk2 -= crnk;
                if(rnk2 >= brnk){
                    rnk2 -= brnk;
                    A23.i.push_back(j - crnk);
                    A23.j.push_back(rnk2);
                    A23.x.push_back(h1/(h1 + h2)/h2);
                }else{
                    A22.i.push_back(j - crnk);
                    A22.j.push_back(rnk2);
                    A22.x.push_back(h1/(h1 + h2)/h2);
                }
            }else{
                A21.i.push_back(j - crnk);
                A21.j.push_back(rnk2);
                A21.x.push_back(h1/(h1 + h2)/h2);
            }
            
            
            dcenter = (2 * h1 + h2)/h1/(h1 + h2);
        }
        
        A22.i.push_back(j - crnk);
        A22.j.push_back(j - crnk);
        A22.x.push_back(dcenter);
    }
    
    
    int status;
    if(A12.nz > 0){
        A21.nz = A21.i.size();
        A21.Ap.resize(A21.n_col + 1, 0);
        A21.Ai.resize(A21.nz, 0);
        A21.Ax.resize(A21.nz, 0);
        status = umfpack_di_triplet_to_col(A21.n_row, A21.n_col, A21.nz, &A21.i[0], &A21.j[0], &A21.x[0], &A21.Ap[0], &A21.Ai[0], &A21.Ax[0], NULL);
        if(status != 0){
            printf("A21: triplet_to_col goes wrong!\n");
        }
    }else if(A12.nz == 0){
        A21.nz = A21.i.size();
        A21.Ap.resize(A21.n_col + 1, 0);
        A21.Ai.resize(A21.nz + 1, 0);
        A21.Ax.resize(A21.nz + 1, 0);
        A21.i.resize(A21.nz + 1, 0);
        A21.j.resize(A21.nz + 1, 0);
        A21.x.resize(A21.nz + 1, 0);
        status = umfpack_di_triplet_to_col(A21.n_row, A21.n_col, A21.nz, &A21.i[0], &A21.j[0], &A21.x[0], &A21.Ap[0], &A21.Ai[0], &A21.Ax[0], NULL);
        if(status != 0){
            printf("A21: triplet_to_col goes wrong! status:%d\n", status);
        }
    }
    
    
    //int status;
    A22.nz = A22.i.size();
    A22.Ap.resize(A22.n_col + 1, 0);
    A22.Ai.resize(A22.nz, 0);
    A22.Ax.resize(A22.nz, 0);
    status = umfpack_di_triplet_to_col(A22.n_row, A22.n_col, A22.nz, &A22.i[0], &A22.j[0], &A22.x[0], &A22.Ap[0], &A22.Ai[0], &A22.Ax[0], NULL);
    if(status != 0){
        printf("A22: triplet_to_col goes wrong!\n");
    }
    
    //int status;
    A23.nz = A23.i.size();
    A23.Ap.resize(A23.n_col + 1, 0);
    A23.Ai.resize(A23.nz, 0);
    A23.Ax.resize(A23.nz, 0);
    status = umfpack_di_triplet_to_col(A23.n_row, A23.n_col, A23.nz, &A23.i[0], &A23.j[0], &A23.x[0], &A23.Ap[0], &A23.Ai[0], &A23.Ax[0], NULL);
    if(status != 0){
        printf("A23: triplet_to_col goes wrong!\n");
    }
    
    return;
}

void FDM_Solver::Construct_Matrix_A31_A32_A33_nuf(){
    A31.n_row = inrnk;
    A31.n_col = crnk;
    
    A32.n_row = inrnk;
    A32.n_col = brnk;
    
    A33.n_row = inrnk;
    A33.n_col = inrnk;
    
    int dsum = crnk + brnk;
    int usum = dsum + inrnk;
    for(int j = dsum; j < usum; ++ j){
        double dcenter = 0;
        TPOINT tp = i2p[j];
        
        // X dimension
        for(int i = 0; i < eleX.size(); ++ i){
            if(fabs(tp.x - eleX[i]) < (stepX * precision)){
                TPOINT tp1 = tp;
                int m = i;
                do{
                    ++ m;
                    tp1.x = eleX[m];
                }while(p2i.count(tp1) <= 0);
                int rnk1 = p2i[tp1];
                
                TPOINT tp2 = tp;
                int k = i;
                do{
                    -- k;
                    tp2.x = eleX[k];
                }while(p2i.count(tp2) <= 0);
                int rnk2 = p2i[tp2];
                
                double h1 = fabs(tp.x - tp1.x);
                double h2 = fabs(tp.x - tp2.x);
                
                if(rnk1 >= crnk){
                    rnk1 -= crnk;
                    if(rnk1 >= brnk){
                        rnk1 -= brnk;
                        A33.i.push_back(j - dsum);
                        A33.j.push_back(rnk1);
                        A33.x.push_back(2/h1/(h1+h2));
                    }else{
                        A32.i.push_back(j - dsum);
                        A32.j.push_back(rnk1);
                        A32.x.push_back(2/h1/(h1+h2));
                    }
                }else{
                    A31.i.push_back(j - dsum);
                    A31.j.push_back(rnk1);
                    A31.x.push_back(2/h1/(h1+h2));
                }
                
                if(rnk2 >= crnk){
                    rnk2 -= crnk;
                    if(rnk2 >= brnk){
                        rnk2 -= brnk;
                        A33.i.push_back(j - dsum);
                        A33.j.push_back(rnk2);
                        A33.x.push_back(2/h2/(h1+h2));
                    }else{
                        A32.i.push_back(j - dsum);
                        A32.j.push_back(rnk2);
                        A32.x.push_back(2/h2/(h1+h2));
                    }
                }else{
                    A31.i.push_back(j - dsum);
                    A31.j.push_back(rnk2);
                    A31.x.push_back(2/h2/(h1+h2));
                }
                
                
                dcenter += -2/h1/h2;
                
                break;
            }
        }
        
        // Y dimension
        for(int i = 0; i < eleY.size(); ++ i){
            if(fabs(tp.y - eleY[i]) < (stepY * precision)){
                TPOINT tp1 = tp;
                int m = i;
                do{
                    ++ m;
                    tp1.y = eleY[m];
                }while(p2i.count(tp1) <= 0);
                int rnk1 = p2i[tp1];
                
                TPOINT tp2 = tp;
                int k = i;
                do{
                    -- k;
                    tp2.y = eleY[k];
                }while(p2i.count(tp2) <= 0);
                int rnk2 = p2i[tp2];
                
                double h1 = fabs(tp.y - tp1.y);
                double h2 = fabs(tp.y - tp2.y);
                
                if(rnk1 >= crnk){
                    rnk1 -= crnk;
                    if(rnk1 >= brnk){
                        rnk1 -= brnk;
                        A33.i.push_back(j - dsum);
                        A33.j.push_back(rnk1);
                        A33.x.push_back(2/h1/(h1+h2));
                    }else{
                        A32.i.push_back(j - dsum);
                        A32.j.push_back(rnk1);
                        A32.x.push_back(2/h1/(h1+h2));
                    }
                }else{
                    A31.i.push_back(j - dsum);
                    A31.j.push_back(rnk1);
                    A31.x.push_back(2/h1/(h1+h2));
                }
                
                if(rnk2 >= crnk){
                    rnk2 -= crnk;
                    if(rnk2 >= brnk){
                        rnk2 -= brnk;
                        A33.i.push_back(j - dsum);
                        A33.j.push_back(rnk2);
                        A33.x.push_back(2/h2/(h1+h2));
                    }else{
                        A32.i.push_back(j - dsum);
                        A32.j.push_back(rnk2);
                        A32.x.push_back(2/h2/(h1+h2));
                    }
                }else{
                    A31.i.push_back(j - dsum);
                    A31.j.push_back(rnk2);
                    A31.x.push_back(2/h2/(h1+h2));
                }
                
                
                dcenter += -2/h1/h2;
                
                break;
            }
        }
        
        // Z dimension
        for(int i = 0; i < eleZ.size(); ++ i){
            if(fabs(tp.z - eleZ[i]) < (stepZ * precision)){
                TPOINT tp1 = tp;
                int m = i;
                do{
                    ++ m;
                    tp1.z = eleZ[m];
                }while(p2i.count(tp1) <= 0);
                int rnk1 = p2i[tp1];
                
                TPOINT tp2 = tp;
                int k = i;
                do{
                    -- k;
                    tp2.z = eleZ[k];
                }while(p2i.count(tp2) <= 0);
                int rnk2 = p2i[tp2];
                
                double h1 = fabs(tp.z - tp1.z);
                double h2 = fabs(tp.z - tp2.z);
                
                if(rnk1 >= crnk){
                    rnk1 -= crnk;
                    if(rnk1 >= brnk){
                        rnk1 -= brnk;
                        A33.i.push_back(j - dsum);
                        A33.j.push_back(rnk1);
                        A33.x.push_back(2/h1/(h1+h2));
                    }else{
                        A32.i.push_back(j - dsum);
                        A32.j.push_back(rnk1);
                        A32.x.push_back(2/h1/(h1+h2));
                    }
                }else{
                    A31.i.push_back(j - dsum);
                    A31.j.push_back(rnk1);
                    A31.x.push_back(2/h1/(h1+h2));
                }
                
                if(rnk2 >= crnk){
                    rnk2 -= crnk;
                    if(rnk2 >= brnk){
                        rnk2 -= brnk;
                        A33.i.push_back(j - dsum);
                        A33.j.push_back(rnk2);
                        A33.x.push_back(2/h2/(h1+h2));
                    }else{
                        A32.i.push_back(j - dsum);
                        A32.j.push_back(rnk2);
                        A32.x.push_back(2/h2/(h1+h2));
                    }
                }else{
                    A31.i.push_back(j - dsum);
                    A31.j.push_back(rnk2);
                    A31.x.push_back(2/h2/(h1+h2));
                }
                
                
                dcenter += -2/h1/h2;
                
                break;
            }
        }
        
        A33.i.push_back(j - dsum);
        A33.j.push_back(j - dsum);
        A33.x.push_back(dcenter);
    }
    
    int status;
    A31.nz = A31.i.size();
    A31.Ap.resize(A31.n_col + 1, 0);
    A31.Ai.resize(A31.nz, 0);
    A31.Ax.resize(A31.nz, 0);
    status = umfpack_di_triplet_to_col(A31.n_row, A31.n_col, A31.nz, &A31.i[0], &A31.j[0], &A31.x[0], &A31.Ap[0], &A31.Ai[0], &A31.Ax[0], NULL);
    if(status != 0){
        printf("A31: triplet_to_col goes wrong!\n");
    }
    
    //int status;
    A32.nz = A32.i.size();
    A32.Ap.resize(A32.n_col + 1, 0);
    A32.Ai.resize(A32.nz, 0);
    A32.Ax.resize(A32.nz, 0);
    status = umfpack_di_triplet_to_col(A32.n_row, A32.n_col, A32.nz, &A32.i[0], &A32.j[0], &A32.x[0], &A32.Ap[0], &A32.Ai[0], &A32.Ax[0], NULL);
    if(status != 0){
        printf("A32: triplet_to_col goes wrong!\n");
    }
    
    //int status;
    A33.nz = A33.i.size();
    A33.Ap.resize(A33.n_col + 1, 0);
    A33.Ai.resize(A33.nz, 0);
    A33.Ax.resize(A33.nz, 0);
    status = umfpack_di_triplet_to_col(A33.n_row, A33.n_col, A33.nz, &A33.i[0], &A33.j[0], &A33.x[0], &A33.Ap[0], &A33.Ai[0], &A33.Ax[0], NULL);
    if(status != 0){
        printf("A33: triplet_to_col goes wrong!\n");
    }
    
    return;
}

std::vector<std::vector<double> > FDM_Solver::convert2Matrix(cs_di cd){
    std::vector<std::vector<double> > retVec;
    
    double** A = new double*[cd.m];
    for(int u = 0; u < cd.m; ++ u){
        A[u] = new double[cd.n];
    }

    for(int v = 0; v < cd.n; ++ v){
        for(int u = 0; u < cd.m; ++ u){
            A[u][v] = 0;
        }
    }

    for(int k = 0; k < cd.n; ++ k){
        for(int j = cd.p[k]; j < cd.p[k + 1]; ++ j){
            A[cd.i[j]][k] = cd.x[j];
        }
    }
    
    for(int i = 0; i < cd.m; ++ i){
        std::vector<double> rowVec;
        for(int j = 0; j < cd.n; ++ j){
            rowVec.push_back(A[i][j]);
        }
        retVec.push_back(rowVec);
    }
    
    for(int i = 0; i < cd.m; ++ i){
        delete[] A[i];
    }
    delete[] A;
    
    return retVec;
}

void FDM_Solver::cmprsBoundary(){
    // checking index for compress, also can be used as a mask table
    std::vector<int> cmp_idx;
    for(int i = 0; i < Cap.size(); ++ i){
        cmp_idx.push_back(i);
    }
    
    std::vector<int> Xsplit;
    for(int i = 0; i < gridX.size(); ++ i){
        if(fabs(gridX[i] - round(gridX[i] / stepX) * stepX) > precision * stepX){
            Xsplit.push_back(i);
        }
    }
    
    std::vector<int> Ysplit;
    for(int i = 0; i < gridY.size(); ++ i){
        if(fabs(gridY[i] - round(gridY[i] / stepY) * stepY) > precision * stepY){
            Ysplit.push_back(i);
        }
    }
    
    std::vector<int> Zsplit;
    for(int i = 0; i < gridZ.size(); ++ i){
        if(fabs(gridZ[i] - round(gridZ[i] / stepZ) * stepZ) > precision * stepZ){
            Zsplit.push_back(i);
        }
    }
    
    printf("x\nXsp size(): %lu\nYsp size(): %lu\nZsp size(): %lu\n", Xsplit.size(), Ysplit.size(), Zsplit.size());
    for(int i = 0; i < Xsplit.size(); ++ i){
        printf("%lf ", gridX[Xsplit[i]]);
    }
    printf("\n");
    
    // X combine, fixed Y
    for(int j = 0; j < Xsplit.size(); ++ j){
        for(int i = 0; i < gridZ.size() - 1; ++ i){
            TPOINT tpd;
            tpd.x = (gridX[Xsplit[j]] + gridX[Xsplit[j] - 1]) / 2;
            tpd.y = gridY[0];
            tpd.z = (gridZ[i] + gridZ[i + 1]) / 2;
            
            TPOINT tpu = tpd;
            tpu.x = (gridX[Xsplit[j]] + gridX[Xsplit[j] + 1]) / 2;
            
            int rd = p2i[tpd];
            int ru = p2i[tpu];
            
            if(cmp_idx[rd] == cmp_idx[ru]){
                continue;
            }
            
            // merge & modify cmp_idx
            if(cmp_idx[rd] < cmp_idx[ru]){
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[rd][u] += Cap[ru][u];
                }
                
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[u][rd] += Cap[u][ru];
                }
                
                cmp_idx[ru] = cmp_idx[rd];
            }else{
                cmp_idx[rd] = cmp_idx[ru];
            }
        }
    }
    
    for(int j = 0; j < Xsplit.size(); ++ j){
        for(int i = 0; i < gridZ.size() - 1; ++ i){
            TPOINT tpd;
            tpd.x = (gridX[Xsplit[j]] + gridX[Xsplit[j] - 1]) / 2;
            tpd.y = gridY[gridY.size() - 1];
            tpd.z = (gridZ[i] + gridZ[i + 1]) / 2;
            
            TPOINT tpu = tpd;
            tpu.x = (gridX[Xsplit[j]] + gridX[Xsplit[j] + 1]) / 2;
            
            int rd = p2i[tpd];
            int ru = p2i[tpu];
            
            if(cmp_idx[rd] == cmp_idx[ru]){
                continue;
            }
            
            // merge & modify cmp_idx
            if(cmp_idx[rd] < cmp_idx[ru]){
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[rd][u] += Cap[ru][u];
                }
                
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[u][rd] += Cap[u][ru];
                }
                
                cmp_idx[ru] = cmp_idx[rd];
            }else{
                cmp_idx[rd] = cmp_idx[ru];
            }
        }
    }
    
    // X combine, fixed Z
    for(int j = 0; j < Xsplit.size(); ++ j){
        for(int i = 0; i < gridY.size() - 1; ++ i){
            TPOINT tpd;
            tpd.x = (gridX[Xsplit[j]] + gridX[Xsplit[j] - 1]) / 2;
            tpd.z = gridZ[0];
            tpd.y = (gridY[i] + gridY[i + 1]) / 2;
            
            TPOINT tpu = tpd;
            tpu.x = (gridX[Xsplit[j]] + gridX[Xsplit[j] + 1]) / 2;
            
            int rd = p2i[tpd];
            int ru = p2i[tpu];
            
            if(cmp_idx[rd] == cmp_idx[ru]){
                continue;
            }
            
            // merge & modify cmp_idx
            if(cmp_idx[rd] < cmp_idx[ru]){
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[rd][u] += Cap[ru][u];
                }
                
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[u][rd] += Cap[u][ru];
                }
                
                cmp_idx[ru] = cmp_idx[rd];
            }else{
                cmp_idx[rd] = cmp_idx[ru];
            }
        }
    }
    
    for(int j = 0; j < Xsplit.size(); ++ j){
        for(int i = 0; i < gridY.size() - 1; ++ i){
            TPOINT tpd;
            tpd.x = (gridX[Xsplit[j]] + gridX[Xsplit[j] - 1]) / 2;
            tpd.z = gridZ[gridZ.size() - 1];
            tpd.y = (gridY[i] + gridY[i + 1]) / 2;
            
            TPOINT tpu = tpd;
            tpu.x = (gridX[Xsplit[j]] + gridX[Xsplit[j] + 1]) / 2;
            
            int rd = p2i[tpd];
            int ru = p2i[tpu];
            
            if(cmp_idx[rd] == cmp_idx[ru]){
                continue;
            }
            
            // merge & modify cmp_idx
            if(cmp_idx[rd] < cmp_idx[ru]){
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[rd][u] += Cap[ru][u];
                }
                
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[u][rd] += Cap[u][ru];
                }
                
                cmp_idx[ru] = cmp_idx[rd];
            }else{
                cmp_idx[rd] = cmp_idx[ru];
            }
        }
    }
    
    // Y combine, fixed X
    for(int j = 0; j < Ysplit.size(); ++ j){
        for(int i = 0; i < gridZ.size() - 1; ++ i){
            TPOINT tpd;
            tpd.y = (gridY[Ysplit[j]] + gridY[Ysplit[j] - 1]) / 2;
            tpd.x = gridX[0];
            tpd.z = (gridZ[i] + gridZ[i + 1]) / 2;
            
            TPOINT tpu = tpd;
            tpu.y = (gridY[Ysplit[j]] + gridY[Ysplit[j] + 1]) / 2;
            
            int rd = p2i[tpd];
            int ru = p2i[tpu];
            
            if(cmp_idx[rd] == cmp_idx[ru]){
                continue;
            }
            
            // merge & modify cmp_idx
            if(cmp_idx[rd] < cmp_idx[ru]){
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[rd][u] += Cap[ru][u];
                }
                
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[u][rd] += Cap[u][ru];
                }
                
                cmp_idx[ru] = cmp_idx[rd];
            }else{
                cmp_idx[rd] = cmp_idx[ru];
            }
        }
    }
    
    for(int j = 0; j < Ysplit.size(); ++ j){
        for(int i = 0; i < gridZ.size() - 1; ++ i){
            TPOINT tpd;
            tpd.y = (gridY[Ysplit[j]] + gridY[Ysplit[j] - 1]) / 2;
            tpd.x = gridX[gridX.size() - 1];
            tpd.z = (gridZ[i] + gridZ[i + 1]) / 2;
            
            TPOINT tpu = tpd;
            tpu.y = (gridY[Ysplit[j]] + gridY[Ysplit[j] + 1]) / 2;
            
            int rd = p2i[tpd];
            int ru = p2i[tpu];
            
            if(cmp_idx[rd] == cmp_idx[ru]){
                continue;
            }
            
            // merge & modify cmp_idx
            if(cmp_idx[rd] < cmp_idx[ru]){
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[rd][u] += Cap[ru][u];
                }
                
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[u][rd] += Cap[u][ru];
                }
                
                cmp_idx[ru] = cmp_idx[rd];
            }else{
                cmp_idx[rd] = cmp_idx[ru];
            }
        }
    }
    
    printf("y\n");
    // Y combine, fixed Z
    for(int j = 0; j < Ysplit.size(); ++ j){
        for(int i = 0; i < gridX.size() - 1; ++ i){
            TPOINT tpd;
            tpd.y = (gridY[Ysplit[j]] + gridY[Ysplit[j] - 1]) / 2;
            tpd.z = gridZ[0];
            tpd.x = (gridX[i] + gridX[i + 1]) / 2;
            
            TPOINT tpu = tpd;
            tpu.y = (gridY[Ysplit[j]] + gridY[Ysplit[j] + 1]) / 2;
            
            int rd = p2i[tpd];
            int ru = p2i[tpu];
            
            if(cmp_idx[rd] == cmp_idx[ru]){
                continue;
            }
            
            // merge & modify cmp_idx
            if(cmp_idx[rd] < cmp_idx[ru]){
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[rd][u] += Cap[ru][u];
                }
                
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[u][rd] += Cap[u][ru];
                }
                
                cmp_idx[ru] = cmp_idx[rd];
            }else{
                cmp_idx[rd] = cmp_idx[ru];
            }
        }
    }
    
    for(int j = 0; j < Ysplit.size(); ++ j){
        for(int i = 0; i < gridX.size() - 1; ++ i){
            TPOINT tpd;
            tpd.y = (gridY[Ysplit[j]] + gridY[Ysplit[j] - 1]) / 2;
            tpd.z = gridZ[gridZ.size() - 1];
            tpd.x = (gridX[i] + gridX[i + 1]) / 2;
            
            TPOINT tpu = tpd;
            tpu.y = (gridY[Ysplit[j]] + gridY[Ysplit[j] + 1]) / 2;
            
            int rd = p2i[tpd];
            int ru = p2i[tpu];
            
            if(cmp_idx[rd] == cmp_idx[ru]){
                continue;
            }
            
            // merge & modify cmp_idx
            if(cmp_idx[rd] < cmp_idx[ru]){
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[rd][u] += Cap[ru][u];
                }
                
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[u][rd] += Cap[u][ru];
                }
                
                cmp_idx[ru] = cmp_idx[rd];
            }else{
                cmp_idx[rd] = cmp_idx[ru];
            }
        }
    }
    
    printf("z\n");
    // Z combine, fixed X
    for(int j = 0; j < Zsplit.size(); ++ j){
        for(int i = 0; i < gridY.size() - 1; ++ i){
            TPOINT tpd;
            tpd.z = (gridZ[Zsplit[j]] + gridZ[Zsplit[j] - 1]) / 2;
            tpd.x = gridX[0];
            tpd.y = (gridY[i] + gridY[i + 1]) / 2;
            
            TPOINT tpu = tpd;
            tpu.z = (gridZ[Zsplit[j]] + gridZ[Zsplit[j] + 1]) / 2;
            
            int rd = p2i[tpd];
            int ru = p2i[tpu];
            
            if(cmp_idx[rd] == cmp_idx[ru]){
                continue;
            }
            
            // merge & modify cmp_idx
            if(cmp_idx[rd] < cmp_idx[ru]){
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[rd][u] += Cap[ru][u];
                }
                
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[u][rd] += Cap[u][ru];
                }
                
                cmp_idx[ru] = cmp_idx[rd];
            }else{
                cmp_idx[rd] = cmp_idx[ru];
            }
        }
    }
    
    for(int j = 0; j < Zsplit.size(); ++ j){
        for(int i = 0; i < gridY.size() - 1; ++ i){
            TPOINT tpd;
            tpd.z = (gridZ[Zsplit[j]] + gridZ[Zsplit[j] - 1]) / 2;
            tpd.x = gridX[gridX.size() - 1];
            tpd.y = (gridY[i] + gridY[i + 1]) / 2;
            
            TPOINT tpu = tpd;
            tpu.z = (gridZ[Zsplit[j]] + gridZ[Zsplit[j] + 1]) / 2;
            
            int rd = p2i[tpd];
            int ru = p2i[tpu];
            
            if(cmp_idx[rd] == cmp_idx[ru]){
                continue;
            }
            
            // merge & modify cmp_idx
            if(cmp_idx[rd] < cmp_idx[ru]){
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[rd][u] += Cap[ru][u];
                }
                
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[u][rd] += Cap[u][ru];
                }
                
                cmp_idx[ru] = cmp_idx[rd];
            }else{
                cmp_idx[rd] = cmp_idx[ru];
            }
        }
    }
    
    // Z combine, fixed Y
    for(int j = 0; j < Zsplit.size(); ++ j){
        for(int i = 0; i < gridX.size() - 1; ++ i){
            TPOINT tpd;
            tpd.z = (gridZ[Zsplit[j]] + gridZ[Zsplit[j] - 1]) / 2;
            tpd.y = gridY[0];
            tpd.x = (gridX[i] + gridX[i + 1]) / 2;
            
            TPOINT tpu = tpd;
            tpu.z = (gridZ[Zsplit[j]] + gridZ[Zsplit[j] + 1]) / 2;
            
            int rd = p2i[tpd];
            int ru = p2i[tpu];
            
            if(cmp_idx[rd] == cmp_idx[ru]){
                continue;
            }
            
            // merge & modify cmp_idx
            if(cmp_idx[rd] < cmp_idx[ru]){
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[rd][u] += Cap[ru][u];
                }
                
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[u][rd] += Cap[u][ru];
                }
                
                cmp_idx[ru] = cmp_idx[rd];
            }else{
                cmp_idx[rd] = cmp_idx[ru];
            }
        }
    }
    
    for(int j = 0; j < Zsplit.size(); ++ j){
        for(int i = 0; i < gridX.size() - 1; ++ i){
            TPOINT tpd;
            tpd.z = (gridZ[Zsplit[j]] + gridZ[Zsplit[j] - 1]) / 2;
            tpd.y = gridY[gridY.size() - 1];
            tpd.x = (gridX[i] + gridX[i + 1]) / 2;
            
            TPOINT tpu = tpd;
            tpu.z = (gridZ[Zsplit[j]] + gridZ[Zsplit[j] + 1]) / 2;
            
            int rd = p2i[tpd];
            int ru = p2i[tpu];
            
            if(cmp_idx[rd] == cmp_idx[ru]){
                continue;
            }
            
            // merge & modify cmp_idx
            if(cmp_idx[rd] < cmp_idx[ru]){
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[rd][u] += Cap[ru][u];
                }
                
                for(int u = 0; u < Cap[rd].size(); ++ u){
                    Cap[u][rd] += Cap[u][ru];
                }
                
                cmp_idx[ru] = cmp_idx[rd];
            }else{
                cmp_idx[rd] = cmp_idx[ru];
            }
        }
    }
    
    std::vector<std::vector<double> > compressed;
    
    for(int i = 0; i < Cap.size(); ++ i){
        if(cmp_idx[i] != i){
            continue;
        }
        std::vector<double> rvec;
        for(int j = 0; j < Cap[i].size(); ++ j){
            if(cmp_idx[j] != j){
                continue;
            }
            rvec.push_back(Cap[i][j]);
        }
        compressed.push_back(rvec);
    }
    
    for(int i = 0; i < Cap.size(); ++ i){
        Cap[i].clear();
    }
    Cap.clear();
    
    Cap = compressed;
    
    return;
}
