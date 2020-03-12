#include <iostream>
#include <fstream>
#include <boost/archive/xml_oarchive.hpp> //includes
#include <boost/archive/xml_iarchive.hpp>
#include <sstream>

class Gear {
  public:
    template<typename Archive>
    void serialize(Archive& ar, unsigned int version) {
    ar & BOOST_SERIALIZATION_NVP(v);
  }
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
      ar & BOOST_SERIALIZATION_NVP(hp); 
      ar & BOOST_SERIALIZATION_NVP(x);
    }
    void setHP (const int& _hp) { hp = _hp; }
    void setGear (Gear* _Gear) { x = _Gear; }
    void status () { std::cout << "hp = " << hp << " Gear with v = " << x->getV() << std::endl; }

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
  c.status();
 
  
  std::ofstream outputStream;
  outputStream.open("Car.xml");
  boost::archive::xml_oarchive outputArchive(outputStream);
  outputArchive << BOOST_SERIALIZATION_NVP(c); 
  //outputStream.close() was making boost throw an exception
  

  Car b;
  std::ifstream inputStream;
  inputStream.open("Car.xml", std::ifstream::in);
  boost::archive::xml_iarchive inputArchive(inputStream);
  inputArchive >> BOOST_SERIALIZATION_NVP(b); 
  b.status();
  return 0;
}