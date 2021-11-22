// 
// Created by mingxin xu on 2021/09/27.
// 

#include <pybind11/pybind11.h>
#include "NeuronMat.h"

namespace py = pybind11;

void py_NeuronMat(py::module_ &m) {
    py::class_<NeuronMat>(m, "NeuronMat")
        .def_property_readonly_static("NODE_INPUT", [](py::object) { return NeuronMat::NODE_INPUT; })
        .def_property_readonly_static("NODE_NET", [](py::object) { return NeuronMat::NODE_NET; })
        .def_property_readonly_static("NODE_OP", [](py::object) { return NeuronMat::NODE_OP; })
        .def_property_readonly_static("NODE_VIA", [](py::object) { return NeuronMat::NODE_VIA; })
        .def_property_readonly_static("NODE_FORWARD", [](py::object) { return NeuronMat::NODE_FORWARD; })
        .def("getForward", &NeuronMat::getForward, py::return_value_policy::reference)
        
        // prevent garbage collection in python
        .def("setForward", [](NeuronMat &self, Mat* a) {
            self.setForward(new Mat(*a));
        });
}
