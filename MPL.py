import sys
from MPL_lib import Player, IOManager, SocketManager_SMMLF
from MPL_lib import cvar, set_seed
from MPL_lib import new_intArray, intArray_setitem, new_stringArray, stringArray_setitem
from python_lib import linear_regression, logistic_regression, bp_nn, lstm_nn


def stringArray(arr):
    """Convert a python list to a c++ string array."""
    str_array = new_stringArray(len(arr))
    for i in range(0, len(arr)):
        stringArray_setitem(str_array, i, arr[i])
    return str_array

def intArray(arr):
    """Convert a python list to a c++ int array."""
    int_array = new_intArray(len(arr))
    for i in range(0, len(arr)):
        intArray_setitem(int_array, i, arr[i])
    return int_array

if __name__ == '__main__':
    if (len(sys.argv) < 2):
        cvar.node_type = int(input("Please enter party index:\n"))
    else:
        cvar.node_type = int(sys.argv[1])
    print("party index: {}".format(cvar.node_type))

    ips = stringArray(["127.0.0.1", "127.0.0.1", "127.0.0.1"])
    ports = intArray([2222, 2223, 2224])

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
