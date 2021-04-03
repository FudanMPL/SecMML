//
// Created by whq on 2020/5/20.
//

#include "feldman_vss.h"
ll feldman_vss::find_prime_root(ll prime) {
    // Euler Totient Function, note that, for a modular prime q, the function result is q-1
    int euler = prime - 1;

    // find all prime factors of euler

}
ll feldman_vss::find_generator(ll p) {
    vector<uint64_t> fact;
    uint64_t phi = p-1,  n = phi;
    for (uint64_t i=2; i*i<=n; ++i)
        if (n % i == 0) {
            fact.push_back (i);
            while (n % i == 0)
                n /= i;
        }
    if (n > 1)
        fact.push_back (n);
    for (uint64_t j = 0; j < fact.size(); ++j) {
        cout << "prime factor: " << fact[j] << endl;
    }
    for (uint64_t res=2; res<=p; ++res) {
        bool ok = true;
        for (size_t i=0; i<fact.size() && ok; ++i){
            ok &= powmod (res, phi / fact[i], p) != 1;
        }
        if (ok)  return res;
    }
    return -1;
}
bool feldman_vss::is_prime(ll number) {
    if (number <= 1) return false;
    if (number <=3 ) return true;

    for (uint64_t i = 5; i*i < number; i = i + 6) {
        if (number%i==0 || number%(i+2)==0)
            return false;
    }
    return true;
}
ll feldman_vss::powmod (ll a, ll b, ll p) {
    ll res = 1;
    while (b) {
        if (b & 1)
            res = res * 1ll * a % p, --b;
        else
            a = a * 1ll * a % p, b >>= 1;
//        cout << "asdsd   " << res << endl;
    }
    return res;
}

ll powmod (ll a, ll b, ll p) {
    int res = 1;
    while (b)
        if (b & 1)
            res = int (res * 1ll * a % p),  --b;
        else
            a = int (a * 1ll * a % p),  b >>= 1;
    return res;
}

int generator (uint64_t p) {
    vector<uint64_t> fact;
    uint64_t phi = p-1,  n = phi;
    for (uint64_t i=2; i*i<=n; ++i)
        if (n % i == 0) {
            fact.push_back (i);
            while (n % i == 0)
                n /= i;
        }
    if (n > 1)
        fact.push_back (n);
    for (uint64_t j = 0; j < fact.size(); ++j) {
        cout << "prime factor: " << fact[j] << endl;
    }
    for (uint64_t res=2; res<=p; ++res) {
        bool ok = true;
//        cout << "Cur: " << res << endl;
        for (size_t i=0; i<fact.size() && ok; ++i){
//            cout << "index: " << i << endl;
            ok &= powmod (res, phi / fact[i], p) != 1;
        }
        if (ok)  cout << res << endl;
    }
    return -1;
}
void test_vss() {
    ll p = MOD;
    feldman_vss feldmanVss;

    cout << feldmanVss.is_prime(p) << endl;
    uint64_t generator = feldmanVss.find_generator(p);
    cout << "Generator: " << generator << endl;
}
//int main() {
//    test_vss();
////
//////    uint64_t p = 100000000000000003;
////    uint64_t p = 761;
////    feldman_vss feldmanVss;
////    cout << feldmanVss.is_prime(p) << endl;
////    cout << generator(p) << endl;
//}