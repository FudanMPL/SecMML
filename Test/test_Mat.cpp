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
}