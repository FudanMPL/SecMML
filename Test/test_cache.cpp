//
// Created by chenshuyu on 2021/12/24.
//

#include "../util/IOManager.h"
#include <assert.h>
#include <ctime>
#include <time.h>

void test_cache_twofile()
{

    IOManager::init("../datasets/test/mnist_train.csv", "../datasets/test/mnist_test.csv");
    Mat train_data = Mat(IOManager::train_data);
    Mat train_label = Mat(IOManager::train_label);
    Mat test_data = Mat(IOManager::test_data);
    Mat test_label = Mat(IOManager::test_label);

    IOManager::init("../datasets/test/mnist_train.csv", "../datasets/test/mnist_test.csv");
    assert(train_data == IOManager::train_data);
    assert(train_label == IOManager::train_label);
    assert(test_data == IOManager::test_data);
    assert(test_label == IOManager::test_label);
}

void test_cache_onefile()
{
    IOManager iOManager = IOManager();
    iOManager.remove_cache("../datasets/test/mnist_train.csv", "../datasets/test/mnist_test.csv");
    iOManager.init("../datasets/test/mnist_train.csv");

    IOManager iOManager2 = IOManager();
    iOManager2.init("../datasets/test/mnist_train.csv");
    assert(iOManager.train_data == iOManager2.train_data);
    assert(iOManager.train_label == iOManager2.train_label);
    assert(iOManager.test_data == iOManager2.test_data);
    assert(iOManager.test_label == iOManager2.test_label);
}

void test_cache_time()
{
    IOManager iOManager = IOManager();

    clock_t startTime, endTime;

    startTime = clock();
    iOManager.init("../datasets/test/mnist_train.csv", "../datasets/test/mnist_test.csv");
    endTime = clock();
    double time_file = (double)(endTime - startTime) / CLOCKS_PER_SEC; // in second
    std::cout << "The time of load the original file: " << time_file << std::endl;

    IOManager iOManager2 = IOManager();
    startTime = clock();
    iOManager2.init("../datasets/test/mnist_train.csv", "../datasets/test/mnist_test.csv");
    endTime = clock();
    double time_cache = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    std::cout << "The time of load the cache file: " << time_cache << std::endl;
}

int main()
{
    test_cache_twofile();
    // test_cache_time();
}