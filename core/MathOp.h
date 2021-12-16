//
// Created by tangdingyi on 2019/12/26.
//

#ifndef MPC_ML_MATHOP_H
#define MPC_ML_MATHOP_H

#include "Op.h"
#include "NeuronMat.h"
#include "../util/SocketOnline.h"
#include "Player.h"
#include "../malicious_lib/reed_solomn_reconstruct.h"
#include "../util/IOManager.h"
#include "../Constant.h"
#include "../util/SocketManager.h"

// Multi thread configuration
#include <thread>


// extern int node_type;
extern Mat metadata;
//extern Mat *input_table_data;
class MathOp {
public:
    class PRandFld;
    class MulPub;
    class PRandBit;
    class PRandM;
    class Reveal;
    class Div2mP;
    class DegRed;
    class RevealD;
    class Add_Mat: public Op {
        NeuronMat *res, *a, *b;
    public:
        Add_Mat();
        Add_Mat(NeuronMat* res, NeuronMat* a, NeuronMat* b);
        void forward();
        void back();
    };
    class Minus_Mat: public Op {
        NeuronMat *res, *a, *b;
    public:
        Minus_Mat();
        Minus_Mat(NeuronMat* res, NeuronMat* a, NeuronMat* b);
        void forward();
        void back();
    };
    class Mul_Mat: public Op {
        NeuronMat *res, *a, *b;
        Mat *temp_a,*temp_b;
        Div2mP *div2mP_f;
        Div2mP *div2mP_b_a, *div2mP_b_b;
    public:
        Mul_Mat();
        Mul_Mat(NeuronMat* res, NeuronMat* a, NeuronMat* b);
        void forward();
        void back();
    };
    class Hada_Mat: public Op {
        NeuronMat *res, *a, *b;
        Mat *temp_a,*temp_b;
        Div2mP *div2mP_f;
        Div2mP *div2mP_b_a, *div2mP_b_b;
    public:
        Hada_Mat();
        Hada_Mat(NeuronMat* res, NeuronMat* a, NeuronMat* b);
        void forward();
        void back();
    };
    class Mul_Const_Trunc: public Op {
        Mat *res, *a, *revlea1;
//        ll128 b;
        double b;
        Div2mP *div2mP;
        Reveal *reveal, *reveal2;
    public:
        Mul_Const_Trunc();
        Mul_Const_Trunc(Mat* res, Mat* a, double b);
        void forward();
        void back();
    };
    class Div_Const_Trunc: public Op {
        Mat *res, *a;
        ll128 b;
        Div2mP *div2mP;
    public:
        Div_Const_Trunc();
        Div_Const_Trunc(Mat* res, Mat* a, ll128 b);
        void forward();
        void back();
    };
    class Div_Const_Trunc_Optimized: public Op {
        Mat *res, *a;
        ll b;
        int exponent;
        Div2mP *div2mP;
    public:
        Div_Const_Trunc_Optimized();
        Div_Const_Trunc_Optimized(Mat* res, Mat* a, ll b);
        void forward();
        void back();
    };
    class Div_Seg_Const_Trunc: public Op {
        Mat *res, *a;
        Mat* b;
        Div2mP *div2mP;
    public:
        Div_Seg_Const_Trunc();
        Div_Seg_Const_Trunc(Mat* res, Mat* a, Mat* b);
        void forward();
        void back();
    };
    class Via: public Op {
        NeuronMat *res, *a;
    public:
        Via();
        Via(NeuronMat* res, NeuronMat* a);
        void forward();
        void back();
    };
    class MeanSquaredLoss: public Op {
        NeuronMat *res, *a, *b;
        Reveal *reveal_a, *reveal_b, *reveal_res;
        Mat *tmp_a, *tmp_b, *tmp_res;
    public:
        MeanSquaredLoss();
        MeanSquaredLoss(NeuronMat* res, NeuronMat* a, NeuronMat* b);
        void forward();
        void back();
    };
    class Similar: public Op {
        NeuronMat *res, *a, *b;
    public:
        Similar();
        Similar(NeuronMat* res, NeuronMat* a, NeuronMat* b);
        void forward();
        void back();
    };
    class Concat: public Op {
        NeuronMat *res, *a, *b;
    public:
        Concat();
        Concat(NeuronMat* res, NeuronMat* a, NeuronMat* b);
        void forward();
        void back();
    };
    class Hstack: public Op {
        NeuronMat *res, *a, *b;
    public:
        Hstack();
        Hstack(NeuronMat* res, NeuronMat* a, NeuronMat* b);
        void forward();
        void back();
    };
    class Vstack: public Op {
        NeuronMat *res, *a, *b;
    public:
        Vstack();
        Vstack(NeuronMat* res, NeuronMat* a, NeuronMat* b);
        void forward();
        void back();
    };
    class Div2mP: public Op {
        Mat *res, *a;
        Mat *r_nd, *r_st, *r_B;
        Mat *r;
        Mat *b;
        int k, m;
        PRandM *pRandM;
        RevealD *pRevealD;
    public:
        Div2mP();
        Div2mP(Mat* res, Mat* a, int k, int m);
        void forward();
        void back();
        void reset_for_multi_call();
    };
    class Reveal: public Op {
        Mat *res, *a;
        Mat *b;
        reed_solomn* rs = new reed_solomn(Config::config->MOD);
    public:
        Reveal();
        Reveal(Mat* res, Mat* a);
        void forward();
        void back();
    };
    class PRandM: public Op {
        Mat *r_nd, *r_st, *b_B;
        int k, m;
        PRandFld *pRandFld;
        PRandBit **pRandBit;
        Reveal *reveal, *reveal1;
//        Mat *tmp, *tmp1;
    public:
        PRandM();
        PRandM(Mat* r_nd, Mat* r_st, Mat* b_B, int k, int m);
        PRandM(Mat* r_nd, Mat* r_st, int k, int m);
        PRandM(int r, int c, int k, int m);
        void PRandM_init(Mat* r_nd, Mat* r_st, Mat* b_B, int k, int m);
        void forward();
        void back();
        void reset_for_multi_call();
    };
    class PRandBit: public Op {
        Mat *res;
        PRandFld *pRandFld;
        MulPub *mulPub;
        Mat *a, *a2;
        Mat *a_r, *a2_r;
    public:
        PRandBit();
        PRandBit(Mat* res);
        void forward();
        void back();
        void reset_for_multi_call();
    };
    class MulPub: public Op {
        Mat *res, *a, *b;
    public:
        MulPub();
        MulPub(Mat* res, Mat* a, Mat* b);
        void forward();
        void back();
    };
    class PRandFld: public Op {
        Mat *res;
        ll128 range;
    public:
        PRandFld();
        PRandFld(Mat* res, ll range);
        void forward();
        void back();
    };
    class Mod2: public Op {
        Mat *res, *a;
        Mat *r_nd, *r_st, *r_B;
        Mat *c;
        int k;
        PRandM *pRandM;
        Reveal *reveal;
    public:
        Mod2();
        Mod2(Mat* res, Mat* a, int k);
        void forward();
        void back();
    };
    class DegRed: public Op {
        Mat *res, *a;
        Mat *tmp;
    public:
        DegRed();
        DegRed(Mat* res, Mat* a);
        void forward();
        void back();
    };
    class PreMulC: public Op {
        Mat *res, *a;
        Mat *m;
        Mat *w, *z;
        Mat *r, *s, *u;
        int k;
        PRandFld **pRandFld_r, **pRandFld_s;
        MulPub **pMulPub_st;
        DegRed **pDegRed;
        MulPub **pMulPub;
    public:
        PreMulC();
        PreMulC(Mat* res, Mat* a, int k);
        void forward();
        void back();
    };
    class BitLT: public Op {
        Mat *res, *a;
        Mat *b_B;
        Mat *d_B, *p_B;
        Mat *d_B_inverse, *p_B_inverse;
        Mat *s;
        int k;
        PreMulC *preMulC;
        Mod2 *pMod2;
        Reveal *reveal, *reveal1, *reveal_a;
        Reveal **reveal_b;
        Mat *tmp_b;
        Mat *tmp, *tmp1, *tmp_a;
    public:
        BitLT();
        BitLT(Mat* res, Mat* a, Mat* b_B, int k);
        void forward();
        void back();
    };
    class RevealD: public Op {
        Mat *res, *a, *b;
        Reveal *pReveal;
        DegRed *pDegRed;
    public:
        RevealD();
        RevealD(Mat* res, Mat* a, Mat* b);
        RevealD(Mat* res, Mat* a);
        void forward();
        void back();
        void reset_for_multi_call();
    };
    class Mod2m: public Op {
        Mat *res, *a;
        Mat *r_nd, *r_st, *r_B;
        Mat *b;
        Mat *u;
        int k, m;
        PRandM *pRandM;
        RevealD *pRevealD;
        BitLT *pBitLT;
    public:
        Mod2m();
        Mod2m(Mat* res, Mat* a, int k, int m);
        void forward();
        void back();
    };
    class Div2m: public Op {
        Mat *res, *a;
        Mat *b;
        int k, m;
        Mod2m *pMod2m;
        Mat * test1, *test2, *test3;
        Reveal *reveal1, *reveal2, *reveal3;
    public:
        Div2m();
        Div2m(Mat* res, Mat* a, int k, int m);
        void forward();
        void back();
    };
    class LTZ: public Op {
        Mat *res, *a;
        int k;
        Div2m *pDiv2m;
        Reveal *reveal, *reveal_tmp, *reveal_a;
        Mat *tmp1, *tmp2, *tmp3;
    public:
        LTZ();
        LTZ(Mat* res, Mat* a, int k);
        void forward();
        void back();
    };
    class KOrCL: public Op {
        Mat *res, *d_B;
        Mat *r_nd, *r_st, *r_B;
        Mat *A, *C, *A_pow;
        Mat *b, *b_st, *B_pub, *B_mul;
        int k, m;
        PRandFld **pRandFld_b, **pRandFld_b_st;
        vector<ll> coefficients;
        MulPub **mul_pub_nd, **mul_pub;
        DegRed **pDegRed;
        PRandM *pRandM;
        Reveal *reveal, *reveal_tmp, *reveal_a;
        Reveal **reveal_a_pow;
        Mat *tmp1, *tmp2, *tmp3;
    public:
        KOrCL();
        KOrCL(Mat* res, Mat* a, int k);
        void forward();
        void back();
    };
    class EQZ: public Op {
        Mat *res, *a;
        Mat *r_nd, *r_st, *r_B;
        Mat *b;
        Mat *u;
        Mat *d_B;
        int k, m;
        PRandM *pRandM;
        Reveal *pReveal;
        Div2m *pDiv2m;
        Reveal *reveal, *reveal_tmp, *reveal_a;
        KOrCL *kOrCl;
        Mat *tmp1, *tmp2, *tmp3;
    public:
        EQZ();
        EQZ(Mat* res, Mat* a, int k);
        void forward();
        void back();
    };
    class EQZ_2LTZ: public Op {
        Mat *res, *a;
        Mat *u_st, *u_nd;
        Mat *u_st_res, *u_nd_res;
        LTZ *pLTZ_f_1, *pLTZ_f_2;

        int k, m;
        PRandM *pRandM;
        Div2m *pDiv2m;
        Reveal *reveal, *reveal_1, *reveal_2;
        KOrCL *kOrCl;
        Mat *tmp1, *tmp2, *tmp3;
    public:
        EQZ_2LTZ();
        EQZ_2LTZ(Mat* res, Mat* a, int k);
        void forward();
        void back();
    };
    class ReLU_Mat: public Op {
        NeuronMat *res, *a;
        LTZ *pLTZ;
        DegRed *pDegRed_f, *pDegRed_b;
        Div2mP *pDiv2mp_f, *pDiv2mp_b;
        Reveal *reveal_a, *reveal_res;
        Mat *tmp_a, *tmp_res;
    public:
        ReLU_Mat();
        ReLU_Mat(NeuronMat* res, NeuronMat* a);
        void forward();
        void back();
    };
    class Sigmoid_Mat: public Op {
        NeuronMat *res, *a;
        Mat *u_st, *u_nd;
        Mat *u_st_res, *u_nd_res;
        LTZ *pLTZ_f_1, *pLTZ_f_2;
//        DegRed *pDegRed_res;
        Div2mP *pDegRed_res;
//        Div2mP *pDiv2mP_aux, *pDiv2mP_a;
        Div2mP *pDiv2mP_b1,*pDiv2mP_b2;

        Mat *tmp;
        Reveal *reveal;
    public:
        Sigmoid_Mat();
        Sigmoid_Mat(NeuronMat* res, NeuronMat* a);
        void forward();
        void back();
    };
    class Argmax: public Op {
        NeuronMat *res, *a;
    public:
        Argmax();
        Argmax(NeuronMat* res, NeuronMat* a);
        void forward();
        void back();
    };
    class Equal: public Op {
        NeuronMat *res, *a, *b;
    public:
        Equal();
        Equal(NeuronMat* res, NeuronMat* a, NeuronMat* b);
        void forward();
        void back();
    };
    class CrossEntropy: public Op {
        NeuronMat *res, *a, *b;
        Reveal *reveal;
    public:
        CrossEntropy();
        CrossEntropy(NeuronMat* res, NeuronMat* a, NeuronMat* b);
        void forward();
        void back();
    };
    static void broadcast(Mat* a);
    static void broadcast_share(Mat* a, int target);
    static void receive_share(Mat* a, int from);
    static void broadcase_rep(Mat* a);
    static void receive(Mat* a);
    static void receive_add(Mat* a);
    static void receive_rep(Mat* a);
    static void random(Mat* a, ll range);
    class Tanh_ex: public Op{
        NeuronMat *a,*res;
        Mat *temp_f,*temp_b;
    public:
        Tanh_ex();
        Tanh_ex(NeuronMat* res, NeuronMat* a);
        void forward();
        void back();
    };
    class Hard_Tanh: public Op{
        NeuronMat *a,*res;
        Mat *temp_f,*temp_b;
    public:
        Hard_Tanh();
        Hard_Tanh(NeuronMat* res, NeuronMat* a);
        void forward();
        void back();
    };
    class Tanh: public Op{
        NeuronMat *a,*res;
        Mat *a_r,*temp_f,*temp_b;
        //const ll128 coefficients_f[10]={10,0,-652,0,15392,0,-164163,0,971505,0};
        const ll128 coefficients_f[6]={3342,0,99999999999909374,0,879282,0};
        Div2mP *div2mP_f1;
        Div2mP *div2mP_f2;
        Div2mP *div2mP_f3;
        Div2mP *div2mP_f4;
        Div2mP *div2mP_f5;
        Div2mP *div2mP_f6;
        Div2mP *div2mP_f7;
        Div2mP *div2mP_f8;
        Div2mP *div2mP_b1;
        Div2mP *div2mP_b2;
    public:
        Tanh();
        Tanh(NeuronMat* res, NeuronMat* a);
        void forward();
        void back();
    };
    class Tanh_Mat: public Op{
        NeuronMat *a,*res;
        Mat *a_r;
        const ll128 coefficients[2]={-246456, 1036312};
        Div2mP *div2mP_f1;
        Div2mP *div2mP_f2;
        Div2mP *div2mP_f3;
        Div2mP *div2mP_b1;
        Div2mP *div2mP_b2;
        Mat *u_st;
        Mat *u_nd;
        Mat *u_rd;
        Mat *u_st_res, *u_nd_res, *u_rd_res;
        Reveal *reveal;
        LTZ *pLTZ_f_1;
        LTZ *pLTZ_f_2;
    public:
        Tanh_Mat();
        Tanh_Mat(NeuronMat* res, NeuronMat* a);
        void forward();
        void back();
    };

    class Hybrid_tanh: public Op{
        NeuronMat *a, *res;
        Mat *a_2;
        const ll128 coefficients[2]={-246456, 1036312};

    };
    class Tanh_change: public Op{
        NeuronMat *a, *res;
    public:
        Tanh_change();
        Tanh_change(NeuronMat* res, NeuronMat* a);
        void forward();
        void back();

    };

    class Raw_Tanh: public Op{
        NeuronMat *a,*res;
        Mat *temp_f,*temp_b;
    public:
        Raw_Tanh();
        Raw_Tanh(NeuronMat* res, NeuronMat* a);
        void forward();
        void back();
    };

};


#endif //MPC_ML_MATHOP_H
