//
// Created by tangdingyi on 2019/12/25.
//
#include <math.h>
#include "Constant.h"

int node_type;
int globalRound;
int DBGtest;
ll Constant::Clock::global_clock[101] = {0};

void Constant::Clock::print_clock(int id)
{
    DBGprint("clock: %d %f\n", id, global_clock[id] * 1.0 * microseconds::period::num / microseconds::period::den);
}

double Constant::Clock::get_clock(int id)
{
    return global_clock[id] * 1.0 * microseconds::period::num / microseconds::period::den;
}

void Constant::Util::int_to_char(char *&p, int u)
{
    *p++ = u & 0xff;
    *p++ = u >> 8 & 0xff;
    *p++ = u >> 16 & 0xff;
    *p++ = u >> 24 & 0xff;
}

void Constant::Util::ll_to_char(char *&p, ll u)
{
    *p++ = u & 0xff;
    *p++ = u >> 8 & 0xff;
    *p++ = u >> 16 & 0xff;
    *p++ = u >> 24 & 0xff;
    *p++ = u >> 32 & 0xff;
    *p++ = u >> 40 & 0xff;
    *p++ = u >> 48 & 0xff;
    *p++ = u >> 56 & 0xff;
}

int Constant::Util::char_to_int(char *&p)
{
    int ret = 0;
    ret = *p++ & 0xff;
    ret |= (*p++ & 0xff) << 8;
    ret |= (*p++ & 0xff) << 16;
    ret |= (*p++ & 0xff) << 24;
    return ret;
}

ll Constant::Util::char_to_ll(char *&p)
{
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

void Constant::Util::int_to_header(char *p, int u)
{
    *p++ = u & 0xff;
    *p++ = u >> 8 & 0xff;
    *p++ = u >> 16 & 0xff;
    *p++ = u >> 24 & 0xff;
}

int Constant::Util::header_to_int(char *p)
{
    int ret = 0;
    ret = *p++ & 0xff;
    ret |= (*p++ & 0xff) << 8;
    ret |= (*p++ & 0xff) << 16;
    ret |= (*p++ & 0xff) << 24;
    return ret;
}

/**
 * 寻找从 begin 开始的下一个数字开始的下标
 *
 * 比如 123, 34 begin如果为0，经过getNext，begin变为3
 *
 * 现在的get函数有个bug，就是当出现空值的时候，如3,,,,,4,那么这些空值会被直接跳过，认为3的下一个数字是4
 *
 */
int Constant::Util::getNext(char *p, int begin)
{
    while (!isdigit(p[begin]))
        begin++;
    while (isdigit(p[begin]))
    {
        begin++;
        if (p[begin] == '.')
        {
            // 下一位是小数点，直接跳过
            ++begin;
        }
    }

    return begin;
}

/**
 * 从p指向的位置开始读整数
 *
 * @param p 开始位置
 *
 * return 返回int型数字
 */
int Constant::Util::getint(char *p, int &begin)
{
    bool isP = true;
    while (!isdigit(p[begin]))
    {
        // 读到负号，是负数
        if (p[begin] == '-')
            isP = false;
        begin++;
    }

    int ret = 0;
    while (isdigit(p[begin]))
    {
        ret = 10 * ret + p[begin] - '0';
        begin++;
    }

    return isP ? ret : -ret;
}

ll Constant::Util::getll(char *p, int &begin)
{
    bool isP = true;
    while (!isdigit(p[begin]))
    {
        // 读到负号，是负数
        if (p[begin] == '-')
            isP = false;
        begin++;
    }

    ll ret = 0;
    while (isdigit(p[begin]))
    {
        ret = 10 * ret + p[begin] - '0';
        begin++;
    }

    return isP ? ret : -ret;
}

int Constant::Util::getfixpoint(char *p, int &begin)
{
    bool isP = true;
    while (!isdigit(p[begin]))
    {
        // 读到负号，是负数
        if (p[begin] == '-')
            isP = false;
        begin++;
    }

    ll ret = 0;
    int pointnum = 0;         // 小数点的位数
    int isfloat = 0;          // 是否是小数点部分，是：给pointnum+1
    while (isdigit(p[begin])) // is number, begin to read
    {
        ret = 10 * ret + (p[begin] - '0');
        pointnum += isfloat;
        begin++;

        if (p[begin] == '.') // 下一位是小数点，直接跳过
        {                    // 浮点数
            isfloat = 1;
            begin++;
        }
    }
    // 先除掉小数点的位数，再乘定点数位数
    int result = (ret * 1.0 / pow(10, pointnum)) * Config::config->IE;

    return isP ? result : -result;
}

ll Constant::Util::randomlong() {
    return rand() % Config::config->MOD;
}

ll128 Constant::Util::get_residual(ll128 a) {
    return (a % Config::config->MOD + Config::config->MOD) % Config::config->MOD;
}

ll128 Constant::Util::get_sign(ll128 a) {
    return a > Config::config->MOD / 2 ? a - Config::config->MOD : a;
}

ll128 Constant::Util::get_abs(ll128 a)
{
    return a > 0 ? a : -a;
}

ll128 Constant::Util::sqrt(ll128 a) {
    return power(a, Config::config->MOD + 1 >> 2);
}

ll128 Constant::Util::inverse(ll128 a, ll128 b)
{
    return power(a, b - 2);
}

ll128 Constant::Util::power(ll128 a, ll128 b)
{
    ll128 ret = 1;
    a = (a%Config::config->MOD+Config::config->MOD)%Config::config->MOD;
    b = (b%Config::config->MOD+Config::config->MOD)%Config::config->MOD;
    if (b == 0)
        return 1;
    while (b > 0)
    {
        if (b & 1)
            ret = ret * a;
        ret = get_residual(ret);
        a = a * a;
        a = get_residual(a);
        b >>= 1;
    }
    return ret;
}

ll128 Constant::Util::cal_perm(ll128 *key, int l, int k)
{
    if (!l)
        return 1;
    vector<ll128> a(l + 1);
    a[0] = 1;
    for (int i = 0; i < Config::config->M; i++)
        if (i != k)
            for (int j = l; j; j--)
            {
                a[j] += a[j - 1] * key[i];
                a[j] = get_residual(a[j]);
            }
    return a[l];
}
std::ostream &
operator<<(std::ostream &dest, __int128_t value)
{
    std::ostream::sentry s(dest);
    if (s)
    {
        __uint128_t tmp = value < 0 ? -value : value;
        char buffer[128];
        char *d = std::end(buffer);
        do
        {
            --d;
            *d = "0123456789"[tmp % 10];
            tmp /= 10;
        } while (tmp != 0);
        if (value < 0)
        {
            --d;
            *d = '-';
        }
        int len = std::end(buffer) - d;
        if (dest.rdbuf()->sputn(d, len) != len)
        {
            dest.setstate(std::ios_base::badbit);
        }
    }
    return dest;
}
