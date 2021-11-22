// 
// Created by mingxin xu on 2021/09/27.
// 

#include <pybind11/pybind11.h>
#include "BPGraph.h"

namespace py = pybind11;

void py_BPGraph(py::module_ &m) {
    py::class_<BPGraph::LR>(m, "BPGraph_LR")
        .def_readwrite("nn", &BPGraph::LR::nn)
        .def_readwrite("train_data", &BPGraph::LR::train_data)
        .def_readwrite("train_label", &BPGraph::LR::train_label)
        .def_readwrite("test_data", &BPGraph::LR::test_data)
        .def_readwrite("test_label", &BPGraph::LR::test_label)
        .def_readwrite("input", &BPGraph::LR::input)
        .def_readwrite("output", &BPGraph::LR::output)
        .def_readwrite("argmax", &BPGraph::LR::argmax)
        .def_readwrite("st_con", &BPGraph::LR::st_con)
        .def_readwrite("st_w", &BPGraph::LR::st_w)
        .def_readwrite("st_b", &BPGraph::LR::st_b)
        .def_readwrite("st_mul", &BPGraph::LR::st_mul)
        .def_readwrite("nd_w", &BPGraph::LR::nd_w)
        .def_readwrite("nd_b", &BPGraph::LR::nd_b)
        .def_readwrite("nd_add", &BPGraph::LR::nd_add)
        .def_readwrite("nd_mul", &BPGraph::LR::nd_mul)
        .def_readwrite("nd_hidden", &BPGraph::LR::nd_hidden)
        .def_readwrite("sd", &BPGraph::LR::sd)
        .def_readwrite("re_st_add", &BPGraph::LR::re_st_add)
        .def_readwrite("re_output", &BPGraph::LR::re_output)
        .def_readwrite("out_sig", &BPGraph::LR::out_sig)
        .def_readwrite("re_out_sig", &BPGraph::LR::re_out_sig)
        .def_readwrite("id", &BPGraph::LR::id)
        .def_readwrite("activation_out", &BPGraph::LR::activation_out)
        .def(py::init<Mat*, Mat*, Mat*, Mat*>(), py::arg("train_data"), py::arg("train_label"), py::arg("test_data"), py::arg("test_label"))
        .def("linear_graph", &BPGraph::LR::linear_graph)
        .def("logistic_graph", &BPGraph::LR::logistic_graph)
        .def("graph", py::overload_cast<>(&BPGraph::LR::graph))
        .def("train", &BPGraph::LR::train)
        .def("test", &BPGraph::LR::test);
}
