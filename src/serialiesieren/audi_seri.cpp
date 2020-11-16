/**
 * @file   audi_seri.cpp
 * @author Shawn
 *
 * @brief Serializes virtual functions
 *
 * @section DESCRIPTION
 *
 * Compilation:
 * g++ -std=c++11 audi_seri.cpp -lboost_serialization -larmadillo
 *
 * After running ./a.out:
 * Audi:
 * owner: Wilma hp: 3 second owner: Rene country: Argentina
 * BMW:
 * owner: Horst hp: 200 country: Germany cyl: 8
 * Count of audi:1
 * Count of audi:0
 * Count of audi:2
 * Audi:
 * owner: Wilma hp: 3 second owner: Rene country: Argentina
 *
**/


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

class Engine
{
  public:
    Engine () {}
    Engine (const int cyl) : _cyl ( cyl ) {}
    int getCyl () const { return _cyl; }

  private:
    int _cyl;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar & _cyl;
    }
};

class Car {
public:
    //Car() = default;
    //virtual double getInfo() = 0;
    virtual char const* type() const = 0;
    virtual void printMember () const = 0;
    virtual void save (const std::string file) const { std::cout << "file" << std::endl; };

    virtual ~Car() = default;

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {};
};

class Audi : public Car, public std::enable_shared_from_this<Car> {
public:

  Audi() {};
  Audi(const std::string owner, const int hp, const std::string second_owner,
    const std::string country, const unsigned int cyl)
    : _owner ( owner ),
      _hp    ( hp ),
      _second_owner ( second_owner ),
      _country      ( country ),
      _eng          ( std::make_shared<Engine>(cyl) )
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
      << " engine: " << _eng->getCyl()
      << std::endl;
  }
  void save (const std::string file) const {
    std::ofstream of(file, std::ofstream::binary);
    std::stringstream strs;
    boost::archive::binary_oarchive ar(of);
    std::shared_ptr<const Car> audi = shared_from_this();
    boost::serialization::make_binary_object(&audi, sizeof(audi));
    ar& audi;
  }

private:
  std::string  _owner;
  int          _hp;
  std::string  _second_owner;
  std::string  _country;
  std::shared_ptr<Engine> _eng;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
      ar & boost::serialization::base_object<Car>(*this);
      ar & _owner;
      ar & _hp;
      ar & _second_owner;
      ar & _country;
      ar & _eng;
  }
};


class BMW : public Car, public std::enable_shared_from_this<Car> 
{
public:
  BMW () {};
  BMW (const std::string owner, const int hp, const std::string country, const unsigned int cyl)
    : _owner   ( owner ),
      _hp      ( hp ),
      _country ( country ),
      _eng     ( std::make_shared<Engine>(cyl) )
  { }

  char const* type() const override { return "BMW"; }

  void printMember () const override
  {
    std::cout
      << this->type() << ":\n"
      << "owner: " << _owner
      << " hp: " << _hp
      << " country: " << _country
      << " engine: " << _eng->getCyl()
      << std::endl;
  }
  void save (const std::string file) const {
    std::ofstream of(file, std::ofstream::binary);
    std::stringstream strs;
    boost::archive::binary_oarchive ar(of);
    std::shared_ptr<const Car> audi = shared_from_this();
    boost::serialization::make_binary_object(&audi, sizeof(audi));
    ar& audi;

  }

private:
  std::string  _owner;
  int          _hp{};
  std::string  _country;
  std::shared_ptr<Engine> _eng;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
      ar & boost::serialization::base_object<Car>(*this);
      ar & _owner;
      ar & _hp;
      ar & _country;
      ar & _eng;
  }
};


Audi loadAudi (const std::string file_name) {
std::cout << "Loading " << file_name << std::endl;
std::ifstream in_f(file_name, std::ifstream::binary);

std::cout << "1" << std::endl;
std::shared_ptr<Car> audi; //= std::make_unique<Audi>();

std::cout << "2" << std::endl;
boost::archive::binary_iarchive ar(in_f);

std::cout << "3" << std::endl;
ar& audi;

std::cout << "4" << std::endl;
return dynamic_cast<Audi &>(*audi);
};

BOOST_CLASS_EXPORT(Audi);
BOOST_CLASS_EXPORT(BMW);
BOOST_CLASS_EXPORT(Engine);
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Car); //Tell Boost that Car is abstract

int main() {
  std::string save_file = "test.dat";
  std::ofstream of(save_file, std::ofstream::binary);
  std::ifstream in_f(save_file, std::ifstream::binary);
  {
    std::shared_ptr<Car> audi = std::make_shared<Audi>("Wilma", 3, "Rene", "Argentina", 16);
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
     std::cout << "Deserialize: Count of audi:" << audi.use_count() << std::endl;

     //std::stringstream strs(f);
     boost::archive::binary_iarchive ar(in_f);
     ar& audi;
     Audi& d = dynamic_cast<Audi &>(*audi);

     std::cout << "Deserialize: Count of audi:" << audi.use_count() << std::endl;
     std::cout << "Print Audi:" << std::endl;
     d.printMember();
   }


  std::shared_ptr<Car> audi = std::make_shared<Audi>("Daniel", 200, "Shawn", "Bayern", 8);
  audi->printMember();

  std::string test_dat = "my-new-audi.dat";
  audi->save(test_dat);
  Audi d =  loadAudi(test_dat);
  d.printMember();

  return 0;
}

