/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-07 10:04:24
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2022-01-13 14:14:07
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
#include <typeinfo>
// #include "../machine_learning/LSTMGraph.h"
using namespace std;

int main(int argc, char **argv)
{
    cout << "begin" << endl;
    Config::config = Config::init("../constant.json");
    cout << Config::config->B << endl;
    cout << Config::config->SQRTINV << endl;
    cout << Config::config->INV2 << endl;
    cout << Config::config->LOCAL_TEST << endl;
    DBGtest = 0;
    if (argc < 2)
    {
        DBGprint("Please enter party index:\n");
        scanf("%d", &node_type);
    }
    else
    {
        node_type = argv[1][0] - '0';
    }
    DBGprint("party index: %d\n", node_type);

    Player::init();

    IOManager::init_local_data();

    SocketManager::SMMLF tel;
    if (Config::config->LOCAL_TEST)
    {
        tel.init(Config::config->IP, Config::config->PORT);
    }
    else
    {
        tel.init();
    }

    // generate the secret mat of the local mat
    // and then
    // exchange the secret mat
    IOManager::exchange_data();
    cout<<"exchange data end"<<endl;


    BPGraph::LR *bp = new BPGraph::LR(&IOManager::train_data, &IOManager::train_label, &IOManager::test_data, &IOManager::test_label);

    if (Config::config->LOGISTIC == 1)
    {
        bp->logistic_graph();
    }
    else if (Config::config->LINEAR == 1)
    {
        bp->linear_graph();
    }
    bp->train();
    return 0;
}