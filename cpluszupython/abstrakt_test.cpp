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
