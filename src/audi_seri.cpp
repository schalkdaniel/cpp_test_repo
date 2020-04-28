#include <armadillo>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/void_cast_fwd.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>


class Car {
public:
    //Car() = default;
    //virtual double getInfo() = 0;
    virtual char const* type() const = 0;
    virtual ~Car() = default;

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {};
};

class Audi : public Car {
public:
    char const* type() const override { return "Audi"; }
    //double getInfo() override { return 2.2; }
    Audi(std::string owner1, int hp1, std::string second_owner1, std::string country1)
        {
        owner = owner1;
        hp = hp1;
        second_owner = second_owner1; 
        country = country1;
        }
    Audi() = default;

    std::string owner;
    int hp{};
    std::string second_owner;
    std::string country;    
private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & boost::serialization::base_object<Car>(*this);
        ar & owner;
        ar & hp;
        ar & second_owner;
        ar & country;
    }

};

BOOST_CLASS_EXPORT(Audi);
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Car); //Tell Boost that Car is abstract



int main() {
    std::string str;
{
        std::unique_ptr<Car> audi = std::make_unique<Audi>("Wilma", 3, "Rene", "Argentina");
        audi->type();

        std::stringstream strs;
        boost::archive::binary_oarchive ar(strs);
        ar& audi;

        str = strs.str();
    }

    {
        std::unique_ptr<Car> audi; //= std::make_unique<Audi>();

        std::stringstream strs(str);
        boost::archive::binary_iarchive ar(strs);
        ar& audi;
        Audi& d =dynamic_cast<Audi &>(*audi);    
        std::cout << "audi: hp=" << d.hp << " owner=" << d.owner <<"\n";

    }
    
}

