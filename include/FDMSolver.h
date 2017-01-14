/*
* FDMSolver.h
*
*  Created on: 8 Jul, 2016
*      Author: ming
*/

#ifndef FDMSOLVER_H_
#define FDMSOLVER_H_

#include <vector>
#include "umfpack.h"
#include "cs.h"
#include <assert.h>
#include <unordered_map>
#include <algorithm>
#include "Matrix_Sparse.h"
#include "Configure.h"
#include "Debugger.h"
#include "shape.h"

class FDM_Solver {
public:
    std::string ans_path = "answer/C.txt";
    Configure* macro_conf;
    int inum = 0;	// inner node number
    int bnum = 0;	// boundary node number
    double uscale[3];    // 1 / the unit scale of each axis
    double uscale2[3];    // the square of ( 1 / unit scale of each axis)
    int*** points;    // store the grid points, and he value is the index of the point in A. If the value == -1, then it means it's not a valid point, such as the 8 vertex of cube
                    // First dim -> axis1, Second dim -> axis2, Third dim -> axis3.
    Matrix_Sparse A11;
    Matrix_Sparse A12;
    Matrix_Sparse A13;
    Matrix_Sparse A21;
    Matrix_Sparse A22;
    Matrix_Sparse A23;
    Matrix_Sparse A31;
    Matrix_Sparse A32;
    Matrix_Sparse A33;
    //Matrix_Sparse tempCap;
    
    std::vector<std::vector<double> > Cap;

    Debugger debg;
    
    struct HashFunc{
        std::size_t operator()(const TPOINT &tp) const{
            using std::size_t;
            using std::hash;
            
            return ((hash<double>()(tp.x)
                ^ (hash<double>()(tp.y) << 1)) >> 1)
                ^ (hash<double>()(tp.z) << 1);
        }
    };
    
    struct EqualKey{
        bool operator () (const TPOINT &lhs, const TPOINT &rhs) const{
            return lhs.x == rhs.x
                && lhs.y == rhs.y
                && lhs.z == rhs.z;
        }
    };
    
    std::unordered_map<int, TPOINT> i2p;
    std::unordered_map<TPOINT, int, HashFunc, EqualKey> p2i;
    
    std::vector<double> gridX;
    std::vector<double> gridY;
    std::vector<double> gridZ;
    
    std::vector<double> eleX;
    std::vector<double> eleY;
    std::vector<double> eleZ;
    
    double precision = 1e-4;
    
    double stepX = 0.;
    double stepY = 0.;
    double stepZ = 0.;
    
    int crnk = 0;
    int brnk = 0;
    int inrnk = 0;
    
    void nonuniform();
    void uniform();
    void cmprsBoundary();
    
    bool isInsideCond(double x, double y, double z);
    cs_di compute_Formula(Matrix_Sparse iA11, Matrix_Sparse iA12, Matrix_Sparse iA21, Matrix_Sparse iA22, int p_row, int p_col);

public:
    FDM_Solver();
    FDM_Solver(Configure *mconf);
    void Construct_Matrix_A11();
    void Construct_Matrix_A12();
    void Construct_Matrix_A21();
    void Construct_Matrix_A22();
    
    // nuf short for nonuniform
    void Construct_Matrix_A11_A12_A13_nuf();
    void Construct_Matrix_A21_A22_A23_nuf();
    void Construct_Matrix_A31_A32_A33_nuf();
    
    void solve();
    std::vector<std::vector<double> > getCap();
    virtual ~FDM_Solver();
    void Debug();
    std::vector<std::vector<double> > convert2Matrix(cs_di cd);
};

#endif /* FDMSOLVER_H_ */
