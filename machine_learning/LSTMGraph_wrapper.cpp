// 
// Created by mingxin xu on 2021/09/29.
// 

#include <pybind11/pybind11.h>
#include "LSTMGraph.h"

namespace py = pybind11;

void py_LSTMGraph(py::module_ &m) {
    py::class_<LSTMGraph::LSTM>(m, "LSTMGraph_LSTM")
        .def_readwrite("nn", &LSTMGraph::LSTM::nn)
        .def_readwrite("train_data", &LSTMGraph::LSTM::train_data)
        .def_readwrite("train_label", &LSTMGraph::LSTM::train_label)
        .def_readwrite("test_data", &LSTMGraph::LSTM::test_data)
        .def_readwrite("test_label", &LSTMGraph::LSTM::test_label)
        .def_readwrite("input", &LSTMGraph::LSTM::input)
        .def_readwrite("output", &LSTMGraph::LSTM::output)
        .def_readwrite("h0", &LSTMGraph::LSTM::h0)
        .def_readwrite("c0", &LSTMGraph::LSTM::c0)
        .def_readwrite("argmax", &LSTMGraph::LSTM::argmax)
        .def_readwrite("st_con", &LSTMGraph::LSTM::st_con)
        .def_readwrite("st_w", &LSTMGraph::LSTM::st_w)
        .def_readwrite("st_b", &LSTMGraph::LSTM::st_b)
        .def_readwrite("st_mul", &LSTMGraph::LSTM::st_mul)
        .def_readwrite("sd", &LSTMGraph::LSTM::sd)
        .def_readwrite("re_st_add", &LSTMGraph::LSTM::re_st_add)
        .def_readwrite("re_output", &LSTMGraph::LSTM::re_output)
        .def_readwrite("out_sig", &LSTMGraph::LSTM::out_sig)
        .def_readwrite("re_out_sig", &LSTMGraph::LSTM::re_out_sig)
        .def_readwrite("id", &LSTMGraph::LSTM::id)
        .def_readwrite("activation_out", &LSTMGraph::LSTM::activation_out)
        .def(py::init<Mat*, Mat*, Mat*, Mat*>(), py::arg("train_data"), py::arg("train_label"), py::arg("test_data"), py::arg("test_label"))
        .def("graph", py::overload_cast<>(&LSTMGraph::LSTM::graph))
        .def("train", &LSTMGraph::LSTM::train)
        .def("test", &LSTMGraph::LSTM::test)

        .def("resize_x", [](LSTMGraph::LSTM &self, std::size_t size) {
            self.x.resize(size);
        }, py::arg("size"))

        .def("get_x", [](LSTMGraph::LSTM &self, int idx) {
            return self.x[idx];
        }, py::arg("idx"))

        .def("set_x", [](LSTMGraph::LSTM &self, int idx, int val) {
            self.x[idx] = val;
        }, py::arg("idx"), py::arg("val"))

        .def("resize_cells", [](LSTMGraph::LSTM &self, std::size_t size) {
            self.cells.resize(size);
        }, py::arg("size"))

        .def("get_cell", [](LSTMGraph::LSTM &self, int idx) {
            return self.cells[idx];
        }, py::arg("idx"), py::return_value_policy::reference)

        // prevent garbage collection in python
        .def("set_cell", [](LSTMGraph::LSTM &self, int idx, Cell* cell) {
            self.cells[idx] = new Cell(*cell);
        }, py::arg("idx"), py::arg("cell"));
}
