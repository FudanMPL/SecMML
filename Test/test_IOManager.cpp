#include <iostream>
#include "../util/IOManager.h"

int main()
{
    // IOManager iOManager = IOManager();
    //  iOManager.init("../datasets/mnist/mnist_train.csv","../datasets/mnist/mnist_test.csv");
    IOManager::init();

    // iOManager.train_data.print();
    // iOManager.test_data.print();
    // iOManager.train_label.print();
    // iOManager.test_label.print();

    return 0;
}