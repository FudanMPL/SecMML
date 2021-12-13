//
// Created by tangdingyi on 2019/12/26.
//

#include "IOManager.h"

// Mat train_data(N,D), train_label(N,1);
// Mat test_data(NM,D), test_label(NM,1);
//  B 128          Batch size
//  USE_D 784      the real used Dimension
//  N 60000        size of train dataset
//  NM 10000       size of test dataset

Mat IOManager::train_data = Mat(D + 1, N + B - 1);
Mat IOManager::train_label = Mat(1, N + B - 1);
Mat IOManager::test_data = Mat(D + 1, NM + B - 1);
Mat IOManager::test_label = Mat(1, NM + B - 1);

// 根据参数值，算出秘密共享值的大小
ll poly_eval(vector<ll> coefficients, ll x)
{
    ll res = coefficients[TN - 1];
    for (int l = TN - 2; l >= 0; --l)
    {
        res = res * x + coefficients[l];
    }
    return res;
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

        if (!LABEL_P)
        { // 第一列是标签
            temp = Constant::Util::getint(ch, begin);
            label(0, i) = temp * IE;
        }

        // 获得特征值
        data(USE_D, i) = IE;
        int j;
        for (j = 0; j < USE_D; j++)
        {
            temp = Constant::Util::getint(ch, begin);
            data(j, i) = temp * IE / 256; // 这里可以改成位移   
        }

        if (LABEL_P)// 最后一列是标签
        {
            if (USE_D < D)
            { // 没有获取所有的，那么还得往后遍历到最后一个
                for (int k = j; k < D; k++)
                {
                    begin = Constant::Util::getNext(ch, begin);
                }
            }
            // else 获取了所有的特征维度，那么下一个位置就是标签值

            temp = Constant::Util::getint(ch, begin);
            label(0, i) = temp * IE;
        }

        i++;
        if (i >= size)
            break;
    }

    for (i; i < size + B - 1; i++)
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
    vector<ll> coefficients(TN);
    //    assert(size*D == data.size());
    vector<ofstream> out_files(M);
    for (int i = 0; i < M; ++i)
    {
        //        ofstream out_file("mnist/mnist_train_"+to_string(M), ios::out);
        //        out_files.push_back(out_file);
        //        string s = "mnist/mnist_train_"+to_string(i)+".csv";
        //        cout << s << endl;
        out_files[i].open("mnist/mnist_" + category + "_" + to_string(i) + ".csv", ios::out);
    }
    srand(time(NULL));
    /// 简化计算，TODO，delete

    int r = data.rows();
    int c = data.cols();
    cout << r << " : " << c << endl;
    for (int i = 0; i < c; ++i)
    {
        for (int j = 1; j < TN; ++j)
        {
            coefficients[j] = Constant::Util::randomlong();
        }
        for (int k = 0; k < M; ++k)
        {
            for (int j = 0; j < r; ++j)
            {
                if (j == 0)
                {
                    coefficients[0] = label.getVal(i);
                    cout << i << " : " << poly_eval(coefficients, k + 2) << endl;
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

    for (int k = 0; k < M; ++k)
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
        data(USE_D, i) = IE;
        for (int j = 0; j < USE_D; j++)
        {
            temp = Constant::Util::getll(ch, begin);
            data(j, i) = temp;
        }
        i++;
        if (i >= size)
            break;
    }
    for (i; i < size + B - 1; i++)
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
    vector<ll> coefficients(TN);
    vector<ofstream> out_files(M);
    cout << prefix << " size: " << size << endl;

    Mat *res = new Mat[M];
    //    Mat res(1, size);
    for (int i = 0; i < M; ++i)
    {
        res[i].init(1, size);
        string filename = "output/" + prefix + "_" + to_string(i) + ".csv";
        cout << "File: " << filename << endl;
        out_files[i].open(filename, ios::out);
    }
    /// 简化计算，TODO，delete
    for (int j = 1; j < TN; ++j)
    {
        coefficients[j] = Constant::Util::randomlong();
    }
    //    for (int j = 0; j < TN; ++j) {
    //        cout << j << ": " << coefficients[j] << endl;
    //    }
    cout << "Size: " << size << endl;
    //    srand(time(0));
    for (int i = 0; i < size; ++i)
    {
        for (int j = 1; j < TN; ++j)
        {
            coefficients[j] = Constant::Util::randomlong();
        }
        coefficients[0] = data[i] * IE;
        for (int k = 0; k < M; ++k)
        {
            ll tmp = poly_eval(coefficients, k + 2);
            out_files[k] << tmp << ",";
            res[k].setVal(i, tmp);
            //            if(k==node_type){
            //                res.setVal(i, tmp);
            //            }
        }
        //        for (int l = 0; l < TN; ++l) {
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
    vector<ll> coefficients(TN);

    Mat *res = new Mat[M];
    /// 简化计算，TODO，delete
    for (int j = 1; j < TN; ++j)
    {
        coefficients[j] = Constant::Util::randomlong();
    }
    cout << "Size: " << size << endl;
    //    srand(time(0));
    for (int l = 0; l < M; ++l)
    {
        res[l].init(size, 1);
    }
    for (int i = 0; i < size; ++i)
    {
        for (int j = 1; j < TN; ++j)
        {
            coefficients[j] = Constant::Util::randomlong();
        }
        coefficients[0] = data[i] * IE;
        for (int k = 0; k < M; ++k)
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
    vector<ll> coefficients(TN);
    vector<ofstream> out_files(M);
    cout << prefix << " size: " << size << endl;

    Mat *res = new Mat[M];
    //    Mat res(1, size);
    for (int i = 0; i < M; ++i)
    {
        res[i].init(1, size);
        string filename = "output/" + prefix + "_" + to_string(i) + ".csv";
        cout << "File: " << filename << endl;
        out_files[i].open(filename, ios::out);
    }
    /// 简化计算，TODO，delete
    for (int j = 1; j < TN; ++j)
    {
        coefficients[j] = Constant::Util::randomlong();
    }
    //    for (int j = 0; j < TN; ++j) {
    //        cout << j << ": " << coefficients[j] << endl;
    //    }
    for (int i = 0; i < size; ++i)
    {
        for (int j = 1; j < TN; ++j)
        {
            coefficients[j] = Constant::Util::randomlong();
        }
        if (isFreq)
        {
            if (data[i] > 0)
            {
                coefficients[0] = IE;
            }
            else
            {
                coefficients[0] = 0;
            }
        }
        else
        {
            coefficients[0] = data[i] * IE;
        }

        for (int k = 0; k < M; ++k)
        {
            ll tmp = poly_eval(coefficients, k + 2);
            out_files[k] << tmp << ",";
            res[k].setVal(i, tmp);
            //            if(k==node_type){
            //                res.setVal(i, tmp);
            //            }
        }
        //        for (int l = 0; l < TN; ++l) {
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
    vector<ll> coefficients(TN);
    int r = data.rows();
    int c = data.cols();

    vector<ofstream> out_files(M);
    Mat *res = new Mat[M];
    for (int i = 0; i < M; ++i)
    {
        res[i].init(r, c);
        //        ofstream out_file("mnist/mnist_train_"+to_string(M), ios::out);
        //        out_files.push_back(out_file);
        //        string s = "mnist/mnist_train_"+to_string(i)+".csv";
        //        cout << s << endl;
        out_files[i].open("dt_" + to_string(i) + ".csv", ios::out);
    }
    /// 简化计算，TODO，delete

    cout << r << " : " << c << endl;
    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            coefficients[0] = data.get(i, j) * IE;
            for (int l = 1; l < TN; ++l)
            {
                coefficients[l] = Constant::Util::randomlong();
            }
            for (int k = 0; k < M; ++k)
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
    for (int k = 0; k < M; ++k)
    {
        out_files[k].close();
    }
    return res;
}

// test和train放在同一个文件中
void IOManager::init(string filename)
{
    DBGprint("load training data......\n");

    ifstream infile(filename);

    load(infile, train_data, train_label, N);
    load(infile, test_data, test_label, NM);

    infile.close();
}

// train 和 test 数据放在两个文件中
void IOManager::init(string train_filename, string test_filename)
{
    DBGprint("load training data......\n");

    ifstream infile(train_filename);
    //    ifstream infile( "datasets/mnist/mnist_train.csv" );
    load(infile, train_data, train_label, N);
    //    secret_share(train_data, train_label, "train");

    //    ifstream infile( "mnist/mnist_train_"+to_string(node_type)+".csv" );
    //    load_ss(infile, train_data, train_label, N);
    infile.close();

    ifstream intest(test_filename);
    //    ifstream intest( "datasets/mnist/mnist_test.csv" );
    load(intest, test_data, test_label, NM);
    //    secret_share(test_data, test_label, "test");
    //    ifstream intest( "mnist/mnist_test_"+to_string(node_type)+".csv" );
    //    load_ss(intest, test_data, test_label, NM);
    intest.close();

    /// TODO: secret sharing save file

    //    train_data.reoeder();
    //    train_label.reoeder();
    //    test_data.reoeder();
    //    test_label.reoeder();
}