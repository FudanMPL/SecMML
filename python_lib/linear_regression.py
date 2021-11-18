from queqiao import Constant, IOManager, NeuronMat, BPGraph_LR


def graph(io:IOManager):
    hidden_num = 1
    output_num = 1
    model = BPGraph_LR(io.train_data, io.train_label, io.test_data, io.test_label)
    nn = model.nn

    model.input = nn.addnode(Constant.D + 1, Constant.B, NeuronMat.NODE_INPUT)
    model.output = nn.addnode(output_num, Constant.B, NeuronMat.NODE_INPUT)
    model.st_w = nn.addnode(hidden_num, Constant.D + 1, NeuronMat.NODE_NET)
    model.st_mul = nn.addnode(hidden_num, Constant.B, NeuronMat.NODE_OP)
    model.out_sig = nn.addnode(output_num, Constant.B, NeuronMat.NODE_OP)
    model.sd = nn.addnode(1, 1, NeuronMat.NODE_OP)
    model.argmax = nn.addnode(1, 1, NeuronMat.NODE_OP)
    print("initializing!")
    nn.global_variables_initializer()
    print("initialized!")
    nn.addOpMul_Mat(model.st_mul, model.st_w, model.input)
    nn.addOpVia(model.out_sig, model.st_mul)
    nn.addOpMeanSquaredLoss(model.sd, model.output, model.out_sig)
    nn.addOpSimilar(model.argmax, model.output, model.out_sig)
    nn.toposort()
    nn.reveal_init(model.out_sig)
    nn.reveal_init(model.output)

    return model