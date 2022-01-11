//
// Created by tangdingyi on 2019/12/26.
// Modified by chenshuyu on 2021/12/27.
//

#include "IOManager.h"
#include "../core/Mat.h"
#include "SocketManager.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <string>

using namespace std::chrono_literals;

Mat IOManager::train_data;
Mat IOManager::train_label;
Mat IOManager::test_data;
Mat IOManager::test_label;

string IOManager::TEST = "test";
string IOManager::TRAIN = "train";

int IOManager::party_num; // the number of party, except myself
int IOManager::train_n = 0;
int IOManager::test_n = 0;

ll poly_eval(vector<ll> coefficients, ll x)
{
    ll res = coefficients[Config::config->TN - 1];
    for (int l = Config::config->TN - 2; l >= 0; --l)
    {
        res = res * x + coefficients[l];
    }
    return res;
}

Mat IOManager::get_train_data()
{
    return train_data;
}

Mat IOManager::get_test_data()
{
    return test_data;
}

Mat IOManager::get_train_label()
{
    return train_label;
}

Mat IOManager::get_test_label()
{
    return test_label;
}

/**
 * @brief 根据输入文件载入数据，载入 test 数据 或者载入 train 数据（含label）
 * @param in 输入的含有特征和标签的数据文件
 * @param data 载入的矩阵数据
 * @param label 载入的标签
 * @param size 载入的数据条数（一行一条）
 *
 */
void IOManager::load(ifstream &in, Mat &data, Mat &label, int size)
{
    int i = 0;
    while (in)
    {
        string s;
        if (!getline(in, s))
            break;
        char *ch;
        ch = const_cast<char *>(s.c_str());
        int temp;
        char c;
        int begin = 0;

        if (!Config::config->LABEL_P)
        { // 第一列是标签
            temp = Constant::Util::getfixpoint(ch, begin);
            if (temp > Config::config->IE)
            { //转化成二分类应该在main函数里面做
                temp = Config::config->IE;
            }
            label(0, i) = temp;
        }

        // 获得特征值
        data(Config::config->USE_D, i) = Config::config->IE;
        int j;
        for (j = 0; j < Config::config->USE_D; j++)
        {
            temp = Constant::Util::getfixpoint(ch, begin);
            /// TODO: 这里本来有一个除256，是因为是像素数据
            data(j, i) = temp / 256; //归一化应该在main函数里面做，调用mat中的算子
        }

        if (Config::config->LABEL_P) // 最后一列是标签
        {
            if (Config::config->USE_D < Config::config->D)
            { // 没有获取所有的，那么还得往后遍历到最后一个
                for (int k = j; k < Config::config->D; k++)
                {
                    begin = Constant::Util::getNext(ch, begin);
                }
            }
            // else 获取了所有的特征维度，那么下一个位置就是标签值

            temp = Constant::Util::getfixpoint(ch, begin);

            label(0, i) = temp;
        }

        i++;
        if (i >= size)
            break;
    }

    for (i; i < size + Config::config->B - 1; i++)
    {
        int tmp_r;
        tmp_r = data.rows();
        for (int j = 0; j < tmp_r; j++)
        {
            data(j, i) = data(j, i - size);
        }
        tmp_r = label.rows();
        for (int j = 0; j < tmp_r; j++)
        {
            label(j, i) = label(j, i - size);
        }
    }
    DBGprint("n=%d\n", i);
}

void IOManager::secret_share(Mat &data, Mat &label, string category)
{
    vector<ll> coefficients(Config::config->TN);
    //    assert(size*D == data.size());
    vector<ofstream> out_files(Config::config->M);
    for (int i = 0; i < Config::config->M; ++i)
    {
        //        ofstream out_file("mnist/mnist_train_"+to_string(Config::config->M), ios::out);
        //        out_files.push_back(out_file);
        //        string s = "mnist/mnist_train_"+to_string(i)+".csv";
        //        cout << s << endl;
        out_files[i].open("../datasets/mnist/mnist_" + category + "_" + to_string(i) + ".csv", ios::out);
    }
    srand(time(NULL));
    /// 简化计算，TODO，delete

    int r = data.rows();
    int c = data.cols();
    // cout << r << " : " << c << endl;
    for (int i = 0; i < c; ++i)
    {
        for (int j = 1; j < Config::config->TN; ++j)
        {
            coefficients[j] = Constant::Util::randomlong();
        }
        for (int k = 0; k < Config::config->M; ++k)
        {
            for (int j = 0; j < r; ++j)
            {
                if (j == 0)
                {
                    coefficients[0] = label.getVal(i);
                    // cout << i << " : " << poly_eval(coefficients, k + 2) << endl;
                    out_files[k] << poly_eval(coefficients, k + 2) << ",";
                }
                if (j == r - 1)
                {
                    coefficients[0] = data.get(j, i);
                    out_files[k] << poly_eval(coefficients, k + 2) << "\n";
                }
                else
                {
                    coefficients[0] = data.get(j, i);
                    out_files[k] << poly_eval(coefficients, k + 2) << ",";
                }
            }
        }
    }

    for (int k = 0; k < Config::config->M; ++k)
    {
        out_files[k].close();
    }
}

void IOManager::load_ss(ifstream &in, Mat &data, Mat &label, int size)
{
    int i = 0;
    while (in)
    {

        string s;
        if (!getline(in, s))
            break;
        char *ch;
        ch = const_cast<char *>(s.c_str());
        ll temp;
        char c;

        int begin = 0;
        temp = Constant::Util::getll(ch, begin);
        //        if (temp > 1)
        //            temp = 1;
        label(0, i) = temp;

        // int nd = min(D, ND);
        data(Config::config->USE_D, i) = Config::config->IE;
        for (int j = 0; j < Config::config->USE_D; j++)
        {
            temp = Constant::Util::getll(ch, begin);
            data(j, i) = temp;
        }
        i++;
        if (i >= size)
            break;
    }
    for (i; i < size + Config::config->B - 1; i++)
    {
        int tmp_r;
        tmp_r = data.rows();
        for (int j = 0; j < tmp_r; j++)
        {
            data(j, i) = data(j, i - size);
        }
        tmp_r = label.rows();
        for (int j = 0; j < tmp_r; j++)
        {
            label(j, i) = label(j, i - size);
        }
    }
    DBGprint("n=%d\n", i);
}

/** For Markov model **/
Mat *IOManager::secret_share_ngram(int *data, int size, string prefix)
{
    vector<ll> coefficients(Config::config->TN);
    vector<ofstream> out_files(Config::config->M);
    cout << prefix << " size: " << size << endl;

    Mat *res = new Mat[Config::config->M];
    //    Mat res(1, size);
    for (int i = 0; i < Config::config->M; ++i)
    {
        res[i].init(1, size);
        string filename = "output/" + prefix + "_" + to_string(i) + ".csv";
        cout << "File: " << filename << endl;
        out_files[i].open(filename, ios::out);
    }
    /// 简化计算，TODO，delete
    for (int j = 1; j < Config::config->TN; ++j)
    {
        coefficients[j] = Constant::Util::randomlong();
    }
    //    for (int j = 0; j < Config::config->TN; ++j) {
    //        cout << j << ": " << coefficients[j] << endl;
    //    }
    cout << "Size: " << size << endl;
    //    srand(time(0));
    for (int i = 0; i < size; ++i)
    {
        for (int j = 1; j < Config::config->TN; ++j)
        {
            coefficients[j] = Constant::Util::randomlong();
        }
        coefficients[0] = data[i] * Config::config->IE;
        for (int k = 0; k < Config::config->M; ++k)
        {
            ll tmp = poly_eval(coefficients, k + 2);
            out_files[k] << tmp << ",";
            res[k].setVal(i, tmp);
            //            if(k==node_type){
            //                res.setVal(i, tmp);
            //            }
        }
        //        for (int l = 0; l < Config::config->TN; ++l) {
        //            DBGprint("%lld, ", coefficients[l]);
        //        }
        //        cout << endl;
    }
    //    res[0].print();
    return res;
}

/** For Decision Tree Bitmap share **/
Mat *IOManager::secret_share_vector(int *data, int size)
{
    vector<ll> coefficients(Config::config->TN);

    Mat *res = new Mat[Config::config->M];
    /// 简化计算，TODO，delete
    for (int j = 1; j < Config::config->TN; ++j)
    {
        coefficients[j] = Constant::Util::randomlong();
    }
    cout << "Size: " << size << endl;
    //    srand(time(0));
    for (int l = 0; l < Config::config->M; ++l)
    {
        res[l].init(size, 1);
    }
    for (int i = 0; i < size; ++i)
    {
        for (int j = 1; j < Config::config->TN; ++j)
        {
            coefficients[j] = Constant::Util::randomlong();
        }
        coefficients[0] = data[i] * Config::config->IE;
        for (int k = 0; k < Config::config->M; ++k)
        {
            ll tmp = poly_eval(coefficients, k + 2);
            res[k].setVal(i, tmp);
        }
    }
    return res;
}

/** For K-V statics **/
Mat *IOManager::secret_share_kv_data(int *data, int size, string prefix, bool isFreq)
{
    vector<ll> coefficients(Config::config->TN);
    vector<ofstream> out_files(Config::config->M);
    cout << prefix << " size: " << size << endl;

    Mat *res = new Mat[Config::config->M];
    //    Mat res(1, size);
    for (int i = 0; i < Config::config->M; ++i)
    {
        res[i].init(1, size);
        string filename = "output/" + prefix + "_" + to_string(i) + ".csv";
        cout << "File: " << filename << endl;
        out_files[i].open(filename, ios::out);
    }
    /// 简化计算，TODO，delete
    for (int j = 1; j < Config::config->TN; ++j)
    {
        coefficients[j] = Constant::Util::randomlong();
    }
    //    for (int j = 0; j < Config::config->TN; ++j) {
    //        cout << j << ": " << coefficients[j] << endl;
    //    }
    for (int i = 0; i < size; ++i)
    {
        for (int j = 1; j < Config::config->TN; ++j)
        {
            coefficients[j] = Constant::Util::randomlong();
        }
        if (isFreq)
        {
            if (data[i] > 0)
            {
                coefficients[0] = Config::config->IE;
            }
            else
            {
                coefficients[0] = 0;
            }
        }
        else
        {
            coefficients[0] = data[i] * Config::config->IE;
        }

        for (int k = 0; k < Config::config->M; ++k)
        {
            ll tmp = poly_eval(coefficients, k + 2);
            out_files[k] << tmp << ",";
            res[k].setVal(i, tmp);
            //            if(k==node_type){
            //                res.setVal(i, tmp);
            //            }
        }
        //        for (int l = 0; l < Config::config->TN; ++l) {
        //            DBGprint("%lld, ", coefficients[l]);
        //        }
        //        cout << endl;
    }
    //    res[0].print();
    return res;
}

/** For Decision Tree **/
Mat *IOManager::secret_share_mat_data(Mat &data, int size)
{
    vector<ll> coefficients(Config::config->TN);
    int r = data.rows();
    int c = data.cols();

    vector<ofstream> out_files(Config::config->M);
    Mat *res = new Mat[Config::config->M];
    for (int i = 0; i < Config::config->M; ++i)
    {
        res[i].init(r, c);
        //        ofstream out_file("mnist/mnist_train_"+to_string(Config::config->M), ios::out);
        //        out_files.push_back(out_file);
        //        string s = "mnist/mnist_train_"+to_string(i)+".csv";
        //        cout << s << endl;
        out_files[i].open("dt_" + to_string(i) + ".csv", ios::out);
    }
    /// 简化计算，TODO，delete

    // cout << r << " : " << c << endl;
    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            coefficients[0] = data.get(i, j) * Config::config->IE;
            for (int l = 1; l < Config::config->TN; ++l)
            {
                coefficients[l] = Constant::Util::randomlong();
            }
            for (int k = 0; k < Config::config->M; ++k)
            {
                ll tmp = poly_eval(coefficients, k + 2);
                res[k].setVal(j * r + i, tmp);
                if (j == r - 1)
                {
                    out_files[k] << tmp << "\n";
                }
                else
                {
                    out_files[k] << poly_eval(coefficients, k + 2) << ",";
                }
            }
        }
    }
    for (int k = 0; k < Config::config->M; ++k)
    {
        out_files[k].close();
    }
    return res;
}

void IOManager::init_mat()
{
    train_data.init(Config::config->D + 1, Config::config->N + Config::config->B - 1);
    train_label.init(1, Config::config->N + Config::config->B - 1);
    test_data.init(Config::config->D + 1, Config::config->NM + Config::config->B - 1);
    test_label.init(1, Config::config->NM + Config::config->B - 1);
}

// test和train放在同一个文件中
void IOManager::load()
{
    DBGprint("load training data......\n");
    if (Config::config->TRAIN_TEST_SAME)
    {
        ifstream infile(Config::config->FILENAME);

        load(infile, train_data, train_label, Config::config->N);
        load(infile, test_data, test_label, Config::config->NM);

        infile.close();
    }
    else
    {
        ifstream infile(Config::config->TRAIN_FILENAME);
        load(infile, train_data, train_label, Config::config->N);
        infile.close();

        ifstream intest(Config::config->TEST_FILENAME);
        load(intest, test_data, test_label, Config::config->NM);
        intest.close();
    }
    // secret_share(train_data, train_label, TRAIN);
    // secret_share(test_data, test_label, TEST);
}

// Get the time period from the last modification of the file to the present
ll128 get_file_modified_time2now(string filepath, auto now)
{
    ll128 ftime = (now - std::filesystem::last_write_time(filepath).time_since_epoch()).count(); // read back from the filesyste
    return ftime;
}

// according to filename,get the cache filenames
void get_cache_name(string cachefiles[])
{
    int i;
    if (Config::config->TRAIN_TEST_SAME)
    {
        int i;
        string filename = Config::config->FILENAME;
        for (i = filename.length() - 1; i >= 0; i--)
        {
            if (filename[i] == '.')
                break;
        }
        string basename = filename.substr(0, i);
        cachefiles[0] = basename + "_traind.dat";
        cachefiles[1] = basename + "_trainl.dat";
        cachefiles[2] = basename + "_testd.dat";
        cachefiles[3] = basename + "_testl.dat";
    }
    else
    {
        string train_filename = Config::config->TRAIN_FILENAME;
        for (i = train_filename.length() - 1; i >= 0; i--)
        {
            if (train_filename[i] == '.')
                break;
        }
        string basename = train_filename.substr(0, i);
        cachefiles[0] = basename + "_traind.dat";
        cachefiles[1] = basename + "_trainl.dat";

        string test_filename = Config::config->TEST_FILENAME;
        for (i = test_filename.length() - 1; i >= 0; i--)
        {
            if (test_filename[i] == '.')
                break;
        }
        basename = test_filename.substr(0, i);
        cachefiles[2] = basename + "_testd.dat";
        cachefiles[3] = basename + "_testl.dat";
    }
}

// Jude whether there is a cache
bool judgecache()
{
    string cachefiles[4];
    get_cache_name(cachefiles);

    if (!std::filesystem::exists(cachefiles[0]))
    {
        return false;
    }

    // get the right now time
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();

    // if the modified time of filename is smaller than the cache file, so the cache is valid, return true
    if (Config::config->TRAIN_TEST_SAME)
    {
        return get_file_modified_time2now(Config::config->FILENAME, now) > get_file_modified_time2now(cachefiles[0], now) ? true : false;
    }
    else
    {
        return get_file_modified_time2now(Config::config->TRAIN_FILENAME, now) > get_file_modified_time2now(cachefiles[0], now) ? true : false;
    }
}

// convert the content of mat to the binary cache ile
// filename: xxx.dat
void tocache(Mat &mat, string filename)
{
    ofstream cachefile(filename, ios::out | ios::binary);
    int r = mat.rows();
    int c = mat.cols();
    int order = mat.getorder();
    cachefile.write((char *)&r, sizeof(r));
    cachefile.write((char *)&c, sizeof(c));
    cachefile.write((char *)&order, sizeof(order));
    ll128 temp;
    vector<ll128> v = mat.get_val();
    for (int i = 0; i < r * c; i++) // every row
    {
        temp = v[i];
        cachefile.write((char *)&temp, sizeof(temp));
    }
    cachefile.close();
}

// load the cache from filename to mat
// filename: xxx.dat
void load_cache(string filename, Mat &mat)
{
    ifstream cachefile(filename, ios::in | ios::binary);
    try
    {
        int r, c, order = 0;
        cachefile.read((char *)&r, sizeof(int));
        cachefile.read((char *)&c, sizeof(int));
        cachefile.read((char *)&order, sizeof(int));
        mat.setrow(r);
        mat.setcol(c);
        mat.setorder(order);
        vector<ll128> v;
        ll128 temp;
        for (int i = 0; i < r * c; i++)
        {
            cachefile.read((char *)&temp, sizeof(temp));
            v.push_back(temp);
        }
        mat.setVal(v);

        // cachefile.read((char *)&mat, mat.get_memory_size());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << " error\n";
    }
    cachefile.close();
}

void IOManager::cache_all_mat()
{
    // get the name list of cache files
    string cachefiles[4];
    get_cache_name(cachefiles);

    tocache(train_data, cachefiles[0]);
    tocache(train_label, cachefiles[1]);
    tocache(test_data, cachefiles[2]);
    tocache(test_label, cachefiles[3]);
}

void IOManager::load_all_cache_to_mat()
{
    // get the name list of cache files
    string cachefiles[4];
    get_cache_name(cachefiles);

    load_cache(cachefiles[0], train_data);
    load_cache(cachefiles[1], train_label);
    load_cache(cachefiles[2], test_data);
    load_cache(cachefiles[3], test_label);
}

// remove all cache
void IOManager::remove_cache()
{
    // get the name list of cache files
    string cachefiles[4];
    get_cache_name(cachefiles);

    for (string cachefile : cachefiles)
    {
        if (std::filesystem::exists(cachefile))
        {
            std::filesystem::remove(cachefile);
        }
    }
}

void IOManager::init_local_data()
{
    // init variable
    IOManager::party_num = Config::config->M; // the number of party, except myself
    IOManager::train_n = 0;
    IOManager::test_n = 0;

    init_mat();

    if (judgecache()) // If there exits a cache, load the cached binary file into mat
    {
        load_all_cache_to_mat();
    }
    else
    {
        load();
        cache_all_mat();
    }
}

// exchange one mat and merge the mat to get the new mat
void IOManager::exchange_mat(Mat &mat)
{
    // the orginal is supposed to be secret shared
    // so generate the secret mats for each mat
    // in fact ,there could be constructed into array, and use foreach to get train and test

    Mat mats[party_num];

    // according the orginal mat to get secret mats
    // mat.get_secret_share(mats, party_num);
    cout << party_num << endl;

    // send secret local data
    // for (int i = 0; i < party_num; i++)
    // {
    //     if (i != node_type) // except myself
    //     {
    //         cout << i << endl;
    //         // SocketManager::send(node_type, i, mats[i]);
    //         SocketManager::send(node_type, i, mat);
    //     }
    // }

    for (int i = 0; i < node_type; i++)
    {

        cout << i << endl;
        // SocketManager::send(node_type, i, mats[i]);
        SocketManager::receive(node_type, i, mats[i]);
    }

    for (int i = 0; i < party_num; i++)
    {
        if (i != node_type) // except myself
        {
            cout << i << endl;
            // SocketManager::send(node_type, i, mats[i]);
            SocketManager::send(node_type, i, mat);
        }
    }
    // receive secret data from other parties
    for (int i = node_type + 1; i < party_num; i++)
    {
        // delete the original data
        SocketManager::receive(node_type, i, mats[i]);
    }

    // merge all secret mats
    mat.merge_mats(mats, party_num);

    // mat.print_part();
}

void IOManager::exchange_data()
{

    // exchange the four mats and get new mats
    cout << "exchange data" << endl;
    exchange_mat(train_data);
    exchange_mat(train_label);
    exchange_mat(test_data);
    exchange_mat(test_label);

    // update the sum of train line and test line
    train_n = train_data.rows();
    test_n = test_data.rows();
}

void IOManager::init()
{
    init_local_data();
    exchange_data();
}