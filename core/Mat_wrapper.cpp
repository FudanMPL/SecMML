// 
// Created by mingxin xu on 2021/09/26.
// 

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Mat.h"

namespace py = pybind11;

void py_Mat(py::module_ &m) {
    py::class_<Mat>(m, "Mat")
        .def(py::init<int, int, ll128>(), py::arg("r"), py::arg("c"), py::arg("b"))
        .def_static("random_neg", &Mat::random_neg)
        .def("cols", &Mat::cols)

        // enable conversion between c++ std::vector and python list by including <pybind11/stl.h>
        .def("__getitem__", [](Mat &self, vector<int> pos) { return self(pos.at(0), pos.at(1)); })
        .def("__setitem__", [](Mat &self, vector<int> pos, ll128 val) { self(pos.at(0), pos.at(1)) = val; })

        // operator overloading
        .def("__add__", [](Mat &self, const Mat &a) { return self + a; }, py::return_value_policy::reference)
        .def("__add__", [](Mat &self, const ll128 &b) { return self + b; }, py::return_value_policy::reference)
        .def("__sub__", [](Mat &self, const Mat &a) { return self - a; }, py::return_value_policy::reference)
        .def("__sub__", [](Mat &self, ll128 b) { return self - b; }, py::return_value_policy::reference)
        .def("__mul__", [](Mat &self, const Mat &a) { return self * a; }, py::return_value_policy::reference)
        .def("__mul__", [](Mat &self, const ll128 &b) { return self * b; }, py::return_value_policy::reference)
        .def("__truediv__", [](Mat &self, Mat a) { return self / a; }, py::return_value_policy::reference)
        .def("__truediv__", [](Mat &self, const ll128 &b) { return self / b; }, py::return_value_policy::reference);
}
