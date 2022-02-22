/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-07 09:52:08
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2022-02-22 15:46:06
 */
#include <iostream>

#include "json/json.h"
#include "util/SocketManager.h"
#include "core/Player.h"
#include "util/IOManager.h"
#include "machine_learning/BPGraph.h"
#include "machine_learning/LSTMGraph.h"
#include <iostream>

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

    
    Config::config = Config::init("constant.json");
    Player::init();
    IOManager::init_local_data();
    for(int i = 0; i < IOManager::train_label.cols(); i++){
        if(IOManager::train_label(0,i) > Config::config->IE){
            IOManager::train_label(0,i) = Config::config->IE;
        }
    }
    for(int i = 0; i < IOManager::test_label.cols(); i++){
        if(IOManager::test_label(0,i) > Config::config->IE){
            IOManager::test_label(0,i) = Config::config->IE;
        }
    }
    // normalization
    IOManager::train_data = IOManager::train_data/256;
    IOManager::test_data = IOManager::test_data/256;
    SocketManager::SMMLF tel;
    if (!Config::config->LOCAL_TEST) {
        tel.init(Config::config->IP,Config::config->PORT);
    } else {
        tel.init();
    }

    if(Config::config->GRAPH_TYPE == 0){
        // Logistic regression
        DBGprint("Logistic Regression begins:");
        BPGraph::LR *bp = new BPGraph::LR(&IOManager::train_data, &IOManager::train_label, &IOManager::test_data, &IOManager::test_label);
        bp->graph();
        bp->logistic_graph();
        bp->train();
    }
    else if(Config::config->GRAPH_TYPE == 1){
        // Linear regression
        DBGprint("Linear Regression begins:");
        BPGraph::LR *bp = new BPGraph::LR(&IOManager::train_data, &IOManager::train_label, &IOManager::test_data, &IOManager::test_label);
        bp->graph();
        bp->linear_graph();
        bp->train();
    }
    else if(Config::config->GRAPH_TYPE == 2){
        // LSTM
        LSTMGraph::LSTM *lstm = new LSTMGraph::LSTM(&IOManager::train_data, &IOManager::train_label, &IOManager::test_data, &IOManager::test_label);
        DBGprint("LSTM constructing ...\n");
        lstm->graph();
        DBGprint("LSTM constructed ...\n");
        lstm->train();
    }
    
    return 0;
}