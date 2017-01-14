/*
* Debugger.h
*
*  Created on: 12 Jul, 2016
*      Author: ming
*/

#ifndef DEBUGGER_H_
#define DEBUGGER_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "umfpack.h"
#include "cs.h"
#include "shape.h"

class Debugger{
    std::string fconf = "debugfiles/confs.txt";
    std::string ford = "debugfiles/ords.txt";
    std::string fa = "debugfiles/";
    std::string fcut = "debugfiles/cuts.txt";
public:
    Debugger();
    ~Debugger();
    void Conf_print(double* ori, double* len, int* sa, double d);
    void Conf_print(double* ori, double* len, int* sa, int nc, double** co, double** cl, int** cs, double d);
    void Order_print(int ***p, int x, int y, int z, int inum, int bnum, double* h, double* h2);
    void Matrix_print(std::string s,int r, int c, int nz, std::vector<int> i, std::vector<int> j, std::vector<double> x, std::vector<int> Ai, std::vector<int> Aj, std::vector<double> Ax);
    void Cut_print(std::vector<double> vx, std::vector<double> vy, std::vector<double> vz, std::vector<double> ex, std::vector<double> ey, std::vector<double> ez, int c, int b, int in);
    void Order_print(std::unordered_map<int, TPOINT> uom);
    void Csdi_print(cs_di cd, std::string nm);
    void Vec_Mtx_print(std::vector<std::vector<double> > vm, std::string nm);
};


#endif /* DEBUGGER_H_ */
