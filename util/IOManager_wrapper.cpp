// 
// Created by mingxin xu on 2021/09/26.
// 

#include <pybind11/pybind11.h>
#include "IOManager.h"

namespace py = pybind11;

void py_IOManager(py::module_ &m) {
    py::class_<IOManager>(m, "IOManager")
        .def_readwrite_static("train_data", &IOManager::train_data)
        .def_readwrite_static("train_label", &IOManager::train_label)
        .def_readwrite_static("test_data", &IOManager::test_data)
        .def_readwrite_static("test_label", &IOManager::test_label)
        .def(py::init<>())
        .def_static("init", &IOManager::init)
        .def_static("init_local_data", &IOManager::init_local_data);
}
