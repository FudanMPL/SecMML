// 
// Created by mingxin xu on 2021/09/26.
// 

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "SocketManager.h"

namespace py = pybind11;

void py_SocketManager(py::module_ &m) {
    py::class_<SocketManager::SMMLF>(m, "SocketManager_SMMLF")
        .def(py::init<>())
        
        // enable conversion between c++ std::vector and python list by including <pybind11/stl.h>
        .def("init", py::overload_cast<const vector<string>&, const vector<int>&>(&SocketManager::SMMLF::init), py::arg("ip"), py::arg("port"))
        .def("init", py::overload_cast<>(&SocketManager::SMMLF::init));
}
