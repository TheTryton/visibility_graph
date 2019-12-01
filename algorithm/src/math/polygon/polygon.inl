#pragma once

#include <math/polygon/polygon.hpp>

MATH_NAMESPACE_BEGIN

template<class T>
inline polygon<T>::polygon(const std::vector<point<T, 2>>& vertices) :
    std::vector<point<T, 2>>(vertices)
{
}

MATH_NAMESPACE_END