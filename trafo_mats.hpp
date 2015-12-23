#pragma once

#include "mat.hpp"

namespace cu
{

template <typename T, std::size_t N>
Mat<T,N,N> makeIdentityMat()
{
  Mat<T,N,N> result;
  for ( std::size_t n = 0; n != N; ++n)
    result[n][n] = 1;
  return result;
}


template <typename T, std::size_t N>
Mat<T,N+1,N+1> makeTranslationMat( const Vec<T,N> & v )
{
  auto result = makeIdentityMat<T,N+1>();
  for ( std::size_t n = 0; n != N; ++n )
    result[n][N] = v[n];
  return result;
}


template <typename T>
Mat<T,3,3> makeRotationMat( Vec<T,3> v )
{
  /// Assuming we normalize v, using Einstein's sum convention we have:
  /// y_i = v_i * v_j * x_j + cos(angle) * (delta_ij - v_i * v_j) * x_j + sin(angle) * epsilon_ijk * v_k * x_j
  ///     = ( cos(angle) * delta_ij + (1-cos(angle)) * v_i * v_j + sin(angle) * epsilon_ijk * v_k ) * x_j
  const auto angle = l2Norm(v);
  if ( angle == 0 )
    return makeIdentityMat<T,3>();

  v /= angle; // normalize v
  const auto cosAngle = std::cos(angle);
  const auto sinAngle = std::sin(angle);
  const auto v_tensor_v = Mat<T,3,3>{
    { v[0]*v[0], v[0]*v[1], v[0]*v[2] },
    { v[1]*v[0], v[1]*v[1], v[1]*v[2] },
    { v[2]*v[0], v[2]*v[1], v[2]*v[2] }
  };
  const auto epsilon_tensor_v = Mat<T,3,3>{
    {   0 ,-v[2], v[1] },
    { v[2],   0 ,-v[0] },
    {-v[1], v[0],   0  },
  };

  return cosAngle * makeIdentityMat<T,3>() +
      (1-cosAngle)* v_tensor_v +
         sinAngle * epsilon_tensor_v;
}


template <typename T, std::size_t N>
Mat<T,N+1,N+1> makeExtendedMat( const Mat<T,N,N> & m )
{
  Mat<T,N+1,N+1> result;
  for ( std::size_t row = 0; row != N; ++row )
    for ( std::size_t col = 0; col != N; ++col )
      result[col][row] = m[col][row];
  result[N][N] = 1;
  return result;
}

} // namespace cu
