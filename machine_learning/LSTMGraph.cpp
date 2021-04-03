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
    int hidden_num = 1;
    //cout<<"h0"<<endl;
    h0=nn->addnode(CH,B,NeuronMat::NODE_NET);
    //cout<<"c0"<<endl;
    c0=nn->addnode(CH,B,NeuronMat::NODE_NET);
    for (int i=0;i<L;i++) {
        //cout<<"x"<<i<<endl;
        x[i] = nn->addnode(D2 + 1, B, NeuronMat::NODE_INPUT);
    }
    //cout<<"output"<<endl;
    output = nn->addnode(CH, B, NeuronMat::NODE_INPUT);
    cells[0]=new Cell(nn,c0,h0,x[0]);
    Mat* w_f,*w_c,*w_i,*w_o;



    for (int i=1;i<L;i++){
        cells[i]=new Cell(nn,cells[i-1]->getC_out(),cells[i-1]->getH_out(),x[i]);
    }
    DBGprint("cells added!\n");
    st_w = nn->addnode(hidden_num, CH+1, NeuronMat::NODE_NET);//536
    int bias = nn->addnode(hidden_num, B, NeuronMat::NODE_INPUT);//537
    st_b=nn->addnode(CH+1,B,NeuronMat::NODE_OP);//538
    st_mul = nn->addnode(hidden_num, B, NeuronMat::NODE_OP);//539
    out_sig = nn->addnode(hidden_num, B, NeuronMat::NODE_OP);//540
    int sd = nn->addnode(1, 1, NeuronMat::NODE_OP);//541
    argmax = nn->addnode(1, 1, NeuronMat::NODE_OP);//542


    DBGprint("initializing!\n");
    nn->global_variables_initializer();
    w_f=new Mat(CH,D2+CH+1,IE/4);
    w_c=new Mat(CH,D2+CH+1,IE/4);
    w_i=new Mat(CH,D2+CH+1,IE/4);
    w_o=new Mat(CH,D2+CH+1,IE/4);
    DBGprint("weight initializing!\n");
    nn->getNeuron(st_w)->setForward(new Mat(hidden_num, CH+1,IE/4));
    Mat::random_neg(w_f);
    Mat::random_neg(w_c);
    Mat::random_neg(w_i);
    Mat::random_neg(w_o);
    Mat::random_neg(nn->getNeuron(st_w)->getForward());
    nn->getNeuron(bias)->setForward(new Mat(hidden_num, B,IE));
    for(int i=0;i<L;i++)
        cells[i]->setWeight(w_f,w_i,w_c,w_o);
    DBGprint("edges adding!\n");
    for (int i=0;i<L;i++){
        cells[i]->addEdges();
    }
    DBGprint("edges added!\n");
    nn->addOpVstack(st_b,cells[L-1]->getH_out(),bias);
    nn->addOpMul_Mat(st_mul, st_w,st_b);
    nn->addOpSigmoid(out_sig,st_mul);

    nn->addOpMeanSquaredLoss(sd, output, out_sig);
    nn->addOpSimilar(argmax, output, out_sig);
    nn->toposort();

    nn->reveal_init(out_sig);
}


void LSTMGraph::LSTM::train() {
    Mat x_batch[L];
    for (int i=0;i<L;i++)
        x_batch[i]=*(new Mat(D2 + 1, B));
    Mat y_batch(1, B);
    clock_train = new Constant::Clock(CLOCK_TRAIN);
    globalRound = 0;
    for (int i = 0; i < 110000 && i < TRAIN_ITE; i++) {
        cout<<i<<endl;
        globalRound++;
        next_batch(x_batch, i * B, train_data, N);
        next_batch(y_batch, i * B, train_label, N);
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
        if ((i+1)%PRINT_PRE_ITE == 0) {
            DBGprint("testing ...\n",i);
            test();
            print_perd(i+1);
        }
    }
}

void LSTMGraph::LSTM::test() {
    Mat x_batch[L];
    for (int i=0;i<L;i++)
        x_batch[i]=*(new Mat(D2 + 1, B));
    Mat y_batch(1, B);
    int total = 0;
    ofstream out_file;
    out_file.open("accuracy_lstm.txt", ios::app);
    for (int i = 0; i < NM / B; i++) {
        globalRound++;
        next_batch(x_batch, i * B, train_data);
        next_batch(y_batch, i * B, train_label);
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
    DBGprint("accuracy: \n", total * 1.0 / (NM / B * B));
    out_file << "accuracy: \n" << total * 1.0 / (NM / B * B) << endl;
    out_file.close();
}

void LSTMGraph::LSTM::LSTM::feed(NN* nn, Mat (&x_batch)[L], Mat &y_batch, int *input, int output) {
    for (int i=0;i<L;i++)
        *nn->getNeuron(input[i])->getForward() = x_batch[i];
    *nn->getNeuron(output)->getForward() = y_batch;
}

void LSTMGraph::LSTM::next_batch(Mat &batch, int start, Mat *A, int mod) {
    A->col(start % mod, start % mod + B, batch);
}
void LSTMGraph::LSTM::next_batch(Mat (&batch)[L], int start, Mat *A, int mod) {
    Mat *temp,*temp_IE,*tempI;
    temp=new Mat(D+1,B);
    tempI=new Mat(D2,B);
    A->col(start % mod, start % mod + B, *temp);
    for (int i=0;i<L;i++){
        *tempI=temp->row(i*D2, (i+1)*D2);
        Mat::hstack(&batch[i],tempI,new Mat(1, B, IE));
    }
}



void LSTMGraph::LSTM::print_perd(int round) {
    ll tot_send = 0, tot_recv = 0;
    for (int i = 0; i < M; i++) {
        if (node_type != i) {
            tot_send += socket_io[node_type][i]->send_num;
            tot_recv += socket_io[node_type][i]->recv_num;
        }
    }
    DBGprint("round: %d tot_time: %.3f ",
             round, clock_train->get());
    DBGprint("tot_send: %lld tot_recv: %lld\n", tot_send, tot_recv);
}