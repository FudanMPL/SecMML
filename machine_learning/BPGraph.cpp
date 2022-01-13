//
// Created by tangdingyi on 2019/12/26.
//

#include "BPGraph.h"

// non-parameter Constructor Function

BPGraph::LR::LR() {}

// Constructor Function

BPGraph::LR::LR(Mat *train_data, Mat *train_label, Mat *test_data, Mat *test_label)
{
    DBGprint("LR constructor\n");
    nn = new NN();
    this->train_data = train_data;
    this->train_label = train_label;
    this->test_data = test_data;
    this->test_label = test_label;
    // train_data->print();
    // train_label->print();
}

// add nodes and edges to initialize graph

void BPGraph::LR::graph()
{
    int hidden_num = 10;
    int output_num = 1;
    input = nn->addnode(Config::config->D + 1, Config::config->B, NeuronMat::NODE_INPUT);
    output = nn->addnode(output_num, Config::config->B, NeuronMat::NODE_INPUT);
    // todo 添加正态随机分布的权重初始化值
    st_w = nn->addnode(hidden_num, Config::config->D + 1, NeuronMat::NODE_NET);
    st_mul = nn->addnode(hidden_num, Config::config->B, NeuronMat::NODE_OP);

    nd_hidden = nn->addnode(hidden_num, Config::config->B, NeuronMat::NODE_OP);
    nd_b = nn->addnode(1, Config::config->B, NeuronMat::NODE_NET);
    nd_add = nn->addnode(hidden_num + 1, Config::config->B, NeuronMat::NODE_OP);
    nd_w = nn->addnode(output_num, hidden_num + 1, NeuronMat::NODE_NET);
    // nd_mul = nn->addnode(1, Config::config->B, NeuronMat::NODE_OP);
    // activation = nn->addnode(hidden_num, Config::config->B, NeuronMat::NODE_OP);
    out_sig = nn->addnode(output_num, Config::config->B, NeuronMat::NODE_OP);
    // activation_out = nn->addnode(hidden_num, Config::config->B, NeuronMat::NODE_OP);
    int sd = nn->addnode(1, 1, NeuronMat::NODE_OP);
    argmax = nn->addnode(1, 1, NeuronMat::NODE_OP);
    DBGprint("initializing!\n");
    nn->global_variables_initializer();
    DBGprint("initialized!\n");

    nn->addOpMul_Mat(st_mul, st_w, input);

    nn->addOpSigmoid(nd_hidden, st_mul);

    nn->addOpVstack(nd_add, nd_hidden, nd_b);
    // nn->addOpMul_Mat(out_sig, nd_w, nd_add);

    // nn->addOpReLU(out_sig, st_mul);
    nn->addOpMul_Mat(out_sig, nd_w, nd_add);

    // nn->addOpVia(out_sig, st_mul);
    // nn->addOpTanh(activation_out,st_mul);
    // nn->addOpHybrid_Tanh(out_sig,st_mul);
    // nn->addOpTanh_ex(out_sig,st_mul);
    // nn->addOpSigmoid(out_sig,st_mul);

    //    nn->addOpHybrid_Tanh(activation_out, st_mul);
    //    nn->addOpTanh_change(out_sig, activation_out);

    nn->addOpMeanSquaredLoss(sd, output, out_sig);
    nn->addOpSimilar(argmax, output, out_sig);
    nn->toposort();

    nn->reveal_init(out_sig);
    // if the output is secret-shared as well
    nn->reveal_init(output);
}

// initialize the graph of linear

void BPGraph::LR::linear_graph()
{
    DBGprint("Linear Regression constructor\n");
    int hidden_num = 1;
    int output_num = 1;
    input = nn->addnode(Config::config->D + 1, Config::config->B, NeuronMat::NODE_INPUT);
    output = nn->addnode(output_num, Config::config->B, NeuronMat::NODE_INPUT);
    // todo 添加正态随机分布的权重初始化值
    st_w = nn->addnode(hidden_num, Config::config->D + 1, NeuronMat::NODE_NET);
    st_mul = nn->addnode(hidden_num, Config::config->B, NeuronMat::NODE_OP);

    out_sig = nn->addnode(output_num, Config::config->B, NeuronMat::NODE_OP);
    int sd = nn->addnode(1, 1, NeuronMat::NODE_OP);
    argmax = nn->addnode(1, 1, NeuronMat::NODE_OP);

    DBGprint("initializing!\n");
    nn->global_variables_initializer();
    DBGprint("initialized!\n");

    nn->addOpMul_Mat(st_mul, st_w, input);

    //    nn->addOpSigmoid(nd_hidden, st_mul);

    //    nn->addOpVstack(nd_add, nd_hidden, nd_b);
    //
    //    nn->addOpMul_Mat(out_sig, nd_w, nd_add);

    // nn->addOpReLU(out_sig, st_mul);
    // nn->addOpMul_Mat(out_sig, nd_w, nd_add);

    nn->addOpVia(out_sig, st_mul);
    //    nn->addOpTanh(activation_out,st_mul);
    // nn->addOpHybrid_Tanh(out_sig,st_mul);
    // nn->addOpTanh_ex(out_sig,st_mul);
    // nn->addOpSigmoid(out_sig,st_mul);

    //    nn->addOpHybrid_Tanh(activation_out, st_mul);
    //    nn->addOpTanh_change(out_sig, activation_out);

    nn->addOpMeanSquaredLoss(sd, output, out_sig);
    nn->addOpSimilar(argmax, output, out_sig);
    nn->toposort();

    nn->reveal_init(out_sig);
    /// if the output is secret-shared as well
    nn->reveal_init(output);
    // nn->output_init(st_w);
}

// initialize the graph of logistic

void BPGraph::LR::logistic_graph()
{
    DBGprint("Logistic Regression constructor\n");
    int hidden_num = 1;
    int output_num = 1;
    input = nn->addnode(Config::config->D + 1, Config::config->B, NeuronMat::NODE_INPUT);
    output = nn->addnode(output_num, Config::config->B, NeuronMat::NODE_INPUT);
    // todo 添加正态随机分布的权重初始化值
    st_w = nn->addnode(hidden_num, Config::config->D + 1, NeuronMat::NODE_NET);
    st_mul = nn->addnode(hidden_num, Config::config->B, NeuronMat::NODE_OP);

    out_sig = nn->addnode(output_num, Config::config->B, NeuronMat::NODE_OP);
    int sd = nn->addnode(1, 1, NeuronMat::NODE_OP);
    argmax = nn->addnode(1, 1, NeuronMat::NODE_OP);

    DBGprint("initializing!\n");
    nn->global_variables_initializer();
    DBGprint("initialized!\n");

    nn->addOpMul_Mat(st_mul, st_w, input);
    if (Config::config->ACTIVATION == Config::config->SIGMOID)
    {
        nn->addOpSigmoid(out_sig, st_mul);
    }
    else if (Config::config->ACTIVATION == Config::config->TANH)
    {
        nn->addOpHybrid_Tanh(out_sig, st_mul);
    }

    nn->addOpMeanSquaredLoss(sd, output, out_sig);
    nn->addOpSimilar(argmax, output, out_sig);
    nn->toposort();

    nn->reveal_init(out_sig);
    /// if the output is secret-shared as well
    nn->reveal_init(output);
}

// training of Linear regression or Logistic regression

void BPGraph::LR::train()
{
    Mat x_batch(Config::config->D + 1, Config::config->B), y_batch(1, Config::config->B);
    clock_train = new Constant::Clock(Config::config->CLOCK_TRAIN);
    globalRound = 0;
    double IOtime = 0;
    test();
    cout << "Train\n";
    for (int i = 0; i < 110000 && i < Config::config->TRAIN_ITE; i++)
    {
        signal(SIGINT, SIG_DFL);
        globalRound++;
        IOtime -= clock_train->get();
        next_batch(x_batch, i * Config::config->B, train_data, Config::config->N);
        next_batch(y_batch, i * Config::config->B, train_label, Config::config->N);
        feed(nn, x_batch, y_batch, input, output);
        IOtime += clock_train->get();
        {
            nn->epoch_init();
            while (nn->forwardHasNext())
            {
                nn->forwardNext();
            }
            while (nn->backHasNext())
            {
                nn->backNext();
            }
            while (nn->updateHasNext())
            {
                nn->update();
            }
            nn->gradUpdate();
        }
        if ((i) % Config::config->PRINT_PRE_ITE == 0)
        {
            test();
            print_perd(i + 1);
            DBGprint("IOTime: %.3f\n", IOtime);
        }
    }
    // nn->output_init();
    // while (nn->outputHasNext()) {
    //     nn->output();
    // }
    // nn->getNeuron(st_w)->getForward()->print();
}

// testing of Linear regression or Logistic regression

void BPGraph::LR::test()
{
    Mat x_batch(Config::config->D + 1, Config::config->B), y_batch(1, Config::config->B);
    int total = 0;
    for (int i = 0; i < Config::config->NM / Config::config->B; i++)
    {
        signal(SIGINT, SIG_DFL);
        globalRound++;
        next_batch(x_batch, i * Config::config->B, test_data);
        next_batch(y_batch, i * Config::config->B, test_label);
        feed(nn, x_batch, y_batch, input, output);
        nn->epoch_init();
        while (nn->forwardHasNext())
        {
            nn->forwardNext();
        }
        //        nn->getNeuron(out_sig)->getForward()->print();
        //        nn->getNeuron(output)->getForward()->print();
        nn->reveal_init();
        while (nn->revealHasNext())
        {
            nn->reveal();
        }
        //        nn->getNeuron(out_sig)->getForward()->print();
        //        DBGprint("-------------\n");
        //        nn->getNeuron(output)->getForward()->print();
        total += nn->getNeuron(output)->getForward()->equal(*nn->getNeuron(out_sig)->getForward()).count();
        // cout << "count:" << total << endl;
    }
    DBGprint("accuracy: %f\n", total * 1.0 / (Config::config->NM / Config::config->B * Config::config->B));
}

// Import data and label into Neurons

void BPGraph::LR::feed(NN *nn, Mat &x_batch, Mat &y_batch, int input, int output)
{
    *nn->getNeuron(input)->getForward() = x_batch;
    *nn->getNeuron(output)->getForward() = y_batch;
}

// take data and label of the next batch

void BPGraph::LR::next_batch(Mat &batch, int start, Mat *A, int mod)
{
    int begin = start % mod;
    int end = start % mod + Config::config->B;
    if (begin <= end)
    {
        A->col(begin, end, batch);
    }
    else
    {
        batch.append(A->col(end, mod));
        batch.append(A->col(0, begin));
    }
}

// print transmitted data size, round and time

void BPGraph::LR::print_perd(int round)
{
    ll tot_send = 0, tot_recv = 0;
    for (int i = 0; i < Config::config->M; i++)
    {
        if (node_type != i)
        {
            tot_send += SocketManager::socket_io[node_type][i]->send_num;
            tot_recv += SocketManager::socket_io[node_type][i]->recv_num;
        }
    }
    DBGprint("round: %d tot_time: %.3f ",
             round, clock_train->get());
    DBGprint("tot_send: %lld tot_recv: %lld\n", tot_send, tot_recv);
}