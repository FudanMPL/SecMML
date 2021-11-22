//
// Created by tangdingyi on 2019/12/26.
//

#include "Op.h"

bool Op::forwardHasNext() {
//    cout << "Op" << forwardRound << ": " << forwardTotal << endl;
    return forwardRound < forwardTotal;
}

bool Op::backHasNext() {
    return backRound < backTotal;
}

void Op::init() {
    init(1, 1);
}

void Op::init(int forwardTotal, int backTotal) {
    forwardRound = 0;
    this->forwardTotal = forwardTotal;
    backRound = 0;
    this->backTotal = backTotal;
    localRound = 0;
}

void Op::reset() {
    forwardRound = 0;
    backRound = 0;
}

void Op::reinit() {
    forwardRound = (localRound == globalRound) ? forwardRound : 0;
    localRound = globalRound;
    forwardRound++;
}

void Op::print() {
    DBGprint("forward round: %d forward total: %d\n", forwardRound, forwardTotal);
}