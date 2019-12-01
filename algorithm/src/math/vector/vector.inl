#pragma once

#include <math/vector/vector.hpp>
#include <math/point/point.hpp>

MATH_NAMESPACE_BEGIN

template<class T, size_t D>
inline vector<T, D>::vector(fill_t, const T& v)
{
    std::fill(_coords.begin(), _coords.end(), v);
}

template<class T, size_t D>
inline vector<T, D>::vector(const std::array<T, D>& coords) :
    _coords(coords) 
{
}

template<class T, size_t D>
vector<T, D>::vector(const point<T, D>& other) :
    _coords(other._coords)
{
}

template<class T, size_t D>
vector<T, D>::vector(point<T, D>&& other) :
    _coords(std::move(other._coords))
{
}

template<class T, size_t D>
inline vector<T, D>& vector<T, D>::operator+=(const vector& other)
{
    for (size_t i = 0; i < D; i++)
    {
        _coords[i] += other._coords[i];
    }

    return *this;
}

template<class T, size_t D>
inline vector<T, D>& vector<T, D>::operator-=(const vector& other)
{
    for (size_t i = 0; i < D; i++)
    {
        _coords[i] -= other._coords[i];
    }

    return *this;
}

template<class T, size_t D>
inline vector<T, D>& vector<T, D>::operator*=(const T& v)
{
    for (size_t i = 0; i < D; i++)
    {
        _coords[i] *= v;
    }

    return *this;
}

template<class T, size_t D>
inline vector<T, D>& vector<T, D>::operator/=(const T& v)
{
    for (size_t i = 0; i < D; i++)
    {
        _coords[i] /= v;
    }

    return *this;
}

template<class T, size_t D>
inline vector<T, D> vector<T, D>::operator+(const vector& other) const
{
    vector result{ *this };
    result += other;
    return result;
}

template<class T, size_t D>
inline vector<T, D> vector<T, D>::operator-(const vector& other) const
{
    vector result{ *this };
    result -= other;
    return result;
}

template<class T, size_t D>
inline vector<T, D> vector<T, D>::operator*(const T& v) const
{
    vector result{ *this };
    result *= v;
    return result;
}

template<class T, size_t D>
inline vector<T, D> vector<T, D>::operator/(const T& v) const
{
    vector result{ *this };
    result /= v;
    return result;
}

template<class T, size_t D>
inline vector<T, D> vector<T, D>::operator-() const
{
    vector result{ *this };
    for (size_t i = 0; i < D; i++)
    {
        result._coords[i] = -result._coords[i];
    }
    return result;
}

template<class T, size_t D>
inline T vector<T, D>::dot_product(const vector& other) const
{
    return std::inner_product(begin(), end(), other.begin(), T(0));
}

template<class T, size_t D>
inline T vector<T, D>::dot(const vector& other) const
{
    return dot_product(other);
}

template<class T, size_t D>
inline T vector<T, D>::length() const
{
    return std::sqrt(dot_product(*this));
}

template<class T, size_t D>
inline T vector<T, D>::magnitude() const
{
    return length();
}

template<class T, size_t D>
inline vector<T, D>& vector<T, D>::normalize()
{
    *this /= length();
    return *this;
}

template<class T, size_t D>
inline vector<T, D> vector<T, D>::normalized()
{
    return *this / length();
}

template<class T, size_t D>
inline auto vector<T, D>::begin()
{
    return _coords.begin();
}

template<class T, size_t D>
inline auto vector<T, D>::begin() const
{
    return _coords.begin();
}

template<class T, size_t D>
inline auto vector<T, D>::end()
{
    return _coords.end();
}

template<class T, size_t D>
inline auto vector<T, D>::end() const
{
    return _coords.end();
}

template<class T, size_t D>
inline auto vector<T, D>::data()
{
    return _coords.data();
}

template<class T, size_t D>
inline auto vector<T, D>::data() const
{
    return _coords.data();
}

template<class T, size_t D>
inline auto vector<T, D>::size() const
{
    return _coords.size();
}

template<class T, size_t D>
inline auto& vector<T, D>::operator[](size_t i)
{
    return _coords[i];
}

template<class T, size_t D>
inline auto& vector<T, D>::operator[](size_t i) const
{
    return _coords[i];
}

template<class T, size_t D>
inline bool vector<T, D>::operator==(const vector& other) const
{
    return std::equal(begin(), end(), other.begin(), [](const auto& c1, const auto& c2) {return std::abs(c1 - c2) < epsilon<T>; });
}

template<class T, size_t D>
inline bool vector<T, D>::operator!=(const vector& other) const
{
    return !std::equal(begin(), end(), other.begin(), [](const auto& c1, const auto& c2) {return std::abs(c1 - c2) < epsilon<T>; });
}

template<class T, size_t D>
T dot(const vector<T, D>& v1, const vector<T, D>& v2)
{
    return v1.dot(v2);
}

template<class T, size_t D>
T dot_product(const vector<T, D>& v1, const vector<T, D>& v2)
{
    return v1.dot(v2);
}

template<class T, size_t D>
vector<T, D> operator*(const T& v, const vector<T, D>& other)
{
    vector<T, D> result{ other };
    result *= v;
    return result;
}

MATH_NAMESPACE_END