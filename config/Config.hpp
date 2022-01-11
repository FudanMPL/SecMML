/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-07 10:46:07
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2022-01-11 16:16:34
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
                                    LEARNING_RATE(root["LEARNING_RATE"].asDouble()),
                                    TRAIN_TEST_SAME(root["TRAIN_TEST_SAME"].asInt()),
                                    TRAIN_FILENAME(root["TRAIN_FILENAME"].asString()),
                                    TEST_FILENAME(root["TEST_FILENAME"].asString()),
                                    FILENAME(root["FILENAME"].asString())
                                    {};
        const int B;                    // Batch size
        const int D;                    // Dimension
        const int PRINT_PRE_ITE;        // Print the loss after how many iterations
        const int OFFLINE_PHASE_ON;     
        const int LOCAL_TEST;
        const int GRAPH_TYPE;
        const int ACTIVATION;
        const int SIGMOID;
        const int TANH;
        // To decide whether to run Linear regression or Logistic regression
        const int LINEAR;               // Run Linear regression
        const int LOGISTIC;             // Run Logistic regression
        const int LEAKEY_RELU_BIAS;
        const long MOD;                 // Global mod number
        const int N;                    
        const int M;                    // Number of parties
        const int L;
        const int D2;
        const int CH;
        const int IE_b;
        const int TN;
        const int MAX_NODE_NUM;
        const int MASTER;
        const int IE;                   // A factor to turn decimal into integer. With this factor, a fix-point rational number can be transformed into integer.
        const int NM;
        const int BIT_LENGTH;           
        const int REDUNDANCY;
        const int BIT_P_LEN;
        const int BUFFER_MAX;           // MAX buffer size for socket
        const int HEADER_LEN;
        const int ND;
        const int DECIMAL_PLACES;       // The bit length for decimal part
        const int HEADER_LEN_OPT;
        const int TRAIN_ITE;            // How many iterations will run during the training part
        const int THREAD_NUM;           
        const int MatColMajor;          // if MatColMajor == 1, then Mat is saved with Column major
        const int MatRowMajor;          // if MatRowMajor == 1, then Mat is saved with Row major
        const int MM_NN;                // for multiplication between two matrix A,B, if MM_NN == 1, then A and B are both saved with Column major
        const int MM_NT;                // for multiplication between two matrix A,B, if MM_NN == 1, then A is saved with Column major while B is saved with Row major
        const int MM_TN;                // for multiplication between two matrix A,B, if MM_NN == 1, then A is saved with Row major while B is saved with Column major
        const int MM_TT;                // for multiplication between two matrix A,B, if MM_NN == 1, then A and B are both saved with Row major
        const int NGRAM;
        const int KEY_NUM;
        const int KEY_BATCH;
        const int MAX_LEN;
        const int INFER_BATCH;
        const int MAX_SMOOTHING_LEVEL;
        const int ALPHABET_SIZE;        // 26, which stands for the size of Alphabet
        const int CLOCK_MAIN;
        const int CLOCK_TRAIN;
        const int FEATURE_DIM;          // the dimension of feature
        const std::vector<std::string> IP;// A vector which restore the IP for each party
        const std::vector<int> PORT;    // A vector which restore the Port for each party
        const long long SQRTINV;        // The following there parameter is calculated automatically.
        const long long INV2;
        const long long INV2_M;
        const int USE_D;                // Show how many columns will be read in TRAIN/TEST file
        const int LABEL_P;              // In which index are Labels stored 
        const double LEARNING_RATE;     // Learning rate of deep learning
        const std::string TRAIN_FILENAME;// If train and test samples are not in one file, the assign the train file name to this value
        const std::string TEST_FILENAME;// If train and test samples are not in one file, the assign the test file name to this value
        const std::string FILENAME;     // If train and test samples are in one file, the assign the name to this value
        const int TRAIN_TEST_SAME;      // Whether train and test samples are in one file
    private:
        
};
