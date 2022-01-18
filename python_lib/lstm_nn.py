from queqiao import Config, IOManager, Mat, NeuronMat, Cell, LSTMGraph_LSTM


def graph():
    hidden_num = 1
    model = LSTMGraph_LSTM(IOManager.train_data, IOManager.train_label, IOManager.test_data, IOManager.test_label)
    model.resize_cells(Config.config.L)
    model.resize_x(Config.config.L)
    nn = model.nn

    model.h0 = nn.addnode(Config.config.CH, Config.config.B, NeuronMat.NODE_NET)
    model.c0 = nn.addnode(Config.config.CH, Config.config.B, NeuronMat.NODE_NET)
    for i in range(0, Config.config.L):
        model.set_x(i, nn.addnode(Config.config.D2 + 1, Config.config.B, NeuronMat.NODE_INPUT))
    model.output = nn.addnode(Config.config.CH, Config.config.B, NeuronMat.NODE_INPUT)
    model.set_cell(0, Cell(nn, model.c0, model.h0, model.get_x(0)))
    for i in range(1, Config.config.L):
        model.set_cell(i, Cell(nn, model.get_cell(i - 1).getC_out(), model.get_cell(i - 1).getH_out(), model.get_x(i)))
    print("cell added!")
    model.st_w = nn.addnode(hidden_num, Config.config.CH + 1, NeuronMat.NODE_NET)
    bias = nn.addnode(hidden_num, Config.config.B, NeuronMat.NODE_INPUT)
    model.st_b = nn.addnode(Config.config.CH + 1,Config.config.B, NeuronMat.NODE_OP)
    model.st_mul = nn.addnode(hidden_num, Config.config.B, NeuronMat.NODE_OP)
    model.out_sig = nn.addnode(hidden_num, Config.config.B, NeuronMat.NODE_OP)
    sd = nn.addnode(1, 1, NeuronMat.NODE_OP)
    model.argmax = nn.addnode(1, 1, NeuronMat.NODE_OP)
    print("initializing!")
    nn.global_variables_initializer()
    w_f = Mat(Config.config.CH, Config.config.D2 + Config.config.CH + 1, Config.config.IE // 4)
    w_c = Mat(Config.config.CH, Config.config.D2 + Config.config.CH + 1, Config.config.IE // 4)
    w_i = Mat(Config.config.CH, Config.config.D2 + Config.config.CH + 1, Config.config.IE // 4)
    w_o = Mat(Config.config.CH, Config.config.D2 + Config.config.CH + 1, Config.config.IE // 4)
    print("weight initializing!")
    nn.getNeuron(model.st_w).setForward(Mat(hidden_num, Config.config.CH + 1, Config.config.IE // 4))
    Mat.random_neg(w_f)
    Mat.random_neg(w_c)
    Mat.random_neg(w_i)
    Mat.random_neg(w_o)
    Mat.random_neg(nn.getNeuron(model.st_w).getForward())
    nn.getNeuron(bias).setForward(Mat(hidden_num, Config.config.B, Config.config.IE))
    for i in range(0, Config.config.L):
        model.get_cell(i).setWeight(w_f, w_i, w_c, w_o)
    print("edges adding!")
    for i in range(0, Config.config.L):
        model.get_cell(i).addEdges()
    print("edges added!")
    nn.addOpVstack(model.st_b, model.get_cell(Config.config.L - 1).getH_out(), bias)
    nn.addOpMul_Mat(model.st_mul, model.st_w, model.st_b)
    nn.addOpSigmoid(model.out_sig, model.st_mul)
    nn.addOpMeanSquaredLoss(sd, model.output, model.out_sig)
    nn.addOpSimilar(model.argmax, model.output, model.out_sig)
    nn.toposort()
    nn.reveal_init(model.out_sig)

    return model