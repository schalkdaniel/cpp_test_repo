#include <iostream>
#include <fstream>
#include <boost/archive/tmpdir.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <armadillo>


int main() {

  arma::mat A = arma::randu<arma::mat>(4,5);
  arma::mat B = arma::randu<arma::mat>(4,5);
   
  std::cout << A<< std::endl;

  A.save("arma_save",arma::raw_binary);

  B.load("arma_save",arma::raw_binary);

  std::cout <<B<<std::endl;


  return 0;
}


