#pragma once

#include <array>
#include <cmath>

namespace cu
{

template <typename T, std::size_t N>
class Vec
    : std::array<T,N>
{
public:
  using std::array<T,N>::array;
};


template <typename T, std::size_t N>
Vec<T,N> & operator+=( Vec<T,N> & lhs, const Vec<T,N> & rhs )
{
  for ( std::size_t idx = 0; idx < N; ++idx )
    lhs[idx] += rhs[idx];

  return lhs;
}


template <typename T, std::size_t N>
Vec<T,N> & operator-=( Vec<T,N> & lhs, const Vec<T,N> & rhs )
{
  for ( std::size_t idx = 0; idx < N; ++idx )
    lhs[idx] -= rhs[idx];

  return lhs;
}


template <typename T, std::size_t N>
Vec<T,N> & operator*=( Vec<T,N> & lhs, const T & rhs )
{
  for ( std::size_t idx = 0; idx < N; ++idx )
    lhs[idx] *= rhs;

  return lhs;
}


template <typename T, std::size_t N>
Vec<T,N> & operator/=( Vec<T,N> & lhs, const T & rhs )
{
  const auto inv = 1 / rhs;
  for ( std::size_t idx = 0; idx < N; ++idx )
    lhs[idx] *= inv;

  return lhs;
}


template <typename T, std::size_t N>
Vec<T,N> operator+( Vec<T,N> lhs, const Vec<T,N>& rhs )
{
  return std::move( lhs+=rhs );
}


template <typename T, std::size_t N>
Vec<T,N> operator-( Vec<T,N> lhs, const Vec<T,N>& rhs )
{
  return std::move( lhs-=rhs );
}


template <typename T, std::size_t N>
Vec<T,N> operator*( Vec<T,N> lhs, const T & rhs )
{
  return std::move( lhs*=rhs );
}


template <typename T, std::size_t N>
Vec<T,N> operator*( const T & lhs, Vec<T,N> rhs )
{
  return rhs*lhs;
}


template <typename T, std::size_t N>
Vec<T,N> operator/( Vec<T,N> lhs, const T & rhs )
{
  return std::move( lhs/=rhs );
}


template <typename T, std::size_t N>
T operator*( const Vec<T,N> & lhs, const Vec<T,N> & rhs )
{
  T sum = 0;
  for ( std::size_t idx = 0; idx < N; ++idx )
    sum += lhs[idx] * rhs[idx];

  return sum;
}


template <typename T, std::size_t N>
T sqrNorm( const Vec<T,N> & vec )
{
  return vec*vec;
}


template <typename T, std::size_t N>
T l2Norm( const Vec<T,N> & vec )
{
  return std::sqrt( sqrNorm(vec) );
}

} // cu
