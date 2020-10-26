/**
 * @file   carma_abstract.cpp
 * @author Shawn
 *
 * @brief Takes Python's Numpy arrays and binds them to C++'s Arma matrices
 *
 * This task is done by using the following packages:
 * carma: https://carma.readthedocs.io/en/latest/basic_usage.html
 * carma is the package that translates a numpy array to an arma matrix
 * pybind11: https://pybind11.readthedocs.io/en/stable/basics.html
 * pybind11: pybind11 is a lightweight header-only library that exposes C++ types in Python and vice versa
 *
 * Compilation:
 * `c++ -O3 -Wall -shared -std=c++14 -fPIC -larmadillo `python3 -m pybind11 --includes` carma_abstract.cpp -o example`python3-config --extension-suffix` `
 *
 * After compilation:
 * run python:
 * >>> import example
 * >>> import numpy as np
 * >>> a = np.array([1, 2, 3])
 * >>> b = example.Derived(a)
 *  1.0000
 *  2.0000
 *  3.0000
**/


#include <ostream>
#include <iostream>
#include <armadillo>
#include <carma/carma.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;
using namespace std;

class Base
{
public:
   arma::mat x;
   explicit Base(py::array_t<double> & arr) {};
};
class Derived: public Base
{
	
public:
    explicit Derived(py::array_t<double> & arr) : Base(arr) {arma::Mat<double> mat = carma::arr_to_mat<double>(arr);
	std::cout << mat<< std::endl;}
};


PYBIND11_MODULE(example,m) {
  py::class_<Base>(m, "Base")
      .def(py::init<py::array_t<double>&>());
  py::class_<Derived, Base>(m, "Derived")
      .def(py::init<py::array_t<double>&>());
}