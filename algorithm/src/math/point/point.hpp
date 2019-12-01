#pragma once

#include <math/common.hpp>

MATH_NAMESPACE_BEGIN

template<class T, size_t D>
class point
{
    friend class vector<T, D>;
private:
    std::array<T, D> _coords;
public:
    point() = default;
    point(fill_t, const T& v);
    point(const std::array<T, D>& coords);
    point(const point& other) = default;
    point(point&& other) = default;
    explicit point(const vector<T, D>& other);
    explicit point(vector<T, D>&& other);
public:
    point& operator=(const point& other) = default;
    point& operator=(point&& other) = default;
public:
    point& operator+=(const vector<T, D>& other);
    point& operator-=(const vector<T, D>& other);

    point operator+(const vector<T, D>& other) const;
    point operator-(const vector<T, D>& other) const;

    vector<T, D> operator-(const point& other) const;
public:
    auto begin();
    auto begin() const;

    auto end();
    auto end() const;

    auto data();
    auto data() const;

    auto size() const;

    auto& operator[](size_t i);
    auto& operator[](size_t i) const;
public:
    bool operator==(const point& other) const;
    bool operator!=(const point& other) const;
};

MATH_NAMESPACE_END