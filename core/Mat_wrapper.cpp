// 
// Created by mingxin xu on 2021/09/26.
// 

#include <pybind11/pybind11.h>
#include "Mat.h"

namespace py = pybind11;

void py_Mat(py::module_ &m) {
    py::class_<Mat>(m, "Mat")
        .def(py::init<int, int, ll128>(), py::arg("r"), py::arg("c"), py::arg("b"))
        .def_static("random_neg", &Mat::random_neg);
}
