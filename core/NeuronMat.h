//
// Created by tangdingyi on 2019/12/26.
//

#ifndef MPC_ML_NEURONMAT_H
#define MPC_ML_NEURONMAT_H

#include "Op.h"
#include "Mat.h"

class NeuronMat {
    bool is_operator;
    bool is_input;
    Op *op;
    Op *op_update;
    Op *op_reveal;
    int r, c;
    int k;
    Mat *f, *g;
    Mat *aux;
public:
    static const int NODE_INPUT = 1;
    static const int NODE_NET = 2;
    static const int NODE_OP = 3;
    static const int NODE_VIA = 4;
    static const int NODE_FORWARD = 5;
    NeuronMat();
    NeuronMat(int r, int c, int k);
    void setOp(Op* op);
    void setOpUpdate(Op *op);
    void setOpReveal(Op *op);
    bool forwardHasNext();
    bool backHasNext();
    bool updateGradHasNext();
    bool revealHasNext();
    void forward();
    void back();
    void update();
    void reveal();
    void update_grad();
    Mat* getForward();
    Mat* getGrad();
    Mat* getBack();
    Mat* getAux();
    void setForward(Mat* a);
    void setGrad(Mat* a);
    void setAux(Mat* a);
    void initForward();
    void initGrad();
    void resetOp();
    bool getIsInput();
    bool getIsNet();
    bool getIsBack();
    void print();
    bool is_net();
    int rows();
    int cols();
};


#endif //MPC_ML_NEURONMAT_H
