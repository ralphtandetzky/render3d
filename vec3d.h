#pragma once

#include <array>
#include <cmath>
#include <cstdint>

template <typename T, std::size_t N>
class Vec
{
public:
    std::array<T,N> x{};
};

template <typename T, std::size_t N>
Vec<T,N> & operator+=( Vec<T,N> & lhs, const Vec<T,N> &rhs )
{
    for ( std::size_t i = 0; i!=N; ++i )
        lhs.x[i] += rhs.x[i];
    return lhs;
}

template <typename T, std::size_t N>
Vec<T,N> & operator-=( Vec<T,N> & lhs, const Vec<T,N> &rhs )
{
    for ( std::size_t i = 0; i!=N; ++i )
        lhs.x[i] -= rhs.x[i];
    return lhs;
}

template <typename T, std::size_t N>
Vec<T,N> & operator*=( Vec<T,N> & lhs, const T &rhs )
{
    for ( std::size_t i = 0; i!=N; ++i )
        lhs.x[i] *= rhs;
    return lhs;
}

template <typename T, std::size_t N>
Vec<T,N> & operator/=( Vec<T,N> & lhs, const T &rhs )
{
    auto inv = 1 / rhs;
    for ( std::size_t i = 0; i!=N; ++i )
        lhs.x[i] *= inv;
    return lhs;
}

template <typename T, std::size_t N>
Vec<T,N> operator+( Vec<T,N> lhs, const Vec<T,N> & rhs )
{
    return lhs += rhs;
}

template <typename T, std::size_t N>
Vec<T,N> operator-( Vec<T,N> lhs, const Vec<T,N> & rhs )
{
    return lhs -= rhs;
}

template <typename T, std::size_t N>
Vec<T,N> operator*( Vec<T,N> lhs, const T & rhs )
{
    return lhs *= rhs;
}

template <typename T, std::size_t N>
Vec<T,N> operator*( const T & lhs, Vec<T,N> rhs )
{
    return rhs *= lhs;
}

template <typename T, std::size_t N>
Vec<T,N> operator/( Vec<T,N> lhs, const T & rhs )
{
    return lhs /= rhs;
}

template <typename T, std::size_t N>
T operator*( const Vec<T,N> & lhs, const Vec<T,N> & rhs )
{
    T sum{};
    for ( size_t i = 0; i != N; ++i )
        sum += lhs.x[i] * rhs.x[i];
    return sum;
}

template <typename T, std::size_t N>
T sqrNorm( const Vec<T,N> & vec )
{
    return vec*vec;
}

template <typename T, std::size_t N>
T norm( const Vec<T,N> & vec )
{
    return std::sqrt(sqrNorm( vec ));
}
