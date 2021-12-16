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

#define B 1          //Batch size
#define D 3          //Dimension
#define USE_D 2      // the real used Dimension
#define PRINT_PRE_ITE 100   //每隔多少个迭代打印一遍
#define LABEL_P -1      // 0: 标签位于第一行，-1：标签位于最后一行，如果标签和特征值是分开放的，这个参数没有意义


#define OFFLINE_PHASE_ON 0  
#define LOCAL_TEST 1
#define GRAPH_TYPE 0
#define ACTIVATION 0

#define SIGMOID 0      
#define TANH 1

#define LINEAR 0
#define LOGISTIC 1

#define LEAKEY_RELU_BIAS IE/2
#define MOD 100000000000000003ll    //field size
#define N 4                     //size of train dataset
#define M 3                         //number of parties

#define L 2
// LSTM
#define D2 D/L
#define CH 128
#define IE_b 10000

#define TN 2
#define MAX_NODE_NUM 2001
#define MASTER 0
#define IE 1048576                  //定点数的精度 2的20次方
#define NM 1                    //测试集的数量
#define BIT_LENGTH 64               //bit length
#define REDUNDANCY 3                
#define BIT_P_LEN 55                //bit length of field
#define BUFFER_MAX 10000001         //通信缓冲区最大大小
#define HEADER_LEN 4                //头的大小
#define DECIMAL_PLACES 20           //定点数位数
#define HEADER_LEN_OPT 2            //socket有关？？
#define TRAIN_ITE 10000             //总迭代次数

#define THREAD_NUM 20               //线程数目

#define MatColMajor 0               //行存
#define MatRowMajor 1               //列存

#define MM_NN 0                     //矩阵相乘的具体模式
#define MM_NT 1
#define MM_TN 2
#define MM_TT 3



// Markov training
#define NGRAM 4                     
#define KEY_NUM 1
#define KEY_BATCH 14

// Markov inference
#define MAX_LEN 10
#define INFER_BATCH 14

#define MAX_SMOOTHING_LEVEL 10
#define ALPHABET_SIZE 10

// Time
#define CLOCK_MAIN 1
#define CLOCK_TRAIN 2

// DT
#define FEATURE_DIM 4
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
    static const ll SQRTINV = (ll128)(MOD+1>>2) * (MOD-2) % (MOD-1);
    static const ll inv2;
    static const ll inv2_m;

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
        static int getNext(char *p, int begin);
        static int getint(char* p,int &begin);
        static ll getll(char* p,int &begin);
        static int getfixpoint(char *p, int &begin);
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
