/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-07 09:52:08
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2021-12-14 19:36:05
 */
#include <iostream>

#include "util/SocketManager.h"
#include "core/Player.h"
#include "util/IOManager.h"
#include "machine_learning/BPGraph.h"
#include "machine_learning/LSTMGraph.h"


int main(int argc, char** argv) {
    srand(time(NULL)); // random seed
    DBGtest=0;
    if (argc < 2) {
        DBGprint("Please enter party index:\n");
        scanf("%d", &node_type);
    }
    else {
        node_type = argv[1][0] - '0';
    }
    DBGprint("party index: %d\n", node_type);


    Player::init();
    IOManager::init("../datasets/test/mnist_train.csv", "../datasets/test/mnist_test.csv");
    SocketManager::SMMLF tel;
    if (!Config::config->LOCAL_TEST) {
        tel.init(Config::config->IP,Config::config->PORT);
    } else {
        tel.init();
    }

    /** LSTM **/
    // LSTMGraph::LSTM *lstm = new LSTMGraph::LSTM(&IOManager::train_data, &IOManager::train_label, &IOManager::test_data, &IOManager::test_label);
    // DBGprint("LSTM constructing ...\n");
    // lstm->graph();
    // DBGprint("LSTM constructed ...\n");
    // lstm->train();

    /** NN, Linear Regression, Logistic Regression **/
    BPGraph::LR *bp = new BPGraph::LR(&IOManager::train_data, &IOManager::train_label, &IOManager::test_data, &IOManager::test_label);

    /** Three-layer Model **/
    // bp->graph();

    if(Config::config->GRAPH_TYPE == Config::config->LOGISTIC){
        bp->logistic_graph();
    }
    else if(Config::config->GRAPH_TYPE == Config::config->LINEAR){
        bp->linear_graph();
    }
    bp->train();
    return 0;
}