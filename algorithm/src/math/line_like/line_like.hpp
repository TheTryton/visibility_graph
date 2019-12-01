#pragma once

#include <math/common.hpp>
#include <math/point/point.hpp>
#include <math/vector/vector.hpp>

MATH_NAMESPACE_BEGIN

template<class T, size_t D>
class line_like
{
protected:
    std::array<point<T, D>, 2> _ps;
public:
    line_like() = default;
    line_like(const point<T, D>& p1, const point<T, D>& p2);
    line_like(const line_like& other) = default;
    line_like(line_like&& other) = default;
public:
    line_like& operator=(const line_like& other) = default;
    line_like& operator=(line_like&& other) = default;
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

    auto p1();
    auto p1() const;
    auto p2();
    auto p2() const;
public:
    vector<T, D> creating_vector() const;
    vector<T, D> unit_creating_vector() const;
public:
    virtual bool algebraically_inside(const T& coefficent) const = 0;
    void rotate_clockwisely(T angle)
    {
        auto x = _ps[1][0];
        auto y = _ps[1][1];
        _ps[1] = point<T, 2>({ x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle) });
        return;
    }
public:
    virtual bool operator==(const line_like& other) const;
    virtual bool operator!=(const line_like& other) const;
};

MATH_NAMESPACE_END