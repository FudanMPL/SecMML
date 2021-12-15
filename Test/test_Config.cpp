/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-07 10:04:24
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2021-12-15 20:47:52
 */

#include <cstdio>
#include <fstream>
#include "json/json.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include "../core/Player.h"
#include "../util/IOManager.h"
using namespace std;

int main()
{
    Config::config = Config::init("../config/parameter/constant.json");
    cout<<Config::config->B<<endl;
    cout<<Config::config->IP[0]<<endl;
    cout<<Config::config->PORT[0]<<endl;
    cout<<Config::config->SQRTINV<<endl;
    cout<<Config::config->INV2<<endl;

    Player::init();
    IOManager::init();
    
}   