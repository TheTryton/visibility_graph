#pragma once

#include <math/point/point.hpp>
#include <math/vector/vector.hpp>

MATH_NAMESPACE_BEGIN

template<class T, size_t D>
inline point<T, D>::point(fill_t, const T& v)
{
    std::fill(_coords.begin(), _coords.end(), v);
}

template<class T, size_t D>
inline point<T, D>::point(const std::array<T, D>& coords) :
    _coords(coords) 
{
}

template<class T, size_t D>
inline point<T, D>::point(const vector<T, D>& other) :
    _coords(other._coords) 
{
}

template<class T, size_t D>
inline point<T, D>::point(vector<T, D>&& other) :
    _coords(std::move(other._coords))
{
}

template<class T, size_t D>
inline point<T, D>& point<T, D>::operator+=(const vector<T, D>& other)
{
    for (size_t i = 0; i < D; i++)
    {
        _coords[i] += other._coords[i];
    }

    return *this;
}

template<class T, size_t D>
inline point<T, D>& point<T, D>::operator-=(const vector<T, D>& other)
{
    for (size_t i = 0; i < D; i++)
    {
        _coords[i] -= other._coords[i];
    }

    return *this;
}

template<class T, size_t D>
inline point<T, D> point<T, D>::operator+(const vector<T, D>& other) const
{
    point result{ *this };
    result += other;
    return result;
}

template<class T, size_t D>
inline point<T, D> point<T, D>::operator-(const vector<T, D>& other) const
{
    point result{ *this };
    result -= other;
    return result;
}

template<class T, size_t D>
inline vector<T, D> point<T, D>::operator-(const point& other) const
{
    vector<T, D> result{ *this };
    result -= vector<T, D>(other);
    return result;
}

template<class T, size_t D>
inline auto point<T, D>::begin()
{
    return _coords.begin();
}

template<class T, size_t D>
inline auto point<T, D>::begin() const
{
    return _coords.begin();
}

template<class T, size_t D>
inline auto point<T, D>::end()
{
    return _coords.end();
}

template<class T, size_t D>
inline auto point<T, D>::end() const
{
    return _coords.end();
}

template<class T, size_t D>
inline auto point<T, D>::data()
{
    return _coords.data();
}

template<class T, size_t D>
inline auto point<T, D>::data() const
{
    return _coords.data();
}

template<class T, size_t D>
inline auto point<T, D>::size() const
{
    return _coords.size();
}

template<class T, size_t D>
inline auto& point<T, D>::operator[](size_t i)
{
    return _coords[i];
}

template<class T, size_t D>
inline auto& point<T, D>::operator[](size_t i) const
{
    return _coords[i];
}

template<class T, size_t D>
inline bool point<T, D>::operator==(const point& other) const
{
    return std::equal(begin(), end(), other.begin(), [](const auto& c1, const auto& c2) {return std::abs(c1 - c2) < epsilon<T>; });
}

template<class T, size_t D>
inline bool point<T, D>::operator!=(const point& other) const
{
    return !std::equal(begin(), end(), other.begin(), [](const auto& c1, const auto& c2) {return std::abs(c1 - c2) < epsilon<T>; });
}

template<class T, size_t D>
point<T, D> operator+(const vector<T, D>& v, const point<T, D>& p)
{
    return p + v;
}

MATH_NAMESPACE_END