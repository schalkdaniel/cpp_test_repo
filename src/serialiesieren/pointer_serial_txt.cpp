#include <iostream>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>
#include <boost/archive/tmpdir.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <armadillo>


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

  //Serialization ohne was zu speichern

    // std::stringstream ss;
    // boost::archive::text_oarchive oa{ss};
    //  oa << c;
  
    // Car b;
    // boost::archive::text_iarchive ia{ss};
    // ia >> b;
    // b.status();
    // return 0;


    std::ofstream outputStream;
    outputStream.open("Car.txt");
    boost::archive::text_oarchive outputArchive(outputStream);
    outputArchive << c;
    outputStream.close();

    Car b;
    std::ifstream inputStream;
    inputStream.open("Car.txt", std::ifstream::in);
    boost::archive::text_iarchive inputArchive(inputStream);
    inputArchive >> b;
    b.status();
    inputStream.close();
    return 0;

}


