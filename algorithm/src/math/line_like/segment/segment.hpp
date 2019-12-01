#pragma once

#include <math/line_like/line_like.hpp>

MATH_NAMESPACE_BEGIN


template<class T, size_t D>
class segment : public line_like<T, D>
{
public:
    segment() = default;
    segment(const point<T, D>& p1, const point<T, D>& p2);
    segment(const segment& other) = default;
    segment(segment&& other) = default;
    explicit segment(const line_like<T, D>& other);
    explicit segment(line_like<T, D>&& other);
public:
    segment& operator=(const segment& other) = default;
    segment& operator=(segment&& other) = default;
public:
    segment& operator+=(const vector<T, D>& other);
    segment& operator-=(const vector<T, D>& other);

    segment operator+(const vector<T, D>& other) const;
    segment operator-(const vector<T, D>& other) const;
public:
    virtual bool algebraically_inside(const T& coefficent) const override;
};

MATH_NAMESPACE_END