/**
 * @file   carma_abstract.cpp
 * @author Shawn
 *
 * @brief Takes Python's Numpy arrays and binds them to C++'s Arma matrices
 *
 * This is a simpler version of the other file "carma_ctor.cpp". This one 
 * focuses on binding an abstract class.
 * This task is done by using the following packages:
 * carma: https://carma.readthedocs.io/en/latest/basic_usage.html
 * carma is the package that translates a numpy array to an arma matrix
 * pybind11: https://pybind11.readthedocs.io/en/stable/basics.html
 * pybind11: pybind11 is a lightweight header-only library that exposes C++ types in Python and vice versa
 *
 * Compilation:
 * c++ -O3 -Wall -shared -std=c++14 -fPIC -larmadillo `python3 -m pybind11 --includes` carma_abstract.cpp -o example`python3-config --extension-suffix` 
 *
 * After compilation:
 * run python:
 * >>> import example
 * >>> import numpy as np
 * >>> a = example.Derived()
 * >>> b = np.array([1, 2, 3])
 * >>> a.printarma(b)
 *   1.0000
 *   2.0000
 *   3.0000
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
    virtual void test() = 0;
    virtual void printarma(py::array_t<double> & arr) = 0;
};
class Derived: public Base
{
	
public:
    void printarma(py::array_t<double> & arr) override {arma::Mat<double> mat = carma::arr_to_mat<double>(arr);
	    std::cout << mat<< std::endl;}
    void test() override {cout << "Test\n";}
};


PYBIND11_MODULE(example,m) {
    py::class_<Base>(m, "Base");

    py::class_<Derived, Base>(m, "Derived")
        .def(py::init<>())
        .def("test", &Derived::test)
	    .def("printarma", &Derived::printarma,py::arg("arr"));
}