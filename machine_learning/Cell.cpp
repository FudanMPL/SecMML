//
// Created by 姜子丰 on 2020/8/22.
//

#include "Cell.h"

Cell::Cell(){};

Cell::Cell(NN* nn, int c_in, int h_in, int x_in){
    this->nn=nn;
    int hidden_num = 1;
    this->c_in=c_in;
    this->h_in=h_in;
    this->x_in=x_in;
    cout<<"cout"<<endl;
    c_out=nn->addnode(CH,B,NeuronMat::NODE_OP);
    cout<<"hout"<<endl;
    h_out=nn->addnode(CH,B,NeuronMat::NODE_OP);

    cout<<"st_m"<<endl;
    st_m=nn->addnode(D2+CH+1,B,NeuronMat::NODE_OP);
    cout<<"st_f"<<endl;
    st_f=nn->addnode(CH,B,NeuronMat::NODE_OP);
    cout<<"st_i"<<endl;
    st_i=nn->addnode(CH,B,NeuronMat::NODE_OP);
    cout<<"st_c"<<endl;
    st_c=nn->addnode(CH,B,NeuronMat::NODE_OP);
    cout<<"st_o"<<endl;
    st_o=nn->addnode(CH,B,NeuronMat::NODE_OP);

    cout<<"sig_f"<<endl;
    sig_f=nn->addnode(CH,B,NeuronMat::NODE_OP);
    cout<<"sig_i"<<endl;
    sig_i=nn->addnode(CH,B,NeuronMat::NODE_OP);
    cout<<"tanh_cc"<<endl;
    tanh_cc=nn->addnode(CH,B,NeuronMat::NODE_OP);
    cout<<"sig_o"<<endl;
    sig_o=nn->addnode(CH,B,NeuronMat::NODE_OP);

    cout<<"mul_f"<<endl;
    mul_f=nn->addnode(CH,B,NeuronMat::NODE_OP);
    cout<<"mul_i"<<endl;
    mul_i=nn->addnode(CH,B,NeuronMat::NODE_OP);

    cout<<"w_f"<<endl;
    w_f=nn->addnode(CH,D2+CH+1,NeuronMat::NODE_NET);
    cout<<"w_i"<<endl;
    w_i=nn->addnode(CH,D2+CH+1,NeuronMat::NODE_NET);
    cout<<"w_c"<<endl;
    w_c=nn->addnode(CH,D2+CH+1,NeuronMat::NODE_NET);
    cout<<"w_o"<<endl;
    w_o=nn->addnode(CH,D2+CH+1,NeuronMat::NODE_NET);

    cout<<"tanh_c"<<endl;
    tanh_c=nn->addnode(CH,B,NeuronMat::NODE_OP);

}

void Cell::addEdges() {
    nn->addOpVstack(st_m,h_in,x_in);

    nn->addOpMul_Mat(st_f, w_f, st_m);
    nn->addOpMul_Mat(st_i, w_i, st_m);
    nn->addOpMul_Mat(st_c, w_c, st_m);
    nn->addOpMul_Mat(st_o, w_o, st_m);



    nn->addOpSigmoid(sig_f,st_f);
    nn->addOpSigmoid(sig_i,st_i);
    // nn->addOpRaw_Tanh(tanh_cc,st_c);
    nn->addOpHybrid_Tanh(tanh_cc,st_c);
    nn->addOpSigmoid(sig_o,st_o);



    nn->addOpHada_Mat(mul_f, c_in, sig_f);
    nn->addOpHada_Mat(mul_i, sig_i, tanh_cc);


    nn->addOpAdd_Mat(c_out,mul_f,mul_i);
    // nn->addOpRaw_Tanh(tanh_c,c_out);
    nn->addOpHybrid_Tanh(tanh_c,c_out);
    nn->addOpHada_Mat(h_out,tanh_c,sig_o);
}

void Cell::setWeight(Mat *w_f, Mat *w_i, Mat *w_c, Mat *w_o){
    nn->getNeuron(this->w_f)->setForward(w_f);
    nn->getNeuron(this->w_i)->setForward(w_i);
    nn->getNeuron(this->w_c)->setForward(w_c);
    nn->getNeuron(this->w_o)->setForward(w_o);
}

int Cell::getC_out(){
    return c_out;
}

int Cell::getH_out() {
    return h_out;
}

int Cell::getW_f(){
    return w_f;
}
int Cell::getW_c(){
    return w_c;
}
int Cell::getW_i() {
    return w_i;
}
int Cell::getW_o(){
    return w_o;
}