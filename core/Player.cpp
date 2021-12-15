//
// Created by tangdingyi on 2019/12/26.
//

#include "Player.h"


/// Degree Reduction, TODO: modify, 现在的方式会导致安全风险
Mat metadata(Config::config->M, Config::config->M);
vector<Player> Player::player;

Player::Player(){}

Player::Player(int id, ll128 key, ll128 lagrange) {
    this->id = id;
    this->key = key;
    /// lagrange polynomial coefficients， TODO： modify for reed_solomn reconstruct
    this->lagrange = lagrange;
}

void Player::init() {
    ll128 key[Config::config->M], lagrange[Config::config->M];
    Mat vandermonde(Config::config->M, Config::config->M), vandermonde_inv(Config::config->M, Config::config->M), des(Config::config->M, Config::config->M), pol(Config::config->M, Config::config->M), van_inv2(Config::config->TN, Config::config->TN), van2(Config::config->TN, Config::config->M);
    for (int i = 0; i < Config::config->M; i++) {
        key[i] = Constant::Util::randomlong();
        key[i] = i + 2;
        vandermonde(i, 1) = key[i];
        vandermonde(i, 0) = 1;
//        DBGprint("%d ", i);
        for (int j = 2; j < Config::config->M; j++) {
//            vandermonde(i, j) = mul(vandermonde(i, j - 1), key[i]);
            vandermonde(i, j) = vandermonde(i, j-1) * key[i];
//            vandermonde(i, j).residual();
            vandermonde(i, j) = Constant::Util::get_residual(vandermonde(i, j));
        }
    }
    DBGprint("init midway\n");
    vandermonde.print();
    for (int i = 0; i < Config::config->M; i++) {
        lagrange[i] = 1;
        for (int j = 0; j < Config::config->M; j++)
            if (j != i) {
//                lagrange[i] = mul(lagrange[i], key[j]);
                lagrange[i] = lagrange[i] * key[j];
//                lagrange[i].residual();
                lagrange[i] = Constant::Util::get_residual(lagrange[i]);
//                lagrange[i] = mul(lagrange[i], inverse(sub(key[j], key[i]), MOD));
                ll128 tmp = key[j] - key[i];
//                tmp.residual();
                tmp = Constant::Util::get_residual(tmp);
                lagrange[i] = lagrange[i] * Constant::Util::inverse(tmp, Config::config->MOD);
//                lagrange[i].residual();
                lagrange[i] = Constant::Util::get_residual(lagrange[i]);
            }
    }

    for (int i = 0; i < Config::config->M; i++) {
        player.push_back(Player(i, key[i], lagrange[i]));
    }
    for (int i = 0; i < Config::config->M; i++) {
        DBGprint("%d, key: %lld, %lld\n", player[i].id, (ll)player[i].key, (ll)player[i].lagrange);
    }
    for (int i = 0; i < Config::config->M; i++) {
        ll128 d;
        d = 1;
        for (int j = 0; j < Config::config->M; j++)
            if (j != i) {
//                d = mul(d, key[j] - key[i]);
                d = d * (key[j] - key[i]);
//                d.residual();
                d = Constant::Util::get_residual(d);
            }
//        d = inverse(d, MOD);
        d = Constant::Util::inverse(d, Config::config->MOD);
        //printf("%I64d ", d);
        for (int j = 0; j < Config::config->M; j++) {
//            vandermonde_inv(i, j) = mul(power(-1, j), cal_perm(key, M-j-1, i));
            vandermonde_inv(i, j) = Constant::Util::power(-1, j) * Constant::Util::cal_perm(key, Config::config->M-j-1, i);
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
        d = Constant::Util::inverse(d, Config::config->MOD);
        van_inv2(0, 0) = player[1].key;
        van_inv2(1, 1) = 1;
        van_inv2(0, 1) = -1;
        van_inv2(1, 0) = 0 - player[0].key;
        for (int i = 0; i < Config::config->TN; i++)
            for (int j = 0; j < Config::config->TN; j++) {
                van_inv2(i, j) = van_inv2(i, j) * d;
                van_inv2(i, j) = Constant::Util::get_residual(van_inv2(i, j));
            }
    }
    for (int i = 0; i < Config::config->M; i++) {
        van2(0, i) = 1;
        van2(1, i) = player[i].key;
    }
    for (int i = 0; i < Config::config->TN; i++)
        des(i, i) = 1;
    vandermonde = vandermonde.transpose();
    pol = vandermonde_inv * des * vandermonde;
    metadata = pol;
    DBGprint("\n");
    DBGprint("init complete\n");
}

Mat Player::getMetadata() {
    return metadata;
}