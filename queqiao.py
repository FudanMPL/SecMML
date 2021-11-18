import sys
from queqiao import set_seed
from queqiao import Constant, Player, IOManager, SocketManager_SMMLF
from python_lib import linear_regression, logistic_regression, bp_nn, lstm_nn


if __name__ == '__main__':
    if (len(sys.argv) < 2):
        Constant.node_type = int(input("Please enter party index:\n"))
    else:
        Constant.node_type = int(sys.argv[1])
    print("party index: {}".format(Constant.node_type))

    ips = ["127.0.0.1", "127.0.0.1", "127.0.0.1"]
    ports = [2222, 2223, 2224]

    set_seed()
    Player.init()
    io = IOManager()
    io.init()
    tel = SocketManager_SMMLF()
    tel.init(ips, ports)

    # Linear Regression Model
    model = linear_regression.graph(io)

    # Logisitc Regression Model
    # model = logistic_regression.graph(io)

    # Three-layer Back Propagation Model
    # model = bp_nn.graph(io)

    # LSTM
    # model = lstm_nn.graph(io)

    model.train()
