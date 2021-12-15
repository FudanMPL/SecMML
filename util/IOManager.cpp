//
// Created by tangdingyi on 2019/12/26.
//

#include "IOManager.h"

//Mat train_data(N,D), train_label(N,1);
//Mat test_data(NM,D), test_label(NM,1);

Mat IOManager::train_data = Mat(Config::config->D + 1,Config::config->N + Config::config->B - 1);
Mat IOManager::train_label = Mat(1, Config::config->N + Config::config->B - 1);
Mat IOManager::test_data = Mat(Config::config->D + 1, Config::config->NM + Config::config->B - 1);
Mat IOManager::test_label = Mat(1, Config::config->NM + Config::config->B - 1);

ll poly_eval(vector<ll>coefficients, ll x) {
    ll res = coefficients[Config::config->TN-1];
    for (int l = Config::config->TN-2; l >= 0; --l) {
        res = res * x + coefficients[l];
    }
    return res;
}
void IOManager::load(ifstream &in, Mat &data, Mat &label, int size) {
    int i=0;
    while(in){

        string s;
        if (!getline(in,s))
            break;
        char* ch;
        ch = const_cast<char *>(s.c_str());
        int temp;
        char c;

        temp = Constant::Util::getint(ch);
        if (temp > 1)
            temp = 1;
        label(0, i) = temp * Config::config->IE;

//        if (i == 3034)
//            printf("%d: %d\n", i, temp);

        int nd = min(Config::config->D, Config::config->ND);
        data(nd, i) = Config::config->IE;
        for(int j=0;j<nd;j++){
            temp = Constant::Util::getint(ch);
            data(j, i) = temp * Config::config->IE / 256;
//            if (!i) {
//                printf("%d ", temp);
//                data(i, j).print();
//                DBGprint(" ");
//            }
        }
//        if (!i)
//            printf("\n");

        i++;
        if (i >= size)
            break;
//        if (i >= 5)
//            break;
//            printf("%d ", i);
//        DBGprint("%d ", i);
    }
//    cout<<"n= "<<i<<endl;
    for (i; i < size + Config::config->B - 1; i++) {
        int tmp_r;
        tmp_r = data.rows();
        for (int j = 0; j < tmp_r; j++) {
            data(j, i) = data(j, i - size);
        }
        tmp_r = label.rows();
        for (int j = 0; j < tmp_r; j++) {
            label(j, i) = label(j, i - size);
        }
    }
    DBGprint("n=%d\n", i);
}

void IOManager::secret_share(Mat &data, Mat &label, string category) {
    vector<ll> coefficients(Config::config->TN);
//    assert(size*D == data.size());
    vector<ofstream> out_files(Config::config->M);
    for (int i = 0; i < Config::config->M; ++i) {
//        ofstream out_file("mnist/mnist_train_"+to_string(Config::config->M), ios::out);
//        out_files.push_back(out_file);
//        string s = "mnist/mnist_train_"+to_string(i)+".csv";
//        cout << s << endl;
        out_files[i].open("mnist/mnist_"+category+"_"+to_string(i)+".csv", ios::out);
    }
    srand(time(NULL));
    /// 简化计算，TODO，delete

    int r = data.rows();
    int c = data.cols();
    cout << r << " : " << c << endl;
    for (int i = 0; i < c; ++i) {
        for (int j = 1; j < Config::config->TN; ++j) {
            coefficients[j] = Constant::Util::randomlong();
        }
        for (int k = 0; k < Config::config->M; ++k) {
            for (int j = 0; j < r; ++j) {
                if (j == 0) {
                    coefficients[0] = label.getVal(i);
                    cout << i << " : " << poly_eval(coefficients, k + 2) << endl;
                    out_files[k] << poly_eval(coefficients, k + 2) << ",";
                }
                if (j == r-1) {
                    coefficients[0] = data.get(j, i);
                    out_files[k] << poly_eval(coefficients, k + 2) << "\n";
                } else{
                    coefficients[0] = data.get(j, i);
                    out_files[k] << poly_eval(coefficients, k + 2) << ",";
                }
            }
//            out_files[k] << "\n";
        }
    }
//    for (int i = 0; i < size; ++i) {
//        for (int j = 1; j < Config::config->TN; ++j) {
//            coefficients[j] = Constant::Util::randomlong();
//        }
//
//        if ( i % data.rows() == 0) {
//            for (int k = 0; k < Config::config->M; ++k) {
//                if (i != 0) {
//                    out_files[k] << "asd\n";
//                    cout << i << endl;
//                }
//                coefficients[0] = label.getVal(i / data.rows());
//                out_files[k] << poly_eval(coefficients, k + 2) << ",";
//            }
//        }
//        for (int k = 0; k < Config::config->M; ++k) {
//            coefficients[0] = data.getVal(i);
//            if (i%data.rows() == 0) {
//
//            }
//            out_files[k] << poly_eval(coefficients, k+2) << ",";
//        }
//
//    }
    for (int k = 0; k < Config::config->M; ++k) {
        out_files[k].close();
    }

}

void IOManager::load_ss(ifstream &in, Mat &data, Mat &label, int size) {
    int i=0;
    while(in){

        string s;
        if (!getline(in,s))
            break;
        char* ch;
        ch = const_cast<char *>(s.c_str());
        ll temp;
        char c;

        temp = Constant::Util::getll(ch);
//        if (temp > 1)
//            temp = 1;
        label(0, i) = temp;

        int nd = min(Config::config->D, Config::config->ND);
        data(nd, i) = Config::config->IE;
        for(int j=0;j<nd;j++) {
            temp = Constant::Util::getll(ch);
            data(j, i) = temp;
        }
        i++;
        if (i >= size)
            break;
    }
    for (i; i < size + Config::config->B - 1; i++) {
        int tmp_r;
        tmp_r = data.rows();
        for (int j = 0; j < tmp_r; j++) {
            data(j, i) = data(j, i - size);
        }
        tmp_r = label.rows();
        for (int j = 0; j < tmp_r; j++) {
            label(j, i) = label(j, i - size);
        }
    }
    DBGprint("n=%d\n", i);
}

/** For Markov model **/
Mat* IOManager::secret_share_ngram(int* data, int size, string prefix) {
    vector<ll> coefficients(Config::config->TN);
    vector<ofstream> out_files(Config::config->M);
    cout << prefix << " size: " << size << endl;

    Mat* res = new Mat[Config::config->M];
//    Mat res(1, size);
    for (int i = 0; i < Config::config->M; ++i) {
        res[i].init(1, size);
        string filename = "output/"+prefix+"_"+to_string(i)+".csv";
        cout << "File: " << filename << endl;
        out_files[i].open(filename, ios::out);
    }
    /// 简化计算，TODO，delete
    for (int j = 1; j < Config::config->TN; ++j) {
        coefficients[j] = Constant::Util::randomlong();
    }
//    for (int j = 0; j < Config::config->TN; ++j) {
//        cout << j << ": " << coefficients[j] << endl;
//    }
    cout << "Size: " << size << endl;
//    srand(time(0));
    for (int i = 0; i < size; ++i) {
        for (int j = 1; j < Config::config->TN; ++j) {
            coefficients[j] = Constant::Util::randomlong();
        }
        coefficients[0] = data[i]*Config::config->IE;
        for (int k = 0; k < Config::config->M; ++k) {
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
Mat* IOManager::secret_share_vector(int* data, int size) {
    vector<ll> coefficients(Config::config->TN);

    Mat* res = new Mat[Config::config->M];
    /// 简化计算，TODO，delete
    for (int j = 1; j < Config::config->TN; ++j) {
        coefficients[j] = Constant::Util::randomlong();
    }
    cout << "Size: " << size << endl;
//    srand(time(0));
    for (int l = 0; l < Config::config->M; ++l) {
        res[l].init(size, 1);
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 1; j < Config::config->TN; ++j) {
            coefficients[j] = Constant::Util::randomlong();
        }
        coefficients[0] = data[i]*Config::config->IE;
        for (int k = 0; k < Config::config->M; ++k) {
            ll tmp = poly_eval(coefficients, k + 2);
            res[k].setVal(i, tmp);
        }
    }
    return res;
}

/** For K-V statics **/
Mat* IOManager::secret_share_kv_data(int* data, int size, string prefix, bool isFreq) {
    vector<ll> coefficients(Config::config->TN);
    vector<ofstream> out_files(Config::config->M);
    cout << prefix << " size: " << size << endl;

    Mat* res = new Mat[Config::config->M];
//    Mat res(1, size);
    for (int i = 0; i < Config::config->M; ++i) {
        res[i].init(1, size);
        string filename = "output/"+prefix+"_"+to_string(i)+".csv";
        cout << "File: " << filename << endl;
        out_files[i].open(filename, ios::out);
    }
    /// 简化计算，TODO，delete
    for (int j = 1; j < Config::config->TN; ++j) {
        coefficients[j] = Constant::Util::randomlong();
    }
//    for (int j = 0; j < Config::config->TN; ++j) {
//        cout << j << ": " << coefficients[j] << endl;
//    }
    for (int i = 0; i < size; ++i) {
        for (int j = 1; j < Config::config->TN; ++j) {
            coefficients[j] = Constant::Util::randomlong();
        }
        if (isFreq) {
            if (data[i] > 0) {
                coefficients[0] = Config::config->IE;
            } else {
                coefficients[0] = 0;
            }
        } else {
            coefficients[0] = data[i] * Config::config->IE;
        }

        for (int k = 0; k < Config::config->M; ++k) {
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
Mat* IOManager::secret_share_mat_data(Mat &data, int size) {
    vector<ll> coefficients(Config::config->TN);
    int r = data.rows();
    int c = data.cols();

    vector<ofstream> out_files(Config::config->M);
    Mat* res = new Mat[Config::config->M];
    for (int i = 0; i < Config::config->M; ++i) {
        res[i].init(r, c);
//        ofstream out_file("mnist/mnist_train_"+to_string(Config::config->M), ios::out);
//        out_files.push_back(out_file);
//        string s = "mnist/mnist_train_"+to_string(i)+".csv";
//        cout << s << endl;
        out_files[i].open("dt_"+to_string(i)+".csv", ios::out);
    }
    /// 简化计算，TODO，delete

    cout << r << " : " << c << endl;
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            coefficients[0] = data.get(i, j)*Config::config->IE;
            for (int l = 1; l < Config::config->TN; ++l) {
                coefficients[l] = Constant::Util::randomlong();
            }
            for (int k = 0; k < Config::config->M; ++k) {
                ll tmp = poly_eval(coefficients, k + 2);
                res[k].setVal(j*r+i, tmp);
                if (j == r-1) {
                    out_files[k] << tmp << "\n";
                } else{
                    out_files[k] << poly_eval(coefficients, k + 2) << ",";
                }
            }
        }

    }
    for (int k = 0; k < Config::config->M; ++k) {
        out_files[k].close();
    }
    return res;
}

void IOManager::init() {
    DBGprint("load training data......\n");

    ifstream infile( "datasets/mnist/mnist_train.csv" );
    load(infile, train_data, train_label, Config::config->N);
    secret_share(train_data, train_label, "train");

    // ifstream infile( "mnist/mnist_train_"+to_string(node_type)+".csv" );
    // load_ss(infile, train_data, train_label, Config::config->N);
    infile.close();

    ifstream intest( "datasets/mnist/mnist_test.csv" );
    load(intest, test_data, test_label, Config::config->NM);
    secret_share(test_data, test_label, "test");
    // ifstream intest( "mnist/mnist_test_"+to_string(node_type)+".csv" );
    // load_ss(intest, test_data, test_label, Config::config->NM);
    intest.close();

    // TODO: secret sharing save file

    // train_data.reoeder();
    // train_label.reoeder();
    // test_data.reoeder();
    // test_label.reoeder();
}