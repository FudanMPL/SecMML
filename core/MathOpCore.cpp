/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-28 16:31:02
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2022-01-05 19:37:15
 */

#include "MathOp.h"

MathOp::Div_Const_Trunc_Optimized::Div_Const_Trunc_Optimized() {}

MathOp::Div_Const_Trunc_Optimized::Div_Const_Trunc_Optimized(Mat *res, Mat *a, ll b) {
    this->res = res;
    this->a = a;

    this->exponent = 1;
    
    if (Config::config->IE >= b) {
        ll128 inverse = Constant::Util::get_residual(Config::config->IE/b);
        // ll128 inverse = Constant::Util::inverse(b, MOD);
        cout << "Inverse: " << inverse << ", b: " <<b << endl;
        // DBGprint("Inverse: %lld\n", inverse);
        this->b = inverse;
        div2mP = new Div2mP(res, res, Config::config->BIT_P_LEN + Config::config->DECIMAL_PLACES, Config::config->DECIMAL_PLACES);
    } else {
        int exponent = log2(b);
        ll residual = b * Config::config->IE / pow(2, exponent);
        cout << "exponent: " << exponent << ", residual: " << residual << endl;
        this->b = Constant::Util::get_residual(Config::config->IE * Config::config->IE/residual);
        div2mP = new Div2mP(res, res, Config::config->BIT_P_LEN + Config::config->DECIMAL_PLACES, exponent + Config::config->DECIMAL_PLACES);
    }
    init(2, 0);
}

void MathOp::Div_Const_Trunc_Optimized::forward() {
    reinit();
    switch (forwardRound) {
        case 1: 
        //    *res = (*a * (Config::config->IE / b));
            // init 
            *res = *a * b;
            break;
        case 2:
            div2mP->forward();
            if (div2mP->forwardHasNext()) {
                forwardRound--;
            }
            break;
    }
}

void MathOp::Div_Const_Trunc_Optimized::back() {}

MathOp::KOrCL::KOrCL() {}

MathOp::KOrCL::KOrCL(Mat* res, Mat *d_B, int k) {
    this->res = res;
    this->d_B = d_B;
    this->k = k;
    int tmp_r, tmp_c;
    tmp_r = res->rows();
    tmp_c = res->cols();
    // PRandM
    r_nd = new Mat(tmp_r, tmp_c);
    r_st = new Mat(tmp_r, tmp_c);
    r_B = new Mat[k];
    for (int i = 0; i < k; i++) {
        r_B[i].init(tmp_r, tmp_c);
    }
    coefficients = {99999999999999948, 33463158713874187, 86737888256286702, 11567549490043411, 99921519514012901, 79328041657849319,
                    71201314937608293, 98977993829566184, 4710723341940302, 29861796160469053, 89279246751037322, 36968306758133112, 58454784438587612,
                    75328931003967543, 86384837554252909, 6683547233687751, 50782373290388091, 86538866472678812, 6325226823853224, 3985933527286092,
                    77424372073292124, 91130324538764507, 30006118432141175, 85528322862801917, 19323778082869053, 55452091436759921,
                    50968410924755803, 15487837657987303, 92765158379360327, 19125538233297694, 64665344039295436, 21511061014140188,
                    38341389469709633, 91138994162178133, 2596434147240363, 18563992726472778, 20314152126546477, 46353985440399574,
                    40509026434273843, 10902356853490174, 78426231192074644, 7357679543159906, 36738669188341968, 97244173271201797,
                    28498238528940992, 19107855296183373, 48624109143330949, 21615215036840124, 31351471151295706, 11808938035990712,
                    27816325723419482, 58708837276119684, 31861430804545569, 85836083318538838, 75971425250598398, 30422588448118748};
    // Random b_i, b_i', c_i = A * b_(i-1) * b_i^-1
    b = new Mat[k];
    b_st = new Mat[k];
    B_pub = new Mat[k];
    B_mul = new Mat[k];
    C = new Mat[k];
    for (int i = 0; i < k; i++) {
        b[i].init(tmp_r, tmp_c);
        b_st[i].init(tmp_r, tmp_c);
        B_pub[i].init(tmp_r, tmp_c);
        B_mul[i].init(tmp_r, tmp_c);
        C[i].init(tmp_r, tmp_c);
    }
    pRandFld_b = new PRandFld *[k];
    pRandFld_b_st = new PRandFld *[k];
    mul_pub = new MulPub *[k];
    for (int i = 0; i < k; i++) {
        pRandFld_b[i] = new PRandFld(b+i, Config::config->MOD);
        pRandFld_b_st[i] = new PRandFld(b_st+i, Config::config->MOD);
        mul_pub[i] = new MulPub(B_pub+i, b+i, b_st+i);
    }

    pDegRed = new DegRed *[k];
    for (int i = 0; i < k; ++i) {
        pDegRed[i] = new DegRed(B_mul+i, B_mul+i);
    }

    mul_pub_nd = new MulPub *[k];
    A = new Mat(tmp_r, tmp_c);
    for (int i = 0; i < k; ++i) {
        mul_pub_nd[i] = new MulPub(C+i, A, B_mul+i);
    }


    A_pow = new Mat[k];
    for (int i = 0; i < k; i++) {
        A_pow[i].init(tmp_r, tmp_c);
    }
//    pRandM = new PRandM(r_nd, r_st, r_B, k, k);
    tmp1 = new Mat(tmp_r, tmp_c);
    tmp2 = new Mat[k];
    for (int i = 0; i < k; ++i) {
        tmp2[i].init(tmp_r, tmp_c);
    }
    reveal_a_pow = new Reveal*[k];
    for (int i = 0; i < k; ++i) {
        reveal_a_pow[i] = new Reveal(tmp2+i, A_pow+i);
    }
    reveal = new Reveal(tmp1, A);
    init(13, 0);
}

void MathOp::KOrCL::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            // Lagrange interpolation f(1) = 0,
            break;
        case 2:
            A->clear();
            for (int i = 0; i < k; ++i) {
                *A = *A + d_B[i];
            }
            *A = *A + 1;
            break;
        case 3:
            reveal->forward();
            if (reveal->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 4:
//            cout << "A\n";
//            tmp1->print();
            break;
        case 5:
            for (int i = 0; i < k; i++) {
                pRandFld_b[i]->forward();
                pRandFld_b_st[i]->forward();
            }
            for (int i = 0; i < k; i++) {
                if (pRandFld_b[i]->forwardHasNext() || pRandFld_b_st[i]->forwardHasNext()) {
                    forwardRound--;
                    break;
                }
            }
            break;
        case 6:
            for (int i = 0; i < k; i++) {
                mul_pub[i]->forward();
            }
            for (int i = 0; i < k; i++) {
                if (mul_pub[i]->forwardHasNext()) {
                    forwardRound--;
                    break;
                }
            }
            break;
        case 7:
            // calculate A^i
            B_mul[0] = b_st[0].dot(B_pub[0].inverse());
            for (int i = 1; i < k; ++i) {
                B_mul[i] = b[i-1].dot(B_pub[i].inverse()).dot(b_st[i]);
            }
            break;
        case 8:
            for (int i = 1; i < k; i++) {
                pDegRed[i]->forward();
                if (pDegRed[i]->forwardHasNext()) {
                    forwardRound--;
                    break;
                }
            }
            break;
        case 9:
            for (int i = 0; i < k; i++) {
                mul_pub_nd[i]->forward();
            }
            for (int i = 0; i < k; i++) {
                if (mul_pub_nd[i]->forwardHasNext()) {
                    forwardRound--;
                    break;
                }
            }
            break;
        case 10:
            for (int i = 0; i < k; ++i) {
                A_pow[i] = b[i];
                for (int j = 0; j <= i; ++j) {
                    A_pow[i] = A_pow[i].dot(C[j]);
                }
            }
            break;
        case 11:
            res->clear();
            *res = *res + coefficients[0];
            // todo: multiply lagrange coefficients
            for (int i = 1; i <= k; ++i) {
                *res += A_pow[i-1] * coefficients[i];
            }
            break;
        case 12:
            for (int i = 0; i < k; ++i) {
                reveal_a_pow[i]->forward();
            }
            for (int i = 0; i < k; i++) {
                if (reveal_a_pow[i]->forwardHasNext()) {
                    forwardRound--;
                    break;
                }
            }
            break;
        case 13:
            cout << "A-pow\n";
//            for (int i = 0; i < k; ++i) {
//                cout << i << endl;
//                tmp2[i].print();
//            }
            break;
}
}

void MathOp::KOrCL::back() {}

MathOp::Hard_Tanh::Hard_Tanh() {}

MathOp::Hard_Tanh::Hard_Tanh(NeuronMat *res, NeuronMat *a){
    this->res = res;
    this->a = a;
    init();
}

void MathOp::Hard_Tanh::forward(){
    reinit();
    *res->getForward()=a->getForward()->hard_tanh();
}
void MathOp::Hard_Tanh::back(){
    backRound++;
    if (!a->getIsBack()){
        *a->getGrad()=res->getForward()->dot(res->getForward()->oneMinus_IE())/Config::config->IE;
        *a->getGrad()=a->getGrad()->dot(*res->getGrad())*2/Config::config->IE;
    }
}

MathOp::Tanh_ex::Tanh_ex() {}

MathOp::Tanh_ex::Tanh_ex(NeuronMat *res, NeuronMat *a){
    this->res = res;
    this->a = a;
    init();
}

void MathOp::Tanh_ex::forward(){
    reinit();
    *res->getForward()=a->getForward()->chebyshev_tanh();
}
void MathOp::Tanh_ex::back(){
    backRound++;
    if (!a->getIsBack()){
        *a->getGrad()=res->getForward()->dot(res->getForward()->oneMinus_IE())/Config::config->IE;
        *a->getGrad()=a->getGrad()->dot(*res->getGrad())*2/Config::config->IE;
    }
}
MathOp::Tanh::Tanh() {}

MathOp::Tanh::Tanh(NeuronMat *res, NeuronMat *a) {
    this->res = res;
    this->a = a;
    a_r = a->getForward();
    int tmp_r, tmp_c;
    tmp_r = a_r->rows();
    tmp_c = a_r->cols();
    temp_f = new Mat(tmp_r, tmp_c);
    temp_b = new Mat(tmp_r, tmp_c);
    div2mP_f1 = new Div2mP(res->getForward(), res->getForward(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    div2mP_f2 = new Div2mP(res->getForward(), res->getForward(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    div2mP_f4 = new Div2mP(res->getForward(), res->getForward(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    div2mP_f3 = new Div2mP(res->getForward(), res->getForward(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    div2mP_f5 = new Div2mP(res->getForward(), res->getForward(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    div2mP_f6 = new Div2mP(res->getForward(), res->getForward(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    div2mP_f7 = new Div2mP(res->getForward(), res->getForward(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    div2mP_f8 = new Div2mP(res->getForward(), res->getForward(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    div2mP_b1 = new Div2mP(a->getGrad(), a->getGrad(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    div2mP_b2 = new Div2mP(a->getGrad(), a->getGrad(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    init(10, 4);
}

void MathOp::Tanh::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            *res->getForward() = *a_r * coefficients_f[0] / Config::config->IE;
            break;
        case 2:
            *res->getForward() = (*res->getForward() + coefficients_f[1]).dot(*a_r);
            break;
        case 3:
            div2mP_f1->forward();
            if (div2mP_f1->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 4:
            *res->getForward() = (*res->getForward() + coefficients_f[2]).dot(*a_r);
            break;
        case 5:
            div2mP_f2->forward();
            if (div2mP_f2->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 6:
            *res->getForward() = (*res->getForward() + coefficients_f[3]).dot(*a_r);
            break;
        case 7:
            div2mP_f3->forward();
            if (div2mP_f3->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 8:
            *res->getForward() = (*res->getForward() + coefficients_f[4]).dot(*a_r);
            break;
        case 9:
            div2mP_f4->forward();
            if (div2mP_f4->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 10:
            *res->getForward() = (*res->getForward() + Config::config->IE) / 2;
            break;
    }
}

void MathOp::Tanh::back() {
    backRound++;
    switch (backRound) {
        case 1: {
            if (!a->getIsBack()) {
                *a->getGrad() = (*res->getForward()).dot(res->getForward()->oneMinus_IE()) * 2;
            }
        }
            break;
        case 2: {
            if (!a->getIsBack()) {
                div2mP_b1->forward();
                if (div2mP_b1->forwardHasNext()) {
                    backRound--;
                }
            }
        }
            break;
        case 3: {
            if (!a->getIsBack()) {
                *a->getGrad() = a->getGrad()->dot(*res->getGrad());
            }
        }
            break;
        case 4: {
            if (!a->getIsBack()) {
                div2mP_b2->forward();
                if (div2mP_b2->forwardHasNext()) {
                    backRound--;
                }
            }
        }
            break;
    }
}

MathOp::Tanh_Mat::Tanh_Mat() {}

MathOp::Tanh_Mat::Tanh_Mat(NeuronMat *res, NeuronMat *a){
    this->res = res;
    this->a = a;
    a_r = a->getForward();
    div2mP_f1 = new Div2mP(res->getForward(), res->getForward(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    div2mP_f2 = new Div2mP(res->getForward(), res->getForward(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    div2mP_f3 = new Div2mP(res->getForward(), res->getForward(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    div2mP_b1 = new Div2mP(a->getGrad(), a->getGrad(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    div2mP_b2 = new Div2mP(a->getGrad(), a->getGrad(), Config::config->BIT_P_LEN, Config::config->DECIMAL_PLACES);
    int tmp_r, tmp_c;
    tmp_r = a->rows();
    tmp_c = a->cols();
    res->setAux(new Mat(tmp_r, tmp_c));
    u_st = new Mat(tmp_r, tmp_c);
    u_nd = new Mat(tmp_r, tmp_c);
    u_rd = new Mat(tmp_r, tmp_c);
    pLTZ_f_1 = new LTZ(u_st, u_st, Config::config->BIT_P_LEN);
    pLTZ_f_2 = new LTZ(u_rd, u_rd, Config::config->BIT_P_LEN);
    init(9,4);
}

void MathOp::Tanh_Mat::forward(){
    reinit();
    switch (forwardRound) {
        case 1:
            *res->getForward() = *a_r * coefficients[0] / Config::config->IE;
            break;
        case 2:
            *res->getForward() = res->getForward()->dot(*a_r);
            break;
        case 3:
            div2mP_f1->forward();
            if (div2mP_f1->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 4:
            *res->getForward() = (*res->getForward() + coefficients[1]).dot(*a_r);
            break;
        case 5:
            div2mP_f2->forward();
            if (div2mP_f2->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 6:
            *u_st = *a->getForward() + Config::config->IE;
            *u_rd = *a->getForward() - Config::config->IE;
            break;
        case 7:
            pLTZ_f_1->forward();
            pLTZ_f_2->forward();
            if (pLTZ_f_1->forwardHasNext() || pLTZ_f_2->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 8:
            *u_rd = u_rd->oneMinus_IE();
            *u_nd = (*u_rd+*u_st).oneMinus_IE();
            *res->getForward() = res->getForward()->dot(*u_nd)+*u_rd*Config::config->IE-*u_st*Config::config->IE;
            break;
        case 9:
            div2mP_f3->forward();
            if (div2mP_f3->forwardHasNext()) {
                forwardRound--;
            }
            break;
    }
}
void MathOp::Tanh_Mat::back(){
    backRound++;
    switch (backRound) {
        case 1: {
            if (!a->getIsBack()) {
                *a->getGrad() = (*res->getForward()+Config::config->IE).dot(res->getForward()->oneMinus_IE());
            }
        }
            break;
        case 2: {
            if (!a->getIsBack()) {
                div2mP_b1->forward();
                if (div2mP_b1->forwardHasNext()) {
                    backRound--;
                }
            }
        }
            break;
        case 3: {
            if (!a->getIsBack()) {
                *a->getGrad() = a->getGrad()->dot(*res->getGrad());
            }
        }
            break;
        case 4: {
            if (!a->getIsBack()) {
                div2mP_b2->forward();
                if (div2mP_b2->forwardHasNext()) {
                    backRound--;
                }
            }
        }
            break;
    }
}

MathOp::Raw_Tanh::Raw_Tanh() {}

MathOp::Raw_Tanh::Raw_Tanh(NeuronMat *res, NeuronMat *a){
    this->res = res;
    this->a = a;
    init();
}

void MathOp::Raw_Tanh::forward(){
    reinit();
    *res->getForward()=a->getForward()->raw_tanh();
}

void MathOp::Raw_Tanh::back(){
    backRound++;
    if (!a->getIsBack()){
        *a->getGrad()=(*res->getForward()+Config::config->IE).dot(res->getForward()->oneMinus_IE())/Config::config->IE;
        *a->getGrad()=a->getGrad()->dot(*res->getGrad())/Config::config->IE;
    }
}

MathOp::Tanh_change::Tanh_change() {}

MathOp::Tanh_change::Tanh_change(NeuronMat *res, NeuronMat *a) {
    this->res = res;
    this->a = a;
    init();
}

void MathOp::Tanh_change::forward() {
    reinit();
//    *res->getForward()=(*a->getForward()+Config::config->IE) / 2;
    *res->getForward()=(*a->getForward()+Config::config->IE)* Constant::Util::inverse(2, Config::config->MOD);   
}

void MathOp::Tanh_change::back() {
    backRound++;
    if (!a->getIsBack()){
        *a->getGrad()=*res->getGrad()*2;
    }
}