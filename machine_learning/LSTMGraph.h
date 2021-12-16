/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-07 09:52:08
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2021-12-16 10:45:10
 */
//
// Created by 姜子丰 on 2020/8/29.
//

#ifndef SMMLF_LSTMGRAPH_H
#define SMMLF_LSTMGRAPH_H

#include "../Constant.h"
#include "NN.h"
#include "../core/Player.h"
#include "Cell.h"

// extern int node_type;

class LSTMGraph {



public:
    class LSTM {
    public:
        NN* nn;
        Constant::Clock *clock_train;
        Mat *train_data, *train_label;
        Mat *test_data, *test_label;
        Cell *cells[PL];
        int input, output, h0, c0;
        int x[PL];
        int argmax, st_con, st_w, st_b, st_mul;
        int sd;
        int re_st_add, re_output;
        int out_sig, re_out_sig;
        int id;
        int activation_out;

        LSTM();
        LSTM(Mat* train_data, Mat* train_label, Mat* test_data, Mat* test_label);
        void graph(NN* nn, int id);
        void train();
        void test();
        void feed(NN* nn, Mat (&x_batch)[PL], Mat& y_batch, int *input, int output);
        void next_batch(Mat &batch, int start, Mat* A, int mod = PNM);
        void next_batch(Mat (&batch)[PL], int start, Mat* A, int mod = PNM);
        void graph();
        void print_perd(int round);
    };

};


#endif //SMMLF_LSTMGRAPH_H
