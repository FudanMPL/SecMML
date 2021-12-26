#include "../util/Cache.h"
#include "../util/IOManager.h"
#include <assert.h>

void test_cache()
{
    IOManager iOManager = IOManager();
    iOManager.init("/home/csy/mnist_test.csv");

    Cache::tocache(iOManager.train_data, "../datasets/test/test.bat");

    IOManager iOManager2 = IOManager();
    iOManager2.init_mat();
    Cache::load_cache("../datasets/test/test.bat", iOManager2.train_data);

    assert(iOManager2.train_data == iOManager.train_data);
    assert(iOManager2.test_data == iOManager.test_data);
    assert(iOManager2.train_label == iOManager.train_label);
    assert(iOManager2.test_label == iOManager.test_label);
}

// void test_cache_time(){
//     IOManager iOManager = IOManager();
//     iOManager.init("../datasets/test/test_int.csv");
// }

int main()
{
    test_cache();
}