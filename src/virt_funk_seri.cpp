#include <armadillo>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/unique_ptr.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

//Serialization for Armadillo Matrices------------------
BOOST_SERIALIZATION_SPLIT_FREE(arma::mat)
namespace boost::serialization {
    template<class Archive>
    void save(Archive & ar, const arma::mat &t, unsigned int version) {
        ar << t.n_elem;
        auto data = t.colptr(0);
        for (size_t K = 0; K < t.n_elem; ++K)
            ar << data[K];
    }

    template<class Archive>
    void load(Archive & ar, arma::mat &t, unsigned int version) {
        size_t n_elem;
        ar >> n_elem;
        t.set_size(n_elem);
        t.zeros();
        auto data = t.colptr(0);
        for (size_t K = 0; K < n_elem; ++K)
            ar >> data[K];
    }
} // boost::serialization

class Car {
public:
    //Car() = default;
    //virtual double getInfo() = 0;
    virtual char const* type() const = 0;
    virtual ~Car() = default;
    std::string owner;
    int hp{};
    arma::mat A;
private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {};
};

class Porsche : public Car {
public:
    char const* type() const override { return "Porsche"; }
    //double getInfo() override { return 1.1; }
    Porsche(std::string owner1, int hp1, arma::mat A1)
        {
        owner = owner1;
        hp = hp1;
        A = A1; 

        }
    std::string owner;
    int hp{};
    arma::mat A;
    Porsche() = default;
private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & boost::serialization::base_object<Car>(*this); //https://theboostcpplibraries.com/boost.serialization-class-hierarchies
        ar & owner;
        ar & hp;
        ar & A;
    }    

    
};

class Audi : public Car {
public:
    char const* type() const override { return "Audi"; }
    //double getInfo() override { return 2.2; }
    Audi(std::string owner1, int hp1, std::string second_owner1, std::string country1,  arma::mat A1)
        {
        owner = owner1;
        hp = hp1;
        second_owner = second_owner1; 
        country = country1;
        A = A1; 
        }
    Audi() = default;
    // void setVariables(std::string owner1, int hp1, std::string second_owner1, std::string country1,  arma::mat A1)
    // {
    //     owner = owner1;
    //     hp = hp1;
    //     second_owner = second_owner1; 
    //     country = country1;
    //     A = A1;
    // }

    std::string owner;
    int hp{};
    std::string second_owner;
    std::string country;
    arma::mat A;

private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & boost::serialization::base_object<Car>(*this); //https://theboostcpplibraries.com/boost.serialization-class-hierarchies
        ar & owner;
        ar & hp;
        ar & second_owner;
        ar & country;
        ar & A;
    }

};

BOOST_CLASS_EXPORT(Audi);
BOOST_CLASS_EXPORT(Porsche);
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Car); //Tell Boost that Car is abstract



int main() {
    std::string str;
   {
        std::unique_ptr<Car> audi = std::make_unique<Audi>("Wilma", 3, "Rene", "Argentina" ,arma::randu<arma::mat>(4,5));
        //audi->type();

       // audi->A1 = arma::randu<arma::mat>(4,5);
       // audi->hp1 = 3;
       // audi->owner1 = "Wilma"
       // audi->second_owner1 = "Rene"
       // audi->country1 = "Argentina"
        //std::unique_ptr<Car> audi = std::make_unique<Audi>(owner1, hp1, second_owner1, country1, A1);

        std::unique_ptr<Car> porsche = std::make_unique<Porsche>("Joe", 14, arma::randu<arma::mat>(6,5));
        //porsche->type();

//        porsche->A = arma::randu<arma::mat>(6,5);
//        porsche->hp = 14;
//        porsche->owner = "Joe";
//
//std::cout<<porsche->A<<;

        // std::ofstream outputStream("bin.dat", std::ios::binary);
        // boost::archive::binary_oarchive ss(outputStream);
        // ss & *audi.get();
        // ss & *porsche.get();

        std::stringstream strs;
        boost::archive::binary_oarchive ar(strs);
        ar& audi;
        ar& porsche;

        str = strs.str();
    }

    {
        std::unique_ptr<Car> audi; //= std::make_unique<Audi>();
        std::unique_ptr<Car> porsche; //= std::make_unique<Porsche>();

        // std::ifstream inputStream("bin.dat", std::ios::binary);
        // boost::archive::binary_iarchive ss(inputStream);
        // ss & *audi.get();
        // ss & *porsche.get();

        std::stringstream strs(str);
        boost::archive::binary_iarchive ar(strs);
        ar& audi;
        ar& porsche;
        Audi& a =dynamic_cast<Audi &>(*audi);  
        Porsche& p =dynamic_cast<Porsche &>(*porsche);    
  


        std::cout << "audi: hp=" << a.hp << "\n";
        std::cout << "porsche: hp=" << p.hp<< " owner=" << p.owner << " A=" << p.A <<"\n";

    }
    
}
