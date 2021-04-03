//
// Created by whq on 2020/5/20.
//

#ifndef SMMLF_REED_SOLOMN_RECONSTRUCT_H
#define SMMLF_REED_SOLOMN_RECONSTRUCT_H


#include <vector>
//#include "Constant.h"
#include "feldman_vss.h"
using namespace std;
class reed_solomn {
private:
    ll128 PRIME = 1007;
    long MAX_CORRUPT = 1;
    long DEGREE = 1;
public:
    reed_solomn(ll128 p);
    virtual ~reed_solomn();

    // ax = 1 mod PRIME
    ll128 base_inverse(ll128 a);
    // extend euclidean algorithm, ax + by = 1
    vector<ll128> base_egcd(ll128 a, ll128 b);

    ll128 base_add(ll128 a, ll128 b);
    ll128 base_sub(ll128 a, ll128 b);
    ll128 base_mul(ll128 a, ll128 b);
    // calculate a * b^-1
    ll128 base_div(ll128 a, ll128 b);


    vector<vector<ll128>> poly_align(vector<ll128> A, vector<ll128> b);
    vector<ll128> poly_strip(vector<ll128> A);
    ll128 lc(vector<ll128> A);
    ll128 degree(vector<ll128> A);

    vector<ll128> poly_add(vector<ll128> A, vector<ll128> b);
    vector<ll128> poly_sub(vector<ll128> A, vector<ll128> b);
    vector<ll128> poly_scalar_mul(vector<ll128> A, ll128 scalar);
    vector<ll128> poly_scalar_div(vector<ll128> A, ll128 scalar);

    vector<ll128> poly_mul(vector<ll128> A, vector<ll128> b);
    vector<vector<ll128>> poly_divmod(vector<ll128> A, vector<ll128> b);
    vector<ll128> poly_div(vector<ll128> A, vector<ll128> b);
    vector<ll128> poly_mod(vector<ll128> A, vector<ll128> b);

    ll128 poly_eval(vector<ll128> poly, ll128 x);
    vector<ll128> lagrange_interpolation(vector<ll128> x, vector<ll128> y);
    vector<vector<ll128>> lagrange_polys(vector<ll128> x);

    vector<vector<ll128>> reed_solomn_decoding(vector<ll128> x, vector<ll128> y, long max_deg, long max_err_count);
    vector<ll128> reed_solomn_reconstruct(vector<ll128> shares);

};

#endif //SMMLF_REED_SOLOMN_RECONSTRUCT_H
