//
// Created by wzx on 2021/12/22.
//

#include "../util/SocketManager.h"
#include "iostream"
#include "sstream"

void test1(int node_type)
{
    if (node_type == 0)
    {
        //party0 and party 1 for test
        stringstream ss1, ss2, ss3;
        std::vector<ll128> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::vector<ll128> b = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        std::vector<ll128> c = {11, 12, 13, 14, 15, 16, 17, 18, 19};
        Mat *mat1, mat2(3, 3), mat3(3, 3);
        mat1 = new Mat(3, 3);
        *mat1 = a;
        mat2 = b;
        mat3 = c;
        SocketManager::send(0, 1, mat1);
        SocketManager::receive(0, 1, mat2);
        for (int i = 0; i < 9; i++)
        {
            ss1 << mat2.getVal(i) << " ";
        }
        if (ss1.str() == "1 4 7 2 5 8 3 6 9 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a) " << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a)" << std::endl;
            std::cout << "False" << std::endl;
        }
        Mat mat[2];
        mat[0] = mat2;
        mat[1] = mat3;
        SocketManager::send(0, 1, mat[1]);
        mat[0] = SocketManager::receive(0, 1);
        for (int i = 0; i < 9; i++)
        {
            ss2 << mat[0].getVal(i) << " ";
        }
        if (ss2.str() == "11 14 17 12 15 18 13 16 19 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from)" << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from) " << std::endl;
            std::cout << "False" << std::endl;
        }
        SocketManager::send(0, 1, mat1);
        SocketManager::receive(0, 1, mat1);
        for (int i = 0; i < 9; i++)
        {
            ss3 << mat1->getVal(i) << " ";
        }
        if (ss3.str() == "11 14 17 12 15 18 13 16 19 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
        }
        //party 0 and party2 for test
        Mat *mat4, mat5(3, 3), mat6(3, 3);
        mat4 = new Mat(3, 3);
        *mat4 = a;
        mat5 = b;
        mat6 = c;
        SocketManager::send(0, 2, mat4);
        SocketManager::receive(0, 2, mat5);
        mat[0] = mat5;
        mat[1] = mat6;
        SocketManager::send(0, 2, mat[1]);
        mat[0] = SocketManager::receive(0, 2);
        SocketManager::send(0, 2, mat4);
        SocketManager::receive(0, 2, mat4);
        free(mat1);
        free(mat4);
    }
    else if (node_type == 1)
    {

        stringstream ss1, ss2, ss3;
        std::vector<ll128> a = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        Mat mat1(3, 3), *mat2, mat3(3, 3);
        mat2 = new Mat(3, 3);
        *mat2 = a;
        mat1 = a;
        SocketManager::receive(1, 0, mat1);
        SocketManager::send(1, 0, mat1);
        for (int i = 0; i < 9; i++)
        {
            ss1 << mat1.getVal(i) << " ";
        }
        if (ss1.str() == "1 4 7 2 5 8 3 6 9 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a) " << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a) " << std::endl;
            std::cout << "False" << std::endl;
        }
        mat3 = SocketManager::receive(1, 0);
        SocketManager::send(1, 0, mat3);
        for (int i = 0; i < 9; i++)
        {
            ss2 << mat3.getVal(i) << " ";
        }
        if (ss2.str() == "11 14 17 12 15 18 13 16 19 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from) " << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from) " << std::endl;
            std::cout << "False" << std::endl;
        }
        SocketManager::receive(1, 0, mat2);
        SocketManager::send(1, 0, mat2);
        for (int i = 0; i < 9; i++)
        {
            ss3 << mat2->getVal(i) << " ";
        }
        if (ss3.str() == "10 10 10 10 10 10 10 10 10 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
        }
    }
    else if (node_type == 2)
    {
        stringstream ss1, ss2, ss3;
        std::vector<ll128> a = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        Mat mat1(3, 3), *mat2, mat3(3, 3);
        mat2 = new Mat(3, 3);
        *mat2 = a;
        mat1 = a;
        SocketManager::receive(2, 0, mat1);
        SocketManager::send(2, 0, mat1);
        for (int i = 0; i < 9; i++)
        {
            ss1 << mat1.getVal(i) << " ";
        }
        if (ss1.str() == "1 4 7 2 5 8 3 6 9 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a) " << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a) " << std::endl;
            std::cout << "False" << std::endl;
        }
        mat3 = SocketManager::receive(2, 0);
        SocketManager::send(2, 0, mat3);
        for (int i = 0; i < 9; i++)
        {
            ss2 << mat3.getVal(i) << " ";
        }
        if (ss2.str() == "11 14 17 12 15 18 13 16 19 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from) " << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from) " << std::endl;
            std::cout << "False" << std::endl;
        }
        SocketManager::receive(2, 0, mat2);
        SocketManager::send(2, 0, mat2);
        for (int i = 0; i < 9; i++)
        {
            ss3 << mat2->getVal(i) << " ";
        }
        if (ss3.str() == "10 10 10 10 10 10 10 10 10 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
        }
    }
}

void test2(int node_type)
{
    if (node_type == 0)
    {
        //party0 and party 1 for test
        stringstream ss1, ss2, ss3;
        std::vector<ll128> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::vector<ll128> b = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        std::vector<ll128> c = {11, 12, 13, 14, 15, 16, 17, 18, 19};
        Mat *mat1, mat2(3, 3), mat3(3, 3);
        mat1 = new Mat(3, 3);
        *mat1 = a;
        mat2 = b;
        mat3 = c;
        mat1->reorder();
        SocketManager::send(0, 1, mat1);
        SocketManager::receive(0, 1, mat2);
        for (int i = 0; i < 9; i++)
        {
            ss1 << mat2.getVal(i) << " ";
        }
        if (ss1.str() == "1 2 3 4 5 6 7 8 9 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a) " << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a)" << std::endl;
            std::cout << "False" << std::endl;
        }
        Mat mat[2];
        mat[0] = mat2;
        mat[1] = mat3;
        mat[0].reorder();
        SocketManager::send(0, 1, mat[1]);
        mat[0] = SocketManager::receive(0, 1);
        for (int i = 0; i < 9; i++)
        {
            ss2 << mat[0].getVal(i) << " ";
        }
        if (ss2.str() == "11 14 17 12 15 18 13 16 19 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from)" << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from) " << std::endl;
            std::cout << "False" << std::endl;
        }
        SocketManager::send(0, 1, mat1);
        SocketManager::receive(0, 1, mat1);
        for (int i = 0; i < 9; i++)
        {
            ss3 << mat1->getVal(i) << " ";
        }
        if (ss3.str() == "3 9 15 5 11 17 7 13 19 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
        }
        //party 0 and party2 for test
        Mat *mat4, mat5(3, 3), mat6(3, 3);
        mat4 = new Mat(3, 3);
        *mat4 = a;
        mat5 = b;
        mat6 = c;
        mat4->reorder();
        SocketManager::send(0, 2, mat4);
        SocketManager::receive(0, 2, mat5);
        mat[0] = mat5;
        mat[1] = mat6;
        mat[0].reorder();
        SocketManager::send(0, 2, mat[1]);
        mat[0] = SocketManager::receive(0, 2);
        SocketManager::send(0, 2, mat4);
        SocketManager::receive(0, 2, mat4);
        free(mat1);
        free(mat4);
    }
    else if (node_type == 1)
    {
        stringstream ss1, ss2, ss3;
        std::vector<ll128> a = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        std::vector<ll128> b = {1, 1, 1, 1, 1, 1, 1, 1, 1};
        Mat mat1(3, 3), *mat2, mat3(3, 3);
        mat2 = new Mat(3, 3);
        *mat2 = b;
        mat1 = a;
        SocketManager::receive(1, 0, mat1);
        SocketManager::send(1, 0, mat1);
        for (int i = 0; i < 9; i++)
        {
            ss1 << mat1.getVal(i) << " ";
        }
        if (ss1.str() == "1 2 3 4 5 6 7 8 9 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a) " << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a) " << std::endl;
            std::cout << "False" << std::endl;
        }
        mat3 = SocketManager::receive(1, 0);
        for (int i = 0; i < 9; i++)
        {
            ss2 << mat3.getVal(i) << " ";
        }
        SocketManager::send(1, 0, mat3);
        if (ss2.str() == "11 14 17 12 15 18 13 16 19 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from) " << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from) " << std::endl;
            std::cout << "False" << std::endl;
        }
        SocketManager::receive(1, 0, mat2);
        for (int i = 0; i < 9; i++)
        {
            ss3 << mat2->getVal(i) << " ";
        }
        mat2->reorder();
        SocketManager::send(1, 0, mat2);
        if (ss3.str() == "2 3 4 5 6 7 8 9 10 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
        }
    }
    else if (node_type == 2)
    {
        stringstream ss1, ss2, ss3;
        std::vector<ll128> a = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        std::vector<ll128> b = {1, 1, 1, 1, 1, 1, 1, 1, 1};
        Mat mat1(3, 3), *mat2, mat3(3, 3);
        mat2 = new Mat(3, 3);
        *mat2 = b;
        mat1 = a;
        SocketManager::receive(2, 0, mat1);
        SocketManager::send(2, 0, mat1);
        for (int i = 0; i < 9; i++)
        {
            ss1 << mat1.getVal(i) << " ";
        }
        if (ss1.str() == "1 2 3 4 5 6 7 8 9 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a) " << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a) " << std::endl;
            std::cout << "False" << std::endl;
        }
        mat3 = SocketManager::receive(2, 0);
        for (int i = 0; i < 9; i++)
        {
            ss2 << mat3.getVal(i) << " ";
        }
        SocketManager::send(2, 0, mat3);
        if (ss2.str() == "11 14 17 12 15 18 13 16 19 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from) " << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from) " << std::endl;
            std::cout << "False" << std::endl;
        }
        SocketManager::receive(2, 0, mat2);
        for (int i = 0; i < 9; i++)
        {
            ss3 << mat2->getVal(i) << " ";
        }
        mat2->reorder();
        SocketManager::send(2, 0, mat2);
        if (ss3.str() == "2 3 4 5 6 7 8 9 10 ")
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
        }
    }
}

void test3(int node_type, Mat test)
{
    Mat *test1;
    test1 = new Mat(20000, 785);
    Mat::random_neg(test1);
    if (node_type == 0)
    {
        //test.print();
        SocketManager::send(0, 1, test);
        SocketManager::send(0, 2, test);
        SocketManager::send(0, 1, test1);
        SocketManager::send(0, 2, test1);
        SocketManager::send(0, 1, test);
        SocketManager::send(0, 2, test);
    }
    else if (node_type == 1)
    {
        Mat mat1;
        mat1 = SocketManager::receive(1, 0);
        if (mat1 == test)
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from) " << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from) " << std::endl;
            std::cout << "False" << std::endl;
        }
        Mat mat2;
        SocketManager::receive(1, 0, mat2);
        if (mat2 == *test1)
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a) " << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a) " << std::endl;
            std::cout << "False" << std::endl;
        }
        Mat *mat3;
        mat3 = new Mat(20000, 785);
        Mat::random_neg(mat3);
        Mat mat4 = *mat3;
        SocketManager::receive(1, 0, mat3);
        if (*mat3 == mat4 + test)
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
        }
    }
    else
    {
        Mat mat1;
        mat1 = SocketManager::receive(2, 0);
        if (mat1 == test)
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from) " << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat &a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from) " << std::endl;
            std::cout << "False" << std::endl;
        }
        Mat mat2;
        SocketManager::receive(2, 0, mat2);
        if (mat2 == *test1)
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a) " << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::send(int node_type, int target, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat &a) " << std::endl;
            std::cout << "False" << std::endl;
        }
        Mat *mat3;
        mat3 = new Mat(20000, 785);
        Mat::random_neg(mat3);
        Mat mat4 = *mat3;
        SocketManager::receive(2, 0, mat3);
        if (*mat3 == mat4 + test)
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "True" << std::endl;
        }
        else
        {
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Test for function SocketManager::receive(int node_type, int from, Mat *a)" << std::endl;
            std::cout << "False" << std::endl;
        }
    }
}

int main(int argc, char **argv)
{
    Config::config = Config::init("../constant.json");
    if (argc < 2)
    {
        DBGprint("Please enter party index:\n");
        scanf("%d", &node_type);
    }
    else
    {
        node_type = argv[1][0] - '0';
    }
    DBGprint("party index: %d\n", node_type);
    SocketManager::SMMLF tel;
    if (Config::config->LOCAL_TEST)
    {
        cout << node_type << endl;
        tel.init(Config::config->IP, Config::config->PORT);
    }
    else
    {
        tel.init();
    }
    test1(node_type);
    std::cout << std::endl
              << std::endl
              << std::endl;
    std::cout << "Test for different matrix storage methods before and after transmission" << std::endl;
    test2(node_type);
    std::cout << std::endl
              << std::endl
              << std::endl;
    std::cout << "Test for larger matrix whose size larger than socket buffer" << std::endl;
    Mat test(20000, 785, 1);
    Mat::random_neg(&test);
    //test.print();
    test3(node_type, test);
    return 0;
}