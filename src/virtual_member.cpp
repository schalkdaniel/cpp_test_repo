/**
 * @file   virtual_member.cpp
 * @author Daniel
 *
 * @brief Example class to see how virtual member work
 *
 * Class defines virtual member function in parent class. This function must be overwritten by
 * the child classes! Also note that the smart unique pointer are automatically deleted after they
 * run out of scope!
 *
 * Compilation:
 * `g++ virtual_member.cpp`
 **/

#include <iostream>
#include <memory>

class Operation
{
  public:

    Operation () {};

    // define `calc()` virtual, so that it can be overwritten:
    virtual double calc () = 0;

    ~Operation () { std::cout << "Operation is deleted!" << std::endl; };
};

class Addition : public Operation
{
  private:

    double a;
    double b;

  public:

    Addition (const double& _a, const double& _b) : a (_a), b (_b) {};

    // define `calc()` for the addition case:
    double calc () { return a + b;};
};

class Multiplication : public Operation
{
  private:

    double a;
    double b;

  public:

    Multiplication (const double& _a, const double& _b) : a (_a), b (_b) {};

    // define `calc()` for the multiplication case:
    double calc () { return a * b;};
};

// This function gest an abstract `Operation` and calculates `calc()` something:
double calculateSomething (const std::unique_ptr<Operation>& uni_ptr_op)
{
  // Here, it doesn't matter if the object is a Addition or Multiplication. The API is unified and
  // we can call `calc()` from any object:
  return uni_ptr_op->calc();
};

int main () {

  double a = 10;
  double b = 5;

  std::unique_ptr<Operation> add = std::make_unique<Addition>(a, b);
  std::unique_ptr<Operation> mult = std::make_unique<Multiplication>(a, b);

  std::cout << a << " + " << b << " = " << calculateSomething(add) << std::endl;
  std::cout << a << " * " << b << " = " << calculateSomething(mult) << std::endl;

  return 0;
};
