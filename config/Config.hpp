/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-07 10:46:07
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2021-12-21 19:43:15
 */

#include <cstdio>
#include <fstream>
#include "json/json.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

class Config{
    public:
        static Config* config;
        static Config* init(std::string file_name);
        static __int128_t inverse(__int128_t a, __int128_t b, long MOD);
        static __int128_t power(__int128_t a, __int128_t b, long MOD);
        static __int128_t get_residual(__int128_t a, long MOD);
        Config(Json::Value root, std::vector<std::string> ip, std::vector<int> port)  : B(root["B"].asInt()), 
                                    D(root["D"].asInt()),
                                    PRINT_PRE_ITE(root["PRINT_PRE_ITE"].asInt()),
                                    OFFLINE_PHASE_ON(root["OFFLINE_PHASE_ON"].asInt()),
                                    LOCAL_TEST(root["LOCAL_TEST"].asInt()),
                                    GRAPH_TYPE(root["GRAPH_TYPE"].asInt()),
                                    ACTIVATION(root["ACTIVATION"].asInt()),
                                    SIGMOID(root["SIGMOID"].asInt()),
                                    TANH(root["TANH"].asInt()),
                                    LINEAR(root["LINEAR"].asInt()),
                                    LOGISTIC(root["LOGISTIC"].asInt()),
                                    LEAKEY_RELU_BIAS(root["IE"].asInt()/2),
                                    MOD(atol(root["MOD"].asString().c_str())),
                                    N(root["N"].asInt()),
                                    M(root["M"].asInt()),
                                    L(root["L"].asInt()),
                                    D2(root["D"].asInt()/root["L"].asInt()),
                                    CH(root["CH"].asInt()),
                                    IE_b(root["IE_b"].asInt()),
                                    TN(root["TN"].asInt()),
                                    MAX_NODE_NUM(root["MAX_NODE_NUM"].asInt()),
                                    MASTER(root["MASTER"].asInt()),
                                    IE(root["IE"].asInt()),
                                    NM(root["NM"].asInt()),
                                    BIT_LENGTH(root["BIT_LENGTH"].asInt()),
                                    REDUNDANCY(root["REDUNDANCY"].asInt()),
                                    BIT_P_LEN(root["BIT_P_LEN"].asInt()),
                                    BUFFER_MAX(root["BUFFER_MAX"].asInt()),
                                    HEADER_LEN(root["HEADER_LEN"].asInt()),
                                    ND(root["ND"].asInt()),
                                    DECIMAL_PLACES(root["DECIMAL_PLACES"].asInt()),
                                    HEADER_LEN_OPT(root["HEADER_LEN_OPT"].asInt()),
                                    TRAIN_ITE(root["TRAIN_ITE"].asInt()),
                                    THREAD_NUM(root["THREAD_NUM"].asInt()),
                                    MatColMajor(root["MatColMajor"].asInt()),
                                    MatRowMajor(root["MatRowMajor"].asInt()),
                                    MM_NN(root["MM_NN"].asInt()),
                                    MM_NT(root["MM_NT"].asInt()),
                                    MM_TN(root["MM_TN"].asInt()),
                                    MM_TT(root["MM_TT"].asInt()),
                                    NGRAM(root["NGRAM"].asInt()),
                                    KEY_NUM(root["KEY_NUM"].asInt()),
                                    KEY_BATCH(root["KEY_BATCH"].asInt()),
                                    MAX_LEN(root["MAX_LEN"].asInt()),
                                    INFER_BATCH(root["INFER_BATCH"].asInt()),
                                    MAX_SMOOTHING_LEVEL(root["MAX_SMOOTHING_LEVEL"].asInt()),
                                    ALPHABET_SIZE(root["ALPHABET_SIZE"].asInt()),
                                    CLOCK_MAIN(root["CLOCK_MAIN"].asInt()),
                                    CLOCK_TRAIN(root["CLOCK_TRAIN"].asInt()),
                                    FEATURE_DIM(root["FEATURE_DIM"].asInt()),
                                    IP(ip),
                                    PORT(port),
                                    SQRTINV( (__int128_t)(atol(root["MOD"].asString().c_str())+1>>2) * (atol(root["MOD"].asString().c_str())-2) % (atol(root["MOD"].asString().c_str())-1) ),
                                    INV2(Config::inverse(2, atoll(root["MOD"].asString().c_str()), atoll(root["MOD"].asString().c_str()))),
                                    INV2_M(1<<Config::inverse(1<<root["DECIMAL_PLACES"].asInt(), atoll(root["MOD"].asString().c_str()), atoll(root["MOD"].asString().c_str()))),
                                    USE_D(root["USE_D"].asInt()),
                                    LABEL_P(root["LABEL_P"].asInt()),
                                    LEARNING_RATE(root["LEARNING_RATE"].asDouble())
                                    {};
        const int B;
        const int D;
        const int PRINT_PRE_ITE;
        const int OFFLINE_PHASE_ON;
        const int LOCAL_TEST;
        const int GRAPH_TYPE;
        const int ACTIVATION;
        const int SIGMOID;
        const int TANH;
        const int LINEAR;
        const int LOGISTIC;
        const int LEAKEY_RELU_BIAS;
        const long MOD;
        const int N;
        const int M; //number of parties
        const int L;
        const int D2;
        const int CH;
        const int IE_b;
        const int TN;
        const int MAX_NODE_NUM;
        const int MASTER;
        const int IE;
        const int NM;
        const int BIT_LENGTH;
        const int REDUNDANCY;
        const int BIT_P_LEN;
        const int BUFFER_MAX;
        const int HEADER_LEN;
        const int ND;
        const int DECIMAL_PLACES;
        const int HEADER_LEN_OPT;
        const int TRAIN_ITE;
        const int THREAD_NUM;
        const int MatColMajor;
        const int MatRowMajor;
        const int MM_NN;
        const int MM_NT;
        const int MM_TN;
        const int MM_TT;
        const int NGRAM;
        const int KEY_NUM;
        const int KEY_BATCH;
        const int MAX_LEN;
        const int INFER_BATCH;
        const int MAX_SMOOTHING_LEVEL;
        const int ALPHABET_SIZE;
        const int CLOCK_MAIN;
        const int CLOCK_TRAIN;
        const int FEATURE_DIM;
        const std::vector<std::string> IP;
        const std::vector<int> PORT;
        const long long SQRTINV;
        const long long INV2;
        const long long INV2_M;
        const int USE_D;
        const int LABEL_P;
        const double LEARNING_RATE;
    private:
        
};
