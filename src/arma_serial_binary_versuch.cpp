/**
 *  @file    pointer_serial.cpp
 *  @author  Shawn
 *
 *  @brief Pointer, saving data with serialization, first draft
 *
 *  @section DESCRIPTION
 *  This section will be updated once the code works as expected.
 *  No virtual functions in this example
 *
 *
 *
 **/


#include <iostream>
#include <fstream>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <sstream>
#include <armadillo>
#include <boost/serialization/split_member.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(arma::mat)




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
      ar & A;
    }


  class archive_type
  {
  template<class Archive>
  void save(Archive & ar, const arma::mat &t, unsigned int version)
  {
	ar & t.n_rows;
	ar & t.n_cols;
	const double *data = t.memptr();
	for(int K=0; K<t.n_elem; ++K)
		ar & data[K];
  }

  template<class Archive>
  void load(Archive & ar, arma::mat &t, unsigned int version)
  {
	int rows, cols;
	ar & rows;
	ar & cols;
	t.set_size(rows, cols);
	double *data = t.memptr();
	for(int K=0; K<t.n_elem; ++K)
		ar & data[K];
  }
  };

  class my_class
  {
    archive_type m_archive;
    static constexpr unsigned in version_number;
    void save_stuff()
    {
        boost::serialization::save(m_archive,*this,version_number);
    }
  };
  // namespace boost { 
  // namespace serialization {

  // template<class Archive>
  // void save(Archive & ar, const arma::mat &t, unsigned int version)
  // {
	// ar & t.n_rows;
	// ar & t.n_cols;
	// const double *data = t.memptr();
	// for(int K=0; K<t.n_elem; ++K)
	// 	ar & data[K];
  // }

  // template<class Archive>
  // void load(Archive & ar, arma::mat &t, unsigned int version)
  // {
	// int rows, cols;
	// ar & rows;
	// ar & cols;
	// t.set_size(rows, cols);
	// double *data = t.memptr();
	// for(int K=0; K<t.n_elem; ++K)
	// 	ar & data[K];
  // }
  // } }  
    
    arma::mat A;// = arma::randu<arma::mat>(4,5);

    void setHP (const int& _hp) { hp = _hp; }
    void setGear (Gear* _Gear) { x = _Gear; }
    void status () { std::cout << "hp = " << hp << " Gear with v = " << x->getV() << A<< std::endl; }

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
  c.A = arma::randu<arma::mat>(4,5);
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

