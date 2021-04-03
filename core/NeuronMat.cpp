//
// Created by tangdingyi on 2019/12/26.
//

#include "NeuronMat.h"

NeuronMat::NeuronMat() {}

NeuronMat::NeuronMat(int r, int c, int k) {
    switch (k) {
        case NODE_INPUT:
            is_operator = false;
            is_input = true;
            break;
        case NODE_NET:
            is_operator = false;
            is_input = false;
            break;
        case NODE_OP:
        case NODE_FORWARD:
        case NODE_VIA:
            is_operator = true;
            is_input = false;
    }
    this->r = r;
    this->c = c;
    this->k = k;
    op = nullptr;
    op_update = nullptr;
    op_reveal = nullptr;
    f = nullptr;
    g = nullptr;
}

void NeuronMat::forward() {
    if (is_operator && op)
        op->forward();
}

void NeuronMat::back() {
    if (is_operator && k != NODE_FORWARD)
        op->back();
}

void NeuronMat::update() {
    if (k == NODE_NET)
        *getForward() = (*getForward()) + (*getGrad());
}

void NeuronMat::reveal() {
    if (op_reveal)
        op_reveal->forward();
}

void NeuronMat::update_grad() {
    if (op_update)
        op_update->forward();
}

void NeuronMat::setOp(Op *op) {
    this->op = op;
}

void NeuronMat::setOpUpdate(Op *op) {
    this->op_update = op;
}

void NeuronMat::setOpReveal(Op *op) {
    this->op_reveal = op;
}

bool NeuronMat::forwardHasNext() {
    if (op) {
        return op->forwardHasNext();
    }
    return 0;
}

bool NeuronMat::backHasNext() {
    if (op && k != NODE_FORWARD)
        return op->backHasNext();
    return 0;
}

bool NeuronMat::updateGradHasNext() {
    if (op_update)
        return op_update->forwardHasNext();
    return 0;
}

bool NeuronMat::revealHasNext() {
    if (op_reveal) {
        return op_reveal->forwardHasNext();
    }
    return 0;
}

Mat* NeuronMat::getForward() {
    return f;
}

Mat* NeuronMat::getGrad() {
    return g;
}

Mat * NeuronMat::getBack() {
    return g;
}

Mat* NeuronMat::getAux() {
    return aux;
}

void NeuronMat::setForward(Mat *a) {
    f = a;
}

void NeuronMat::setGrad(Mat *a) {
    g = a;
}

void NeuronMat::setAux(Mat *a) {
    aux = a;
}

void NeuronMat::initForward() {
    f = new Mat(r, c);
}

void NeuronMat::initGrad() {
    g = new Mat(r, c);
}

void NeuronMat::resetOp() {
    if (op != nullptr)
        op->reset();
    if (op_update != nullptr)
        op_update->reset();
    if (op_reveal != nullptr)
        op_reveal->reset();
}

bool NeuronMat::getIsInput() {
    return is_input;
}

bool NeuronMat::getIsNet() {
    return k == NODE_NET;
}

bool NeuronMat::getIsBack() {
    return is_input || k == NODE_FORWARD;
}

void NeuronMat::print() {
    printf("%d %d %d\n", r, c, k);
}

bool NeuronMat::is_net() {
    return k == NODE_NET;
}

int NeuronMat::rows() {
    return r;
}

int NeuronMat::cols() {
    return c;
}