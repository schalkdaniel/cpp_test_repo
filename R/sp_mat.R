library(Matrix)

src1 = "
arma::sp_mat toArmaSpMat (const arma::Row<unsigned int>& row_idx, const arma::Row<unsigned int>& col_idx, const arma::vec& values)
{
  arma::umat idx(2, row_idx.size());
  idx.row(0) = row_idx;
  idx.row(1) = col_idx;

  arma::sp_mat out(idx, values);

  return out;
}
"

src2 = "
arma::vec valuesFromSpMat (const arma::sp_mat& X)
{
  arma::vec out(X.n_nonzero);

  arma::sp_mat::const_iterator start = X.begin();
  arma::sp_mat::const_iterator end   = X.end();

  unsigned int k = 0;
  for(arma::sp_mat::const_iterator it = start; it != end; ++it) {
    out(k) = (*it);
    k += 1;
  }
  return out;
}
"

Rcpp::cppFunction(code = src1, depends = "RcppArmadillo")
Rcpp::cppFunction(code = src2, depends = "RcppArmadillo")

row_idx = seq_len(5)
col_idx = c(1,1,2,2,1)
x = rnorm(5)

X_sp = toArmaSpMat(row_idx - 1, col_idx - 1, x)
valuesFromSpMat(X_sp)


