//
// Created by tangdingyi on 2019/12/26.
//

#ifndef MPC_ML_BPGRAPH_H
#define MPC_ML_BPGRAPH_H

#include "../Constant.h"
#include "NN.h"
#include "../core/Player.h"

extern int node_type;

class BPGraph {
public:
    class LR {
        NN* nn;
        Constant::Clock *clock_train;
        Mat *train_data, *train_label;
        Mat *test_data, *test_label;
        int input, output;
        int argmax, st_con, st_w, st_b, st_mul;
        int nd_w, nd_b, nd_add, nd_mul, nd_hidden;
        int sd;
        int re_st_add, re_output;
        int out_sig, re_out_sig;
        int id;
        int activation_out;
    public:
        LR();
        LR(Mat* train_data, Mat* train_label, Mat* test_data, Mat* test_label);
        void graph(NN* nn, int id);
        void linear_graph();
        void logistic_graph();
        void train();
        void test();
        void feed(NN* nn, Mat& x_batch, Mat& y_batch, int input, int output);
        void next_batch(Mat& batch, int start, Mat* A, int mod = NM);
        void graph();
        void print_perd(int round);
    };
};


#endif //MPC_ML_BPGRAPH_H
