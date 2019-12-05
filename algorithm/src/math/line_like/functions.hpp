#pragma once

#include <math/line_like/line_like.hpp>
#include <math/line_like/line/line.hpp>

MATH_NAMESPACE_BEGIN

enum class side_of_line
{
    left,
    on,
    right
};

template<class T>
side_of_line get_side_of_line(const line_like<T, 2>& l, const point<T, 2>& point)
{
    auto& a = l.p1();
    auto& b = l.p2();
    auto& c = point;

    auto v = (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);

    if (std::abs(v) < epsilon<T>)
    {
        return side_of_line::on;
    }
    else if (v < -epsilon<T>)
    {
        return side_of_line::left;
    }
    else
    {
        return side_of_line::right;
    }
}

template<class T>
std::optional<point<T, 2>> intersection(const line_like<T, 2>& l1, const line_like<T, 2>& l2)
{
    // algebraic version of checking if two line-like object intersect themselves
    // returns point if l1 and l2 are intersecting in one point (case of colinear is
    // treated as if there is no intersection)

    // sometimes it returns invalid output: eg. when two segments intersect at one of their ending points
    // then algorithm can sometimes calculate invalid t or s parameters because of floating point errors

    auto p2p1 = l2[0] - l1[0];
    auto v1 = l1.creating_vector();
    auto v2 = l2.creating_vector();

    T W = - v1[0] * v2[1] + v1[1] * v2[0];
    if (std::abs(W) < std::numeric_limits<T>::epsilon())
    {
        return std::nullopt;
    }
    T Wt = -p2p1[0] * v2[1] + p2p1[1] * v2[0];
    T Ws = v1[0] * p2p1[1] - v1[1] * p2p1[0];

    T t = Wt / W;
    T s = Ws / W;

    if (!l1.algebraically_inside(t)) // checking if parameter t is inside l1 range eg. for segment t: [0,1], for ray t: [0,inf]
    {
        return std::nullopt;
    }

    if (!l2.algebraically_inside(s)) // checking if parameter s is inside l2 range eg. for segment s: [0,1], for ray s: [0,inf]
    {
        return std::nullopt;
    }

    return v1 * t + l1[0]; // return intersection point
}

template<class T, size_t D>
std::optional<point<T, D>> projection(const point<T, D>& p, const line_like<T, D>& l)
{
    // projection of point p on line-like object l

    auto v1 = l.creating_vector();
    auto ptp1 = l[0] - p;
    auto t = -dot_product(ptp1, v1) / dot_product(v1, v1);

    if (!l.algebraically_inside(t)) // checking if parameter t is inside l1 range eg. for segment t: [0,1], for ray t: [0,inf]
    {
        return std::nullopt;
    }

    return v1 * t + l[0];
}

template<class T, size_t D>
bool contains(const line_like<T, D>& l, const point<T, D>& p)
{
    // function checking if point p is colinear to points of line-like object l
    // and then checking if parameter t is inside object's range

    auto pv = p - l[0];
    auto lv = l.creating_vector();
    auto t1 = pv[0] / lv[0];
    auto t2 = pv[1] / lv[1];
    if (std::abs(t1 - t2) >= epsilon<T>)
    {
        return false;
    }

    return l.algebraically_inside(t1);
}

template<class T, size_t D>
T distance_to_line(const point<T, D>& p, const line<T, D>& l)
{
    // shorthand for calculating perpendicular distance of point p
    // to line l
    return (p - *projection(p, l)).length();
}

MATH_NAMESPACE_END