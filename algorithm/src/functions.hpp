#pragma once

#include <structures.hpp>

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

    return v > 0.0f ? side_of_line::left : (v < 0.0f ? side_of_line::right : side_of_line::on);
}

template<class T>
std::optional<point<T, 2>> intersection(const line_like<T, 2>& l1, const line_like<T, 2>& l2)
{
    auto p2p1 = l2[0] - l1[0];
    auto v1 = l1.creating_vector();
    auto v2 = l2.creating_vector();

    float W = v1[1] * v2[0] - v1[0] * v2[1];
    if (std::abs(W) < std::numeric_limits<float>::epsilon())
    {
        return std::nullopt;
    }
    float Wt = p2p1[1] * v2[0] - p2p1[0] * v2[1];
    float Ws = v1[1] * p2p1[0] - v1[0] * p2p1[1];

    float t = Wt / W;
    float s = Ws / W;

    if (!l1.algebraically_inside(t))
    {
        return std::nullopt;
    }

    if (!l2.algebraically_inside(t))
    {
        return std::nullopt;
    }

    return v1 * t + l1[0];
}

template<class T, size_t D>
std::optional<point<T, D>> projection(const point<T, D>& p, const line_like<T, D>& l)
{
    auto v1 = l.creating_vector();
    auto ptp1 = line[0] - p;
    auto t = -dot_product(ptp1, v1) / dot_product(v1, v1);

    if (!l1.algebraically_inside(t))
    {
        return std::nullopt;
    }

    return v1 * t + line[0];
}

template<class T, size_t D>
bool contains(const line_like<T, D>& l, const point<T, D>& p)
{
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
    return (p - *projection(p, l)).length();
}

enum class polygon_point_classification
{
    outside = 0,
    inside = 1,
    on_edge = 2,
};

template<class T>
polygon_point_classification contains(const polygon<T>& poly, const point<T, 2>& p)
{
    auto ray_from_point = ray<T, 2>(p, p + vector<T, 2>({ 1,0 }));

    size_t count_intersections = 0;

    for (size_t i = 0; i < poly.size(); i++)
    {
        auto edge = segment<T, 2>(poly[i], poly[(i + 1) % poly.size()]);
        auto intersection_point_opt = intersection(ray_from_point, edge);
        if (intersection_point_opt)
        {
            auto& intersection_point = *intersection_point_opt;

            if (intersection_point == p)
            {
                return polygon_point_classification::on_edge;
            }
            else
            {
                count_intersections++;
            }
        }
    }

    return count_intersections % 2 ? polygon_point_classification::inside : polygon_point_classification::outside;
}

template<class T>
bool intersects_interior(const segment<T, 2>& s, const polygon<T>& p)
{
    size_t count_intersections = 0;

    for (size_t i = 0; i < p.size(); i++)
    {
        auto edge = segment<T, 2>(p[i], p[(i + 1) % p.size()]);
        auto intersection_point_opt = intersection(s, edge);
        if (intersection_point_opt)
        {
            auto& intersection_point = *intersection_point_opt;

            if (intersection_point != s[0] && intersection_point != s[1])
            {
                count_intersections++;
            }
        }
    }

    return count_intersections > 0;
}