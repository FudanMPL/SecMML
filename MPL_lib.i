// 
// Created by mingxin xu on 2021/07/21.
// 

// module name
%module MPL_lib

// include header files
%{
#include "Constant.h"
#include "machine_learning/BPGraph.h"
#include "machine_learning/LSTMGraph.h"
#include "machine_learning/Cell.h"
#include "machine_learning/NN.h"
#include "core/Mat.h"
#include "core/MathOp.h"
#include "core/NeuronMat.h"
#include "core/Op.h"
#include "core/Player.h"
#include "malicious_lib/feldman_vss.h"
#include "malicious_lib/reed_solomn_reconstruct.h"
#include "util/IOManager.h"
#include "util/SocketManager.h"
#include "util/SocketOnline.h"
%}

using namespace std;

// deal with nested classes
// neccessary if the target language doesn't support nested classes
%feature("flatnested");

// deal with std::string
%include "std_string.i"

// deal with c++ arrays
%include "carrays.i"
%array_functions(int, intArray);
%array_functions(string, stringArray);

// deal with ll128
// only support 64-bit integer currently
%typemap(in) ll128 = long;
// support overloading
%typemap(typecheck) ll128 = long;

// declarations not in header files and to be wrapped
%inline %{
int node_type;
int globalRound;
const int CONST_B = B;
const int CONST_D = D;
const int CONST_L = L;
const int CONST_D2 = D2;
const int CONST_CH = CH;
const int CONST_IE = IE;
static void set_seed() {
    srand(time(NULL));
}
%}

// declarations to be wrapped
class Player {
public:
    static void init();
};

class IOManager {
public:
    static Mat train_data, train_label;
    static Mat test_data, test_label;
    static void init();
};

// the visible nested classes will be moved to the same namespace as the containing class when using "flatnested"
// use %rename to hold the form OuterClass_InnerClass (Optional)
%rename (SocketManager_SMMLF) SocketManager::SMMLF;
class SocketManager {
public:
    class SMMLF {
    public:
        void init(string* ip, int* port);
    };
};

class Mat {
public:
    Mat(int r, int c);
    Mat(int r, int c, ll128 b);
    static void random_neg(Mat *a);
};

class NeuronMat {
public:
    static const int NODE_INPUT = 1;
    static const int NODE_NET = 2;
    static const int NODE_OP = 3;
    static const int NODE_VIA = 4;
    static const int NODE_FORWARD = 5;
    Mat* getForward();
};
%extend NeuronMat {
    // prevent garbage collection in python
    void setForward(Mat* a) {
        self->setForward(new Mat(*a));
    }
}

class NN {
public:
    NN();
    void global_variables_initializer();
    void epoch_init();
    void reveal_init(int u);
    void reveal_init();
    void addedge(int u, int v);
    int addnode(int r, int c, int k);
    NeuronMat* getNeuron(int u);
    void setOp(int u, Op* op);
    void addOpAdd_Mat(int res, int a, int b);
    void addOpMinus_Mat(int res, int a, int b);
    void addOpMul_Mat(int res, int a, int b);
    void addOpMul_Const_Mat(int res, int a, double b);
    void addOpDiv_Const_Mat(int res, int a, ll128 b);
    void addOpDiv_Const_Optimized_Mat(int res, int a, ll b);
    void addOpDiv_Seg_Const_Mat(int res, int a, int b);
    void addOpHada_Mat(int res, int a, int b);
    void addOpMeanSquaredLoss(int res, int a, int b);
    void addOpSimilar(int res, int a, int b);
    void addOpConcat(int res, int a, int b);
    void addOpHstack(int res, int a, int b);
    void addOpVstack(int res, int a, int b);
    void addOpVia(int res, int a);
    void addOpTanh(int res, int a);
    void addOpHard_Tanh(int res, int a);
    void addOpHybrid_Tanh(int res, int a);
    void addOpRaw_Tanh(int res, int a);
    void addOpTanh_change(int res, int a);
    void addOpTanh_ex(int res, int a);
    void addOpSigmoid(int res, int a);
    void addOpReLU(int res, int a);
    void addOpArgmax(int res, int a);
    void addOpLTZ(int res, int a);
    void addOpEQZ(int res, int a);
    void addOpEQZ_2LTZ(int res, int a);
    void addOpEqual(int res, int a, int b);
    void toposort();
    void gradUpdate();
    bool forwardHasNext();
    void forwardNext();
    void backNext();
    bool backHasNext();
    bool updateHasNext();
    void update();
    bool revealHasNext();
    void reveal();
    int getTot();
};

class Cell {
public:
    Cell(NN* nn, int c_in, int h_in, int x_in);
    void addEdges();
    int getC_out();
    int getH_out();
};
%extend Cell {
    // prevent garbage collection in python
    void setWeight(Mat *w_f, Mat *w_i, Mat *w_c, Mat *w_o) {
        self->setWeight(new Mat (*w_f), new Mat (*w_i), new Mat (*w_c), new Mat (*w_o));
    }
}

%rename (BPGraph_LR) BPGraph::LR;
class BPGraph {
public:
    class LR{
    public:
        NN* nn;
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
        LR(Mat* train_data, Mat* train_label, Mat* test_data, Mat* test_label);
        void linear_graph();
        void logistic_graph();
        void graph();
        void train();
        void test();
    };
};
%extend BPGraph::LR {
    NN* get_nn() {
        return self->nn;
    }
}

%rename (LSTMGraph_LSTM) LSTMGraph::LSTM;
class LSTMGraph {
public:
    class LSTM {
    public:
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
        LSTM(Mat* train_data, Mat* train_label, Mat* test_data, Mat* test_label);
        void graph();
        void train();
        void test();
    };
};
%extend LSTMGraph::LSTM {
    NN* get_nn() {
        return self->nn;
    }

    // prevent garbage collection in python
    void set_cell(int index, Cell* cell) {
        self->cells[index] = new Cell(*cell);
    }

    Cell* get_cell(int index) {
        return self->cells[index];
    }
}
