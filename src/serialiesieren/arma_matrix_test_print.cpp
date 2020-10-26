#include <iostream>
#include <armadillo>

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



int main() {
        //arma::mat m= arma::randu(10,10);
        arma::mat A(3,3, arma::fill::randu);
A(1,1) = 0;
A(1,2) = 0;
A(0,0) = 0;
A(2,1) = 0;
A(2,0) = 0;
arma::sp_mat B = arma::sp_mat(A);
arma::vec D(5);
//D[0] = 3;
//arma::sp_mat::const_iterator start = B.begin();
//arma::sp_mat::const_iterator end   = B.end();
//for(arma::sp_mat::const_iterator it = start; it != end; ++it)
//  {
//        std::cout << "location: " << it.row() << "," << it.col() << "  ";
//          std::cout << "value: " << (*it) << std::endl;
//            }

arma::mat F(3,3, arma::fill::randu);

double* mem = F.colptr(2);
double pp = & mem[1];
std::cout<<pp<<std::endl;
// std::cout<<locationRow(B)<<std::endl;

// std::cout<<locationColumn(B)<<std::endl;

// std::cout<<locationColumn(B).n_elem<<std::endl;

//std::cout<<D[0]<<std::endl;
//std::cout<<A<<std::endl;
//std::cout<<B<<std::endl;
//std::cout<<B.n_cols<<std::endl;
//std::cout<<B.n_nonzero<<std::endl;
//std::cout<<B(1,0)<<std::endl;
//std::cout<<B(1,2)<<std::endl;
//std::cout<<valuesFromSpMat(B)<<std::endl;
return 0;
}
