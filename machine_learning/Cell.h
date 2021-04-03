//
// Created by 姜子丰 on 2020/8/22.
//

#ifndef SMMLF_CELL_H
#define SMMLF_CELL_H

#include "../Constant.h"
#include "NN.h"
#include "../core/Player.h"

class Cell {
    NN* nn;
    int c_in, c_out, h_in, h_out, x_in;
    int st_m, st_f, st_i, st_c, st_o;
    int w_f, w_i, w_c, w_o;
    int sig_f, sig_i, tanh_cc, sig_o;
    int mul_f, mul_i;
    int tanh_c;
public:
    Cell();
    Cell(NN* nn, int c_in, int h_in, int x_in);
    void addEdges();
    void setWeight(Mat *w_f, Mat *w_i, Mat *w_c, Mat *w_o);
    int getC_out();
    int getH_out();
    int getW_f();
    int getW_i();
    int getW_c();
    int getW_o();

};


#endif //SMMLF_CELL_H
