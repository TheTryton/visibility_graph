#pragma once

#include <math/line_like/line/line.hpp>

MATH_NAMESPACE_BEGIN

template<class T, size_t D>
inline line<T, D>::line(const point<T, D>& p1, const point<T, D>& p2) :
    line_like<T, D>(p1, p2) 
{
}

template<class T, size_t D>
inline line<T, D>::line(const line_like<T, D>& other) :
    line_like<T, D>(other)
{
}

template<class T, size_t D>
inline line<T, D>::line(line_like<T, D>&& other) :
    line_like<T, D>(other) 
{
}

template<class T, size_t D>
inline line<T, D>& line<T, D>::operator+=(const vector<T, D>& other)
{
    line_like<T, D>::_ps[0] += other;
    line_like<T, D>::_ps[1] += other;

    return *this;
}

template<class T, size_t D>
inline line<T, D>& line<T, D>::operator-=(const vector<T, D>& other)
{
    line_like<T, D>::_ps[0] -= other;
    line_like<T, D>::_ps[1] -= other;

    return *this;
}

template<class T, size_t D>
inline line<T, D> line<T, D>::operator+(const vector<T, D>& other) const
{
    line result{ *this };
    result += other;
    return result;
}

template<class T, size_t D>
inline line<T, D> line<T, D>::operator-(const vector<T, D>& other) const
{
    line result{ *this };
    result -= other;
    return result;
}

template<class T, size_t D>
inline bool line<T, D>::algebraically_inside(const T& coefficent) const
{
    return true;
}

template<class T, size_t D>
line<T, D> operator+(const vector<T, D>& v, const line<T, D>& p)
{
    return p + v;
}

MATH_NAMESPACE_END