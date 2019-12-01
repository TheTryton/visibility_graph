#pragma once

#include <math/line_like/segment/segment.hpp>

MATH_NAMESPACE_BEGIN

template<class T, size_t D>
inline segment<T, D>::segment(const point<T, D>& p1, const point<T, D>& p2) :
    line_like<T, D>(p1, p2)
{
}

template<class T, size_t D>
inline segment<T, D>::segment(const line_like<T, D>& other) :
    line_like<T, D>(other)
{
}

template<class T, size_t D>
inline segment<T, D>::segment(line_like<T, D>&& other) :
    line_like<T, D>(other)
{
}

template<class T, size_t D>
inline segment<T, D>& segment<T, D>::operator+=(const vector<T, D>& other)
{
    _ps[0] += other;
    _ps[1] += other;

    return *this;
}

template<class T, size_t D>
inline segment<T, D>& segment<T, D>::operator-=(const vector<T, D>& other)
{
    _ps[0] -= other;
    _ps[1] -= other;

    return *this;
}

template<class T, size_t D>
inline segment<T, D> segment<T, D>::operator+(const vector<T, D>& other) const
{
    segment result{ *this };
    result += other;
    return result;
}

template<class T, size_t D>
inline segment<T, D> segment<T, D>::operator-(const vector<T, D>& other) const
{
    segment result{ *this };
    result -= other;
    return result;
}

template<class T, size_t D>
inline bool segment<T, D>::algebraically_inside(const T& coefficent) const
{
    return coefficent >= 0 && coefficent <= 1;
}

template<class T, size_t D>
segment<T, D> operator+(const vector<T, D>& v, const segment<T, D>& p)
{
    return p + v;
}

MATH_NAMESPACE_END