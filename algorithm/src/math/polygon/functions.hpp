#pragma once

#include <math/polygon/polygon.hpp>
#include <math/line_like/segment/segment.hpp>

MATH_NAMESPACE_BEGIN

enum class polygon_point_classification
{
    outside = 0,
    inside = 1,
    on_edge = 2,
};

template<class T>
polygon_point_classification contains(const polygon<T>& poly, const point<T, 2>& p)
{
    // https://en.wikipedia.org/wiki/Point_in_polygon

    auto ray_from_point1 = ray<T, 2>(p, p + vector<T, 2>({ 1,0 }));
    auto ray_from_point2 = ray<T, 2>(p, p + vector<T, 2>({ 0,1 }));

    size_t count_intersections1 = 0;
    size_t count_intersections2 = 0;

    for (size_t i = 0; i < poly.size(); i++)
    {
        auto edge = segment<T, 2>(poly[i], poly[(i + 1) % poly.size()]);

        if (get_side_of_line(edge, p) == side_of_line::on)
        {
            return polygon_point_classification::on_edge;
        }

        auto intersection_point_opt1 = intersection(ray_from_point1, edge);
        auto intersection_point_opt2 = intersection(ray_from_point2, edge);

        if (intersection_point_opt1)
        {
            count_intersections1++;
        }

        if (intersection_point_opt2)
        {
            count_intersections2++;
        }
    }

    return count_intersections1 % 2 == 1 || count_intersections2 % 2 == 1 ? polygon_point_classification::inside : polygon_point_classification::outside;
}

template<class T>
bool intersects_interior(const segment<T, 2>& s, const polygon<T>& poly)
{
    // derived from https://en.wikipedia.org/wiki/Point_in_polygon
    // however this algorithm is not 100% accurate because of imposibility
    // to reliably check if given segment is contained within polygon when
    // both of point of the segment are on polygon edges

    size_t count_intersections = 0;

    for (size_t i = 0; i < poly.size(); i++)
    {
        auto edge = segment<T, 2>(poly[i], poly[(i + 1) % poly.size()]);

        if (get_side_of_line(edge, s[0]) == side_of_line::on ||
            get_side_of_line(edge, s[1]) == side_of_line::on)
        {
            continue;
        }

        auto intersection_point_opt = intersection(s, edge);
        if (intersection_point_opt)
        {
            auto& intersection_point = *intersection_point_opt;

            count_intersections++;
        }
    }

    return count_intersections > 0;
}

MATH_NAMESPACE_END