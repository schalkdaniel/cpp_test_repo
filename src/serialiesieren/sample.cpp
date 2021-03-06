/**
 *  @file    template.cpp
 *  @author  Daniel
 *
 *  @brief Test file
 *
 *  @section DESCRIPTION
 *
 *
 *
 *  Compilation:
 *  `g++ sample.cpp -o sample`
 **/

#include <stdio.h>
#include <iostream>
#include <armadillo>

class Car {
  private:
    unsigned int cyl = 4;

  public:
    Car (unsigned int _cyl) : cyl ( _cyl ) { std::cout << "Create car with " << cyl << " cylinder." << std::endl; }
};

int main () {

  std::cout << "Test" << std::endl;

  Car test (8);

  arma::mat A(5, 5, arma::fill::zeros);

  A.print("A:");

  return 0;
}
