#pragma once

#include <math/line_like/line_like.hpp>
#include <math/vector/vector.inl>
#include <math/point/point.inl>

MATH_NAMESPACE_BEGIN

template<class T, size_t D>
inline line_like<T, D>::line_like(const point<T, D>& p1, const point<T, D>& p2) :
    _ps({ p1,p2 })
{
}

template<class T, size_t D>
inline auto line_like<T, D>::begin()
{
    return _ps.begin();
}

template<class T, size_t D>
inline auto line_like<T, D>::begin() const
{
    return _ps.begin();
}

template<class T, size_t D>
inline auto line_like<T, D>::end()
{
    return _ps.end();
}

template<class T, size_t D>
inline auto line_like<T, D>::end() const
{
    return _ps.end();
}

template<class T, size_t D>
inline auto line_like<T, D>::data()
{
    return _ps.data();
}

template<class T, size_t D>
inline auto line_like<T, D>::data() const
{
    return _ps.data();
}

template<class T, size_t D>
inline auto line_like<T, D>::size() const
{
    return _ps.size();
}

template<class T, size_t D>
inline auto& line_like<T, D>::operator[](size_t i)
{
    return _ps[i];
}

template<class T, size_t D>
inline auto& line_like<T, D>::operator[](size_t i) const
{
    return _ps[i];
}

template<class T, size_t D>
inline decltype(auto) line_like<T, D>::p1()
{
    return _ps[0];
}

template<class T, size_t D>
inline decltype(auto) line_like<T, D>::p1() const
{
    return _ps[0];
}

template<class T, size_t D>
inline decltype(auto) line_like<T, D>::p2()
{
    return _ps[1];
}

template<class T, size_t D>
inline decltype(auto) line_like<T, D>::p2() const
{
    return _ps[1];
}

template<class T, size_t D>
inline vector<T, D> line_like<T, D>::creating_vector() const
{
    return _ps[1] - _ps[0];
}

template<class T, size_t D>
inline vector<T, D> line_like<T, D>::unit_creating_vector() const
{
    return creating_vector().normalized();
}

template<class T, size_t D>
inline bool line_like<T, D>::operator==(const line_like& other) const
{
    return std::equal(_ps.begin(), _ps.end(), other._ps.begin()) || std::equal(_ps.rbegin(), _ps.rend(), other._ps.begin());
}

template<class T, size_t D>
inline bool line_like<T, D>::operator!=(const line_like& other) const
{
    return !std::equal(_ps.begin(), _ps.end(), other._ps.begin()) && !std::equal(_ps.rbegin(), _ps.rend(), other._ps.begin());
}

MATH_NAMESPACE_END