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
        
        // convert python list to c++ std::vector by including <pybind11/stl.h>
        // then convert std::vector to array
        .def("init", [](SocketManager::SMMLF &self, vector<string> ip, vector<int> port) {
            self.init(&ip.at(0), &port.at(0));
        }, py::arg("ip"), py::arg("port"));
}
