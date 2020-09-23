//Dieses Program wrapped eine abstrakte Klasse in C++, um in Python zu verwenden.
//Man kann mit den folgenden Befehlen den Code benutzten:
//c++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` pybind_class_test.cpp -o example`python3-config     --extension-suffix`
//dann offne python
// import example
// a = example.Derived()
// a.test()
// Das Ergebnis wird von der abgeleiteten Klasse





#include <pybind11/pybind11.h>
#include <ostream>
#include <iostream>
namespace py = pybind11;
using namespace std;

class Base
{
public:
    virtual void test() = 0;

};
class Derived: public Base
{
public:
    
    void test() {cout << "Test";}
};


PYBIND11_MODULE(example,m) {
    py::class_<Base>(m, "Base");

    py::class_<Derived, Base>(m, "Derived")
        .def(py::init<>())
        .def("test", &Derived::test);
}
