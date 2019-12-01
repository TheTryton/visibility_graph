#pragma once

#include <math/line_like/line_like.hpp>

MATH_NAMESPACE_BEGIN

template<class T, size_t D>
class ray : public line_like<T, D>
{
public:
    ray() = default;
    ray(const point<T, D>& p1, const point<T, D>& p2);
    ray(const ray& other) = default;
    ray(ray&& other) = default;
    explicit ray(const line_like<T, D>& other);
    explicit ray(line_like<T, D>&& other);
public:
    ray& operator=(const ray& other) = default;
    ray& operator=(ray&& other) = default;
public:
    ray& operator+=(const vector<T, D>& other);
    ray& operator-=(const vector<T, D>& other);

    ray operator+(const vector<T, D>& other) const;
    ray operator-(const vector<T, D>& other) const;
public:
    virtual bool algebraically_inside(const T& coefficent) const override;
};

MATH_NAMESPACE_END