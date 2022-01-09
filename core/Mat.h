//
// Created by tangdingyi on 2019/12/25.
//

#ifndef MPC_ML_MAT_H
#define MPC_ML_MAT_H

#include "../Constant.h"
#include <thread>

class Mat
{
private:
    int r, c;
    int order; // 0为列存 1为行存
    vector<ll128> val;

public:
    Config *config;
    Mat(int r, int c);
    Mat(int r, int c, ll128 b);
    Mat();
    Mat(const Mat &a);
    ~Mat();
    void init(int r, int c);
    ll128 &operator()(int a, int b);
    vector<ll128> getVal() const;
    ll128 get(int a, int b) const;
    ll128 &getVal(int a);
    void setVal(vector<ll128> v);
    void setVal(int index, ll128 v);
    int rows() const;
    int cols() const;
    int getorder() const;
    void setrow(int row);
    void setcol(int col);
    void setorder(int order);
    int size() const;
    Mat &operator=(const Mat &a);
    Mat &operator=(vector<ll128> &a);
    Mat &operator=(char *&p);
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
    vector<double> backtoFloat();
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
    void AddDot(int k, ll128 *x, int incx, ll128 *y, ll128 *gamma);
    void sign();
    void reorder();
    void transorder();
    void truncated_normal();
    void truncated_normal(double mean, double stddev);
    void random_normal();
    void constant(double b);
    void col(int u, vector<ll128> &a);
    void clear();
    void print() const;
    void printSign();
    void toString(char *p);
    int toString_pos(char *p) const;
    void toBuffer_pos(char *p) const;
    void toBuffer(char *p, int i) const;
    void to_Buffer(char *p, int i) const;
    int getStringLen();
    void get_Buffer(char *&p ,int i);
    void getBuffer(char *&p, int i);
    void getFrom_buf(char *&p);
    void getFrom_pos(char *&p);
    void add_Buffer(char *&p, int i);
    void addBuffer(char *&p, int i);
    void addFrom_buf(char *&p);
    void addFrom_pos(char *&p);
    Mat SmoothLevel();
    ll count_sum();
    ll get_memory_size();
    static bool fill(Mat *a, Mat *a_r, Mat *b, Mat *b_r);
    static void concat(Mat *res, Mat *a, Mat *b);
    static void reconcat(Mat *res, Mat *a, bool fa, Mat *b, bool fb);
    static void vstack(Mat *res, Mat *a, Mat *b);
    static void re_vstack(Mat *res, Mat *a, bool fa, Mat *b, bool fb);
    static void hstack(Mat *res, Mat *a, Mat *b);
    static void re_hstack(Mat *res, Mat *a, bool fa, Mat *b, bool fb);
    static int pair_order_type(Mat *a, const Mat *b);
    static void random_neg(Mat *a);
};

#endif // MPC_ML_MAT_H
