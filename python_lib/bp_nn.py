from MPL_lib import cvar, IOManager, NeuronMat, NN, BPGraph_LR


def graph(io:IOManager):
    hidden_num = 10
    output_num = 1
    model = BPGraph_LR(io.train_data, io.train_label, io.test_data, io.test_label)
    nn = model.get_nn() # type: NN

    model.input = nn.addnode(cvar.CONST_D + 1, cvar.CONST_B, NeuronMat.NODE_INPUT)
    model.output = nn.addnode(output_num, cvar.CONST_B, NeuronMat.NODE_INPUT)
    model.st_w = nn.addnode(hidden_num, cvar.CONST_D + 1, NeuronMat.NODE_NET)
    model.st_mul = nn.addnode(hidden_num, cvar.CONST_B, NeuronMat.NODE_OP)
    model.nd_hidden = nn.addnode(hidden_num, cvar.CONST_B, NeuronMat.NODE_OP)
    model.nd_b = nn.addnode(1, cvar.CONST_B, NeuronMat.NODE_NET)
    model.nd_add = nn.addnode(hidden_num + 1, cvar.CONST_B, NeuronMat.NODE_OP)
    model.nd_w = nn.addnode(output_num, hidden_num + 1, NeuronMat.NODE_NET)
    model.out_sig = nn.addnode(output_num, cvar.CONST_B, NeuronMat.NODE_OP)
    model.sd = nn.addnode(1, 1, NeuronMat.NODE_OP)
    model.argmax = nn.addnode(1, 1, NeuronMat.NODE_OP)
    print("initializing!")
    nn.global_variables_initializer()
    print("initialized!")
    nn.addOpMul_Mat(model.st_mul, model.st_w, model.input)
    nn.addOpSigmoid(model.nd_hidden, model.st_mul)
    nn.addOpVstack(model.nd_add, model.nd_hidden, model.nd_b)
    nn.addOpMul_Mat(model.out_sig, model.nd_w, model.nd_add)
    nn.addOpMeanSquaredLoss(model.sd, model.output, model.out_sig)
    nn.addOpSimilar(model.argmax, model.output, model.out_sig)
    nn.toposort()
    nn.reveal_init(model.out_sig)
    nn.reveal_init(model.output)

    return model