// 
// Created by mingxin xu on 2021/09/29.
// 

#include <pybind11/pybind11.h>
#include "Cell.h"

namespace py = pybind11;

void py_Cell(py::module_ &m) {
    py::class_<Cell>(m, "Cell")
        .def(py::init<NN*, int, int, int>(), py::arg("nn"), py::arg("c_in"), py::arg("h_in"), py::arg("x_in"))
        .def("getC_out", &Cell::getC_out)
        .def("getH_out", &Cell::getH_out)
        .def("addEdges", &Cell::addEdges)

        // prevent garbage collection in python
        .def("setWeight", [](Cell &self, Mat *w_f, Mat *w_i, Mat *w_c, Mat *w_o) {
            self.setWeight(new Mat (*w_f), new Mat (*w_i), new Mat (*w_c), new Mat (*w_o));
        }, py::arg("w_f"), py::arg("w_i"), py::arg("w_c"), py::arg("w_o"));
}
