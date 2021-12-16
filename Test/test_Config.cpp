/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-07 10:04:24
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2021-12-16 00:12:26
 */

#include <cstdio>
#include <fstream>
#include "json/json.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include "../util/SocketManager.h"
#include "../core/Player.h"
#include "../util/IOManager.h"
#include "../machine_learning/BPGraph.h"
// #include "../machine_learning/LSTMGraph.h"
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
    SocketManager::SMMLF tel;
    tel.init(Config::config->IP,Config::config->PORT);

    BPGraph::LR *bp = new BPGraph::LR(&IOManager::train_data, &IOManager::train_label, &IOManager::test_data, &IOManager::test_label);

    if(Config::config->GRAPH_TYPE == Config::config->LOGISTIC){
        bp->logistic_graph();
    }
    else if(Config::config->GRAPH_TYPE == Config::config->LINEAR){
        bp->linear_graph();
    }
    bp->train();
    return 0;
}   