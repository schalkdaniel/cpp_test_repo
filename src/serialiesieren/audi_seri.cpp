#include <armadillo>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/void_cast_fwd.hpp>
#include <boost/serialization/binary_object.hpp>

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <vector>


class Car {
public:
    //Car() = default;
    //virtual double getInfo() = 0;
    virtual char const* type() const = 0;
    virtual void printMember () const = 0;

    virtual ~Car() = default;

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {};
};

class Audi : public Car {
public:

  Audi() {};
  Audi(const std::string owner, const int hp, const std::string second_owner,
    const std::string country)
    : _owner ( owner ),
      _hp    ( hp ),
      _second_owner ( second_owner ),
      _country      ( country )
  { }

  char const* type() const override { return "Audi"; }

  void printMember () const override
  {
    std::cout
      << this->type() << ":\n"
      << "owner: " << _owner
      << " hp: " << _hp
      << " second owner: " << _second_owner
      << " country: " << _country
      << std::endl;
  }

private:
  std::string  _owner;
  int          _hp;
  std::string  _second_owner;
  std::string  _country;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
      ar & boost::serialization::base_object<Car>(*this);
      ar & _owner;
      ar & _hp;
      ar & _second_owner;
      ar & _country;
  }
};

class BMW : public Car
{
public:
  BMW () {};
  BMW (const std::string owner, const int hp, const std::string country, const unsigned int cyl)
    : _owner   ( owner ),
      _hp      ( hp ),
      _country ( country ),
      _cyl     ( cyl )
  { }

  char const* type() const override { return "BMW"; }

  void printMember () const override
  {
    std::cout
      << this->type() << ":\n"
      << "owner: " << _owner
      << " hp: " << _hp
      << " country: " << _country
      << " cyl: " << _cyl
      << std::endl;
  }

private:
  std::string  _owner;
  int          _hp{};
  std::string  _country;
  unsigned int _cyl;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
      ar & boost::serialization::base_object<Car>(*this);
      ar & _owner;
      ar & _hp;
      ar & _country;
      ar & _cyl;
  }
};



BOOST_CLASS_EXPORT(Audi);
BOOST_CLASS_EXPORT(BMW);
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Car); //Tell Boost that Car is abstract

int main() {
  std::string save_file = "test.dat";
  std::ofstream of(save_file, std::ofstream::binary);
  std::ifstream in_f(save_file, std::ifstream::binary);
  {
    std::shared_ptr<Car> audi = std::make_shared<Audi>("Wilma", 3, "Rene", "Argentina");
    audi->printMember();

    std::shared_ptr<Car> bmw = std::make_shared<BMW>("Horst", 200, "Germany", 8);
    bmw->printMember();

    std::stringstream strs;
    boost::archive::binary_oarchive ar(of);
    ar& audi;

    // audi_save_dir = strs.str();
    std::cout << "Count of audi:" << audi.use_count() << std::endl;

    ar << boost::serialization::make_binary_object(&audi, sizeof(audi));
  }


   {
     std::shared_ptr<Car> audi; //= std::make_unique<Audi>();
     std::cout << "Count of audi:" << audi.use_count() << std::endl;

     //std::stringstream strs(f);
     boost::archive::binary_iarchive ar(in_f);
     ar& audi;
     Audi& d = dynamic_cast<Audi &>(*audi);

     std::cout << "Count of audi:" << audi.use_count() << std::endl;
     d.printMember();
   }

  return 0;
}

