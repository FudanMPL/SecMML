//
// Created by tangdingyi on 2019/12/26.
//

#ifndef MPC_ML_IOMANAGER_H
#define MPC_ML_IOMANAGER_H
#include <cassert>
#include "../core/Mat.h"
#include "../Constant.h"
<<<<<<< HEAD
=======
#include "SocketManager.h"
>>>>>>> dev
// extern int node_type;
class IOManager {
public:
    static Mat train_data, train_label;
    static Mat test_data, test_label;
<<<<<<< HEAD
    static void load(ifstream& in, Mat& data, Mat& label, int size);
    static void secret_share(Mat& data, Mat& label, string category);
    static void load_ss(ifstream& in, Mat& data, Mat& label, int size);
    static Mat* secret_share_ngram(int* data, int size, string prefix);
    static Mat* secret_share_vector(int* data, int size);
    static Mat* secret_share_kv_data(int* data, int size, string prefix, bool isFreq);
    static Mat* secret_share_mat_data(Mat &data, int size);
    static void init();
=======

    static void init();
    static void init_local_data();
    static void exchange_data();

    static void remove_cache();

    static Mat get_train_data();
    static Mat get_test_data();
    static Mat get_train_label();
    static Mat get_test_label();
    static int get_train_n();
    static int get_test_n();

private:
    static void secret_share(Mat &data, Mat &label, string category);
    static void load_ss(ifstream &in, Mat &data, Mat &label, int size);
    static Mat *secret_share_ngram(int *data, int size, string prefix);
    static Mat *secret_share_vector(int *data, int size);
    static Mat *secret_share_kv_data(int *data, int size, string prefix, bool isFreq);
    static Mat *secret_share_mat_data(Mat &data, int size);
    static void init_mat();
    static void exchange_mat(Mat &mat);
    static void load(ifstream &in, Mat &data, Mat &label, int size);

    // load the train and test data from filename
    static void load();
    // static void load(string filename);

    // load cache file under basedir to the mat
    static void load_all_cache_to_mat();

    // // convert the content of mat to the binary cache ile
    // static void tocache(Mat &mat, string filename);

    // // load the cache
    // static void load_cache(string filename, Mat &mat);

    static void cache_all_mat();

private:
    static string TEST;
    static string TRAIN;

    static int party_num;
    static int train_n;
    static int test_n;
>>>>>>> dev
};


#endif //MPC_ML_IOMANAGER_H
