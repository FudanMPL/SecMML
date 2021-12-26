//
// Created by chenshuyu on 2021/12/25.
//

#include "Cache.h"
#include "../core/Mat.h"

// convert the content of mat to the binary cache ile
// filename: xxx.mat
void Cache::tocache(Mat mat, string filename)
{
    ofstream cachefile(filename, ios::out | ios::binary);
    cachefile.write((char *)&mat, sizeof(mat));
    cachefile.close();
}

// load the cache
void Cache::load_cache(string filename, Mat mat)
{
    ifstream cachefile(filename,ios::in|ios::binary);
    try
    {
        cachefile.read(reinterpret_cast<char *>(&mat),sizeof(mat));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}