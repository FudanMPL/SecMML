// 
// Created by mingxin xu on 2021/09/27.
// 

#include <pybind11/pybind11.h>
#include "NN.h"

namespace py = pybind11;

void py_NN(py::module_ &m) {
    py::class_<NN>(m, "NN")
        .def(py::init<>())
        .def("global_variables_initializer", &NN::global_variables_initializer)
        .def("epoch_init", &NN::epoch_init)
        .def("reveal_init", py::overload_cast<int>(&NN::reveal_init), py::arg("u"))
        .def("reveal_init", py::overload_cast<>(&NN::reveal_init))
        .def("addedge", &NN::addedge, py::arg("u"), py::arg("v"))
        .def("addnode", &NN::addnode, py::arg("r"), py::arg("c"), py::arg("k"))
        .def("getNeuron", &NN::getNeuron, py::arg("u"), py::return_value_policy::reference)
        .def("setOp", &NN::setOp, py::arg("u"), py::arg("op"))
        .def("addOpAdd_Mat", &NN::addOpAdd_Mat, py::arg("res"), py::arg("a"), py::arg("b"))
        .def("addOpMinus_Mat", &NN::addOpMinus_Mat, py::arg("res"), py::arg("a"), py::arg("b"))
        .def("addOpMul_Mat", &NN::addOpMul_Mat, py::arg("res"), py::arg("a"), py::arg("b"))
        .def("addOpMul_Const_Mat", &NN::addOpMul_Const_Mat, py::arg("res"), py::arg("a"), py::arg("b"))
        .def("addOpDiv_Const_Mat", &NN::addOpDiv_Const_Mat, py::arg("res"), py::arg("a"), py::arg("b"))
        .def("addOpDiv_Const_Optimized_Mat", &NN::addOpDiv_Const_Optimized_Mat, py::arg("res"), py::arg("a"), py::arg("b"))
        .def("addOpDiv_Seg_Const_Mat", &NN::addOpDiv_Seg_Const_Mat, py::arg("res"), py::arg("a"), py::arg("b"))
        .def("addOpHada_Mat", &NN::addOpHada_Mat, py::arg("res"), py::arg("a"), py::arg("b"))
        .def("addOpMeanSquaredLoss", &NN::addOpMeanSquaredLoss, py::arg("res"), py::arg("a"), py::arg("b"))
        .def("addOpSimilar", &NN::addOpSimilar, py::arg("res"), py::arg("a"), py::arg("b"))
        .def("addOpConcat", &NN::addOpConcat, py::arg("res"), py::arg("a"), py::arg("b"))
        .def("addOpHstack", &NN::addOpHstack, py::arg("res"), py::arg("a"), py::arg("b"))
        .def("addOpVstack", &NN::addOpVstack, py::arg("res"), py::arg("a"), py::arg("b"))
        .def("addOpVia", &NN::addOpVia, py::arg("res"), py::arg("a"))
        .def("addOpTanh", &NN::addOpTanh, py::arg("res"), py::arg("a"))
        .def("addOpHard_Tanh", &NN::addOpHard_Tanh, py::arg("res"), py::arg("a"))
        .def("addOpHybrid_Tanh", &NN::addOpHybrid_Tanh, py::arg("res"), py::arg("a"))
        .def("addOpRaw_Tanh", &NN::addOpRaw_Tanh, py::arg("res"), py::arg("a"))
        .def("addOpTanh_change", &NN::addOpTanh_change, py::arg("res"), py::arg("a"))
        .def("addOpTanh_ex", &NN::addOpTanh_ex, py::arg("res"), py::arg("a"))
        .def("addOpSigmoid", &NN::addOpSigmoid, py::arg("res"), py::arg("a"))
        .def("addOpReLU", &NN::addOpReLU, py::arg("res"), py::arg("a"))
        .def("addOpArgmax", &NN::addOpArgmax, py::arg("res"), py::arg("a"))
        .def("addOpLTZ", &NN::addOpLTZ, py::arg("res"), py::arg("a"))
        .def("addOpEQZ", &NN::addOpEQZ, py::arg("res"), py::arg("a"))
        .def("addOpEQZ_2LTZ", &NN::addOpEQZ_2LTZ, py::arg("res"), py::arg("a"))
        .def("addOpEqual", &NN::addOpEqual, py::arg("res"), py::arg("a"), py::arg("b"))
        .def("toposort", &NN::toposort)
        .def("gradUpdate", &NN::gradUpdate)
        .def("forwardHasNext", &NN::forwardHasNext)
        .def("forwardNext", &NN::forwardNext)
        .def("backNext", &NN::backNext)
        .def("backHasNext", &NN::backHasNext)
        .def("updateHasNext", &NN::updateHasNext)
        .def("update", &NN::update)
        .def("revealHasNext", &NN::revealHasNext)
        .def("reveal", &NN::reveal)
        .def("getTot", &NN::getTot);
}
