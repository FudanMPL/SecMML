//
// Created by 吴豪奇 on 4/16/20.
//
#include "reed_solomn_reconstruct.h"
#include <iostream>
#include <cassert>

reed_solomn::reed_solomn(ll128 p) {
    PRIME = p;
}

reed_solomn::~reed_solomn() {

}

void write(ll128 x)
{
    if(x < 0)
    {
        putchar('-');
        x = -x;
    }
    if(x > 9)
        write(x / 10);
    putchar(x % 10 + '0');
}

void print_vec(vector<ll128> vec) {
    for (int i = 0; i < vec.size(); ++i) {
//        write(vec[i]);
        printf("%lld, ", vec[i]);
//        cout << vec[i] << ", ";
    }
    cout << endl;
}

vector<ll128> reed_solomn::base_egcd(ll128 a, ll128 b) {
    ll128 r0 = a, r1 = b;
    ll128 s0 = 1, s1 = 0;
    ll128 t0 = 0, t1 = 1;
    ll128 div = 0, mod, tmp1, tmp2;
    while (r1 != 0) {
        div = r0 / r1;
        mod = r0 % r1;
        tmp1 = s0 - s1 * div;
        tmp2 = t0 - t1 * div;

        r0 = r1;
        s0 = s1;
        t0 = t1;

        r1 = mod;
        s1 = tmp1;
        t1 = tmp2;
    }
    vector<ll128> res;
    res.push_back(r0);
    res.push_back(s0);
    res.push_back(t0);
    return res;
}

ll128 reed_solomn::base_inverse(ll128 a) {
    ll128 a_inverse = Constant::Util::inverse(a, PRIME);
//    ll128 a_inverse = base_egcd(a, PRIME)[1];
    printf("a: %lld, inverse: %lld\n", a, a_inverse);
    printf("%lld\n", (a_inverse)*a%PRIME);
    return a_inverse >= 0 ? a_inverse : a_inverse + PRIME;
}

inline ll128 reed_solomn::base_add(ll128 a, ll128 b) {
    return ((a + b) % PRIME + PRIME) % PRIME;
}

inline ll128 reed_solomn::base_sub(ll128 a, ll128 b) {
    return ((a - b) % PRIME + PRIME) % PRIME;
}

ll128 reed_solomn::base_mul(ll128 a, ll128 b) {
    return ((a * b) % PRIME + PRIME) % PRIME;
}

ll128 reed_solomn::base_div(ll128 a, ll128 b) {
    return base_mul(base_inverse(b), a);
}


ll128 reed_solomn::degree(vector<ll128> A) {
    for (int i = A.size() - 1; i >= 0; --i) {
        if (A[i] != 0) {
            return i;
        }
    }
    return 0;
}
vector<ll128> reed_solomn::poly_strip(vector<ll128> A) {
    vector<ll128> res;
    for (int i = A.size()-1; i >=0 ; --i) {
        if (A[i] !=0) {
            for (int j = 0; j <= i; ++j) {
                res.push_back(A[j]);
            }
            break;
        }
    }
    return res;
}

vector<vector<ll128>> reed_solomn::poly_align(vector<ll128> A, vector<ll128> b) {
    int diff = A.size() - b.size();
    vector<vector<ll128>> res;
    if (diff > 0) {
        for (int i = 0; i < diff; ++i) {
            b.push_back(0);
        }
    } else {
        diff = -diff;
        for (int i = 0; i < diff; ++i) {
            A.push_back(0);
        }
    }
    res.push_back(A);
    res.push_back(b);
    return res;
}

ll128 reed_solomn::lc(vector<ll128> A) {
    int deg = degree(A);
    cout << "deg: " << deg << endl;
    return A[deg];
}

vector<ll128> reed_solomn::poly_add(vector<ll128> A, vector<ll128> b) {
    vector<ll128> res;
    vector<vector<ll128>> tmp = poly_align(A, b);
    A = tmp[0];
    b = tmp[1];
    assert(A.size() == b.size());
    for (int i = 0; i < A.size(); ++i) {
        res.push_back(base_add(A[i], b[i]));
    }
    return poly_strip(res);
}

vector<ll128> reed_solomn::poly_sub(vector<ll128> A, vector<ll128> b) {
    vector<ll128> res;
    vector<vector<ll128>> tmp = poly_align(A, b);
    A = tmp[0];
    b = tmp[1];
    assert(A.size() == b.size());
    for (int i = 0; i < A.size(); ++i) {
        res.push_back(base_sub(A[i], b[i]));
    }
    return poly_strip(res);
}

vector<ll128> reed_solomn::poly_scalar_mul(vector<ll128> A, ll128 scalar) {
    vector<ll128> res;
    for (int i = 0; i < A.size(); ++i) {
        res.push_back(base_mul(A[i], scalar));
    }
    return poly_strip(res);
}

vector<ll128> reed_solomn::poly_scalar_div(vector<ll128> A, ll128 scalar) {
    vector<ll128> res;
    for (int i = 0; i < A.size(); ++i) {
        res.push_back(base_mul(A[i], base_inverse(scalar)));
    }
    return poly_strip(res);
}

vector<ll128> reed_solomn::poly_mul(vector<ll128> A, vector<ll128> b) {
//    vector<vector<ll128>> tmp = poly_align(A, b);
//    A = tmp[0];
//    b = tmp[1];
//    assert(A.size() == b.size());
    vector<ll128> res;
    for (int i = 0; i < A.size() + b.size() - 1; ++i) {
        res.push_back(0);
    }
    for (int i = 0; i < A.size(); ++i) {
        for (int j = 0; j < b.size(); ++j) {
            res[i + j] = base_add(res[i + j], base_mul(A[i], b[j]));
        }
    }
    return poly_strip(res);
}

vector<vector<ll128>> reed_solomn::poly_divmod(vector<ll128> A, vector<ll128> b) {
    ll128 scalar = base_inverse(lc(b));
    // 不然会出错
    b = poly_strip(b);
//    printf("scalar %ld\n", scalar);
    vector<ll128> Q;
    for (int i = 0; i < A.size(); ++i) {
        Q.push_back(0);
    }
    vector<ll128> R(A);
//    ll128 degA = degree(A), degb = degree(b);
    for (int j = A.size() - b.size(); j >= 0; --j) {
        Q[j] = base_mul(scalar, R[j + b.size() - 1]);
//        printf("Q %ld\n", Q[j]);
        for (int k = 0; k < b.size(); ++k) {
            R[j + k] = base_sub(R[j + k], base_mul(Q[j], b[k]));
        }
    }

    vector<vector<ll128>> res;
    res.push_back(poly_strip(Q));
    res.push_back(poly_strip(R));
    return res;
}

vector<ll128> reed_solomn::poly_div(vector<ll128> A, vector<ll128> b) {
    return poly_divmod(A, b)[0];
}

vector<ll128> reed_solomn::poly_mod(vector<ll128> A, vector<ll128> b) {
    return poly_divmod(A, b)[1];
}
ll128 reed_solomn::poly_eval(vector<ll128> poly, ll128 x) {
    ll128 res = 0;
    for (int i = poly.size()-1; i >= 0; --i) {
        res = base_add(poly[i], base_mul(res, x));
    }
    return res;
}

vector<vector<ll128>> reed_solomn::lagrange_polys(vector<ll128> x) {
    vector<vector<ll128>> res;
    for (int i = 0; i < x.size(); ++i) {
        vector<ll128> poly;
        poly.push_back(1);
        ll128 scalar = 1;
        for (int j = 0; j < x.size(); ++j) {
            if (i == j) continue;
            vector<ll128> up;
            up.push_back(base_sub(0, x[j]));
            up.push_back(1);
            poly = poly_mul(poly, up);
//            scalar = base_mul(scalar, base_sub(x[i], x[j]));
            scalar = scalar * (x[i]- x[j]);
        }
        poly = poly_scalar_div(poly, Constant::Util::get_residual(scalar));
//        print_vec(poly);
        res.push_back(poly);
    }
    return res;
}

vector<ll128> reed_solomn::lagrange_interpolation(vector<ll128> x, vector<ll128> y) {
    print_vec(x);
    print_vec(y);
    vector<vector<ll128>> polys = lagrange_polys(x);
    assert(x.size() == y.size());
    vector<ll128> res;
    for (int i = 0; i < x.size(); ++i) {
        print_vec(polys[i]);
        vector<ll128> term = poly_scalar_mul(polys[i], y[i]);
        res = poly_add(res, term);
    }
    printf("------------END of Lagrange\n");
    ll128 tmp = 0;
    for (int j = 0; j < res.size(); ++j) {
        tmp = base_add(tmp, res[j]);
    }
    printf("tmp: %lld\n", tmp);
    return res;
}
vector<vector<ll128>> reed_solomn::reed_solomn_decoding(vector<ll128> x, vector<ll128> y, long max_degree, long max_err_count) {
    assert(x.size() == y.size());

    // interpolate based on given points
    vector<ll128> H = lagrange_interpolation(x, y);
    cout << "lagrange: " << endl;
    print_vec(H);
    vector<ll128> F;
    F.push_back(1);

    for (int i = 0; i < x.size(); ++i) {
        vector<ll128> term;
        term.push_back(base_sub(0, x[i]));
        term.push_back(1);

        F = poly_mul(F, term);
    }
    print_vec(F);
    // EEA-algorithm, find EEA triple
    vector<ll128> R0(F), R1(H), S0, S1, T0, T1;
    S0.push_back(1);
    T1.push_back(1);

    while (true){
        printf("asd\n");
        vector<ll128> Q = poly_div(R0, R1);
        vector<ll128> R2 = poly_mod(R0, R1);
        print_vec(R0);
        printf("%ld\n", degree(R0));

        if (degree(R0) < max_degree + max_err_count) {
            printf("Over!\n");
            print_vec(R0);
            print_vec(T0);
            vector<ll128> G = poly_div(R0, T0);
            vector<ll128> left = poly_mod(R0, T0);
            vector<vector<ll128>> res;
            print_vec(G);
            print_vec(left);

            if (degree(left) == 0) {
                printf("Over!\n");
                res.push_back(G);
                res.push_back(T0);
                return res;
            } else{
                return res;
            }
        }

        vector<ll128> tmp1 = poly_sub(S0, poly_mul(S1, Q));
        vector<ll128> tmp2 = poly_sub(T0, poly_mul(T1, Q));

        R0 = R1;
        S0 = S1;
        T0 = T1;
        R1 = R2;
        S1 = tmp1;
        T1 = tmp2;
    }
}

vector<ll128> reed_solomn::reed_solomn_reconstruct(vector<ll128> shares) {
    vector<ll128> xs;
    for (int i = 1; i <= shares.size(); ++i) {
        xs.push_back(i);
    }

    vector<vector<ll128>> res = reed_solomn_decoding(xs, shares, DEGREE+1, MAX_CORRUPT);
    vector<ll128> poly;
    printf("size, %ld\n", res.size());
    if (res.size()!=0) {
        poly = res[0];
        print_vec(poly);
        cout << poly_eval(poly, 0)<<endl;
//        printf("Secret is %lld", poly_eval(poly, 0));
    }
    return poly;
}

//int main() {
//    ll128 a = 4, b = 2;
//    reed_solomn rs(MOD);
//    ll128 c = (ll128)MOD * MOD;
//    ll128 d = 2 * 2;
////    printf("%lld, %lld\n", c, d);
//    cout << c << ", " << d << endl;
//    cout << sizeof(ll128) << " " << sizeof(ll128) << endl;
////    printf("a / b: %ld\n", rs.base_div(a, b));
////    printf("a inverse: %ld\n", rs.base_inverse(b));
////
////    vector<ll128> A;
////    A.push_back(7);
////    A.push_back(4);
////    A.push_back(5);
////    A.push_back(4);
////    A.push_back(0);
////
////
////    vector<ll128> b;
////    b.push_back(1);
////    b.push_back(0);
////    b.push_back(1);
////    b.push_back(0);
////
////
//////    b.push_back(4);
////    print_vec(rs.poly_align(A, b)[1]);
////    print_vec(rs.poly_mul(A, b));
////
////    cout << "asda" <<endl;
////    print_vec(rs.poly_div(A, b));
////    print_vec(rs.poly_mod(A, b));
////
////    vector<ll128> X;
////    X.push_back(10);
////    X.push_back(20);
////    X.push_back(30);
////    X.push_back(40);
////
////    vector<ll128> Y;
////    Y.push_back(321);
////    Y.push_back(375);
////    Y.push_back(163);
////    Y.push_back(118);
////    print_vec(rs.lagrange_interpolation(X, Y));
//
//    long NUM = 4;
//    long T = 5;
//    long K = 1;
//
//    vector<ll128> poly;
//    poly.push_back(5);
//    poly.push_back(431);
////    poly.push_back(105);
////    poly.push_back(146);
////    poly.push_back(364);
////    poly.push_back(285);
//    feldman_vss feldmanVss;
//    cout << feldmanVss.is_prime(MOD) << endl;
//    ll generator = feldmanVss.find_generator(MOD);
//    cout << "Generator: " << generator << endl;
//
//    vector<ll> cs(2);
//    for (int j = 0; j < cs.size(); ++j) {
//        cout << (ll)poly[j] << endl;
////        cs[j] = feldmanVss.powmod(generator, (ll)poly[j], MOD);
//        cs[j] = Constant::Util::power(generator, poly[j]);
//        cout << cs[j] << endl;
//    }
//
//    cout << "--------" << endl;
//    vector<ll128> shares;
//    for (int i = 1; i <= NUM; ++i) {
//        shares.push_back(rs.poly_eval(poly, i));
//        ll128 total1 = 1;
//        ll128 total2 = 0;
//        for (int j = 0; j < cs.size(); ++j) {
//            ll128 cur = Constant::Util::power(cs[j], Constant::Util::power(i, j));
////            ll128 cur = feldmanVss.powmod(cs[j], feldmanVss.powmod(i, j, MOD), MOD);
////            total1 = total1 * cur % MOD;
//            total1 = Constant::Util::get_residual(total1 * cur);
//            total2 += feldmanVss.powmod(i, j, MOD) * poly[j];
//        }
//        cout << total2 << " : " << shares[i-1] << endl;
////        ll128 tmp = feldmanVss.powmod(generator, shares[i-1], MOD);
//        ll128 tmp = Constant::Util::power(generator, shares[i-1]);
//        cout << total1 << " asdasd: " << tmp << endl;
//    }
//    print_vec(shares);
////    return 0;
//    shares[1] = 12;
////    shares[10] = 123;
////    cout << "`````, "<<rs.poly_eval(poly, 0) << endl;
//    rs.reed_solomn_reconstruct(shares);
//}
