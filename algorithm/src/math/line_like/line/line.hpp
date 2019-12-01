#pragma once

#include <math/line_like/line_like.hpp>

MATH_NAMESPACE_BEGIN

template<class T, size_t D>
class line : public line_like<T, D>
{
public:
    line() = default;
    line(const point<T, D>& p1, const point<T, D>& p2);
    line(const line& other) = default;
    line(line&& other) = default;
    explicit line(const line_like<T, D>& other);
    explicit line(line_like<T, D>&& other);
public:
    line& operator=(const line& other) = default;
    line& operator=(line&& other) = default;
public:
    line& operator+=(const vector<T, D>& other);
    line& operator-=(const vector<T, D>& other);

    line operator+(const vector<T, D>& other) const;
    line operator-(const vector<T, D>& other) const;
public:
    virtual bool algebraically_inside(const T& coefficent) const override;
};

MATH_NAMESPACE_END