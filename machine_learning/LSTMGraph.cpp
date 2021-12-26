//
// Created by 姜子丰 on 2020/8/29.
//

#include "LSTMGraph.h"
//
// Created by tangdingyi on 2019/12/26.
//

LSTMGraph::LSTM::LSTM() {}

LSTMGraph::LSTM::LSTM(Mat *train_data, Mat *train_label, Mat *test_data, Mat *test_label) {
    DBGprint("LSTM constructor\n");
    nn = new NN();
    this->train_data = train_data;
    this->train_label = train_label;
    this->test_data = test_data;
    this->test_label = test_label;
}

void LSTMGraph::LSTM::graph() {
    cells.resize(Config::config->L);
    x.resize(Config::config->L);
    int hidden_num = 1;
    //cout<<"h0"<<endl;
    h0=nn->addnode(Config::config->CH,Config::config->B,NeuronMat::NODE_NET);
    //cout<<"c0"<<endl;
    c0=nn->addnode(Config::config->CH,Config::config->B,NeuronMat::NODE_NET);
    for (int i=0;i<Config::config->L;i++) {
        //cout<<"x"<<i<<endl;
        x[i] = nn->addnode(Config::config->D2 + 1, Config::config->B, NeuronMat::NODE_INPUT);
    }
    //cout<<"output"<<endl;
    output = nn->addnode(Config::config->CH, Config::config->B, NeuronMat::NODE_INPUT);
    cells[0]=new Cell(nn,c0,h0,x[0]);
    Mat* w_f,*w_c,*w_i,*w_o;



    for (int i=1;i<Config::config->L;i++){
        cells[i]=new Cell(nn,cells[i-1]->getC_out(),cells[i-1]->getH_out(),x[i]);
    }
    DBGprint("cells added!\n");
    st_w = nn->addnode(hidden_num, Config::config->CH+1, NeuronMat::NODE_NET);//536
    int bias = nn->addnode(hidden_num, Config::config->B, NeuronMat::NODE_INPUT);//537
    st_b=nn->addnode(Config::config->CH+1,Config::config->B,NeuronMat::NODE_OP);//538
    st_mul = nn->addnode(hidden_num, Config::config->B, NeuronMat::NODE_OP);//539
    out_sig = nn->addnode(hidden_num, Config::config->B, NeuronMat::NODE_OP);//540
    int sd = nn->addnode(1, 1, NeuronMat::NODE_OP);//541
    argmax = nn->addnode(1, 1, NeuronMat::NODE_OP);//542


    DBGprint("initializing!\n");
    nn->global_variables_initializer();
    w_f=new Mat(Config::config->CH,Config::config->D2+Config::config->CH+1,Config::config->IE/4);
    w_c=new Mat(Config::config->CH,Config::config->D2+Config::config->CH+1,Config::config->IE/4);
    w_i=new Mat(Config::config->CH,Config::config->D2+Config::config->CH+1,Config::config->IE/4);
    w_o=new Mat(Config::config->CH,Config::config->D2+Config::config->CH+1,Config::config->IE/4);
    DBGprint("weight initializing!\n");
    nn->getNeuron(st_w)->setForward(new Mat(hidden_num, Config::config->CH+1,Config::config->IE/4));
    Mat::random_neg(w_f);
    Mat::random_neg(w_c);
    Mat::random_neg(w_i);
    Mat::random_neg(w_o);
    Mat::random_neg(nn->getNeuron(st_w)->getForward());
    nn->getNeuron(bias)->setForward(new Mat(hidden_num, Config::config->B,Config::config->IE));
    for(int i=0;i<Config::config->L;i++)
        cells[i]->setWeight(w_f,w_i,w_c,w_o);
    DBGprint("edges adding!\n");
    for (int i=0;i<Config::config->L;i++){
        cells[i]->addEdges();
    }
    DBGprint("edges added!\n");
    nn->addOpVstack(st_b,cells[Config::config->L-1]->getH_out(),bias);
    nn->addOpMul_Mat(st_mul, st_w,st_b);
    nn->addOpSigmoid(out_sig,st_mul);

    nn->addOpMeanSquaredLoss(sd, output, out_sig);
    nn->addOpSimilar(argmax, output, out_sig);
    nn->toposort();

    nn->reveal_init(out_sig);
}


void LSTMGraph::LSTM::train() {
    vector<Mat> x_batch;
    for (int i=0;i<Config::config->L;i++){
        x_batch.push_back(*(new Mat(Config::config->D2 + 1, Config::config->B)));
    }
    Mat y_batch(1, Config::config->B);
    clock_train = new Constant::Clock(Config::config->CLOCK_TRAIN);
    globalRound = 0;
    for (int i = 0; i < 110000 && i < Config::config->TRAIN_ITE; i++) {
        signal(SIGINT, SIG_DFL);
        cout<<i<<endl;
        globalRound++;
        next_batch(x_batch, i * Config::config->B, train_data, Config::config->N);
        next_batch(y_batch, i * Config::config->B, train_label, Config::config->N);        
        feed(nn, x_batch, y_batch, x, output);
        DBGprint("batch %d feeded ...\n",i);
        {
            nn->epoch_init();

            DBGprint("forwarding ...\n",i);
            while (nn->forwardHasNext()) {
                nn->forwardNext();
            }
            DBGprint("backwarding ...\n",i);
            while (nn->backHasNext()) {
                nn->backNext();

            }
            DBGprint("updating ...\n",i);
            while (nn->updateHasNext()) {
                nn->update();

            }
            nn->gradUpdate();
            // cout<<"w_f:"<<endl;
            // nn->getNeuron(cells[0]->getW_f())->getForward()->print();
            // cout<<"w_c:"<<endl;
            // nn->getNeuron(cells[0]->getW_c())->getForward()->print();
            // cout<<"w_i:"<<endl;
            // nn->getNeuron(cells[0]->getW_i())->getForward()->print();
            // cout<<"w_o:"<<endl;
        }
        if ((i+1)%Config::config->PRINT_PRE_ITE == 0) {
            DBGprint("testing ...\n",i);
            test();
            print_perd(i+1);
        }
    }
}

void LSTMGraph::LSTM::test() {
    vector<Mat> x_batch(Config::config->L);
    for (int i=0;i<Config::config->L;i++)
        x_batch[i]=*(new Mat(Config::config->D2 + 1, Config::config->B));
    Mat y_batch(1, Config::config->B);
    int total = 0;
    ofstream out_file;
    out_file.open("accuracy_lstm.txt", ios::app);
    for (int i = 0; i < Config::config->NM / Config::config->B; i++) {
        signal(SIGINT, SIG_DFL);
        globalRound++;
        next_batch(x_batch, i * Config::config->B, train_data, Config::config->NM);
        next_batch(y_batch, i * Config::config->B, train_label, Config::config->NM);
        feed(nn, x_batch, y_batch, x, output);
        nn->epoch_init();
        DBGprint("forwarding ...\n",i);
        while (nn->forwardHasNext()) {
            nn->forwardNext();
        }
        DBGprint("revealing ...\n",i);
        nn->reveal_init();
        while (nn->revealHasNext()) {
            nn->reveal();
        }

        total += nn->getNeuron(output)->getForward()->equal(*nn->getNeuron(out_sig)->getForward()).count();\
    }
    DBGprint("accuracy: %f\n", total * 1.0 / (Config::config->NM / Config::config->B * Config::config->B));
    out_file << "accuracy: \n" << total * 1.0 / (Config::config->NM / Config::config->B * Config::config->B) << endl;
    out_file.close();
}

void LSTMGraph::LSTM::LSTM::feed(NN* nn, vector<Mat> &x_batch, Mat &y_batch, vector<int> input, int output) {
    for (int i=0;i<Config::config->L;i++)
        *nn->getNeuron(input[i])->getForward() = x_batch[i];
    *nn->getNeuron(output)->getForward() = y_batch;
}

void LSTMGraph::LSTM::next_batch(Mat &batch, int start, Mat *A, int mod) {
    A->col(start % mod, start % mod + Config::config->B, batch);
}
void LSTMGraph::LSTM::next_batch(vector<Mat> &batch, int start, Mat *A, int mod) {
    Mat *temp,*temp_IE,*tempI;
    temp=new Mat(Config::config->D+1,Config::config->B);
    temp_IE=new Mat(1,Config::config->B,Config::config->IE);
    tempI=new Mat(Config::config->D2,Config::config->B);
    A->col(start % mod, start % mod + Config::config->B, *temp);
    for (int i=0;i<Config::config->L;i++){
        *tempI=temp->row(i*Config::config->D2, (i+1)*Config::config->D2);
        Mat::concat(&batch[i],tempI,temp_IE);
    }
    delete temp;
    delete temp_IE;
    delete tempI;
}



void LSTMGraph::LSTM::print_perd(int round) {
    ll tot_send = 0, tot_recv = 0;
    for (int i = 0; i < Config::config->M; i++) {
        if (node_type != i) {
            tot_send += SocketManager::socket_io[node_type][i]->send_num;
            tot_recv += SocketManager::socket_io[node_type][i]->recv_num;
        }
    }
    DBGprint("round: %d tot_time: %.3f ",
             round, clock_train->get());
    DBGprint("tot_send: %lld tot_recv: %lld\n", tot_send, tot_recv);
}