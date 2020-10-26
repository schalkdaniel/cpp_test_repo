#include <pybind11/pybind11.h>

namespace py = pybind11;

class Base {
public:
	virtual ~Base() = default;

	virtual void test() = 0;
};


class Derived : public Base {
public:
	using Base::Base;

	void test() override {
		PYBIND11_OVERLOAD_PURE(void, Base, test)
	}
};

PYBIND11_MODULE(example, m) {
	py::class_<Base, Derived>(m, "Base")
			.def(py::init<>())
			.def("test", &Base::test)
            ;
}
