/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-07 10:04:24
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2021-12-14 10:23:26
 */

#include <cstdio>
#include <fstream>
#include "json/json.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include "../config/Config.hpp"
using namespace std;



int main()
{
    Config* config = Config::init("../config/parameter/constant.json");
    cout<<config->B<<endl;
    cout<<config->IP[0]<<endl;
    cout<<config->PORT[0]<<endl;
}   