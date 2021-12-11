/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-07 10:04:24
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2021-12-11 14:39:13
 */

#include <cstdio>
#include <fstream>
#include "json/json.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include "../Config.hpp"
using namespace std;



int main()
{
    Config* config = Config::init();
    cout<<config->B<<endl;
    cout<<config->IP[0]<<endl;
    cout<<config->PORT[0]<<endl;
}   