// 
// Created by mingxin xu on 2021/09/26.
// 

#include <pybind11/pybind11.h>
#include "Player.h"

namespace py = pybind11;

void py_Player(py::module_ &m) {
    py::class_<Player>(m, "Player")
        .def_static("init", &Player::init);
}
