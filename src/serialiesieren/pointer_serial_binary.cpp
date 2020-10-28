/**
 *  @file    pointer_serial_binary.cpp
 *  @author  Shawn
 *
 *  @brief Pointer, saving data with serialization, first draft
 *
 *  @section DESCRIPTION
 * 
 * Compilation:
 * g++ -std=c++11 pointer_serial_binary.cpp -lboost_serialization -larmadillo
 * 
 * After running ./a.out:
 * v = 2.5
 * hp = 80 Gear with v = 2.5
 * 
**/


#include <iostream>
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <sstream>
#include <boost/serialization/split_member.hpp>


class Gear {
  public:
    template<typename Archive>
    void serialize(Archive& ar, unsigned int version) { ar & v; }

    void setV (const double& _v) { v = _v; }
    double getV () { return v; }

    void status () { std::cout << "v = " << v << std::endl; }

  private:
    double v;
};

class Car {
  public:
    template<typename Archive>
    void serialize(Archive& ar, unsigned int version) {
      ar & hp;
      ar & x;
    }

    void setHP (const int& _hp) { hp = _hp; }
    void setGear (Gear* _Gear) { x = _Gear; }
    void status () { std::cout << "hp = " << hp << " Gear with v = " << x->getV() <<  std::endl; }

  private:
    int hp;
    Gear *x;
};

int main() {
  // Define new Gear:
  Gear* g = new Gear();
  g->setV(2.5);
  g->status();

  // Expectation is Car sets up the Gear.
  Car c;
  c.setHP(80);
  c.setGear(g);
  //c.status();
  



  std::ofstream outputStream;
  outputStream.open("bin.dat");
  std::ostringstream oss;
  boost::archive::binary_oarchive oa(outputStream);
  oa & c;
  outputStream.close();

  Car b;
  std::ifstream inputStream;
  inputStream.open("bin.dat", std::ifstream::in);
  boost::archive::binary_iarchive ia(inputStream);
  ia & b;
  b.status();
  return 0;


}

