import sys
from queqiao import set_seed
from queqiao import Config, Constant, Player, IOManager, SocketManager_SMMLF
from python_lib import linear_regression, logistic_regression, bp_nn, lstm_nn


if __name__ == '__main__':
    if (len(sys.argv) < 2):
        Constant.node_type = int(input("Please enter party index:\n"))
    else:
        Constant.node_type = int(sys.argv[1])
    print("party index: {}".format(Constant.node_type))

    set_seed()
    Config.config = Config.init("constant.json")
    Player.init()
    IOManager.init_local_data()

    for i in range(0, IOManager.train_label.cols()):
        if IOManager.train_label[0, i] > Config.config.IE:
            IOManager.train_label[0, i] = Config.config.IE
    for i in range(0, IOManager.test_label.cols()):
        if IOManager.test_label[0, i] > Config.config.IE:
            IOManager.test_label[0, i] = Config.config.IE
    IOManager.train_data = IOManager.train_data / 256
    IOManager.test_data = IOManager.test_data / 256

    tel = SocketManager_SMMLF()
    if not Config.config.LOCAL_TEST:
        tel.init(Config.config.IP, Config.config.PORT)
    else:
        tel.init()

    if Config.config.GRAPH_TYPE == Config.config.LINEAR:
        model = linear_regression.graph()
    elif Config.config.GRAPH_TYPE == Config.config.LOGISTIC:
        model = logistic_regression.graph()

    # Three-layer Back Propagation Model
    # model = bp_nn.graph()

    # LSTM
    # model = lstm_nn.graph()

    model.train()
