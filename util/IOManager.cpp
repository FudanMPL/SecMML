//
// Created by tangdingyi on 2019/12/26.
// Modified by chenshuyu on 2021/12/27.
//

#include "IOManager.h"
#include "../core/Mat.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <string>

using namespace std::chrono_literals;

// Mat train_data(N,D), train_label(N,1);
// Mat test_data(NM,D), test_label(NM,1);

// Mat IOManager::train_data = Mat(Config::config->D + 1, Config::config->N + Config::config->B - 1);
// Mat IOManager::train_label = Mat(1, Config::config->N + Config::config->B - 1);
// Mat IOManager::test_data = Mat(Config::config->D + 1, Config::config->NM + Config::config->B - 1);
// Mat IOManager::test_label = Mat(1, Config::config->NM + Config::config->B - 1);
Mat IOManager::train_data;
Mat IOManager::train_label;
Mat IOManager::test_data;
Mat IOManager::test_label;

string IOManager::TEST = "test";
string IOManager::TRAIN = "train";

// Mat IOManager::train_data = Mat(D + 1, N + B - 1);
// Mat IOManager::train_label = Mat(1, N + B - 1);
// Mat IOManager::test_data = Mat(D + 1, NM + B - 1);
// Mat IOManager::test_label = Mat(1, NM + B - 1);

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
void IOManager::load(string filename)
{
    DBGprint("load training data......\n");

    ifstream infile(filename);

    load(infile, train_data, train_label, Config::config->N);
    load(infile, test_data, test_label, Config::config->NM);

    infile.close();
}

// train 和 test 数据放在两个文件中
void IOManager::load(string train_filename, string test_filename)
{
    DBGprint("load training data......\n");

    ifstream infile(train_filename);
    load(infile, train_data, train_label, Config::config->N);
    secret_share(train_data, train_label, TRAIN);
    // secret_share(train_data, train_label, "train");

    // ifstream infile( "../datasets/mnist/mnist_train_"+to_string(node_type)+".csv" );
    // load_ss(infile, train_data, train_label, Config::config->N);
    infile.close();

    ifstream intest(test_filename);
    load(intest, test_data, test_label, Config::config->NM);
    secret_share(test_data, test_label, TEST);
    // secret_share(test_data, test_label, "test");
    // ifstream intest( "../datasets/mnist/mnist_test_"+to_string(node_type)+".csv" );
    // load_ss(intest, test_data, test_label, Config::config->NM);
    intest.close();

    /// TODO: secret sharing save file

    //    train_data.reoeder();
    //    train_label.reoeder();
    //    test_data.reoeder();
    //    test_label.reoeder();
}

// Get the time period from the last modification of the file to the present
ll128 get_file_modified_time2now(string filepath, auto now)
{
    ll128 ftime = (now - std::filesystem::last_write_time(filepath).time_since_epoch()).count(); // read back from the filesyste
    return ftime;
}

// according to filename,get the cache filenames
void get_cache_name(string filename, string cachefiles[])
{
    int i;
    for (i = filename.length() - 1; i >= 0; i--)
    {
        if (filename[i] == '.')
            break;
    }
    string basename = filename.substr(0, i);
    cachefiles[0] = basename + "_traind.bat";
    cachefiles[1] = basename + "_trainl.bat";
    cachefiles[2] = basename + "_testd.bat";
    cachefiles[3] = basename + "_testl.bat";
}

void get_cache_name(string train_filename, string test_filename, string cachefiles[])
{
    int i;
    for (i = train_filename.length() - 1; i >= 0; i--)
    {
        if (train_filename[i] == '.')
            break;
    }
    string basename = train_filename.substr(0, i);
    cachefiles[0] = basename + "_traind.bat";
    cachefiles[1] = basename + "_trainl.bat";

    for (i = test_filename.length() - 1; i >= 0; i--)
    {
        if (test_filename[i] == '.')
            break;
    }
    basename = test_filename.substr(0, i);
    cachefiles[2] = basename + "_testd.bat";
    cachefiles[3] = basename + "_testl.bat";
}

// Jude whether there is a cache
bool judgecache(string filename)
{
    string cachefiles[4];
    get_cache_name(filename, cachefiles);

    if (!std::filesystem::exists(cachefiles[0]))
    {
        return false;
    }

    // get the right now time
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();

    // if the modified time of filename is smaller than the cache file, so the cache is valid, return true
    return get_file_modified_time2now(filename, now) > get_file_modified_time2now(cachefiles[0], now) ? true : false;
}

// convert the content of mat to the binary cache ile
// filename: xxx.bat
void tocache(Mat &mat, string filename)
{
    ofstream cachefile(filename, ios::out | ios::binary);
    cachefile.write((char *)&mat, sizeof(mat));
    cachefile.close();
}

// load the cache from filename to mat
// filename: xxx.bat
void load_cache(string filename, Mat &mat)
{
    ifstream cachefile(filename, ios::in | ios::binary);
    try
    {
        cachefile.read((char *)&mat, sizeof(mat));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << " error\n";
    }
    cachefile.close();
}

void IOManager::cache_all_mat(string filename)
{
    // get the name list of cache files
    string cachefiles[4];
    get_cache_name(filename, cachefiles);

    tocache(train_data, cachefiles[0]);
    tocache(train_label, cachefiles[1]);
    tocache(test_data, cachefiles[2]);
    tocache(test_label, cachefiles[3]);
}

void IOManager::cache_all_mat(string train_filename, string test_filename)
{
    // get the name list of cache files
    string cachefiles[4];
    get_cache_name(train_filename, test_filename, cachefiles);

    tocache(train_data, cachefiles[0]);
    tocache(train_label, cachefiles[1]);
    tocache(test_data, cachefiles[2]);
    tocache(test_label, cachefiles[3]);
}

void IOManager::load_all_cache_to_mat(string filename)
{
    // get the name list of cache files
    string cachefiles[4];
    get_cache_name(filename, cachefiles);

    load_cache(cachefiles[0], train_data);
    load_cache(cachefiles[1], train_label);
    load_cache(cachefiles[2], test_data);
    load_cache(cachefiles[3], test_label);
}

void IOManager::load_all_cache_to_mat(string train_filename, string test_filename)
{
    // get the name list of cache files
    string cachefiles[4];
    get_cache_name(train_filename, test_filename, cachefiles);

    load_cache(cachefiles[0], train_data);
    load_cache(cachefiles[1], train_label);
    load_cache(cachefiles[2], test_data);
    load_cache(cachefiles[3], test_label);
}

// remove all cache
void IOManager::remove_cache(string filename)
{
    // get the name list of cache files
    string cachefiles[4];
    get_cache_name(filename, cachefiles);

    for (string cachefile : cachefiles)
    {
        if (std::filesystem::exists(cachefile))
        {
            std::filesystem::remove(cachefile);
        }
    }
}

// remove all cache
void IOManager::remove_cache(string train_filename, string test_filename)
{
    // get the name list of cache files
    string cachefiles[4];
    get_cache_name(train_filename, test_filename, cachefiles);

    for (string cachefile : cachefiles)
    {
        if (std::filesystem::exists(cachefile))
        {
            std::filesystem::remove(cachefile);
        }
    }
}

void IOManager::init(string filename)
{
    init_mat();

    if (judgecache(filename)) // If there exits a cache, load the cached binary file into mat
    {
        load_all_cache_to_mat(filename);
    }
    else
    {
        load(filename);
        cache_all_mat(filename);
    }
}

void IOManager::init(string train_filename, string test_filename)
{
    init_mat();
    // load(train_filename, test_filename);
    if (judgecache(train_filename)) // If there exits a cache, load the cached binary file into matƒ
    {
        load_all_cache_to_mat(train_filename, test_filename);
    }
    else
    {
        load(train_filename, test_filename);
        cache_all_mat(train_filename, test_filename);
    }
}