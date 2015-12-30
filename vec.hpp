#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iterator>
#include <type_traits>

namespace cu
{

template <typename T, std::size_t N>
class Vec
    : public std::array<T,N>
{
private:
  using Base = std::array<T,N>;

public:
  using Base::Base;

  Vec()
    : Base{}
  {}

  Vec( std::initializer_list<T> list )
  {
    assert( list.size() == N );
    for ( size_t i = 0; i != N; ++i )
        (*this)[i] = std::move( *(list.begin()+i) );
  }
};


template <typename ...Ts>
auto makeVec( Ts &&... args ) -> Vec<std::common_type_t<Ts...>,sizeof...(args)>
{
    return { std::forward<Ts>(args)... };
}


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


template <typename T, std::size_t N, typename Factor>
Vec<T,N> & operator*=( Vec<T,N> & lhs, const Factor & rhs )
{
  for ( std::size_t idx = 0; idx < N; ++idx )
    lhs[idx] *= rhs;

  return lhs;
}


template <typename T, std::size_t N, typename Factor>
Vec<T,N> & operator/=( Vec<T,N> & lhs, const Factor & rhs )
{
  const auto inv = T(1) / rhs;
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


template <typename T, std::size_t N>
Vec<T,N> normalize( const Vec<T,N> & vec )
{
  return vec / l2Norm(vec);
}


template <typename T, std::size_t N>
bool operator==( const Vec<T,N> & lhs, const Vec<T,N> & rhs )
{
  for ( std::size_t idx = 0; idx < N; ++idx )
    if ( lhs[idx] != rhs[idx] )
      return false;
  return true;
}


template <typename T>
Vec<T,3> crossProduct( const Vec<T,3> & u,
                       const Vec<T,3> & v )
{
  return { u[1]*v[2] - u[2]*v[1],
           u[2]*v[0] - u[0]*v[2],
           u[0]*v[1] - u[1]*v[0] };
}


template <typename T>
Vec<T,3> normalVector( const Vec<T,3> & P,
                       const Vec<T,3> & Q,
                       const Vec<T,3> & R )
{
  const auto v = crossProduct( Q-P, R-P );
  const auto n = sqrNorm(v);
  if ( n == 0 )
    return {0,0,0};
  return v / std::sqrt(n);
}


namespace detail
{

  template <typename T, std::size_t N, std::size_t ...indexes>
  Vec<T,N-1> popBackImpl( const Vec<T,N> & vec, std::index_sequence<indexes...> )
  {
    return {{ vec[indexes]... }};
  }

} // namespace detail

template <typename T, std::size_t N>
Vec<T,N-1> popBack( const Vec<T,N> & vec )
{
  return detail::popBackImpl( vec, std::make_index_sequence<N-1>() );
}

} // cu
