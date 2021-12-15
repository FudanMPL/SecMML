//
// Created by tangdingyi on 2019/12/25.
//

#ifndef MPC_ML_CONSTANT_H
#define MPC_ML_CONSTANT_H

#ifndef UNIX_PLATFORM
#define UNIX_PLATFORM
#endif

//#ifndef MAC_OS
//#define MAC_OS
//#endif
#include "config/Config.hpp"
#include <iostream>
#include <cstdio>
#include <vector>
#include <fstream>
#include <queue>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <chrono>
#include <cmath>
#include <limits>
#ifdef UNIX_PLATFORM
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#else
#include <winsock2.h>
#endif

#define DEBUG
#ifdef DEBUG
#define DBGprint(...) printf(__VA_ARGS__)
#else
#define DBGprint(...)
#endif

using namespace std;
using namespace chrono;
typedef long long ll;       // 64-bit
typedef __int128_t ll128;   // 128-bit
typedef __float128 ff128;

extern int node_type;
extern int globalRound;
extern int DBGtest;
class Constant {
public:
    static string getDateTime() {
        time_t t = std::time(nullptr);
        struct tm * now = localtime(&t);
        char buf[80];
        strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", now);
        return string(buf);
    }
    class Clock {
        int id;
        system_clock::time_point start;
        static ll global_clock[101];
    public:
        static double get_clock(int id);
        static void print_clock(int id);
        Clock(int id) : id(id) {
            start = system_clock::now();
        };
        ~Clock() {
            system_clock::time_point end = system_clock::now();
            decltype(duration_cast<microseconds>(end - start)) time_span = duration_cast<microseconds>(end - start);
            global_clock[id] += time_span.count();
        };
        double get() {
            system_clock::time_point end = system_clock::now();
            decltype(duration_cast<microseconds>(end - start)) time_span = duration_cast<microseconds>(end - start);
            return time_span.count() * 1.0 * microseconds::period::num / microseconds::period::den;
        }
        void print() {
            system_clock::time_point end = system_clock::now();
            decltype(duration_cast<microseconds>(end - start)) time_span = duration_cast<microseconds>(end - start);
            DBGprint("duration: %f\n", time_span.count() * 1.0 * microseconds::period::num / microseconds::period::den);
        }
    };
    class Util {
    public:
        static void int_to_char(char* &p, int u);
        static void ll_to_char(char* &p, ll u);
        static int char_to_int(char* &p);
        static ll char_to_ll(char* &p);
        static void int_to_header(char* p, int u);
        static int header_to_int(char* p);
        static int getint(char* &p);
        static ll getll(char* &p);
        static ll randomlong();
        static ll128 get_residual(ll128 a);
        static ll128 get_sign(ll128 a);
        static ll128 get_abs(ll128 a);
        static ll128 sqrt(ll128 a);
        static ll128 inverse(ll128 a, ll128 b);
        static ll128 power(ll128 a, ll128 b);
        static ll128 cal_perm(ll128 key[], int l, int k);
    };
};
std::ostream&
operator<<( std::ostream& dest, __int128_t value );
#endif //MPC_ML_CONSTANT_H
