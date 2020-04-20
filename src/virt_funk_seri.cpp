#include <armadillo>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_member.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Car); //Tell Boost that Car is abstract

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
      virtual double getInfo() = 0;

private:
//    template <typename Archive>
//    virtual void serialize(Archive& ar, unsigned int version) = 0;
};

class Porsche : public Car {
public:
    double getInfo() override { return 1.1; }

    template <typename Archive>
    void serialize(Archive& ar, unsigned int) {
        ar & owner;
        ar & hp;
        ar & A;
    }

public:
    std::string owner;
    int hp{};
    arma::mat A;
};

class Audi : public Car {
public:
    double getInfo() override { return 2; }

    template <typename Archive>
    void serialize(Archive& ar, unsigned int) {
        ar & owner;
        ar & hp;
        ar & second_owner;
        ar & country;
        ar & A;
    }

public:
    std::string owner;
    int hp{};
    std::string second_owner;
    std::string country;
 arma::mat A;
};

int main() {
    {
        std::unique_ptr<Audi> audi(new Audi);
        audi->getInfo();
        audi->A = arma::randu<arma::mat>(4,5);
        audi->hp = 3;

        std::unique_ptr<Porsche> porsche(new Porsche);
        porsche->getInfo();
        porsche->A = arma::randu<arma::mat>(6,5);
        porsche->hp = 14;
        porsche->owner = "Joe";

        std::ofstream outputStream("bin.dat", std::ios::binary);
//      boost::archive::text_oarchive ss(outputStream);
        boost::archive::binary_oarchive ss(outputStream);
        ss & *audi.get();
        ss & *porsche.get();
    }

    {
        std::unique_ptr<Audi> audi(new Audi);
        std::unique_ptr<Porsche>porsche(new Porsche);

        std::ifstream inputStream("bin.dat", std::ios::binary);
//      boost::archive::text_oarchive ss(outputStream);
        boost::archive::binary_iarchive ss(inputStream);
        ss & *audi.get();
        ss & *porsche.get();

        std::cout << "audi: hp=" << audi->hp << "\n";
        std::cout << "porsche: hp=" << porsche->hp << " owner=" << porsche->owner << " A=" << porsche-> A <<"\n";

    }
}