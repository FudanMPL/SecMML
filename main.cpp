#include <iostream>

#include "util/SocketManager.h"
#include "core/Player.h"
#include "util/IOManager.h"
#include "machine_learning/BPGraph.h"
#include "machine_learning/LSTMGraph.h"

int node_type;
SocketManager::SMMLF tel;
int globalRound;
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
    IOManager::init();
    tel.init();

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

    /** Logisitc Regression Model **/
    // bp->logistic_graph();

    /** Linear Regression Model **/
    bp->linear_graph();
    bp->train();
    return 0;
}