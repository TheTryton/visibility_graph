#pragma once

#include <math/includes.hpp>

MATH_NAMESPACE_BEGIN

template<class T>
constexpr T epsilon = std::numeric_limits<T>::epsilon();

struct fill_t {};
static fill_t fillv;

template<class T, size_t D>
class vector;

template<class T, size_t D>
class point;

template<class T, size_t D>
class line_like;

template<class T, size_t D>
class line;

template<class T, size_t D>
class segment;

template<class T, size_t D>
class ray;

template<class T>
class polygon;

MATH_NAMESPACE_END