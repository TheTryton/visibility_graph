#pragma once

#include <math/line_like/ray/ray.hpp>

MATH_NAMESPACE_BEGIN

template<class T, size_t D>
inline ray<T, D>::ray(const point<T, D>& p1, const point<T, D>& p2) :
    line_like<T, D>(p1, p2)
{
}

template<class T, size_t D>
inline ray<T, D>::ray(const line_like<T, D>& other) :
    line_like<T, D>(other)
{
}

template<class T, size_t D>
inline ray<T, D>::ray(line_like<T, D>&& other) :
    line_like<T, D>(other)
{
}

template<class T, size_t D>
inline ray<T, D>& ray<T, D>::operator+=(const vector<T, D>& other)
{
    _ps[0] += other;
    _ps[1] += other;

    return *this;
}

template<class T, size_t D>
inline ray<T, D>& ray<T, D>::operator-=(const vector<T, D>& other)
{
    _ps[0] -= other;
    _ps[1] -= other;

    return *this;
}

template<class T, size_t D>
inline ray<T, D> ray<T, D>::operator+(const vector<T, D>& other) const
{
    ray result{ *this };
    result += other;
    return result;
}

template<class T, size_t D>
inline ray<T, D> ray<T, D>::operator-(const vector<T, D>& other) const
{
    ray result{ *this };
    result -= other;
    return result;
}

template<class T, size_t D>
inline bool ray<T, D>::algebraically_inside(const T& coefficent) const
{
    return coefficent > 0;
}

template<class T, size_t D>
ray<T, D> operator+(const vector<T, D>& v, const ray<T, D>& p)
{
    return p + v;
}

MATH_NAMESPACE_END