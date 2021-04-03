//
// Created by tangdingyi on 2019/12/25.
//

#ifndef MPC_ML_MAT_H
#define MPC_ML_MAT_H

#include "../Constant.h"

class Mat {
    int r, c;
    int order;
    vector<ll128> val;
public:
    Mat(int r, int c);
    Mat(int r, int c, ll128 b);
    Mat();
    Mat(const Mat &a);
    ~Mat();
    void init(int r, int c);
    ll128&operator()(int a, int b);
    ll128 get(int a, int b) const;
    ll128&getVal(int a);
    void setVal(int index, ll128 v);
    int rows() const;
    int cols() const;
    int size() const;
    Mat& operator=(const Mat &a);
    Mat& operator=(vector<ll128> &a);
    Mat& operator=(char* &p);
    bool operator==(Mat p);
    Mat transpose() const;
    Mat operator+(const Mat &a);
    void operator+=(const Mat &a);
    Mat operator+(const ll128 &b);
    Mat operator-(const Mat &a);
    Mat operator-(ll128 b);
    Mat operator*(const Mat &a);
    Mat operator*(const ll128 &b);
    Mat operator/(Mat a);
    Mat operator/(const ll128 &b);
    Mat operator<<(int b) const;
    Mat operator>>(int b) const;
    Mat operator&(int b) const;
    Mat operator&(const Mat &a) const;
    Mat oneMinus();
    Mat oneMinus_IE();
    Mat dot(const Mat &a);
    vector<ll128> row(int a);
    void init_row(vector<ll128> a, int b);
    Mat resize(int a, int b);
    Mat argmax();
    Mat equal(const Mat &a);
    Mat eq(const Mat &a);
    Mat getSign();
    Mat softmax();
    Mat sqrt();
    Mat inverse();
    Mat sqrt_inv();
    Mat divideBy2();
    Mat relu();
    Mat d_relu();
    Mat sigmoid() const;
    Mat hard_tanh();
    Mat LTZ();
    Mat tanh();
    Mat raw_tanh();
    Mat cross_entropy();
    Mat chebyshev_tanh();
    Mat not_eqz() const;
    Mat row(int st, int ed) const;
    Mat col(int st, int ed) const;
    Mat get_bit(int b) const;
    Mat opposite() const;
    Mat toOneHot() const;
    void col(int st, int ed, Mat &a) const;
    void append(int st, int ed, Mat *a);
    Mat mod(ll b);
    int count();
    int count(const ll128 &b);
    int countNe(const ll128 &b) const;
    int count_not_eqz() const;
    bool is_nan();
    bool is_zero();
    bool fill(const Mat &a);
    void init(const ll128 &b);
    void cp(const Mat &a, int st, int len);
    void cp(const Mat &a, const Mat &mask);
    void residual();
    void AddDot(int k, ll128* x, int incx, ll128* y, ll128* gamma);
    void sign();
    void reoeder();
    void transorder();
    void truncated_normal();
    void truncated_normal(double mean, double stddev);
    void random_normal();
    void constant(double b);
    void col(int u, vector<ll128>& a);
    void clear();
    void print() const;
    void printSign();
    void toString(char* p);
    int toString_pos(char* p) const;
    int getStringLen();
    void getFrom_pos(char* &p);
    void addFrom_pos(char* &p);
    Mat SmoothLevel();
    ll count_sum();
    static bool fill(Mat* a, Mat* a_r, Mat* b, Mat* b_r);
    static void concat(Mat* res, Mat* a, Mat* b);
    static void reconcat(Mat* res, Mat* a, bool fa, Mat* b, bool fb);
    static void vstack(Mat* res, Mat* a, Mat* b);
    static void re_vstack(Mat* res, Mat* a, bool fa, Mat* b, bool fb);
    static void hstack(Mat* res, Mat* a, Mat* b);
    static void re_hstack(Mat* res, Mat* a, bool fa, Mat* b, bool fb);
    static int pair_order_type(Mat* a, const Mat* b);
    static void random_neg(Mat *a);
};


#endif //MPC_ML_MAT_H
