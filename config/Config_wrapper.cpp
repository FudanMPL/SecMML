// 
// Created by mingxin xu on 2022/01/17.
// 

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Config.hpp"

namespace py = pybind11;

void py_Config(py::module_ &m) {
    py::class_<Config>(m, "Config")
        .def_readwrite_static("config", &Config::config)
        .def_static("init", &Config::init, py::arg("file_name"), py::return_value_policy::reference)
        .def_readonly("B", &Config::B)
        .def_readonly("D", &Config::D)
        .def_readonly("L", &Config::L)
        .def_readonly("D2", &Config::D2)
        .def_readonly("CH", &Config::CH)
        .def_readonly("IE", &Config::IE)
        .def_readonly("LOCAL_TEST", &Config::LOCAL_TEST)
        .def_readonly("GRAPH_TYPE", &Config::GRAPH_TYPE)
        .def_readonly("ACTIVATION", &Config::ACTIVATION)
        .def_readonly("SIGMOID", &Config::SIGMOID)
        .def_readonly("TANH", &Config::TANH)
        .def_readonly("LINEAR", &Config::LINEAR)
        .def_readonly("LOGISTIC", &Config::LOGISTIC)
        // enable conversion between c++ std::vector and python list by including <pybind11/stl.h>
        .def_readonly("IP", &Config::IP)
        .def_readonly("PORT", &Config::PORT);
}
