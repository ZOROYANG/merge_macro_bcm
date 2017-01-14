/*
* Debugger.cpp
*
*  Created on: 12 Jul, 2016
*      Author: ming
*/

#include "Debugger.h"

Debugger::Debugger(){

}

Debugger::~Debugger(){

}

void Debugger::Conf_print(double* ori, double* len, int* sa, int nc, double** co, double** cl, int** cs, double d){
    FILE*  confs;
    confs = fopen(this->fconf.c_str(), "w");
    if(confs == NULL){
        printf("Cann't open %s\n", this->fconf.c_str());
        exit(1);
    }
    fprintf(confs, "origin point:\n    (%f, %f, %f)\n", ori[0], ori[1], ori[2]);
    fprintf(confs, "length in axis1, axis2, axis3:\n    (%f, %f, %f)\n", len[0], len[1], len[2]);
    fprintf(confs, "segmentation in axis1, axis2, axis3\n    (%d, %d, %d)\n", sa[0], sa[1], sa[2]);
    fprintf(confs, "conductor amount: %d\n", nc);
    for(int i = 0; i < nc; ++ i){
        fprintf(confs, "conductor %d:\norigin point:(%f, %f, %f)\nxAxis:(%f, 0, 0)\nyAxis:(0, %f, 0)\nzAxis:(0, 0, %f)\nsegmentation:(%d, %d, %d)\n",
                    i, co[i][0], co[i][1], co[i][2], cl[i][0], cl[i][1], cl[i][2], cs[i][0], cs[i][1], cs[i][2]);
    }
    fprintf(confs, "dielectric: %f\n", d);
    fclose(confs);
    return;
}

void Debugger::Conf_print(double* ori, double* len, int* sa, double d){
    FILE*  confs;
    confs = fopen(this->fconf.c_str(), "w");
    if(confs == NULL){
        printf("Cann't open %s\n", this->fconf.c_str());
        exit(1);
    }
    fprintf(confs, "origin point:\n    (%f, %f, %f)\n", ori[0], ori[1], ori[2]);
    fprintf(confs, "length in axis1, axis2, axis3:\n    (%f, %f, %f)\n", len[0], len[1], len[2]);
    fprintf(confs, "segmentation in axis1, axis2, axis3\n    (%d, %d, %d)\n", sa[0], sa[1], sa[2]);
    fprintf(confs, "dielectric: %f\n", d);
    fclose(confs);
    return;
}

void Debugger::Order_print(int ***p, int x, int y, int z, int inum, int bnum, double* h, double* h2){
    FILE*  ords;
    ords = fopen(this->ford.c_str(), "w");
    if(ords == NULL){
        printf("Cann't open %s\n", this->ford.c_str());
        exit(1);
    }

    fprintf(ords, "number of inner points: %d\nnumber of boundary points: %d\n", inum, bnum);
    fprintf(ords, "the reciprocal of minimum unit: (%2f, %2f, %2f)", h[0], h[1], h[2]);
    fprintf(ords, "the reciprocal of the square of minimum unit: (%2f, %2f, %2f)", h2[0], h2[1], h2[2]);

    fprintf(ords, "Order of every point in cubic.\n");
    for(int i = 0; i < x; ++ i){
        for(int j = 0; j < y; ++ j){
            for(int k = 0; k < z; ++ k){
                fprintf(ords, "Point at (%d, %d, %d) is encoded as: %d\n", i, j, k, p[i][j][k]);
            }
        }
    }

    fclose(ords);
    return;
}

void Debugger::Matrix_print(std::string s, int r, int c, int nz, std::vector<int> i, std::vector<int> j, std::vector<double> x, std::vector<int> Ai, std::vector<int> Ap, std::vector<double> Ax){
    FILE*  as;
    as = fopen((this->fa + s).c_str(), "w");
    if(as == NULL){
        printf("Cann't open %s\n", (this->fa + s).c_str());
        exit(1);
    }
/*
    fprintf(as, "rows:\n    %d\ncols:\n    %d\nnone zeros:\n    %d\n", r, c, nz);
    fprintf(as, "i.size():\n    %lu\nj.size():\n    %lu\nx.size():\n    %lu\n", i.size(), j.size(), x.size());
    fprintf(as, "Ai.size():\n    %lu\nAp.size():\n    %lu\nAx.size():\n    %lu\n", Ai.size(), Ap.size(), Ax.size());

    fprintf(as, "\ni:\n");
    for(int u = 0; u < nz; ++ u){
        fprintf(as, "%d ", i[u]);
    }
    fprintf(as, "\n");

    fprintf(as, "\nj:\n");
    for(int u = 0; u < nz; ++ u){
        fprintf(as, "%d ", j[u]);
    }
    fprintf(as, "\n");

    fprintf(as, "\nx:\n");
    for(int u = 0; u < nz; ++ u){
        fprintf(as, "%f ", x[u]);
    }
    fprintf(as, "\n");
*/
    // matrix
    printf("r: %d, c: %d\n", r, c);

    double** A = new double*[r];
    for(int u = 0; u < r; ++ u){
        A[u] = new double[c];
    }

    for(int u = 0; u < r; ++ u){
        for(int v = 0; v < c; ++ v){
            A[u][v] = 0;
        }
    }

    for(int u = 0; u < nz; ++ u){
        A[i[u]][j[u]] = x[u];
    }

    fprintf(as, "\n");
    for(int u = 0; u < r; ++ u){
        for(int v = 0; v < c; ++ v){
            fprintf(as, "%f ", A[u][v]);
        }
        fprintf(as, ";\n");
    }

    /*
    fprintf(as, "\nAi:\n");
    for(int u = 0; u < nz; ++ u){
        fprintf(as, "%d ", Ai[u]);
    }
    fprintf(as, "\n");

    fprintf(as, "\nAp:\n");
    for(int u = 0; u <= c; ++ u){
        fprintf(as, "%d ", Ap[u]);
    }
    fprintf(as, "\n");

    fprintf(as, "\nAx:\n");
    for(int u = 0; u < nz; ++ u){
        fprintf(as, "%f ", Ax[u]);
    }
    fprintf(as, "\n");
    */

    for(int u = 0; u < r; ++ u){
        delete[] A[u];
    }
    delete[] A;

    fclose(as);
    return;
}

void Debugger::Cut_print(std::vector<double> vx, std::vector<double> vy, std::vector<double> vz, std::vector<double> ex, std::vector<double> ey, std::vector<double> ez, int c, int b, int in){
    FILE*  cuts;
    cuts = fopen(this->fcut.c_str(), "w");
    if(cuts == NULL){
        printf("Cann't open %s\n", this->fcut.c_str());
        exit(1);
    }
    
    fprintf(cuts, "The splitting strategy is:\n");
    fprintf(cuts, "    Dimension X has %lu cut points.\n", vx.size());
    for(int i = 0; i < vx.size(); ++ i){
        fprintf(cuts, "%lf ", vx[i]);
    }
    fprintf(cuts, "\n***********\n");
    
    fprintf(cuts, "    Dimension Y has %lu cut points.\n", vy.size());
    for(int i = 0; i < vy.size(); ++ i){
        fprintf(cuts, "%lf ", vy[i]);
    }
    fprintf(cuts, "\n***********\n");
    
    fprintf(cuts, "    Dimension Z has %lu cut points.\n", vz.size());
    for(int i = 0; i < vz.size(); ++ i){
        fprintf(cuts, "%lf ", vz[i]);
    }
    fprintf(cuts, "\n***********\n");
    
    
    fprintf(cuts, "    Dimension X has %lu ele points.\n", ex.size());
    for(int i = 0; i < ex.size(); ++ i){
        fprintf(cuts, "%lf ", ex[i]);
    }
    fprintf(cuts, "\n***********\n");
    
    fprintf(cuts, "    Dimension Y has %lu ele points.\n", ey.size());
    for(int i = 0; i < ey.size(); ++ i){
        fprintf(cuts, "%lf ", ey[i]);
    }
    fprintf(cuts, "\n***********\n");
    
    fprintf(cuts, "    Dimension Z has %lu ele points.\n", ez.size());
    for(int i = 0; i < ez.size(); ++ i){
        fprintf(cuts, "%lf ", ez[i]);
    }
    fprintf(cuts, "\n***********\n");
    fprintf(cuts, "conductor rank: %d\nboundary rank: %d\ninner rank: %d\n", c, b, in);
    
    fclose(cuts);
}

void Debugger::Order_print(std::unordered_map<int, TPOINT> uom){
    printf("Begin Print Order.\n");
    FILE*  ords;
    ords = fopen(this->ford.c_str(), "w");
    if(ords == NULL){
        printf("Cann't open %s\n", this->ford.c_str());
        exit(1);
    }

    fprintf(ords, "Total Number: %lu\n", uom.size());

    fprintf(ords, "Order of every point in cubic:\n");
    for(auto it = uom.begin(); it != uom.end(); ++ it){
        fprintf(ords, "<%d, (%lf, %lf, %lf)>\n", it->first, it->second.x, it->second.y, it->second.z);
    }

    fclose(ords);
    printf("End Print Order.\n");
    return;
}

void Debugger::Csdi_print(cs_di cd, std::string nm){
    FILE* fcd;
    fcd = fopen((this->fa + nm).c_str(), "w");
    if(fcd == NULL){
        printf("Cann't open %s\n", (this->fa + nm).c_str());
    }
    
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

    fprintf(fcd, "\n");
    for(int u = 0; u < cd.m; ++ u){
        for(int v = 0; v < cd.n; ++ v){
            fprintf(fcd, "%lf ", A[u][v]);
        }
        fprintf(fcd, ";\n");
    }

    fclose(fcd);
    for(int u = 0; u < cd.m; ++ u){
        delete[] A[u];
    }
    delete[] A;
    
    return;
}

void Debugger::Vec_Mtx_print(std::vector<std::vector<double> > vm, std::string nm){
    FILE* fcd;
    fcd = fopen((this->fa + nm).c_str(), "w");
    if(fcd == NULL){
        printf("Cann't open %s\n", (this->fa + nm).c_str());
    }
    
    fprintf(fcd, "\n");
    for(int u = 0; u < vm.size(); ++ u){
        for(int v = 0; v < vm[u].size(); ++ v){
            fprintf(fcd, "%.20lf ", vm[u][v]);
        }
        fprintf(fcd, ";\n");
    }
    
    fclose(fcd);
    return;
}