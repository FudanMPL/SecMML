from MPL_lib import cvar, IOManager, Mat, NeuronMat, NN, Cell, LSTMGraph_LSTM
from MPL_lib import intArray_setitem, intArray_getitem


def graph(io:IOManager):
    hidden_num = 1
    model = LSTMGraph_LSTM(io.train_data, io.train_label, io.test_data, io.test_label)
    nn = model.get_nn() # type: NN

    model.h0 = nn.addnode(cvar.CONST_CH, cvar.CONST_B, NeuronMat.NODE_NET)
    model.c0 = nn.addnode(cvar.CONST_CH, cvar.CONST_B, NeuronMat.NODE_NET)
    for i in range(0, cvar.CONST_L):
        intArray_setitem(model.x, i, nn.addnode(cvar.CONST_D2 + 1, cvar.CONST_B, NeuronMat.NODE_INPUT))
    model.output = nn.addnode(cvar.CONST_CH, cvar.CONST_B, NeuronMat.NODE_INPUT)
    model.set_cell(0, Cell(nn, model.c0, model.h0, intArray_getitem(model.x, 0)))
    for i in range(1, cvar.CONST_L):
        model.set_cell(i, Cell(nn, model.get_cell(i - 1).getC_out(), model.get_cell(i - 1).getH_out(), intArray_getitem(model.x, i)))
    print("cell added!")
    model.st_w = nn.addnode(hidden_num, cvar.CONST_CH + 1, NeuronMat.NODE_NET)
    bias = nn.addnode(hidden_num, cvar.CONST_B, NeuronMat.NODE_INPUT)
    model.st_b = nn.addnode(cvar.CONST_CH + 1,cvar.CONST_B, NeuronMat.NODE_OP)
    model.st_mul = nn.addnode(hidden_num, cvar.CONST_B, NeuronMat.NODE_OP)
    model.out_sig = nn.addnode(hidden_num, cvar.CONST_B, NeuronMat.NODE_OP)
    sd = nn.addnode(1, 1, NeuronMat.NODE_OP)
    model.argmax = nn.addnode(1, 1, NeuronMat.NODE_OP)
    print("initializing!")
    nn.global_variables_initializer()
    w_f = Mat(cvar.CONST_CH, cvar.CONST_D2 + cvar.CONST_CH + 1, cvar.CONST_IE // 4)
    w_c = Mat(cvar.CONST_CH, cvar.CONST_D2 + cvar.CONST_CH + 1, cvar.CONST_IE // 4)
    w_i = Mat(cvar.CONST_CH, cvar.CONST_D2 + cvar.CONST_CH + 1, cvar.CONST_IE // 4)
    w_o = Mat(cvar.CONST_CH, cvar.CONST_D2 + cvar.CONST_CH + 1, cvar.CONST_IE // 4)
    print("weight initializing!")
    nn.getNeuron(model.st_w).setForward(Mat(hidden_num, cvar.CONST_CH + 1, cvar.CONST_IE // 4))
    Mat.random_neg(w_f)
    Mat.random_neg(w_c)
    Mat.random_neg(w_i)
    Mat.random_neg(w_o)
    Mat.random_neg(nn.getNeuron(model.st_w).getForward())
    nn.getNeuron(bias).setForward(Mat(hidden_num, cvar.CONST_B, cvar.CONST_IE))
    for i in range(0, cvar.CONST_L):
        model.get_cell(i).setWeight(w_f, w_i, w_c, w_o)
    print("edges adding!")
    for i in range(0, cvar.CONST_L):
        model.get_cell(i).addEdges()
    print("edges added!")
    nn.addOpVstack(model.st_b, model.get_cell(cvar.CONST_L - 1).getH_out(), bias)
    nn.addOpMul_Mat(model.st_mul, model.st_w, model.st_b)
    nn.addOpSigmoid(model.out_sig, model.st_mul)
    nn.addOpMeanSquaredLoss(sd, model.output, model.out_sig)
    nn.addOpSimilar(model.argmax, model.output, model.out_sig)
    nn.toposort()
    nn.reveal_init(model.out_sig)

    return model