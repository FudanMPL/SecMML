// 
// Created by mingxin xu on 2021/09/26.
// 

#include <pybind11/pybind11.h>

namespace py = pybind11;

void py_Constant(py::module_ &);
void py_Player(py::module_ &);
void py_IOManager(py::module_ &);
void py_SocketManager(py::module_ &);
void py_Mat(py::module_ &);
void py_NeuronMat(py::module_ &);
void py_NN(py::module_ &);
void py_Cell(py::module_ &);
void py_BPGraph(py::module_ &);
void py_LSTMGraph(py::module_ &);

PYBIND11_MODULE(queqiao, m) {
    m.def("set_seed", []() { srand(time(NULL)); });
    py_Constant(m);
    py_Player(m);
    py_IOManager(m);
    py_SocketManager(m);
    py_Mat(m);
    py_NeuronMat(m);
    py_NN(m);
    py_Cell(m);
    py_BPGraph(m);
    py_LSTMGraph(m);
}
