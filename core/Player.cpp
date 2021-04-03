//
// Created by tangdingyi on 2019/12/26.
//

#include "Player.h"

Player player[M];
/// Degree Reduction, TODO: modify, 现在的方式会导致安全风险
Mat metadata(M, M);

Player::Player() {}

Player::Player(int id, ll128 key, ll128 lagrange) {
    this->id = id;
    this->key = key;
    /// lagrange polynomial coefficients， TODO： modify for reed_solomn reconstruct
    this->lagrange = lagrange;
}

void Player::init() {
    ll128 key[M], lagrange[M];
    Mat vandermonde(M, M), vandermonde_inv(M, M), des(M, M), pol(M, M), van_inv2(TN, TN), van2(TN, M);
    for (int i = 0; i < M; i++) {
        key[i] = Constant::Util::randomlong();
        key[i] = i + 2;
        vandermonde(i, 1) = key[i];
        vandermonde(i, 0) = 1;
//        DBGprint("%d ", i);
        for (int j = 2; j < M; j++) {
//            vandermonde(i, j) = mul(vandermonde(i, j - 1), key[i]);
            vandermonde(i, j) = vandermonde(i, j-1) * key[i];
//            vandermonde(i, j).residual();
            vandermonde(i, j) = Constant::Util::get_residual(vandermonde(i, j));
        }
    }
    DBGprint("init midway\n");
    vandermonde.print();
    for (int i = 0; i < M; i++) {
        lagrange[i] = 1;
        for (int j = 0; j < M; j++)
            if (j != i) {
//                lagrange[i] = mul(lagrange[i], key[j]);
                lagrange[i] = lagrange[i] * key[j];
//                lagrange[i].residual();
                lagrange[i] = Constant::Util::get_residual(lagrange[i]);
//                lagrange[i] = mul(lagrange[i], inverse(sub(key[j], key[i]), MOD));
                ll128 tmp = key[j] - key[i];
//                tmp.residual();
                tmp = Constant::Util::get_residual(tmp);
                lagrange[i] = lagrange[i] * Constant::Util::inverse(tmp, MOD);
//                lagrange[i].residual();
                lagrange[i] = Constant::Util::get_residual(lagrange[i]);
            }
    }

    for (int i = 0; i < M; i++) {
        player[i].id = i;
        player[i].key = key[i];
        player[i].lagrange = lagrange[i];
    }
    for (int i = 0; i < M; i++) {
        DBGprint("%d, key: %lld, %lld\n", player[i].id, (ll)player[i].key, (ll)player[i].lagrange);
    }
    for (int i = 0; i < M; i++) {
        ll128 d;
        d = 1;
        for (int j = 0; j < M; j++)
            if (j != i) {
//                d = mul(d, key[j] - key[i]);
                d = d * (key[j] - key[i]);
//                d.residual();
                d = Constant::Util::get_residual(d);
            }
//        d = inverse(d, MOD);
        d = Constant::Util::inverse(d, MOD);
        //printf("%I64d ", d);
        for (int j = 0; j < M; j++) {
//            vandermonde_inv(i, j) = mul(power(-1, j), cal_perm(key, M-j-1, i));
            vandermonde_inv(i, j) = Constant::Util::power(-1, j) * Constant::Util::cal_perm(key, M-j-1, i);
//            vandermonde_inv(i, j).residual();
            vandermonde_inv(i, j) = Constant::Util::get_residual(vandermonde_inv(i, j));
//            vandermonde_inv(i, j) = mul(vandermonde_inv(i, j), d);
            vandermonde_inv(i, j) = vandermonde_inv(i, j) * d;
//            vandermonde_inv(i, j).residual();
            vandermonde_inv(i, j) = Constant::Util::get_residual(vandermonde_inv(i, j));
        }
    }
    {
        ll128 d;
        d = player[1].key - player[0].key;
        d = Constant::Util::inverse(d, MOD);
        van_inv2(0, 0) = player[1].key;
        van_inv2(1, 1) = 1;
        van_inv2(0, 1) = -1;
        van_inv2(1, 0) = 0 - player[0].key;
        for (int i = 0; i < TN; i++)
            for (int j = 0; j < TN; j++) {
                van_inv2(i, j) = van_inv2(i, j) * d;
//                van_inv2(i, j).residual();
                van_inv2(i, j) = Constant::Util::get_residual(van_inv2(i, j));
            }
    }

//    van2(0, 0) = 1;
//    van2(0, 1) = 1;
//    van2(1, 0) = player[0].key;
//    van2(1, 1) = player[1].key;
    for (int i = 0; i < M; i++) {
        van2(0, i) = 1;
        van2(1, i) = player[i].key;
    }
//    van2.print();
//    van_inv2.print();
    for (int i = 0; i < TN; i++)
        des(i, i) = 1;
    vandermonde = vandermonde.transpose();
    pol = vandermonde_inv * des * vandermonde;
    metadata = pol;
//    metadata2 = van_inv2 * van2;
//    metadata2.print();
//    for (int i = 0; i < M; i++) {
//        DBGprint("%I64d ", player[i].lagrange);
//    }
//    for (int i = 0; i < M; i++) {
//        DBGprint("%d, %d, %lld ", player[i].id, player[i].key, (ll)player[i].lagrange);
//    }
//    metadata.print();
    DBGprint("\n");



//    for (int i = 1; i <= BIT_LENGTH + 1; i++) {
//        if (i == 2)
//            continue;
//        denominator = denominator * (i-2);
//        denominator.residual();
//    }
//    denominator.print();
//    DBGprint("\n");
//    init_lagrange(BIT_LENGTH);
//    for (int i = 1; i < BIT_LENGTH; i++)
//        init_lagrange(i);
//    for (int i = 0; i <= BIT_LENGTH; i++) {
//        lagrange_co(0, i).print();
//        DBGprint(" ");
//    }
//    DBGprint("\n");
//    for (int i = 0; i < BIT_LENGTH; i++)
//        lagrange_co1(0, i) -= lagrange_f(BIT_LENGTH-2, i);
//    for (int i = 0; i < BIT_LENGTH - 1; i++)
//        lagrange_co1(0, i+1) += lagrange_f(BIT_LENGTH-2, i);
//    for (int i = 0; i < BIT_LENGTH - 1; i++)
//        lagrange_co2(0, i+1) += lagrange_f(BIT_LENGTH-2, i);

//    SInt128 sym = sym_bool(1, 8);
//    sym.print();
//    printf("\n");
//    DBGprint("\n");

//    for (int i = 0; i < BIT_LENGTH; i++)
//        bit_p[i] = MOD >> i & 1;

    DBGprint("init complete\n");
}

Mat Player::getMetadata() {
    return metadata;
}