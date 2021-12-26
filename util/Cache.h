//
// Created by chenshuyu on 2021/12/24.
//

#ifndef MPC_ML_CACHE_H
#define MPC_ML_CACHE_H
#include "../core/Mat.h"

class Cache
{
public:

    // convert the content of mat to the binary cache ile
    static void tocache(Mat mat, string filename);
    // load the cache 
    static void load_cache(string filename, Mat mat);

};

#endif //MPC_ML_CACHE_H