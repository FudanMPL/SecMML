/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-19 11:02:28
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2021-12-19 11:51:01
 */
#include <iostream>
#include "../util/IOManager.h"

int main()
{
    IOManager iOManager = IOManager();
    //iOManager.init("../datasets/mnist/mnist_train.csv","../datasets/mnist/mnist_test.csv");
    iOManager.init("../datasets/test/test_int.csv");
    iOManager.train_data.print();
    iOManager.test_data.print();
    iOManager.train_label.print();
    iOManager.test_label.print();
    
    return 0;
}