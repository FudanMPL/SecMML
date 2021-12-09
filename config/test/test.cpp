/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-07 10:04:24
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2021-12-09 17:24:38
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
    cout<<config->MOD<<endl;
    cout<<config->LEAKEY_RELU_BIAS<<endl;
}   