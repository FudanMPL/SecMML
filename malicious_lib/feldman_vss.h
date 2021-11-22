//
// Created by whq on 2020/5/20.
//

#ifndef SMMLF_FELDMAN_VSS_H
#define SMMLF_FELDMAN_VSS_H


#include <cmath>
#include <vector>
#include <iostream>
#include "../Constant.h"
using namespace std;

class feldman_vss {
public:
    ll find_prime_root(ll prime);
    bool is_prime(ll number);
    ll find_generator(ll prime);
    ll powmod(ll a, ll b, ll p);
};


#endif //SMMLF_FELDMAN_VSS_H
