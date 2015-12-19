#pragma once

#include <utility>
#include <vec.hpp>

namespace cu
{

/// A matrix is an array of row vectors.
template <typename T, std::size_t nRows, std::size_t nCols>
class Mat
    : public std::array<Vec<T,nCols>,nRows>
{
private:
  using Base = std::array<Vec<T,nCols>,nRows>;

  template <std::size_t ...rowIndices>
  Base make_matrix_from_init_list(
      const std::initializer_list<Vec<T,nCols> > & list,
      std::index_sequence<rowIndices...> )
  {
    return { *(list.begin()+rowIndices)... };
  }

public:
  using Base::Base;

  Mat()
    : Base{}
  {}

  Mat( const std::initializer_list<Vec<T,nCols> > & list )
    : Base( make_matrix_from_init_list( list, std::make_index_sequence<nRows>() ) )
  {
    assert( list.size() == nRows );
  }
};


// op-assignment operators

template <typename T, std::size_t nRows, std::size_t nCols>
Mat<T,nRows,nCols> & operator+=( Mat<T,nRows,nCols> & lhs, const Mat<T,nRows,nCols> & rhs )
{
  for ( std::size_t nRow = 0; nRow < nRows; ++nRow )
    lhs[nRow] += rhs[nRow];
  return lhs;
}


template <typename T, std::size_t nRows, std::size_t nCols>
Mat<T,nRows,nCols> & operator-=( Mat<T,nRows,nCols> & lhs, const Mat<T,nRows,nCols> & rhs )
{
  for ( std::size_t nRow = 0; nRow < nRows; ++nRow )
    lhs[nRow] -= rhs[nRow];
  return lhs;
}


template <typename T, std::size_t nRows, std::size_t nCols, typename Factor>
Mat<T,nRows,nCols> & operator*=( Mat<T,nRows,nCols> & lhs, const Factor & rhs )
{
  for ( std::size_t nRow = 0; nRow < nRows; ++nRow )
    lhs[nRow] *= rhs;
  return lhs;
}


template <typename T, std::size_t nRows, std::size_t nCols, typename Factor>
Mat<T,nRows,nCols> & operator/=( Mat<T,nRows,nCols> & lhs, const Factor & rhs )
{
  for ( std::size_t nRow = 0; nRow < nRows; ++nRow )
    lhs[nRow] /= rhs;
  return lhs;
}


// plain itemwise operators

template <typename T, std::size_t nRows, std::size_t nCols>
Mat<T,nRows,nCols> operator+( Mat<T,nRows,nCols> lhs, const Mat<T,nRows,nCols> & rhs )
{
  return std::move( lhs+=rhs );
}


template <typename T, std::size_t nRows, std::size_t nCols>
Mat<T,nRows,nCols> operator-( Mat<T,nRows,nCols> lhs, const Mat<T,nRows,nCols> & rhs )
{
  return std::move( lhs-=rhs );
}


template <typename T, std::size_t nRows, std::size_t nCols, typename Factor>
Mat<T,nRows,nCols> operator*( Mat<T,nRows,nCols> lhs, const Factor & rhs )
{
  return std::move( lhs*=rhs );
}


template <typename T, std::size_t nRows, std::size_t nCols, typename Factor>
Mat<T,nRows,nCols> operator*( const Factor & lhs, Mat<T,nRows,nCols> rhs )
{
  return std::move( rhs*=lhs );
}


template <typename T, std::size_t nRows, std::size_t nCols, typename Factor>
Mat<T,nRows,nCols> operator/( Mat<T,nRows,nCols> lhs, const Factor & rhs )
{
  return std::move( lhs/=rhs );
}


// multiplication operators

namespace detail
{
  template <typename T, std::size_t nRows, std::size_t nCols, std::size_t ...indexes>
  Vec<T,nRows> mul_mat_vec_impl(
      const Mat<T,nRows,nCols> & lhs,
      const Vec<T,nCols> & rhs,
      std::index_sequence<indexes...> )
  {
    return { lhs[indexes] * rhs... };
  }
}

template <typename T, std::size_t nRows, std::size_t nCols>
Vec<T,nRows> operator*( const Mat<T,nRows,nCols> & lhs, const Vec<T,nCols> & rhs )
{
  return detail::mul_mat_vec_impl( lhs, rhs, std::make_index_sequence<nRows>() );
}


namespace detail
{

  template <std::size_t colIndex,
            typename T,
            std::size_t nRows,
            std::size_t nCols,
            std::size_t ...rowIndices>
  Vec<T,nRows> transpose_col_impl(
      const Mat<T,nRows,nCols> & mat,
      std::index_sequence<rowIndices...> )
  {
    static_assert( colIndex < nCols, "" );
    return { mat[rowIndices][colIndex]... };
  }

  template <typename T,
            std::size_t nRows,
            std::size_t nCols,
            std::size_t ...colIndices>
  Mat<T,nCols,nRows> transpose_impl(
      const Mat<T,nRows,nCols> & mat,
      std::index_sequence<colIndices...> )
  {
    return { transpose_col_impl<colIndices>(
            mat, std::make_index_sequence<nRows>())... };
  }

} // namespace datail

template <typename T, std::size_t nRows, std::size_t nCols>
Mat<T,nCols,nRows> transpose( const Mat<T,nRows,nCols> & mat )
{
  return detail::transpose_impl(
        mat,
        std::make_index_sequence<nCols>() );
}


template <typename T, std::size_t L, std::size_t M, std::size_t N>
Mat<T,L,N> operator*( const Mat<T,L,M> & A, const Mat<T,M,N> & B )
{
  Mat<T,L,N> result{};
  for ( size_t n = 0; n != N; ++n )
    for ( size_t l = 0; l != L; ++l )
      for ( size_t m = 0; m != M; ++m )
        result[l][n] += A[l][m] * B[m][n];
  return result;
}

} // namespace cu
