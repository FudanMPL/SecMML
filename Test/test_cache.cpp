//
// Created by chenshuyu on 2021/12/24.
//

#include "../util/IOManager.h"
#include <assert.h>
#include <ctime>
#include <time.h>

void print_part_data(Mat mat)
{
    cout << mat.cols() << " " << mat.rows() << endl;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < mat.rows() && j < 20; j++)
        {
            // Get the value of row and column
            cout << mat.get(j, i) << " ";
        }
        cout << endl;
    }
}

void test_cache_twofile()
{

    IOManager::init();
    Mat train_data = Mat(IOManager::train_data);
    Mat train_label = Mat(IOManager::train_label);
    Mat test_data = Mat(IOManager::test_data);
    Mat test_label = Mat(IOManager::test_label);
    print_part_data(train_data);
    print_part_data(train_label);
    print_part_data(test_data);
    print_part_data(test_label);

    IOManager::init();
    print_part_data(IOManager::train_data);
    print_part_data(IOManager::train_label);
    print_part_data(IOManager::test_data);
    print_part_data(IOManager::test_label);
    assert(train_data == IOManager::train_data);
    assert(train_label == IOManager::train_label);
    assert(test_data == IOManager::test_data);
    assert(test_label == IOManager::test_label);
}

void test_cache_onefile()
{
    IOManager iOManager = IOManager();
    iOManager.init();

    IOManager iOManager2 = IOManager();
    iOManager2.init();
    assert(iOManager.train_data == iOManager2.train_data);
    assert(iOManager.train_label == iOManager2.train_label);
    assert(iOManager.test_data == iOManager2.test_data);
    assert(iOManager.test_label == iOManager2.test_label);
}

void test_cache_time()
{

    clock_t startTime, endTime;
    IOManager::remove_cache();
    startTime = clock();
    IOManager::init_local_data();
    endTime = clock();
    double time_file = (double)(endTime - startTime) / CLOCKS_PER_SEC; // in second
    std::cout << "The time of load the original file: " << time_file << std::endl;

    startTime = clock();
    IOManager::init_local_data();
    endTime = clock();
    double time_cache = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    std::cout << "The time of load the cache file: " << time_cache << std::endl;
}

int main()
{
    // test_cache_twofile();
    Config::config = Config::init("../constant.json");
    test_cache_time();
}