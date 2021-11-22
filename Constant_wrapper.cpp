// 
// Created by mingxin xu on 2021/09/26.
// 

#include <pybind11/pybind11.h>
#include "Constant.h"

namespace py = pybind11;

void py_Constant(py::module_ &m) {
    py::class_<Constant>(m, "Constant")
        .def_property_readonly_static("B", [](py::object) { return B; })
        .def_property_readonly_static("D", [](py::object) { return D; })
        .def_property_readonly_static("L", [](py::object) { return L; })
        .def_property_readonly_static("D2", [](py::object) { return D2; })
        .def_property_readonly_static("CH", [](py::object) { return CH; })
        .def_property_readonly_static("IE", [](py::object) { return IE; })
        .def_readwrite_static("node_type", &node_type);
}
