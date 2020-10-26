//Dieses Program wrapped eine Klasse in C++, um in Python zu verwenden.
//Man kann mit den folgenden Befehlen den Code benutzten:
//c++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` pybind_class_test.cpp -o example`python3-config --extension-suffix`\n
//dann offne python
// import example
// a = example.Test(4)
// a.printname()
//wird dann "4" rausgeben

#include <pybind11/pybind11.h>
#include <ostream>
#include <iostream>
namespace py = pybind11;

//
//
//
//
class Test {

    public:
    int i;
    void printname()

    {
    std::cout << i <<std::endl;

    }
};

PYBIND11_MODULE(example,m) {
    py::class_<Test>(m, "Test")
        //def(py::init<int &>());
        .def(py::init<const int &>())
        .def("printname", &Test::printname);
//        .def("i", &Test::i);

}
