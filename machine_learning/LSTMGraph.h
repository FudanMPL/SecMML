//
// Created by 姜子丰 on 2020/8/29.
//

#ifndef SMMLF_LSTMGRAPH_H
#define SMMLF_LSTMGRAPH_H

#include "../Constant.h"
#include "NN.h"
#include "../core/Player.h"
#include "Cell.h"

extern int node_type;

class LSTMGraph {



public:
    class LSTM {
        NN* nn;
        Constant::Clock *clock_train;
        Mat *train_data, *train_label;
        Mat *test_data, *test_label;
        Cell *cells[L];
        int input, output, h0, c0;
        int x[L];
        int argmax, st_con, st_w, st_b, st_mul;
        int sd;
        int re_st_add, re_output;
        int out_sig, re_out_sig;
        int id;
        int activation_out;

    public:
        LSTM();
        LSTM(Mat* train_data, Mat* train_label, Mat* test_data, Mat* test_label);
        void graph(NN* nn, int id);
        void train();
        void test();
        void feed(NN* nn, Mat (&x_batch)[L], Mat& y_batch, int *input, int output);
        void next_batch(Mat &batch, int start, Mat* A, int mod = NM);
        void next_batch(Mat (&batch)[L], int start, Mat* A, int mod = NM);
        void graph();
        void print_perd(int round);
    };

};


#endif //SMMLF_LSTMGRAPH_H
