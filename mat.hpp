#pragma once

#include <cassert>
#include <utility>
#include <vec.hpp>

namespace cu
{

/// A matrix is an array of row vectors.
template <typename T, std::size_t nRows = 0, std::size_t nCols = 0>
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
    return {{ *(list.begin()+rowIndices)... }};
  }

  using Base::size; // private and therefore disabled

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

  static constexpr std::size_t getNRows() { return nRows; }
  static constexpr std::size_t getNCols() { return nCols; }
};


template <typename T>
class Mat<T,0,0>
{
  template <typename U>
  class RowViewImpl
  {
  public:
    RowViewImpl( U data[], std::size_t size )
      : data_(data)
      , size_(size)
    {}

    U & at( std::size_t col )
    {
      if ( col >= size_ )
        throw std::out_of_range( "Column index out of range." );
      return (*this)[col];
    }

    const U & at( std::size_t col ) const
    {
      if ( col >= size_ )
        throw std::out_of_range( "Column index out of range." );
      return (*this)[col];
    }

    U & operator[]( std::size_t col )
    {
      assert( col < size_ );
      return data_[col];
    }

    const U & operator[]( std::size_t col ) const
    {
      assert( col < size_ );
      return data_[col];
    }

    U*       begin()       { return data_      ; }
    U*       end  ()       { return data_+size_; }
    const U* begin() const { return data_      ; }
    const U* end  () const { return data_+size_; }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

  private:
    U * data_;
    std::size_t size_;
  };

  using RowView = RowViewImpl<T>;
  using ConstRowView = RowViewImpl<const T>;

  template <typename U>
  class IteratorImpl
      : public std::iterator<
          std::random_access_iterator_tag,
          RowViewImpl<U>,
          std::ptrdiff_t,
          RowViewImpl<U>*,
          RowViewImpl<U>> // note that the reference type is actually a value type!
  {
    template <typename V>
    static constexpr bool is_same_modulo_const_v =
        std::is_same<std::remove_const_t<U>,std::remove_const_t<V>>::value;
  public:
    IteratorImpl( U data[], std::size_t nCols )
      : data_(data)
      , nCols_(nCols)
    {}

    RowViewImpl<U> operator*() const { return { data_, nCols_ }; }
    RowViewImpl<U> operator[]( std::ptrdiff_t x ) { return *(*this+x); }
    IteratorImpl &operator++(   ) { data_ += nCols_; return *this; }
    IteratorImpl  operator++(int) { IteratorImpl tmp(*this); ++*this; return tmp; }
    IteratorImpl &operator--(   ) { data_ -= nCols_; return *this; }
    IteratorImpl  operator--(int) { IteratorImpl tmp(*this); --*this; return tmp; }
    IteratorImpl &operator+=( std::ptrdiff_t x ) { data_ += x*nCols_; return *this; }
    IteratorImpl &operator-=( std::ptrdiff_t x ) { data_ -= x*nCols_; return *this; }
    IteratorImpl operator+( std::ptrdiff_t x ) { return IteratorImpl(*this)+=x; }
    IteratorImpl operator-( std::ptrdiff_t x ) { return IteratorImpl(*this)-=x; }
    template <typename V>
    std::enable_if_t<is_same_modulo_const_v<V>,
      bool> operator==( const IteratorImpl<V> & other ) const
    {
      assert( nCols_ == other.nCols_ );
      return data_ == other.data_;
    }
    template <typename V>
    std::enable_if_t<is_same_modulo_const_v<V>,
      bool> operator!=( const IteratorImpl<V> & other ) const
    {
      return !(*this == other);
    }
    template <typename V>
    std::enable_if_t<is_same_modulo_const_v<V>,
      bool> operator<( const IteratorImpl<V> & other ) const
    {
      return data_ < other.data_;
    }
    template <typename V>
    std::enable_if_t<is_same_modulo_const_v<V>,
      bool> operator>( const IteratorImpl<V> & other ) const
    {
      return other < *this;
    }
    template <typename V>
    std::enable_if_t<is_same_modulo_const_v<V>,
      bool> operator>=( const IteratorImpl<V> & other ) const
    {
      return !(*this < other);
    }
    template <typename V>
    std::enable_if_t<is_same_modulo_const_v<V>,
      bool> operator<=( const IteratorImpl<V> & other ) const
    {
      return !(other < *this);
    }

  private:
    U *data_;
    std::size_t nCols_;
  };

  using Iterator = IteratorImpl<T>;
  using ConstIterator = IteratorImpl<const T>;

public:
  Mat() = default;
  Mat( std::size_t nRows,
       std::size_t nCols )
    : data_( new T[nRows*nCols] )
    , nRows_(nRows)
    , nCols_(nCols)
  {}

  Mat( std::size_t nRows,
       std::size_t nCols,
       const T & value )
    : Mat( nRows, nCols )
  {
    std::fill_n( &data_[0], nRows*nCols, value );
  }

  std::size_t getNRows() const { return nRows_; }
  std::size_t getNCols() const { return nCols_; }

  RowView operator[]( std::size_t row )
  {
    assert( data_ );
    assert( row < nRows_ );
    return { &data_[row*nCols_], nCols_ };
  }

  ConstRowView operator[]( std::size_t row ) const
  {
    assert( data_ );
    assert( row < nRows_ );
    return { &data_[row*nCols_], nCols_ };
  }

  Iterator begin() { return { &data_[0            ], nCols_ }; }
  Iterator end()   { return { &data_[nRows_*nCols_], nCols_ }; }
  ConstIterator begin() const { return { &data_[0            ], nCols_ }; }
  ConstIterator end()   const { return { &data_[nRows_*nCols_], nCols_ }; }

  const T * data() const
  {
    return data_.get();
  }

  T * data()
  {
    return data_.get();
  }

private:
  std::unique_ptr<T[]> data_;
  std::size_t nRows_{};
  std::size_t nCols_{};
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


template <typename T>
Mat<T,0,0> transpose( const Mat<T,0,0> & mat )
{
  Mat<T,0,0> result( mat.getNCols(), mat.getNRows() );
  for ( std::size_t resultCol = 0; resultCol != mat.getNRows(); ++resultCol )
    for ( std::size_t resultRow = 0; resultRow != mat.getNCols(); ++resultRow )
      result[resultRow][resultCol] = mat[resultCol][resultRow];
  return result;
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
