//
// Created by tangdingyi on 2019/12/26.
//

#ifndef MPC_ML_IOMANAGER_H
#define MPC_ML_IOMANAGER_H
#include <cassert>
#include "../core/Mat.h"
#include "../Constant.h"
// extern int node_type;
class IOManager
{
public:
    static Mat train_data, train_label;
    static Mat test_data, test_label;

    static void init(string filename);
    static void init(string train_filename, string test_filename);

    static void remove_cache(string filename);
    static void remove_cache(string train_filename, string test_filename);

    static Mat get_train_data();
    static Mat get_test_data();
    static Mat get_train_label();
    static Mat get_test_label();

private:
    static void secret_share(Mat &data, Mat &label, string category);
    static void load_ss(ifstream &in, Mat &data, Mat &label, int size);
    static Mat *secret_share_ngram(int *data, int size, string prefix);
    static Mat *secret_share_vector(int *data, int size);
    static Mat *secret_share_kv_data(int *data, int size, string prefix, bool isFreq);
    static Mat *secret_share_mat_data(Mat &data, int size);
    static void init_mat();
    static void load(ifstream &in, Mat &data, Mat &label, int size);

    // load the train and test data from filename
    static void load(string train_filename, string test_filename);
    static void load(string filename);

    // load cache file under basedir to the mat
    static void load_all_cache_to_mat(string train_filename, string test_filename);
    static void load_all_cache_to_mat(string filename);

    // // convert the content of mat to the binary cache ile
    // static void tocache(Mat &mat, string filename);

    // // load the cache
    // static void load_cache(string filename, Mat &mat);

    static void cache_all_mat(string train_filename, string test_filename);
    static void cache_all_mat(string filename);

private:
    static string TEST;
    static string TRAIN;
};

#endif // MPC_ML_IOMANAGER_H
