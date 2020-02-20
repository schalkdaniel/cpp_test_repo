/**
 *  @file    pointer_serial.cpp
 *  @author  Shawn
 *
 *  @brief Pointer, saving data, first draft
 *
 *  @section DESCRIPTION
 *  First draft of serialization task. 
 *  This section will be updated once the code works as expected.  
 *  
 *
 *  
 * 
 **/


#include <iostream>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


class Car {
public:
template<typenme Archive>
void serialize(Archive& ar, unsigned int version) {
  ar & hp;
  ar & x;
}

private:
  int hp;
  Gear *x; 
};

class Gear {
public:
  void serialize(Archive& ar, unsigned int version) {
    ar & v;
  }
private:
  double v;
};

int main() {
  Car c;
  // Expectation is Car sets up the Gear.
  Gear* g = new Gear();
  c.x = g;
  std::ofstream ofs("myfile.txt");
  boost::archive::text_oarchive oa(ofs);
  oa << c;
  ofs.close()
  std::ifstream ifs("myfile.txt");
  boost::archive::text_oarchive ia(ifs);
  Car c2;
  ia >> c2;
}
