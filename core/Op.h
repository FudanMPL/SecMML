//
// Created by tangdingyi on 2019/12/26.
//

#ifndef MPC_ML_OP_H
#define MPC_ML_OP_H

#include "../Constant.h"

extern int globalRound;

class Op {
protected:
    int forwardRound, forwardTotal;
    int backRound, backTotal;
    int localRound;
public:
    virtual void forward() = 0;
    virtual void back() = 0;
    bool forwardHasNext();
    bool backHasNext();
    void init();
    void init(int forwardTotal, int backTotal);
    void reset();
    void reinit();
    void print();
};


#endif //MPC_ML_OP_H
