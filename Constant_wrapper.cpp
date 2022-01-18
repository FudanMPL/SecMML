// 
// Created by mingxin xu on 2021/09/26.
// 

#include <pybind11/pybind11.h>
#include "Constant.h"

namespace py = pybind11;

void py_Constant(py::module_ &m) {
    py::class_<Constant>(m, "Constant")
        .def_readwrite_static("node_type", &node_type);
}
