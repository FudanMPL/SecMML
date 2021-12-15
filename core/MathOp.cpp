//
// Created by tangdingyi on 2019/12/26.
//

#include "MathOp.h"

MathOp::Add_Mat::Add_Mat() {}

MathOp::Add_Mat::Add_Mat(NeuronMat *res, NeuronMat *a, NeuronMat *b) {
    this->res = res;
    this->a = a;
    this->b = b;
    init();
}

void MathOp::Add_Mat::forward() {
    reinit();
    *res->getForward() = (*a->getForward()) + (*b->getForward());
}

void MathOp::Add_Mat::back() {
    backRound++;
    if (!a->getIsBack())
        *a->getGrad() = (*a->getGrad()) + (*res->getGrad());
    if (!b->getIsBack())
        *b->getGrad() = (*b->getGrad()) + (*res->getGrad());
}

MathOp::Minus_Mat::Minus_Mat() {}

MathOp::Minus_Mat::Minus_Mat(NeuronMat *res, NeuronMat *a, NeuronMat *b) {
    this->res = res;
    this->a = a;
    this->b = b;
    init();
}

void MathOp::Minus_Mat::forward() {
    reinit();
    *res->getForward() = (*a->getForward()) - (*b->getForward());
}

void MathOp::Minus_Mat::back() {
    backRound++;
    if (!a->getIsBack())
        *a->getGrad() = (*a->getGrad()) - (*res->getGrad());
    if (!b->getIsBack())
        *b->getGrad() = (*b->getGrad()) - (*res->getGrad());
}


MathOp::Mul_Mat::Mul_Mat() {}

MathOp::Mul_Mat::Mul_Mat(NeuronMat *res, NeuronMat *a, NeuronMat *b) {
    this->res = res;
    this->a = a;
    this->b = b;
    temp_a=new Mat(a->getForward()->rows(),a->getForward()->cols());
    temp_b=new Mat(b->getForward()->rows(),b->getForward()->cols());
    div2mP_f = new Div2mP(res->getForward(), res->getForward(), BIT_P_LEN, DECIMAL_PLACES);
    div2mP_b_a = new Div2mP(temp_a,temp_a, BIT_P_LEN, DECIMAL_PLACES);
    div2mP_b_b = new Div2mP(temp_b,temp_b, BIT_P_LEN, DECIMAL_PLACES);
    init(2, 3);
}

void MathOp::Mul_Mat::forward() {
    reinit();
    switch (forwardRound) {
        case 1: {
            *res->getForward() = (*a->getForward()) * (*b->getForward());
        }
            break;
        case 2: {
            div2mP_f->forward();
            if (div2mP_f->forwardHasNext()) {
                forwardRound--;
            }
        }
            break;
    }
}

void MathOp::Mul_Mat::back() {
    backRound++;
    switch (backRound) {
        case 1: {
            if (!a->getIsBack()) {
                b->getForward()->transorder();
                *temp_a = (*res->getGrad()) * (*b->getForward());
                b->getForward()->transorder();
            }
            if (!b->getIsBack()) {
                a->getForward()->transorder();
                *temp_b = (*a->getForward()) * (*res->getGrad());
                a->getForward()->transorder();
            }
        }
            break;
        case 2:
            if (!a->getIsBack()) {
                div2mP_b_a->forward();
            }
            if (!b->getIsBack()) {
                div2mP_b_b->forward();
            }
            if ((!a->getIsBack() && div2mP_b_a->forwardHasNext()) ||
                (!b->getIsBack() && div2mP_b_b->forwardHasNext())) {
                backRound--;
            }
            break;
        case 3:
            *a->getGrad()+=*temp_a;
            *b->getGrad()+=*temp_b;
            break;
    }
}

MathOp::Hada_Mat::Hada_Mat() {}

MathOp::Hada_Mat::Hada_Mat(NeuronMat *res, NeuronMat *a, NeuronMat *b) {
    this->res = res;
    this->a = a;
    this->b = b;
    temp_a=new Mat(a->getForward()->rows(),a->getForward()->cols());
    temp_b=new Mat(b->getForward()->rows(),b->getForward()->cols());
    div2mP_f = new Div2mP(res->getForward(), res->getForward(), BIT_P_LEN, DECIMAL_PLACES);
    div2mP_b_a = new Div2mP(temp_a,temp_a, BIT_P_LEN, DECIMAL_PLACES);
    div2mP_b_b = new Div2mP(temp_b,temp_b, BIT_P_LEN, DECIMAL_PLACES);
    init(2, 3);
}

void MathOp::Hada_Mat::forward() {
    reinit();
    switch (forwardRound) {
        case 1: {
            *res->getForward() = (*a->getForward()).dot(*b->getForward());
        }
            break;
        case 2: {
            div2mP_f->forward();
            if (div2mP_f->forwardHasNext()) {
                forwardRound--;
            }
        }
            break;
    }
}

void MathOp::Hada_Mat::back() {
    backRound++;
    switch (backRound) {
        case 1: {
            if (!a->getIsBack()) {
                *temp_a = (*res->getGrad()).dot(*b->getForward());
            }
            if (!b->getIsBack()) {
                *temp_b = (*res->getGrad()).dot(*a->getForward());
            }
        }
            break;
        case 2:
            if (!a->getIsBack()) {
                div2mP_b_a->forward();
            }
            if (!b->getIsBack()) {
                div2mP_b_b->forward();
            }
            if ((!a->getIsBack() && div2mP_b_a->forwardHasNext()) ||
                (!b->getIsBack() && div2mP_b_b->forwardHasNext())) {
                backRound--;
            }
            break;
        case 3:
            *a->getGrad()+=*temp_a;
            *b->getGrad()+=*temp_b;
    }
}

MathOp::Mul_Const_Trunc::Mul_Const_Trunc() {}

MathOp::Mul_Const_Trunc::Mul_Const_Trunc(Mat *res, Mat *a, double b) {
    this->res = res;
    this->a = a;
    this->b = b;
    this->revlea1 = new Mat(res->rows(), res->cols());
    *revlea1 = *res;
    div2mP = new Div2mP(res, res, BIT_P_LEN, DECIMAL_PLACES);
    reveal = new Reveal(revlea1, revlea1);
    reveal2 = new Reveal(res, res);
    init(1, 0);
}

void MathOp::Mul_Const_Trunc::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            // *res = (*a) * (ll)(b * IE);
            // we can directly perform local truncation by IE since the learning rate etc. are constants
            *res = (*a) * (ll)(b * IE)/ IE; 
            break;
        case 2:
            div2mP->forward();
            if (div2mP->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 3:
            reveal->forward();
            if (reveal->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 4:
            revlea1->print();
            break;
        case 5:
            div2mP->forward();
            if (div2mP->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 6:
            reveal2->forward();
            if (reveal2->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 7:
            res->print();
            break;
    }
}

void MathOp::Mul_Const_Trunc::back() {}

MathOp::Div_Const_Trunc::Div_Const_Trunc() {}

MathOp::Div_Const_Trunc::Div_Const_Trunc(Mat *res, Mat *a, ll128 b) {
    this->res = res;
    this->a = a;
   ll128 inverse = Constant::Util::get_residual(IE/b);
    cout << "Inverse: " << inverse << ", b: " <<b << endl;
    this->b = inverse;
    div2mP = new Div2mP(res, res, BIT_P_LEN, DECIMAL_PLACES);
    init(2, 0);
}

void MathOp::Div_Const_Trunc::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
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

void MathOp::Div_Const_Trunc::back() {}


MathOp::Div_Seg_Const_Trunc::Div_Seg_Const_Trunc() {}

MathOp::Div_Seg_Const_Trunc::Div_Seg_Const_Trunc(Mat *res, Mat *a, Mat* b) {
    this->res = res;
    this->a = a;
    this->b = b;
    div2mP = new Div2mP(res, res, BIT_P_LEN, DECIMAL_PLACES);
    init(2, 0);
}

void MathOp::Div_Seg_Const_Trunc::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            ll inverse;
            for (int i = 0; i < b->size(); ++i) {
                inverse = Constant::Util::get_residual(IE/b->getVal(i));
                b->setVal(i, inverse);
            }
            *res = (*a).dot(*b);
            break;
        case 2:
            div2mP->forward();
            if (div2mP->forwardHasNext()) {
                forwardRound--;
            }
            break;
    }
}

void MathOp::Div_Seg_Const_Trunc::back() {}

MathOp::Via::Via() {}

MathOp::Via::Via(NeuronMat *res, NeuronMat *a) {
    this->res = res;
    this->a = a;
    init();
}

void MathOp::Via::forward() {
    reinit();
    *res->getForward() = *a->getForward();
}

void MathOp::Via::back() {
    backRound++;
    if (!a->getIsBack())
        *a->getGrad() = (*a->getGrad()) + (*res->getGrad());
}

MathOp::MeanSquaredLoss::MeanSquaredLoss() {}

MathOp::MeanSquaredLoss::MeanSquaredLoss(NeuronMat *res, NeuronMat *a, NeuronMat *b) {
    this->res = res;
    this->a = a;
    this->b = b;
    int tmp_r = a->getForward()->rows();
    int tmp_c = a->getForward()->cols();

    tmp_a = new Mat(tmp_r, tmp_c);
    tmp_b = new Mat(tmp_r, tmp_c);
    tmp_res = new Mat(tmp_r, tmp_c);
    reveal_a = new Reveal(tmp_a, a->getForward());
    reveal_b = new Reveal(tmp_b, b->getForward());
    init(0, 1);
}

void MathOp::MeanSquaredLoss::forward() {}

void MathOp::MeanSquaredLoss::back() {
    backRound++;
    switch (backRound) {
        case 1:
            *b->getGrad() = (*b->getGrad()) + (*a->getForward()) - (*b->getForward());
            break;
        case 2:
            reveal_a->forward();
            reveal_b->forward();
            if (reveal_a->forwardHasNext() || reveal_b->forwardHasNext()) {
                backRound--;
            }
            break;
        case 3:
            cout << "a, b, loss\n";
            tmp_a->print();
            tmp_b->print();
            break;
    }

}


MathOp::CrossEntropy::CrossEntropy() {}

MathOp::CrossEntropy::CrossEntropy(NeuronMat *res, NeuronMat *a, NeuronMat* b) {
    this->res = res;
    this->a = a;
    this->b = b;

    reveal = new Reveal(res->getForward(), a->getForward());
    init(0, 0);
}

void MathOp::CrossEntropy::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            reveal->forward();
            if (reveal->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 2:
            // todo: add secret version
            *res->getForward() = res->getForward()->cross_entropy();
            break;
    }
}

void MathOp::CrossEntropy::back() {
    backRound++;
    // todo
}

MathOp::Similar::Similar() {}

MathOp::Similar::Similar(NeuronMat *res, NeuronMat *a, NeuronMat *b) {
    this->res = res;
    this->a = a;
    this->b = b;
    init(1, 0);
}

void MathOp::Similar::forward() {
    reinit();
    res->getForward()->operator()(0, 0) = a->getForward()->equal(*b->getForward()).count();
}

void MathOp::Similar::back() {}

MathOp::Concat::Concat() {}

MathOp::Concat::Concat(NeuronMat *res, NeuronMat *a, NeuronMat *b) {
    this->res = res;
    this->a = a;
    this->b = b;
    init();
}

void MathOp::Concat::forward() {
    reinit();
    Mat::concat(res->getForward(), a->getForward(), b->getForward());
}

void MathOp::Concat::back() {
    backRound++;
    Mat::reconcat(res->getGrad(), a->getGrad(), !a->getIsBack(), b->getGrad(), !b->getIsBack());
}

MathOp::Hstack::Hstack() {}

MathOp::Hstack::Hstack(NeuronMat *res, NeuronMat *a, NeuronMat *b) {
    this->res = res;
    this->a = a;
    this->b = b;
    init();
}

void MathOp::Hstack::forward() {
    reinit();
    Mat::hstack(res->getForward(), a->getForward(), b->getForward());
}

void MathOp::Hstack::back() {
    backRound++;
    Mat::re_hstack(res->getGrad(), a->getGrad(), !a->getIsBack(), b->getGrad(), !b->getIsBack());
}

MathOp::Vstack::Vstack() {}

MathOp::Vstack::Vstack(NeuronMat *res, NeuronMat *a, NeuronMat *b) {
    this->res = res;
    this->a = a;
    this->b = b;
    init();
}

void MathOp::Vstack::forward() {
    reinit();
    Mat::concat(res->getForward(), a->getForward(), b->getForward());
}

void MathOp::Vstack::back() {
    backRound++;
    Mat::reconcat(res->getBack(), a->getBack(), !a->getIsBack(), b->getBack(), !b->getIsBack());
}

MathOp::Div2mP::Div2mP() {}

MathOp::Div2mP::Div2mP(Mat *res, Mat *a, int k, int m) {
    this->res = res;
    this->a = a;
    this->k = k;
    this->m = m;
    int tmp_r, tmp_c;
    tmp_r = a->rows();
    tmp_c = a->cols();
    r_nd = new Mat(tmp_r, tmp_c);
    r_st = new Mat(tmp_r, tmp_c);
    r_B = new Mat[m];
    for (int i = 0; i < m; i++) {
        r_B[i].init(tmp_r, tmp_c);
    }
    r = new Mat(tmp_r, tmp_c);
    b = new Mat(tmp_r, tmp_c);
    pRandM = new PRandM(r_nd, r_st, r_B, k, m);
    pRevealD = new RevealD(b, a, r);
    init(5, 0);
}

void MathOp::Div2mP::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            // added by curious 2020.6.3, fix bug. each time, the variables need reinit
            if (OFFLINE_PHASE_ON) {
                r_nd->clear();
                r_st->clear();
                for (int i = 0; i < m; i++) {
                    r_B[i].clear();
                }
                r->clear();
                b->clear();
            }
            break;
        case 2:
            // todo: offline
            if (OFFLINE_PHASE_ON) {
                pRandM->forward();
                if (pRandM->forwardHasNext()) {
                    forwardRound--;
                }
            }
            break;
        case 3:
            *r = *r_nd * (1ll << m) + *r_st;
            *r = *a + (1ll << BIT_P_LEN - 1) + *r;
            break;
        case 4:
            pRevealD->forward();
            if (pRevealD->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 5:
            *b = b->mod(1ll << m);
//            b->print();
            *res = (*a - (*b - *r_st)) * Constant::Util::inverse(1ll << m, MOD);
            break;
    }
}

void MathOp::Div2mP::back() {}

void MathOp::Div2mP::reset_for_multi_call() {
    reset();
    pRandM->reset_for_multi_call();
    pRevealD->reset_for_multi_call();
}

MathOp::Reveal::Reveal() {}

MathOp::Reveal::Reveal(Mat *res, Mat *a) {
    this->res = res;
    this->a = a;
    int tmp_r, tmp_c;
    tmp_r = a->rows();
    tmp_c = a->cols();
    b = new Mat(tmp_r, tmp_c);
    init(2, 0);
}

///TODO: add reed_solomn reconstruct
void MathOp::Reveal::forward() {
    reinit();
    Mat tmp[M];
    switch (forwardRound) {
        case 1:
            *b = *a * player[node_type].lagrange;
            broadcase_rep(b);
            break;
        case 2:
            *b = *a * player[node_type].lagrange;
            receive_add(b);
            *res = *b;
            break;
    }
}

void MathOp::Reveal::back() {}

MathOp::PRandM::PRandM() {}

MathOp::PRandM::PRandM(Mat *r_nd, Mat *r_st, Mat *b_B, int k, int m) {
    PRandM_init(r_nd, r_st, b_B, k, m);
}

MathOp::PRandM::PRandM(Mat *r_nd, Mat *r_st, int k, int m) {
    int tmp_r, tmp_c;
    tmp_r = r_nd->rows();
    tmp_c = r_nd->cols();
    b_B = new Mat[m];
    for (int i = 0; i < m; i++) {
        b_B[i].init(tmp_r, tmp_c);
    }
    PRandM_init(r_nd, r_st, b_B, k, m);
}

MathOp::PRandM::PRandM(int r, int c, int k, int m) {
    r_nd = new Mat(r, c);
    r_st = new Mat(r, c);
    b_B = new Mat[m];
    for (int i = 0; i < m; i++) {
        b_B[i].init(r, c);
    }
    PRandM_init(r_nd, r_st, b_B, k, m);
}

void MathOp::PRandM::PRandM_init(Mat *r_nd, Mat *r_st, Mat *b_B, int k, int m) {
    this->r_nd = r_nd;
    this->r_st = r_st;
    this->b_B = b_B;
    this->k = k;
    this->m = m;
    pRandFld = new PRandFld(r_nd, 1ll << (k - m)); // 这里少了括号 (k-m)
    pRandBit = new PRandBit *[m];
    for (int i = 0; i < m; i++) {
        pRandBit[i] = new PRandBit(b_B+i);
    }
    init(4, 0);
//    tmp = new Mat(r_nd->rows(), r_nd->cols());
//    tmp1 = new Mat(r_nd->rows(), r_nd->cols());
//    reveal = new Reveal(tmp, r_nd);
//    reveal1 = new Reveal(tmp1, r_st);
}

void MathOp::PRandM::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            r_st->clear();
            break;
        case 2:
            pRandFld->forward();
            if (pRandFld->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 3:
            for (int i = 0; i < m; i++) {
                pRandBit[i]->forward();
            }
            for (int i = 0; i < m; i++) {
                if (pRandBit[i]->forwardHasNext()) {
                    forwardRound--;
                    break;
                }
            }
            break;
        case 4:
            for (int i = 0; i < m; i++) {
                *r_st = *r_st + b_B[i] * (1ll << i);
            }
            break;
//        case 4:
//            reveal->forward();
//            if (reveal->forwardHasNext()) {
//                reveal->forward();
//            }
//            break;
//        case 5:
//            cout << "r_nd: " <<endl;
//            tmp->print();
//            break;
//        case 6:
//            reveal1->forward();
//            if (reveal1->forwardHasNext()) {
//                reveal1->forward();
//            }
//            break;
//        case 7:
//            cout << "r_st: " <<endl;
//            tmp1->print();
//            break;
    }
}

void MathOp::PRandM::back() {}

void MathOp::PRandM::reset_for_multi_call() {
    reset();
    pRandFld->reset();
    for (int i = 0; i < m; ++i) {
        pRandBit[i]->reset_for_multi_call();
    }
}

MathOp::PRandBit::PRandBit() {}

MathOp::PRandBit::PRandBit(Mat *res) {
    this->res = res;
    int tmp_r, tmp_c;
    tmp_r = res->rows();
    tmp_c = res->cols();
    a = new Mat(tmp_r, tmp_c);
    a_r = new Mat(1, tmp_r * tmp_c + REDUNDANCY);
    a2 = new Mat(tmp_r, tmp_c);
    a2_r = new Mat(1, tmp_r * tmp_c + REDUNDANCY);
    pRandFld = new PRandFld(a_r, MOD);
    mulPub = new MulPub(a2_r, a_r, a_r);
    init(3, 0);
}

void MathOp::PRandBit::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            pRandFld->forward();
            if (pRandFld->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 2:
            mulPub->forward();
            if (mulPub->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 3:
            if (a2_r->count() > REDUNDANCY) {
                forwardRound = 0;
                break;
            }
            Mat::fill(a2, a2_r, a, a_r);
            *a2 = a2->sqrt_inv();
            *a2 = a2->dot(*a) + 1;
            *res = a2->divideBy2();
            break;
    }
}

void MathOp::PRandBit::back() {}

void MathOp::PRandBit::reset_for_multi_call() {
    reset();
    pRandFld->reset();
    mulPub->reset();
}

MathOp::MulPub::MulPub() {}

MathOp::MulPub::MulPub(Mat *res, Mat *a, Mat *b) {
    this->res = res;
    this->a = a;
    this->b = b;
    init(2, 0);
}

void MathOp::MulPub::forward() {
    reinit();
    Mat tmp[M];

    switch (forwardRound) {
        case 1:
            *res = a->dot(*b);
            *res = *res * player[node_type].lagrange;
            for (int i = 0; i < M; i++) {
                if (i != node_type) {
                    tmp[i] = *res;
                }
            }
            broadcast(tmp);
            break;
        case 2:
            receive(tmp);
            for (int i = 0; i < M; i++) {
                if (i != node_type) {
                    *res = *res + tmp[i];
                }
            }

    }
}

void MathOp::MulPub::back() {}

MathOp::PRandFld::PRandFld() {}

MathOp::PRandFld::PRandFld(Mat *res, ll range) {
    this->res = res;
    this->range = range;
    init(2, 0);
}

void MathOp::PRandFld::forward() {
    reinit();
    Mat a[M];
    switch (forwardRound) {
        case 1:
            int r, c;
            r = res->rows();
            c = res->cols();
            for (int i = 0; i < M; i++) {
                a[i].init(r, c);
            }
            random(a, range);
            *res = a[node_type];
            broadcast(a);
            break;
        case 2:
            receive(a);
            for (int i = 0; i < M; i++) {
                if (i != node_type) {
                    *res = *res + a[i];
                }
            }
            break;
    }
}

void MathOp::PRandFld::back() {}

MathOp::Mod2::Mod2() {}

MathOp::Mod2::Mod2(Mat *res, Mat *a, int k) {
    this->res = res;
    this->a = a;
    this->k = k;
    int tmp_r, tmp_c;
    tmp_r = a->rows();
    tmp_c = a->cols();
    r_nd = new Mat(tmp_r, tmp_c);
    r_st = new Mat(tmp_r, tmp_c);
    r_B = new Mat[1];
    r_B[0].init(tmp_r, tmp_c);
    c = new Mat(tmp_r, tmp_c);
    pRandM = new PRandM(r_nd, r_st, r_B, k, 1);
    reveal = new Reveal(c, c);
    init(5, 0);
}

void MathOp::Mod2::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            r_nd->clear();
            r_st->clear();
            r_B[0].clear();
            c->clear();
            // todo: offline
            break;
        case 2:
            if (OFFLINE_PHASE_ON) {
                pRandM->forward();
                if (pRandM->forwardHasNext()) {
                    forwardRound--;
                }
            }
            break;
        case 3:
            *c = *a + (1ll << BIT_P_LEN - 1) + (*r_nd * 2) + *r_st;
            break;
        case 4:
            reveal->forward();
            if (reveal->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 5:
            *res = (*c & 1) + *r_st - (*c & 1).dot(*r_st) * 2;
            r_nd->clear();
            r_st->clear();
            r_B[0].clear();
            c->clear();
            break;
    }
}

void MathOp::Mod2::back() {}

MathOp::DegRed::DegRed() {}

MathOp::DegRed::DegRed(Mat *res, Mat *a) {
    this->res = res;
    this->a = a;
    tmp = new Mat[M];
    init(2, 0);
}

void MathOp::DegRed::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            for (int i = 0; i < M; i++) {
                if (i == node_type) {
                    continue;
                }
                tmp[i] = *a * metadata(node_type, i);
            }
            broadcast(tmp);
            break;
        case 2:
            *res = *a * metadata(node_type, node_type);
            receive(tmp);
            for (int i = 0; i < M; i++) {
                if (i != node_type) {
                    *res = *res + tmp[i];
                }
            }
    }
}

void MathOp::DegRed::back() {}

MathOp::PreMulC::PreMulC() {}

MathOp::PreMulC::PreMulC(Mat *res, Mat *a, int k) {
    this->res = res;
    this->a = a;
    this->k = k;
    int tmp_r, tmp_c;
    tmp_r = a[0].rows();
    tmp_c = a[0].cols();
    r = new Mat[k];
    s = new Mat[k];
    u = new Mat[k];
    for (int i = 0; i < k; i++) {
        r[i].init(tmp_r, tmp_c);
        s[i].init(tmp_r, tmp_c);
        u[i].init(tmp_r, tmp_c);
    }
    pRandFld_r = new PRandFld *[k];
    pRandFld_s = new PRandFld *[k];
    pMulPub_st = new MulPub *[k];
    pDegRed = new DegRed *[k];
    for (int i = 0; i < k; i++) {
        pRandFld_r[i] = new PRandFld(r+i, MOD);
        pRandFld_s[i] = new PRandFld(s+i, MOD);
        pMulPub_st[i] = new MulPub(u+i, r+i, s+i);
    }

    m = new Mat[k];
    w = new Mat[k];
    z = new Mat[k];
    for (int i = 0; i < k; i++) {
        m[i].init(tmp_r, tmp_c);
        w[i].init(tmp_r, tmp_c);
        z[i].init(tmp_r, tmp_c);
    }
    for (int i = 1; i < k; i++) {
        pDegRed[i] = new DegRed(w+i, w+i);
    }
    pMulPub = new MulPub *[k];
    for (int i = 0; i < k; i++) {
        pMulPub[i] = new MulPub(m+i, w+i, a+i);
    }
    init(7, 0);
}

void MathOp::PreMulC::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
             if (OFFLINE_PHASE_ON) {
                for (int i = 0; i < k; i++) {
                    pRandFld_r[i]->forward();
                    pRandFld_s[i]->forward();
                }
                for (int i = 0; i < k; i++) {
                    if (pRandFld_r[i]->forwardHasNext() || pRandFld_s[i]->forwardHasNext()) {
                        forwardRound--;
                        break;
                    }
                }
            }
            break;
        case 2:
            if (OFFLINE_PHASE_ON) {
                for (int i = 0; i < k; i++) {
                    pMulPub_st[i]->forward();
                }
                for (int i = 0; i < k; i++) {
                    if (pMulPub_st[i]->forwardHasNext()) {
                        forwardRound--;
                        break;
                    }
                }
            }
            break;
        case 3:
            w[0] = r[0];
            for (int i = 1; i < k; i++) {
                w[i] = r[i].dot(s[i-1]);
            }
            break;
        case 4:
            for (int i = 1; i < k; i++) {
                pDegRed[i]->forward();
                if (pDegRed[i]->forwardHasNext()) {
                    forwardRound--;
                    break;
                }
            }
            break;
        case 5:
            {
                // optimization: make use of memory localization
                Mat inverse_tmp [k];
                std::vector<std::thread> thrds;
                int thread_num = THREAD_NUM;
                int seg_len = ceil(k * 1.0 / thread_num);
                for (int i = 0; i < thread_num; i++) {
                    thrds.emplace_back(std::thread([this, i, seg_len, &inverse_tmp]() {
                        for (int j = i*seg_len; j < (i+1)*seg_len && j < k; j++) {
                            inverse_tmp[j] = u[j].inverse();
                        }
                    }));
                }
                for (auto& t: thrds) t.join();

                for (int i = 1; i < k; i++) {
                    w[i] = w[i].dot(inverse_tmp[i-1]);
                }
                for (int i = 0; i < k; i++) {
                    z[i] = s[i].dot(inverse_tmp[i]);
                }
            }
            break;
        case 6:
            for (int i = 0; i < k; i++) {
                pMulPub[i]->forward();
            }
            for (int i = 0; i < k; i++) {
                if (pMulPub[i]->forwardHasNext()) {
                    forwardRound--;
                    break;
                }
            }
            break;
        case 7:
            res[0] = a[0];
            for (int i = 1; i < k; i++) {
                m[i] = m[i].dot(m[i-1]);
            }
            for (int i = 1; i < k; i++) {
                res[i] = m[i].dot(z[i]);
            }
            break;
    }
}

void MathOp::PreMulC::back() {}

MathOp::BitLT::BitLT() {}

MathOp::BitLT::BitLT(Mat *res, Mat *a, Mat *b_B, int k) {
    this->res = res;
    this->a = a;
    this->b_B = b_B;
    this->k = k;
    int tmp_r, tmp_c;
    tmp_r = a->rows();
    tmp_c = a->cols();
    d_B = new Mat[k];
    p_B = new Mat[k];
    for (int i = 0; i < k; i++) {
        d_B[i].init(tmp_r, tmp_c);
        p_B[i].init(tmp_r, tmp_c);
    }
    d_B_inverse = new Mat[k];
    p_B_inverse = new Mat[k];
    for (int i = 0; i < k; i++) {
        d_B_inverse[i].init(tmp_r, tmp_c);
        p_B_inverse[i].init(tmp_r, tmp_c);
    }
    s = new Mat(tmp_r, tmp_c);
    preMulC = new PreMulC(p_B, d_B_inverse, k);
    pMod2 = new Mod2(res, s, k);
    init(4, 0);
}

void MathOp::BitLT::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            for (int i = 0; i < k; i++) {
                d_B[i] = a->get_bit(i) + b_B[i] - a->get_bit(i).dot(b_B[i]) * 2 + 1; // d_B[k-1-i] -> d_B[i]
            }
            // for (int i = 0; i < k; i++) {
            //     d_B[i] = d_B[i] + 1;
            // }

            // inverse input, considering SufMul
            for (int j = 0; j < k; ++j) {
                d_B_inverse[j] = d_B[k - 1 - j];
            }
            break;
        case 2:
            preMulC->forward();
            if (preMulC->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 3:
            // inverse output, considering SufMul
            for (int j = 0; j < k; ++j) {
                p_B_inverse[j] = p_B[k - 1 - j];
            }
            *s = a->get_bit(k-1).oneMinus().dot(d_B[k-1]-1);
            for (int i = 0; i < k-1; i++) {
                *s = *s + a->get_bit(i).oneMinus().dot(p_B_inverse[i] - p_B_inverse[i+1]);
            }
            break;
        case 4:
            pMod2->forward();
            if (pMod2->forwardHasNext()) {
                forwardRound--;
            }
            break;
    }
}

void MathOp::BitLT::back() {}

MathOp::RevealD::RevealD() {}

MathOp::RevealD::RevealD(Mat *res, Mat *a, Mat *b) {
    this->res = res;
    this->a = a;
    this->b = b;
    pReveal = new Reveal(res, b);
    pDegRed = new DegRed(a, a);
    init(1, 0);
}

MathOp::RevealD::RevealD(Mat *res, Mat *a) {
    this->res = res;
    this->a = a;
    pReveal = new Reveal(res, a);
    init(1, 0);
}

void MathOp::RevealD::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            pDegRed->forward();
            pReveal->forward();
            if (pReveal->forwardHasNext() || pDegRed->forwardHasNext()) {
                forwardRound--;
            }
            break;
    }
}

void MathOp::RevealD::back() {}

void MathOp::RevealD::reset_for_multi_call() {
    reset();
    pReveal->reset();
    pDegRed->reset();
}

MathOp::Mod2m::Mod2m() {}

MathOp::Mod2m::Mod2m(Mat *res, Mat *a, int k, int m) {
    this->res = res;
    this->a = a;
    this->k = k;
    this->m = m;
    int tmp_r, tmp_c;
    tmp_r = a->rows();
    tmp_c = a->cols();
    r_nd = new Mat(tmp_r, tmp_c);
    r_st = new Mat(tmp_r, tmp_c);
    r_B = new Mat[m];
    for (int i = 0; i < m; i++) {
        r_B[i].init(tmp_r, tmp_c);
    }
    b = new Mat(tmp_r, tmp_c);
    u = new Mat(tmp_r, tmp_c);
    pRandM = new PRandM(r_nd, r_st, r_B, k, m);
    pRevealD = new RevealD(b, a, b);
    pBitLT = new BitLT(u, b, r_B, m);
    init(6, 0);
}

void MathOp::Mod2m::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
//            r_nd->clear();
//            r_st->clear();
//            for (int i = 0; i < m; i++) {
//                r_B[i].clear();
//            }
//            u->clear();
//            b->clear();
            if (OFFLINE_PHASE_ON) {
                pRandM->forward();
                if (pRandM->forwardHasNext()) {
                    forwardRound--;
                }
            }
            break;
        case 2:
            // todo: m+r > q 的问题
            *b = *a + (1ll << (BIT_P_LEN - 1)) + (*r_nd) * (1ll << m) + (*r_st);
            break;
        case 3:
            pRevealD->forward();
            if (pRevealD->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 4:
//            cout << "Test Mod2m\n";
//            b->print();
            *b = b->mod(1ll << m);
//            b->print();
            break;
        case 5:
            pBitLT->forward();
            if (pBitLT->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 6:
            *res = *b - (*r_st) + (*u) * (1ll << m);
            r_nd->clear();
            r_st->clear();
            for (int i = 0; i < m; i++) {
                r_B[i].clear();
            }
            u->clear();
            b->clear();
            break;
    }
}

void MathOp::Mod2m::back() {}

MathOp::Div2m::Div2m() {}

MathOp::Div2m::Div2m(Mat *res, Mat *a, int k, int m) {
    this->res = res;
    this->a = a;
    this->k = k;
    this->m = m;
    int tmp_r, tmp_c;
    tmp_r = a->rows();
    tmp_c = a->cols();
    b = new Mat(tmp_r, tmp_c);
    pMod2m = new Mod2m(b, a, k, m);
    init(2, 0);
}

void MathOp::Div2m::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            pMod2m->forward();
            if (pMod2m->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 2:
            *res = (*a - *b) * Constant::Util::inverse(1ll << m, MOD); // Div2mp 需要降次
            b->clear();
            break;
    }
}

void MathOp::Div2m::back() {}

MathOp::LTZ::LTZ() {}

MathOp::LTZ::LTZ(Mat *res, Mat *a, int k) {
    this->res = res;
    this->a = a;
    this->k = k;
    pDiv2m = new Div2m(res, a, k, k-1);
    reveal = new Reveal(res, a);
    init(2, 0);
}

void MathOp::LTZ::forward() {
    reinit();
    switch (forwardRound) {
       case 1:
            pDiv2m->forward();
            if (pDiv2m->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 2:
            // return IE, 0
            *res = res->opposite() * IE;
            // return 1, 0
            // *res = res->opposite();
            break;
    }
}

//MathOp::LTZ::LTZ(Mat *res, Mat *a, int k) {
//    this->res = res;
//    this->a = a;
//    this->k = k;
//    reveal = new Reveal(res, a);
//    init(2, 0);
//}
//
//void MathOp::LTZ::forward() {
//    reinit();
//    switch (forwardRound) {
//        case 1:
//            reveal->forward();
//            if (reveal->forwardHasNext()) {
//                forwardRound--;
//            }
//            break;
//        case 2:
//            *res = res->opposite();
//            *res = res->oneMinus_IE();
//            *res = res->LTZ();
//            break;
//    }
//}

void MathOp::LTZ::back() {}



MathOp::EQZ::EQZ() {}

MathOp::EQZ::EQZ(Mat* res, Mat *a, int k) {
    this->res = res;
    this->a = a;
    this->k = k;

    int tmp_r, tmp_c;
    tmp_r = a->rows();
    tmp_c = a->cols();
    r_nd = new Mat(tmp_r, tmp_c);
    r_st = new Mat(tmp_r, tmp_c);
    r_B = new Mat[k];
    for (int i = 0; i < k; i++) {
        r_B[i].init(tmp_r, tmp_c);
    }
    b = new Mat(tmp_r, tmp_c);
    u = new Mat(tmp_r, tmp_c);

    d_B = new Mat[k];
    for (int i = 0; i < k; i++) {
        d_B[i].init(tmp_r, tmp_c);
    }
    tmp1 = new Mat(tmp_r, tmp_c);
    reveal_tmp = new Reveal(tmp1, a);
    pRandM = new PRandM(r_nd, r_st, r_B, k, k);
    pReveal = new Reveal(b, b);
    kOrCl = new KOrCL(res, d_B, k);
    init(6, 0);
}

void MathOp::EQZ::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            if (OFFLINE_PHASE_ON) {
                pRandM->forward();
                if (pRandM->forwardHasNext()) {
                    forwardRound--;
                }
            }
            break;
        case 2:
            // todo: c'=r' mod 2^k
            *b = *a + (1ll << (BIT_P_LEN)) + (*r_nd) * (1ll << k) + (*r_st); // 2^(k-1) mod 2^k 存在问题
            break;
        case 3:
            pReveal->forward();
            reveal_tmp->forward();
            if (pReveal->forwardHasNext() || reveal_tmp->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 4:
            cout << "a\n";
            tmp1->print();
            for (int i = 0; i < k; i++) {
                d_B[i] = b->get_bit(i) + r_B[i] - b->get_bit(i).dot(r_B[i]) * 2; // d_B[k-1-i] -> d_B[i]
            }
            break;
        case 5:
            kOrCl->forward();
            if (kOrCl->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 6:
            *res = res->oneMinus();
            r_nd->clear();
            r_st->clear();
            for (int i = 0; i < k; i++) {
                r_B[i].clear();
            }
            break;
    }
}

void MathOp::EQZ::back() {}

MathOp::EQZ_2LTZ::EQZ_2LTZ() {}

MathOp::EQZ_2LTZ::EQZ_2LTZ(Mat* res, Mat *a, int k) {
    this->res = res;
    this->a = a;
    this->k = k;
    int tmp_r, tmp_c;
    tmp_r = a->rows();
    tmp_c = a->cols();

    u_st = new Mat(tmp_r, tmp_c);
    u_st_res = new Mat(tmp_r, tmp_c);
    u_nd = new Mat(tmp_r, tmp_c);
    u_nd_res = new Mat(tmp_r, tmp_c);
    pLTZ_f_1 = new LTZ(u_st_res, u_st, BIT_P_LEN);
    pLTZ_f_2 = new LTZ(u_nd_res, u_nd, BIT_P_LEN);
    reveal = new Reveal(res, res);
    reveal_1 = new Reveal(u_st_res, u_st_res);
    reveal_2 = new Reveal(u_nd_res, u_nd_res);
    init(4, 0);
}

void MathOp::EQZ_2LTZ::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            *u_st = *a;
            *u_nd = a->opposite();
            break;
        case 2:
            pLTZ_f_1->forward();
            pLTZ_f_2->forward();
            if (pLTZ_f_1->forwardHasNext() || pLTZ_f_2->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 3:
            // added by curious 2020.6.4 -1 --> -IE
            // modified by curious 2020.8.27 IE- --> 1-, 目的是不需要Degree Reduction
            *u_st_res = u_st_res->oneMinus();
            *u_nd_res = u_nd_res->oneMinus();
            *res = u_st_res->dot(*u_nd_res);
            break;
        case 4:
            reveal->forward();
            reveal_1->forward();
            reveal_2->forward();
            if (reveal->forwardHasNext() || reveal_1->forwardHasNext() || reveal_2->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 5:
            cout << "EQZ_2LTZ\n";
            u_st_res->print();
            u_nd_res->print();
            res->print();
            break;
    }
}

void MathOp::EQZ_2LTZ::back() {}

MathOp::ReLU_Mat::ReLU_Mat() {}

MathOp::ReLU_Mat::ReLU_Mat(NeuronMat *res, NeuronMat *a) {
    this->res = res;
    this->a = a;
    int tmp_r, tmp_c;
    tmp_r = a->rows();
    tmp_c = a->cols();
    res->setAux(new Mat(tmp_r, tmp_c));
    pLTZ = new LTZ(res->getAux(), a->getForward(), BIT_P_LEN);
//    pDegRed_f = new DegRed(res->getForward(), res->getForward());
//    pDegRed_b = new DegRed(a->getGrad(), a->getGrad());
    pDiv2mp_f = new Div2mP(res->getForward(), res->getForward(), BIT_P_LEN, DECIMAL_PLACES);
    pDiv2mp_b = new Div2mP(a->getGrad(), a->getGrad(), BIT_P_LEN, DECIMAL_PLACES);

    tmp_a = new Mat(tmp_r, tmp_c);
    tmp_res = new Mat(tmp_r, tmp_c);
    reveal_a = new Reveal(tmp_a, a->getForward());
    reveal_res = new Reveal(tmp_res, res->getForward());
    init(6, 2);
}

void MathOp::ReLU_Mat::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            pLTZ->forward();
            if (pLTZ->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 2:
            *res->getAux() = res->getAux()->oneMinus_IE();
            break;
        case 3:
            *res->getForward() = a->getForward()->dot(*res->getAux());
            break;
        case 4:
            // pDegRed_f->forward();
            // if (pDegRed_f->forwardHasNext()) {
            //     forwardRound--;
            // }
            pDiv2mp_f->forward();
            if (pDiv2mp_f->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 5:
            reveal_a->forward();
            reveal_res->forward();
            if (reveal_a->forwardHasNext() || reveal_res->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 6:
            cout << "a---\n";
            tmp_a->print();
            cout << "res---\n";
            tmp_res->print();
            break;
    }
}

void MathOp::ReLU_Mat::back() {
    backRound++;
    switch (backRound) {
        case 1:
            // modified to Leakey ReLU, curious 10.26
            *a->getGrad() = res->getGrad()->dot(*res->getAux()) + res->getAux()->oneMinus_IE() * LEAKEY_RELU_BIAS;
            break;
        case 2:
            // pDegRed_b->back();
            // if (pDegRed_b->backHasNext()) {
            //     backRound--;
            // }
            pDiv2mp_b->back();
            if (pDiv2mp_b->backHasNext()) {
                backRound--;
            }
            break;
    }
}

MathOp::Sigmoid_Mat::Sigmoid_Mat() {}

MathOp::Sigmoid_Mat::Sigmoid_Mat(NeuronMat *res, NeuronMat *a) {
    this->res = res;
    this->a = a;
    int tmp_r, tmp_c;
    tmp_r = a->rows();
    tmp_c = a->cols();
    res->setAux(new Mat(tmp_r, tmp_c));
    u_st = new Mat(tmp_r, tmp_c);
    u_st_res = new Mat(tmp_r, tmp_c);
    u_nd = new Mat(tmp_r, tmp_c);
    u_nd_res = new Mat(tmp_r, tmp_c);
    pLTZ_f_1 = new LTZ(u_st_res, u_st, BIT_P_LEN);
    pLTZ_f_2 = new LTZ(u_nd_res, u_nd, BIT_P_LEN);
//    pDegRed_res = new DegRed(res->getForward(), res->getForward());
//    pDegRed_res = new Div2mP(res->getForward(), res->getForward(), BIT_P_LEN, DECIMAL_PLACES);
//    pDiv2mP_a = new Div2mP(a->getBack(), a->getBack(), BIT_P_LEN, DECIMAL_PLACES);
//    pDiv2mP_aux = new Div2mP(res->getAux(), res->getAux(), BIT_P_LEN, DECIMAL_PLACES);
    pDegRed_res = new Div2mP(res->getForward(), res->getForward(), BIT_P_LEN, DECIMAL_PLACES);
    pDiv2mP_b1 = new Div2mP(a->getGrad(), a->getGrad(), BIT_P_LEN, DECIMAL_PLACES);
    pDiv2mP_b2 = new Div2mP(a->getGrad(), a->getGrad(), BIT_P_LEN, DECIMAL_PLACES);

    tmp = new Mat(tmp_r, tmp_c);
    reveal = new Reveal(tmp, u_st_res);
    init(5, 1);
}
//
void MathOp::Sigmoid_Mat::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            *u_st = *a->getForward() + (IE >> 1);
            *u_nd = *a->getForward() - (IE >> 1);
            break;
        case 2:
            pLTZ_f_1->forward();
            pLTZ_f_2->forward();
            if (pLTZ_f_1->forwardHasNext() || pLTZ_f_2->forwardHasNext()) {
                forwardRound--;
            }
            break;
        case 3:
            // pLTZ_f_2->forward();
            // if (pLTZ_f_2->forwardHasNext()) {
            //     forwardRound--;
            // }
            break;
        case 4:
            // added by curious 2020.6.4 -1 --> -IE
            *u_st = u_st_res->oneMinus_IE();
            *u_nd = u_nd_res->oneMinus_IE();
            *res->getForward() = (*a->getForward() + (IE >> 1)).dot(*u_st) - (*a->getForward() - (IE >> 1)).dot(*u_nd);
//            *res->getAux() = a->getForward()->dot(a->getForward()->oneMinus_IE());
            break;
        case 5:
            pDegRed_res->forward();
            if (pDegRed_res->forwardHasNext()) {
                forwardRound--;
            }
            break;
        // case 5:
        //     reveal->forward();
        //     if (reveal->forwardHasNext()) {
        //         forwardRound--;
        //     }
        //     break;
        // case 6:
        //     cout << "tmp\n";
        //     tmp->print();
        //     break;
    }
}

void MathOp::Sigmoid_Mat::back() {
    backRound++;
    switch (backRound) {
        case 1:
            // Case: Cross Entropy + Sigmoid, the backward gradient can be simplified to (y_out - y)
            *a->getGrad() = *res->getGrad();
            break;
//         case 1:
//             *a->getBack() = res->getBack()->dot(*res->getAux());
//             *a->getGrad() = res->getForward()->dot(res->getForward()->oneMinus_IE());
//             break;
        case 2:
            pDiv2mP_b1->forward();
            if (pDiv2mP_b1->forwardHasNext()) {
                backRound--;
            }
            break;
        case 3:
            *a->getGrad()=res->getGrad()->dot(*a->getGrad());
            break;
        case 4:
            pDiv2mP_b2->forward();
            if (pDiv2mP_b2->forwardHasNext()) {
                backRound--;
            }
            break;
    }
}

MathOp::Argmax::Argmax() {}

MathOp::Argmax::Argmax(NeuronMat *res, NeuronMat *a) {
    this->res = res;
    this->a = a;
    init(1, 0);
}

void MathOp::Argmax::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            *res->getForward() = a->getForward()->argmax();
            break;
    }
}

void MathOp::Argmax::back() {}

MathOp::Equal::Equal() {}

MathOp::Equal::Equal(NeuronMat *res, NeuronMat *a, NeuronMat *b) {
    this->res = res;
    this->a = a;
    this->b = b;
    init(1, 0);
}

void MathOp::Equal::forward() {
    reinit();
    switch (forwardRound) {
        case 1:
            (*res->getForward())(0, 0) = a->getForward()->eq(*b->getForward()).count();
            break;
    }
}

void MathOp::Equal::back() {}

void MathOp::broadcast(Mat *a) {
    for (int i = 0; i < M; i++) {
        if (i != node_type) {
            socket_io[node_type][i]->send_message(a[i]); 
        }
    }
}
void MathOp::broadcast_share(Mat *a, int target) {
    socket_io[node_type][target]->send_message(a);
}

void MathOp::receive_share(Mat* a, int from) {
    socket_io[node_type][from]->recv_message(*a);
}

void MathOp::broadcase_rep(Mat *a) {
    for (int i = 0; i < M; i++) {
        if (i != node_type) {
            socket_io[node_type][i]->send_message(a);
        }
    }
}

void MathOp::receive(Mat* a) {
    for (int i = 0; i < M; i++) {
        if (i != node_type) {
            a[i] = socket_io[node_type][i]->recv_message();
        }
    }
}

void MathOp::receive_add(Mat *a) {
    for (int i = 0; i < M; i++) {
        if (i != node_type) {
            socket_io[node_type][i]->recv_message(a);
        }
    }
}

void MathOp::receive_rep(Mat *a) {
    for (int i = 0; i < M; i++) {
        if (i != node_type) {
            socket_io[node_type][i]->recv_message(*(a+i));
        }
    }
}

void MathOp::random(Mat *a, ll range) {
    int len = a[0].rows() * a[0].cols();
    ll128 coefficient[TN];
    for (int i = 0; i < len; i++) {
        coefficient[0] = (Constant::Util::randomlong() % range);
        for (int j = 1; j < TN; j++) {
            coefficient[j] = Constant::Util::randomlong();
        }
        for (int j = 0; j < M; j++) {
            ll128 tmp = coefficient[0];
            ll128 key = player[j].key;
            for (int k = 1; k < TN; k++) {
                tmp += coefficient[k] * key;
                key *= player[j].key;
                key = Constant::Util::get_residual(key);
            }
            a[j].getVal(i) = tmp;
        }
    }
}


