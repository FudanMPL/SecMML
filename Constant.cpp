//
// Created by tangdingyi on 2019/12/25.
//

#include "Constant.h"

int DBGtest;

ll Constant::Clock::global_clock[101] = {0};


void Constant::Clock::print_clock(int id) {
    DBGprint("clock: %d %f\n", id, global_clock[id] * 1.0 * microseconds::period::num / microseconds::period::den);
}

double Constant::Clock::get_clock(int id) {
    return global_clock[id] * 1.0 * microseconds::period::num / microseconds::period::den;
}

void Constant::Util::int_to_char(char* &p, int u) {
    *p++ = u & 0xff;
    *p++ = u >> 8 & 0xff;
    *p++ = u >> 16 & 0xff;
    *p++ = u >> 24 & 0xff;
}

void Constant::Util::ll_to_char(char *&p, ll u) {
    *p++ = u & 0xff;
    *p++ = u >> 8 & 0xff;
    *p++ = u >> 16 & 0xff;
    *p++ = u >> 24 & 0xff;
    *p++ = u >> 32 & 0xff;
    *p++ = u >> 40 & 0xff;
    *p++ = u >> 48 & 0xff;
    *p++ = u >> 56 & 0xff;
}

int Constant::Util::char_to_int(char* &p) {
    int ret = 0;
    ret = *p++ & 0xff;
    ret |= (*p++ & 0xff) << 8;
    ret |= (*p++ & 0xff) << 16;
    ret |= (*p++ & 0xff) << 24;
    return ret;
}

ll Constant::Util::char_to_ll(char *&p) {
    ll ret = 0;
    ret = (ll)(*p++ & 0xff);
    ret |= (ll)(*p++ & 0xff) << 8;
    ret |= (ll)(*p++ & 0xff) << 16;
    ret |= (ll)(*p++ & 0xff) << 24;
    ret |= (ll)(*p++ & 0xff) << 32;
    ret |= (ll)(*p++ & 0xff) << 40;
    ret |= (ll)(*p++ & 0xff) << 48;
    ret |= (ll)(*p++ & 0xff) << 56;
    return ret;
}


void Constant::Util::int_to_header(char *p, int u) {
    *p++ = u & 0xff;
    *p++ = u >> 8 & 0xff;
    *p++ = u >> 16 & 0xff;
    *p++ = u >> 24 & 0xff;
}

int Constant::Util::header_to_int(char *p) {
    int ret = 0;
    ret = *p++ & 0xff;
    ret |= (*p++ & 0xff) << 8;
    ret |= (*p++ & 0xff) << 16;
    ret |= (*p++ & 0xff) << 24;
    return ret;
}

int Constant::Util::getint(char *&p) {
    while (!isdigit(*p))
        p++;
    int ret = 0;
    while (isdigit(*p)) {
        ret = 10*ret + *p - '0';
        p++;
    }
    return ret;
}

ll Constant::Util::getll(char *&p) {
    while (!isdigit(*p))
        p++;
    ll ret = 0;
    while (isdigit(*p)) {
        ret = 10*ret + *p - '0';
        p++;
    }
    return ret;
}

ll Constant::Util::randomlong() {
    return rand() % MOD;
}

ll128 Constant::Util::get_residual(ll128 a) {
    return (a % MOD + MOD) % MOD;
}

ll128 Constant::Util::get_sign(ll128 a) {
    return a > MOD / 2 ? a - MOD : a;
}

ll128 Constant::Util::get_abs(ll128 a) {
    return a > 0 ? a : -a;
}

ll128 Constant::Util::sqrt(ll128 a) {
    return power(a, MOD + 1 >> 2);
}

ll128 Constant::Util::inverse(ll128 a, ll128 b) {
    return power(a, b-2);
}

ll128 Constant::Util::power(ll128 a, ll128 b) {
    ll128 ret = 1;
    a = (a%MOD+MOD)%MOD;
    b = (b%MOD+MOD)%MOD;
    if (b == 0)
        return 1;
    while (b > 0) {
        if (b&1) ret = ret * a;
        ret = get_residual(ret);
        a = a * a;
        a = get_residual(a);
        b >>= 1;
    }
    return ret;
}

ll128 Constant::Util::cal_perm(ll128 *key, int l, int k) {
    if (!l)
        return 1;
    vector<ll128> a(l+1);
    a[0] = 1;
    for (int i = 0; i < M; i++)
        if (i != k)
            for (int j = l; j; j--) {
                a[j] += a[j - 1] * key[i];
                a[j] = get_residual(a[j]);
            }
    return a[l];
}
std::ostream&
operator<<( std::ostream& dest, __int128_t value )
{
    std::ostream::sentry s( dest );
    if ( s ) {
        __uint128_t tmp = value < 0 ? -value : value;
        char buffer[ 128 ];
        char* d = std::end( buffer );
        do
        {
            -- d;
            *d = "0123456789"[ tmp % 10 ];
            tmp /= 10;
        } while ( tmp != 0 );
        if ( value < 0 ) {
            -- d;
            *d = '-';
        }
        int len = std::end( buffer ) - d;
        if ( dest.rdbuf()->sputn( d, len ) != len ) {
            dest.setstate( std::ios_base::badbit );
        }
    }
    return dest;
}
const ll Constant::inv2 = Constant::Util::inverse(2, MOD);

const ll Constant::inv2_m = Constant::Util::inverse(1 << DECIMAL_PLACES, MOD);