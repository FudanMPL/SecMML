#include "iostream"
#include "vector"
#include "../core/Mat.h"
#include "sstream"

void test_operator_bracket()
{
    std::stringstream ss;
    std::vector<ll128> a = {1, 2, 3, 4, 5, 6};
    Mat mat(2, 3);
    mat = a;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            ss << mat.operator()(i, j) << " ";
        }
    }
    if (ss.str() == "1 2 3 4 5 6 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_get()
{
    std::stringstream ss;
    std::vector<ll128> a = {1, 2, 3, 4, 5, 6};
    Mat mat(2, 3);
    mat = a;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            ss << mat.get(i, j) << " ";
        }
    }
    if (ss.str() == "1 2 3 4 5 6 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_getVal()
{
    std::stringstream ss;
    std::vector<ll128> a = {1, 2, 3, 4, 5, 6};
    Mat mat(2, 3);
    mat = a;
    for (int i = 0; i < 6; i++)
    {
        ss << mat.getVal(i) << " ";
    }
    if (ss.str() == "1 4 2 5 3 6 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_setVal()
{
    std::stringstream ss;
    std::vector<ll128> a = {1, 2, 3, 4, 5, 6};
    Mat mat(2, 3);
    mat = a;
    for (int i = 0; i < 6; i++)
    {
        mat.setVal(i, i + 2);
    }
    for (int i = 0; i < 6; i++)
    {
        ss << mat.getVal(i) << " ";
    }
    if (ss.str() == "2 3 4 5 6 7 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_operator_assign()
{
    std::stringstream ss;
    std::vector<ll128> a = {1, 2, 3, 4, 5, 6};
    Mat mat(2, 3), mat1(2, 3);
    mat = a;
    mat1 = mat;
    for (int i = 0; i < 6; i++)
    {
        ss << mat1.getVal(i) << " ";
    }
    if (ss.str() == "1 4 2 5 3 6 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_operator_equal()
{
    std::stringstream ss;
    std::vector<ll128> a = {1, 2, 3, 4, 5, 6};
    std::vector<ll128> b = {11, 12, 13, 14, 14, 13};
    Mat mat(2, 3), mat1(2, 3);
    mat = a;
    mat1 = b;
    ss << (mat == mat1);
    if (ss.str() == "0")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_operator_add()
{
    std::stringstream ss;
    std::vector<ll128> a = {1, 2, 3, 4, 5, 6};
    std::vector<ll128> b = {11, 12, 13, 14, 14, 13};
    Mat mat(2, 3), mat1(2, 3);
    mat = a;
    mat1 = b;
    mat = mat + mat1;
    for (int i = 0; i < 6; i++)
    {
        ss << mat.getVal(i) << " ";
    }
    if (ss.str() == "12 18 14 19 16 19 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_operator_add_assign()
{
    std::stringstream ss;
    std::vector<ll128> a = {1, 2, 3, 4, 5, 6};
    std::vector<ll128> b = {11, 12, 13, 14, 14, 13};
    Mat mat(2, 3), mat1(2, 3);
    mat = a;
    mat1 = b;
    mat += mat1;
    for (int i = 0; i < 6; i++)
    {
        ss << mat.getVal(i) << " ";
    }
    if (ss.str() == "12 18 14 19 16 19 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_operator_sub()
{
    std::stringstream ss;
    std::vector<ll128> a = {1, 2, 3, 4, 5, 6};
    std::vector<ll128> b = {11, 12, 13, 14, 14, 13};
    Mat mat(2, 3), mat1(2, 3);
    mat = a;
    mat1 = b;
    mat = mat - mat1;
    for (int i = 0; i < 6; i++)
    {
        ss << mat.getVal(i) << " ";
    }
    if (ss.str() == "-10 -10 -10 -9 -10 -7 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_operator_mul()
{
    std::stringstream ss;
    std::vector<ll128> a = {1, 2, 3, 4, 5, 6};
    std::vector<ll128> b = {6, 5, 4, 3, 2, 1};
    Mat mat(2, 3), mat1(2, 3), mat2(3, 2), mat3(2, 3), mat4(3, 2);
    mat1 = a;
    mat2 = a;
    mat3 = b;
    mat4 = b;
    mat3.reorder();
    mat4.reorder();
    mat = mat1 * mat2;
    for (int i = 0; i < 4; i++)
    {
        ss << mat.getVal(i) << " ";
    }
    mat = mat1 * mat4;
    for (int i = 0; i < 4; i++)
    {
        ss << mat.getVal(i) << " ";
    }
    mat = mat3 * mat2;
    for (int i = 0; i < 4; i++)
    {
        ss << mat.getVal(i) << " ";
    }
    mat = mat3 * mat4;
    for (int i = 0; i < 4; i++)
    {
        ss << mat.getVal(i) << " ";
    }
    if (ss.str() == "22 49 28 64 20 56 14 41 41 14 56 20 64 28 49 22 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_operator_div()
{
    std::stringstream ss;
    std::vector<ll128> a = {1, 2, 3, 4, 5, 6};
    std::vector<ll128> b = {11, 12, 9, 16, 15, 12};
    Mat mat(2, 3), mat1(2, 3);
    mat = a;
    mat1 = b;
    mat = mat1 / mat;
    for (int i = 0; i < 6; i++)
    {
        ss << mat.getVal(i) << " ";
    }
    if (ss.str() == "11 4 6 3 3 2 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_operator_and()
{
    std::stringstream ss;
    std::vector<ll128> a = {1, 2, 3, 4, 5, 6};
    std::vector<ll128> b = {11, 12, 9, 16, 15, 12};
    Mat mat(2, 3), mat1(2, 3);
    mat = a;
    mat1 = b;
    mat = mat1 & mat;
    for (int i = 0; i < 6; i++)
    {
        ss << mat.getVal(i) << " ";
    }
    if (ss.str() == "1 0 0 5 1 4 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_dot()
{
    std::stringstream ss;
    std::vector<ll128> a = {1, 2, 3, 4, 5, 6};
    std::vector<ll128> b = {11, 12, 9, 16, 15, 12};
    Mat mat(2, 3), mat1(2, 3);
    mat = a;
    mat1 = b;
    mat = mat.dot(mat1);
    for (int i = 0; i < 6; i++)
    {
        ss << mat.getVal(i) << " ";
    }
    if (ss.str() == "11 64 24 75 27 72 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_row()
{
    std::stringstream ss;
    std::vector<ll128> a;
    std::vector<ll128> b = {11, 12, 9, 16, 15, 12};
    Mat mat1(2, 3);
    mat1 = b;
    a = mat1.row(1);
    for (int i = 0; i < 3; i++)
    {
        ss << a[i] << " ";
    }
    if (ss.str() == "16 15 12 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_init_row()
{
    std::stringstream ss;
    std::vector<ll128> a = {21, 22, 23};
    std::vector<ll128> b = {11, 12, 9, 16, 15, 12};
    Mat mat1(2, 3);
    mat1 = b;
    mat1.init_row(a, 1);
    for (int i = 0; i < 6; i++)
    {
        ss << mat1.getVal(i) << " ";
    }
    if (ss.str() == "11 21 12 22 9 23 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_resize()
{
    std::stringstream ss;
    std::vector<ll128> a = {21, 22, 23, 11, 12, 9, 16, 15, 12};
    Mat mat(2, 2), mat1(3, 3);
    mat1 = a;
    mat = mat1.resize(2, 2);
    for (int i = 0; i < 4; i++)
    {
        ss << mat1.getVal(i) << " ";
    }
    if (ss.str() == "21 11 16 22 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_eq()
{
    std::stringstream ss;
    std::vector<ll128> a = {11, 22, 23, 16, 15, 11};
    std::vector<ll128> b = {11, 12, 9, 16, 15, 12};
    Mat mat(2, 3), mat1(2, 3), mat2(2, 3);
    mat = a;
    mat1 = b;
    mat2 = mat.eq(mat1);
    for (int i = 0; i < 6; i++)
    {
        ss << mat2.getVal(i) << " ";
    }
    if (ss.str() == "1 1 0 1 0 0 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_take_row()
{
    std::stringstream ss;
    std::vector<ll128> a = {21, 22, 23, 11, 12, 9, 16, 15, 12};
    Mat mat(2, 3), mat1(3, 3);
    mat1 = a;
    mat = mat1.row(1, 3);
    for (int i = 0; i < 6; i++)
    {
        ss << mat.getVal(i) << " ";
    }
    if (ss.str() == "11 16 12 15 9 12 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_take_col()
{
    std::stringstream ss;
    std::vector<ll128> a = {21, 22, 23, 11, 12, 9, 16, 15, 12};
    Mat mat(3, 2), mat1(3, 3);
    mat1 = a;
    mat1.col(1, 3, mat);
    for (int i = 0; i < 6; i++)
    {
        ss << mat.getVal(i) << " ";
    }
    if (ss.str() == "22 12 15 23 9 12 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_fill()
{
    std::stringstream ss;
    std::vector<ll128> p = {11, 0, 22, 23, 0, 16, 0, 15, 11};
    std::vector<ll128> q = {11, 12, 9, 16, 15, 12, 15, 19, 4};
    Mat a(2, 3), a_r(3, 3), b(2, 3), b_r(3, 3);
    a_r = p;
    b_r = q;
    Mat::fill(&a, &a_r, &b, &b_r);
    for (int i = 0; i < 6; i++)
    {
        ss << a.getVal(i) << " ";
    }
    for (int i = 0; i < 6; i++)
    {
        ss << b.getVal(i) << " ";
    }
    if (ss.str() == "11 23 15 22 16 11 11 16 19 9 12 4 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_concat()
{
    std::stringstream ss;
    std::vector<ll128> a = {11, 22, 23, 16, 15, 11};
    std::vector<ll128> b = {11, 12, 9, 16, 15, 12};
    Mat mat(2, 3), mat1(2, 3), mat2(4, 3);
    mat = a;
    mat1 = b;
    Mat::concat(&mat2, &mat, &mat1);
    for (int i = 0; i < 12; i++)
    {
        ss << mat2.getVal(i) << " ";
    }
    if (ss.str() == "11 16 11 16 22 15 12 15 23 11 9 12 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_reconcat()
{
    std::stringstream ss1, ss2;
    std::vector<ll128> a = {11, 22, 23, 16, 15, 11, 11, 12, 9, 16, 15, 12};
    Mat mat(3, 3), mat1(1, 3), mat2(4, 3);
    mat2 = a;
    Mat::reconcat(&mat2, &mat, true, &mat1, true);
    for (int i = 0; i < 9; i++)
    {
        ss1 << mat.getVal(i) << " ";
    }
    for (int i = 0; i < 3; i++)
    {
        ss2 << mat1.getVal(i) << " ";
    }
    if (ss1.str() == "11 16 11 22 15 12 23 11 9 " && ss2.str() == "16 15 12 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_hstack()
{
    std::stringstream ss;
    std::vector<ll128> a = {11, 22, 23, 16, 15, 11};
    std::vector<ll128> b = {11, 12, 9, 16, 15, 12};
    Mat mat(2, 3), mat1(2, 3), mat2(4, 3);
    mat = a;
    mat1 = b;
    Mat::hstack(&mat2, &mat, &mat1);
    for (int i = 0; i < 12; i++)
    {
        ss << mat2.getVal(i) << " ";
    }
    if (ss.str() == "11 16 22 15 23 11 11 16 12 15 9 12 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

void test_re_hstack()
{
    std::stringstream ss1, ss2;
    std::vector<ll128> a = {11, 22, 23, 16, 15, 11, 11, 12, 9, 16, 15, 12};
    Mat mat(3, 3), mat1(1, 3), mat2(4, 3);
    mat2 = a;
    Mat::re_hstack(&mat2, &mat, true, &mat1, true);
    for (int i = 0; i < 9; i++)
    {
        ss1 << mat.getVal(i) << " ";
    }
    for (int i = 0; i < 3; i++)
    {
        ss2 << mat1.getVal(i) << " ";
    }
    if (ss1.str() == "11 16 11 16 22 15 12 15 23 " && ss2.str() == "11 9 12 ")
    {
        std::cout << "True" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
}

int main()
{
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::operator()(int row, int col)" << std::endl;
    test_operator_bracket();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::get(int row, int col)" << std::endl;
    test_get();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::getVal(int a)" << std::endl;
    test_getVal();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::setVal(int index, ll128 v)" << std::endl;
    test_setVal();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::operator=(vector<ll128> &a)" << std::endl;
    test_operator_assign();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::operator==(Mat p)" << std::endl;
    test_operator_equal();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::operator+(const Mat &a)" << std::endl;
    test_operator_add();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::operator+=(const Mat &a)" << std::endl;
    test_operator_add_assign();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::operator-(const Mat &a)" << std::endl;
    test_operator_sub();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::operator*(const Mat &a)" << std::endl;
    test_operator_mul();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::operator/(Mat a)" << std::endl;
    test_operator_div();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::operator&(const Mat &a)" << std::endl;
    test_operator_and();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::dot(const Mat &a)" << std::endl;
    test_dot();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::row(int a)" << std::endl;
    test_row();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::init_row(vector<ll128> a, int b)" << std::endl;
    test_init_row();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::resize(int a, int b)" << std::endl;
    test_resize();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::eq(const Mat &a)" << std::endl;
    test_eq();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::row(int st, int ed)" << std::endl;
    test_take_row();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::col(int st, int ed, Mat &a)" << std::endl;
    test_take_col();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::fill(Mat *a, Mat *a_r, Mat *b, Mat *b_r)" << std::endl;
    test_fill();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::concat(Mat *res, Mat *a, Mat *b)" << std::endl;
    test_concat();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::reconcat(Mat *res, Mat *a, bool fa, Mat *b, bool fb)" << std::endl;
    test_reconcat();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::hstack(Mat *res, Mat *a, Mat *b)" << std::endl;
    test_hstack();

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Test Function Mat::re_hstack(Mat *res, Mat *a, bool fa, Mat *b, bool fb)" << std::endl;
    test_re_hstack();

    return 0;
}