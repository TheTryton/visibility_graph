#pragma once

#include <math/common.hpp>

MATH_NAMESPACE_BEGIN

template<class T, size_t D>
class vector
{
    friend class point<T, D>;
private:
    std::array<T, D> _coords;
public:
    vector() = default;
    vector(fill_t, const T& v);
    vector(const std::array<T, D>& coords);
    vector(const vector& other) = default;
    vector(vector&& other) = default;
    explicit vector(const point<T, D>& other);
    explicit vector(point<T, D>&& other);
public:
    vector& operator=(const vector& other) = default;
    vector& operator=(vector&& other) = default;
public:
    vector& operator+=(const vector& other);
    vector& operator-=(const vector& other);
    vector& operator*=(const T& v);
    vector& operator/=(const T& v);

    vector operator+(const vector& other) const;
    vector operator-(const vector& other) const;
    vector operator*(const T& v) const;
    vector operator/(const T& v) const;

    vector operator-() const;
public:
    T dot_product(const vector& other) const;
    T dot(const vector& other) const;
    T length() const;
    T magnitude() const;
    vector& normalize();
    vector normalized();
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
    bool operator==(const vector& other) const;
    bool operator!=(const vector& other) const;
};

MATH_NAMESPACE_END