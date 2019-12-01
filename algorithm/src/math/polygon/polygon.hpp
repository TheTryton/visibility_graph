#pragma once

#include <math/common.hpp>

MATH_NAMESPACE_BEGIN

template<class T>
class polygon : public std::vector<point<T, 2>>
{
public:
    polygon() = default;
    polygon(const std::vector<point<T, 2>>& vertices);
    polygon(const polygon& other) = default;
    polygon(polygon&& other) = default;
public:
    polygon& operator=(const polygon& other) = default;
    polygon& operator=(polygon&& other) = default;
};

MATH_NAMESPACE_END