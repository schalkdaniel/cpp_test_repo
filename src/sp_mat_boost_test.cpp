//https://www.boost.org/doc/libs/1_47_0/libs/serialization/doc/serialization.html#splittingfreefunctions


#include <iostream>
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <armadillo>
#include <boost/serialization/split_member.hpp>

//BOOST_SERIALIZATION_SPLIT_FREE(arma::sp_mat)
BOOST_SERIALIZATION_SPLIT_FREE(arma::vec)


namespace boost { 
namespace serialization {

template<class Archive>
void save(Archive & ar, const arma::vec &t, unsigned int version)
{
	// ar & t.n_rows;
    // ar & t.n_cols;
    ar << t.n_elem;
	const double *data = t.colptr(0);
	for(int K=0; K<t.n_elem; ++K)
		ar << data[K];

}

template<class Archive>
void load(Archive & ar, arma::vec &t, unsigned int version)
{
	// int rows, cols;
    size_t nelem;
    ar >> nelem;
	// ar & rows;
	// ar & cols;
	t.set_size(nelem);
	auto data = t.colptr(0);
	for(size_t K=0; K<nelem; ++K)
		ar >> data[K];


        // size_t n_elem;
        // ar >> n_elem;
        // t.set_size(n_elem);
        // //t.zeros();
        // auto data = t.colptr(0);
        // for (size_t K = 0; K < n_elem; ++K)
        //     ar >> data[K];


}
}}

arma::vec locationRow(const arma::sp_mat &X)
{

  arma::vec out(X.n_nonzero);  
  arma::sp_mat::const_iterator start = X.begin();
  arma::sp_mat::const_iterator end   = X.end();  
  unsigned int k = 0;
  for(arma::sp_mat::const_iterator it = start; it != end; ++it) 
  {
    out(k) = it.row();
    k += 1;
    
  }
  return out;

}

arma::vec locationColumn(const arma::sp_mat &X)
{

  arma::vec out(X.n_nonzero);  
  arma::sp_mat::const_iterator start = X.begin();
  arma::sp_mat::const_iterator end   = X.end();  
  unsigned int k = 0;
  for(arma::sp_mat::const_iterator it = start; it != end; ++it) 
  {
    out(k) = it.col();
    k += 1;
    
  }
  return out;

}




arma::vec valuesFromSpMat (const arma::sp_mat& X)
{
      arma::vec out(X.n_nonzero);  
      arma::sp_mat::const_iterator start = X.begin();
      arma::sp_mat::const_iterator end   = X.end();  
      unsigned int k = 0;
      for(arma::sp_mat::const_iterator it = start; it != end; ++it) 
      {
        out(k) = (*it);
        k += 1;
      }
      return out;


}
int main() {

  arma::mat A(3,3, arma::fill::randu);
  A(1,1) = 0;
  A(1,2) = 0;
  A(0,0) = 0;
  A(2,1) = 0;
  A(2,0) = 0;
  arma::sp_mat B = arma::sp_mat(A);

  arma::vec C = valuesFromSpMat(B);
  std::ofstream outputStream;
  outputStream.open("bin.dat");
  boost::archive::binary_oarchive oa(outputStream);
  oa & C;
  std::cout<<C<<std::endl;
  outputStream.close();

  arma::vec D;
  std::ifstream inputStream;
  inputStream.open("bin.dat", std::ifstream::in);
  boost::archive::binary_iarchive ia(inputStream);
  ia & D;
  inputStream.close();
  std::cout<<D<<std::endl; 



  return 0;
}


