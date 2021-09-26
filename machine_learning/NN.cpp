//
// Created by tangdingyi on 2019/12/26.
//

#include "NN.h"

NN::NN() {
    tot = 0;
    cur = 0;
    adj.resize(MAX_NODE_NUM, vector<int>(0));
    neuron.resize(MAX_NODE_NUM);
    vst.resize(MAX_NODE_NUM);
    q.resize(MAX_NODE_NUM);
    to.resize(MAX_NODE_NUM);
}

NN& NN::operator=(NN &a) {
    for (int i = 1; i <= tot; i++) {
        *getNeuron(i)->getForward() = *a.getNeuron(i)->getForward();
        *getNeuron(i)->getGrad() = *a.getNeuron(i)->getGrad();
    }
    return *this;
}

void NN::global_variables_initializer() {
    DBGprint("tot: %d\n",tot);
    for (int i = 1; i <= tot; i++) {
        DBGprint("%d\n",i);
        if (neuron[i]->getForward() == nullptr) {
            neuron[i]->initForward();
        }
        if (neuron[i]->getGrad() == nullptr) {
            neuron[i]->initGrad();
        }
        if (neuron[i]->getIsNet()) {
            neuron[i]->setOpUpdate(new MathOp::Mul_Const_Trunc(getNeuron(i)->getGrad(), getNeuron(i)->getGrad(), 0.0001));
        }
    }
    curForward = 1;
    curGrad = tot;
    curUpdate=1;
}

void NN::epoch_init() {
    for (int i = 1; i <= tot; i++) {
        if (!neuron[i]->getIsBack()) {
            neuron[i]->getGrad()->clear();
        }
    }
    for (int i = 1; i <= tot; i++) {
        neuron[i]->resetOp();
    }
    curForward = 1;
    curGrad = tot;
    curUpdate=1;
}

void NN::reveal_init(int u) {
    neuron[u]->setOpReveal(new MathOp::Reveal(getNeuron(u)->getForward(), getNeuron(u)->getForward()));
}

void NN::reveal_init() {
    curReveal=1;
    for (int i = 1; i <= tot; i++) {
            neuron[i]->resetOp();
    }
}

void NN::output_init(int u) {
    neuron[u]->setOpOutput(new MathOp::Reveal(getNeuron(u)->getForward(), getNeuron(u)->getForward()));
}

void NN::output_init() {
    curOutput=1;
    for (int i = 1; i <= tot; i++) {
            neuron[i]->resetOp();
    }
}

void NN::addedge(int u, int v) {
    adj[u].push_back(v);
    to[v]++;
}

int NN::addnode(int r, int c, int k) {
    neuron[++tot] = new NeuronMat(r, c, k);
    cout<<tot<<endl;
    return tot;
}

NeuronMat* NN::getNeuron(int u) {
    return neuron[u];
}

void NN::setOp(int u, Op *op) {
    neuron[u]->setOp(op);
}

void NN::addOpAdd_Mat(int res, int a, int b) {
    addedge(a, res);
    addedge(b, res);
    setOp(res, new MathOp::Add_Mat(getNeuron(res), getNeuron(a), getNeuron(b)));
}

void NN::addOpMinus_Mat(int res, int a, int b) {
    addedge(a, res);
    addedge(b, res);
    setOp(res, new MathOp::Minus_Mat(getNeuron(res), getNeuron(a), getNeuron(b)));
}

void NN::addOpMul_Mat(int res, int a, int b) {
    addedge(a, res);
    addedge(b, res);
    setOp(res, new MathOp::Mul_Mat(getNeuron(res), getNeuron(a), getNeuron(b)));
}

//void NN::addOpMul_Const_Mat(int res, int a, ll128 b) {
//    addedge(a, res);
//    setOp(res, new MathOp::Mul_Const_Trunc(getNeuron(res)->getForward(), getNeuron(a)->getForward(), b));
//}

// for multiply learning rate
void NN::addOpMul_Const_Mat(int res, int a, double b) {
    addedge(a, res);
    setOp(res, new MathOp::Mul_Const_Trunc(getNeuron(res)->getForward(), getNeuron(a)->getForward(), b));
}

void NN::addOpDiv_Const_Mat(int res, int a, ll128 b) {
    addedge(a, res);
    setOp(res, new MathOp::Div_Const_Trunc(getNeuron(res)->getForward(), getNeuron(a)->getForward(), b));
}

void NN::addOpDiv_Const_Optimized_Mat(int res, int a, ll b) {
    addedge(a, res);
    setOp(res, new MathOp::Div_Const_Trunc_Optimized(getNeuron(res)->getForward(), getNeuron(a)->getForward(), b));
}

void NN::addOpDiv_Seg_Const_Mat(int res, int a, int b) {
    addedge(a, res);
    setOp(res, new MathOp::Div_Seg_Const_Trunc(getNeuron(res)->getForward(), getNeuron(a)->getForward(), getNeuron(b)->getForward()));
}

void NN::addOpHada_Mat(int res, int a, int b) {
    addedge(a, res);
    addedge(b, res);
    setOp(res, new MathOp::Hada_Mat(getNeuron(res), getNeuron(a), getNeuron(b)));
}

void NN::addOpMeanSquaredLoss(int res, int a, int b) {
    addedge(a, res);
    addedge(b, res);
    setOp(res, new MathOp::MeanSquaredLoss(getNeuron(res), getNeuron(a), getNeuron(b)));
}

void NN::addOpSimilar(int res, int a, int b) {
    addedge(a, res);
    addedge(b, res);
    setOp(res, new MathOp::Similar(getNeuron(res), getNeuron(a), getNeuron(b)));
}

void NN::addOpConcat(int res, int a, int b) {
    addedge(a, res);
    addedge(b, res);
    setOp(res, new MathOp::Concat(getNeuron(res), getNeuron(a), getNeuron(b)));
}

void NN::addOpHstack(int res, int a, int b) {
    addedge(a, res);
    addedge(b, res);
    setOp(res, new MathOp::Hstack(getNeuron(res), getNeuron(a), getNeuron(b)));
}

void NN::addOpVstack(int res, int a, int b) {
    addedge(a, res);
    addedge(b, res);
    setOp(res, new MathOp::Vstack(getNeuron(res), getNeuron(a), getNeuron(b)));
}

void NN::addOpVia(int res, int a) {
    addedge(a, res);
    setOp(res, new MathOp::Via(getNeuron(res), getNeuron(a)));
}

void NN::addOpTanh(int res, int a) {
    addedge(a, res);
    setOp(res, new MathOp::Tanh(getNeuron(res), getNeuron(a)));
}

void NN::addOpHard_Tanh(int res, int a) {
    addedge(a, res);
    setOp(res, new MathOp::Hard_Tanh(getNeuron(res), getNeuron(a)));
}
void NN::addOpHybrid_Tanh(int res, int a) {
    addedge(a, res);
    setOp(res, new MathOp::Tanh_Mat(getNeuron(res), getNeuron(a)));
}

void NN::addOpRaw_Tanh(int res, int a) {
    addedge(a, res);
    setOp(res, new MathOp::Raw_Tanh(getNeuron(res), getNeuron(a)));
}

void NN::addOpTanh_change(int res, int a) {
    addedge(a, res);
    setOp(res, new MathOp::Tanh_change(getNeuron(res), getNeuron(a)));
}
void NN::addOpTanh_ex(int res, int a) {
    addedge(a, res);
    setOp(res, new MathOp::Tanh_ex(getNeuron(res), getNeuron(a)));
}

void NN::addOpSigmoid(int res, int a) {
    addedge(a, res);
    setOp(res, new MathOp::Sigmoid_Mat(getNeuron(res), getNeuron(a)));
}

void NN::addOpReLU(int res, int a) {
    addedge(a, res);
    setOp(res, new MathOp::ReLU_Mat(getNeuron(res), getNeuron(a)));
}

void NN::addOpLTZ(int res, int a) {
    addedge(a, res);
    setOp(res, new MathOp::LTZ(getNeuron(res)->getForward(), getNeuron(a)->getForward(), BIT_P_LEN));
}

void NN::addOpEQZ(int res, int a) {
    addedge(a, res);
    setOp(res, new MathOp::EQZ(getNeuron(res)->getForward(), getNeuron(a)->getForward(), BIT_P_LEN));
}

void NN::addOpEQZ_2LTZ(int res, int a) {
    addedge(a, res);
    setOp(res, new MathOp::EQZ_2LTZ(getNeuron(res)->getForward(), getNeuron(a)->getForward(), BIT_P_LEN));
}

void NN::addOpArgmax(int res, int a) {
    addedge(a, res);
    setOp(res, new MathOp::Argmax(getNeuron(res), getNeuron(a)));
}

void NN::addOpEqual(int res, int a, int b) {
    addedge(a, res);
    addedge(b, res);
    setOp(res, new MathOp::Equal(getNeuron(res), getNeuron(a), getNeuron(b)));
}

void NN::toposort() {
    int l, r;
    vst = vector<bool>(MAX_NODE_NUM, 0);
    l = r = 0;
    for (int i = 1; i <= tot; i++) {
        if (!to[i]) {
            q[++r] = i;
            vst[i] = 1;
        }
    }
    while (l < r) {
        int u = q[++l];
        int len = adj[u].size();
        for (int i = 0; i < len; i++) {
            int j = adj[u][i];
            to[j]--;
            if (!to[j]) {
                vst[j] = 1;
                q[++r] = j;
            }
        }
    }
    for (int i = 1; i <= tot; i++) {
        DBGprint("%d ", q[i]);
    }
    DBGprint("\n");
}

void NN::gradUpdate() {
    for (int i = 1; i <= tot; i++)
        neuron[i]->update();
}

bool NN::forwardHasNext() {
//    cout << curForward << ": " << tot << endl;
    return curForward < tot || neuron[q[curForward]]->forwardHasNext();
}

void NN::forwardNext() {
    while (!neuron[q[curForward]]->forwardHasNext()) {
        curForward++;
        if (curForward > tot)
            return;
    }
//    cout << "NN: " << curForward << endl;
    neuron[q[curForward]]->forward();

}

bool NN::backHasNext() {
    return curGrad > 1 || neuron[q[curGrad]]->backHasNext();
}

void NN::backNext() {
    if (!backHasNext())
        return;
    if (neuron[q[curGrad]]->backHasNext()) {
        //cout<<q[curGrad]<<endl;
        neuron[q[curGrad]]->back();
    }
    else {
        //cout<<q[curGrad-1]<<endl;
        neuron[q[--curGrad]]->back();
    }
}

bool NN::updateHasNext() {
    return curUpdate < tot;
//    || neuron[curUpdate]->updateGradHasNext();
//    for (int i = 1; i <= tot; i++) {
//        if (neuron[i]->updateGradHasNext())
//            return 1;
//    }
//    return 0;
}

void NN::update() {
    while (!neuron[curUpdate]->updateGradHasNext()) {
        curUpdate++;
        if (curUpdate > tot)
            return;
    }
    //cout<<curUpdate<<endl;
    neuron[curUpdate]->update_grad();
//    for (int i = 1; i <= tot; i++) {
//        cout<<i<<endl;
//        neuron[i]->update_grad();
//    }
}

bool NN::revealHasNext() {
//    return curReveal < tot;
    for (int i = 1; i <= tot; i++) {
        if (neuron[i]->revealHasNext())
            return 1;
    }
    return 0;
}

void NN::reveal() {
//    while (!neuron[curReveal]->revealHasNext()) {
//        curReveal++;
//        if (curReveal > tot)
//            return;
//    }
//    //cout<<curUpdate<<endl;
//    neuron[curReveal]->reveal();
    for (int i = 1; i <= tot; i++) {
            neuron[i]->reveal();
    }
}

bool NN::outputHasNext() {
    for (int i = 1; i <= tot; i++) {
        if (neuron[i]->outputHasNext())
            return 1;
    }
    return 0;
}

void NN::output() {
//    while (!neuron[curReveal]->revealHasNext()) {
//        curReveal++;
//        if (curReveal > tot)
//            return;
//    }
//    //cout<<curUpdate<<endl;
//    neuron[curReveal]->reveal();
    for (int i = 1; i <= tot; i++) {
            neuron[i]->output();
    }
}

int NN::getTot() {
    return tot;
}
