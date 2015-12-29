#pragma once

#include "mat.hpp"
#include "vec.hpp"

#include <algorithm>


namespace cu
{

template <typename Coord>
std::array<Vec<Coord,2>,3> getPointsSortedByYValue(
    const Vec<Coord,2> & A,
    const Vec<Coord,2> & B,
    const Vec<Coord,2> & C)
{
  std::array<Vec<Coord,2>,3> sortedPoints = { A, B, C };
  const auto swapIfNotSorted = []( auto & lhs, auto & rhs )
  { if ( rhs[1] < lhs[1] ) swap( rhs, lhs ); };
  swapIfNotSorted( sortedPoints[0], sortedPoints[1] );
  swapIfNotSorted( sortedPoints[1], sortedPoints[2] );
  swapIfNotSorted( sortedPoints[0], sortedPoints[1] );
  return sortedPoints;
}


template <typename T>
void drawHorizontalLine( Mat<T> & img,
                         std::size_t y,
                         std::ptrdiff_t left,
                         std::ptrdiff_t right,
                         T color )
{
  assert( left <= right );
  right = std::min( std::ptrdiff_t(img.getNCols()), right );
  for ( left  = std::max( 0*left, left ); left < right; ++left )
    img[y][left] = color;
}


template <typename T, typename Coord>
void drawHorizontalBaseTriangleImpl( Mat<T> & img,
                                     Vec<Coord,2> P,
                                     std::ptrdiff_t minY,
                                     std::ptrdiff_t maxY,
                                     Coord lXStep,
                                     Coord rXStep,
                                     T color )
{
  Coord l = P[0] + lXStep * (minY - P[1]);
  Coord r = P[0] + rXStep * (minY - P[1]);
  for ( ; minY < maxY; ++minY, l+=lXStep, r+=rXStep )
    drawHorizontalLine( img, minY, (std::ptrdiff_t)ceil(l),
                                   (std::ptrdiff_t)ceil(r), color );
}


template <typename T, typename Coord>
void drawHorizontalBaseTriangle( Mat<T> & img,
                                 Vec<Coord,2> A,
                                 Coord lXStep,
                                 Coord rXStep,
                                 Coord bottom,
                                 T color )
{
  assert( A[1] < bottom );
  drawHorizontalBaseTriangleImpl(
      img, A,
      ceil(A[1]),
      ceil(bottom),
      std::min(lXStep,rXStep),
      std::max(lXStep,rXStep),
      color );
}


template <typename T, typename Coord>
void drawHorizontalBaseTriangle( Mat<T> & img,
                                 Coord top,
                                 Coord lXStep,
                                 Coord rXStep,
                                 Vec<Coord,2> C,
                                 T color )
{
  assert( C[1] > top );
  drawHorizontalBaseTriangleImpl(
      img, C,
      ceil(top),
      ceil(C[1]),
      std::max(lXStep,rXStep),
      std::min(lXStep,rXStep),
      color );
}

template <typename T, typename Coord>
void drawTriangle( Mat<T> & img,
                   Vec<Coord,2> A,
                   Vec<Coord,2> B,
                   Vec<Coord,2> C,
                   T color )
{
  const auto sortedPoints = getPointsSortedByYValue( A, B, C );
  A = sortedPoints[0];
  B = sortedPoints[1];
  C = sortedPoints[2];
  if ( A[1] == C[1] )
    return;
  const auto xStepAC = ( C[0] - A[0] ) / ( C[1] - A[1] );

  if ( A[1] != B[1] )
  {
    const auto xStepAB = ( B[0] - A[0] ) / ( B[1] - A[1] );
    drawHorizontalBaseTriangle(
          img,
          A,
          xStepAB,
          xStepAC,
          B[1],
          color );
  }

  if ( B[1] != C[1] )
  {
    const auto xStepBC = ( C[0] - B[0] ) / ( C[1] - B[1] );
    drawHorizontalBaseTriangle(
          img,
          B[1],
          xStepAC,
          xStepBC,
          C,
          color );
  }
}


} // namespace cu
