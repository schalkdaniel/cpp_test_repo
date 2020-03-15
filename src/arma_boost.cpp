//https://www.boost.org/doc/libs/1_47_0/libs/serialization/doc/serialization.html#splittingfreefunctions


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
#include <boost/serialization/split_member.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(arma::mat)

namespace boost { 
namespace serialization {

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
}}
int main() {

  arma::mat A = arma::randu<arma::mat>(4,5);

  std::ofstream outputStream;
  outputStream.open("bin.dat");
  std::ostringstream oss;
  boost::archive::binary_oarchive oa(outputStream);
  oa & A;
  outputStream.close();

  arma::mat B;
  std::ifstream inputStream;
  inputStream.open("bin.dat", std::ifstream::in);
  boost::archive::binary_iarchive ia(inputStream);
  ia & B;
  return 0;
}


