//Dieses Program wrapped eine abstrakte Klasse in C++, um in Python zu verwenden.
//Man kann mit den folgenden Befehlen den Code benutzten:
//c++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` pybind_class_test.cpp -o example`python3-config     --extension-suffix`
//dann offne python
// import example
// a = example.Derived()
// a.test()
// Das Ergebnis wird von der abgeleiteten Klasse
//c++ -O3 -Wall -shared -std=c++14 -fPIC -larmadillo `python3 -m pybind11 --includes` abstrakt_test.cpp -o example`python3-config --extension-suffix` 
//das ist neu ^^^ für den Fall mit Arma + Numpy




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
    void test() override {cout << "Test";}
};


PYBIND11_MODULE(example,m) {
    py::class_<Base>(m, "Base");

    py::class_<Derived, Base>(m, "Derived")
        .def(py::init<>())
        .def("test", &Derived::test)
	.def("printarma", &Derived::printarma,py::arg("arr"));
} //als constructor geben, auch double übergeben:
// construc: arma::mat x; ArmaMat (const arma::mat& x0) : x (x0) {} -- vom Beispiel Daniel
//
