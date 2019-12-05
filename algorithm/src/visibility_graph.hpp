#pragma once

#include <math/math.hpp>
#include "skip_list.hpp"
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <memory>

#include <exception>

using namespace data_structures;
using namespace math;

template<class V, class E>
class graph
{
public:
    class vertex;
    class edge;

    class edge final
    {
        friend class graph<V, E>;
        friend class vertex;
    private:
        E _data;
        size_t _from;
        size_t _to;
    private:
        edge(size_t from, size_t to) : _from(from), _to(to) {}
        template<class EO>
        edge(size_t from, size_t to, EO&& data) : _data(std::forward<EO>(data)), _from(from), _to(to) {}
    public:
        size_t from() const { return _from; }
        size_t to() const { return _to; }
        const E& data() const { return _data; }
        E& data() { return _data; }
    public:
        bool operator==(const edge& other);
        bool operator!=(const edge& other);
    };

    class vertex final
    {
        friend class graph<V, E>;
    private:
        V _data;
        size_t _id;
        std::vector<edge> _ingoing_edges;
        std::vector<edge> _outgoing_edges;
    private:
        vertex(size_t id) : _id(id) {}
        template<class VO>
        vertex(size_t id, VO&& data) : _data(std::forward<VO>(data)), _id(id) {}
    private:
        edge& add_edge(const edge& e)
        {
            if (e.from() == *this)
            {
                _outgoing_edges.push_back(e);
                return _outgoing_edges.back();
            }
            else
            {
                _ingoing_edges.push_back(e);
                return _ingoing_edges.back();
            }
        }
        void remove_edge(size_t from, size_t to)
        {
            if (from == _id)
            {
                auto edge_to_remove = std::find_if(_outgoing_edges.begin(), _outgoing_edges.end(), [to](const edge& e) {return e.to() == to; });
                _outgoing_edges.erase(edge_to_remove);
            }
            else
            {
                auto edge_to_remove = std::find_if(_ingoing_edges.begin(), _ingoing_edges.end(), [to](const edge& e) {return e.from() == to; });
                _ingoing_edges.erase(edge_to_remove);
            }
        }
    public:
        std::vector<edge>& ingoing_edges() { return _ingoing_edges; }
        const std::vector<edge>& ingoing_edges() const { return _ingoing_edges; }

        std::vector<edge>& outgoing_edges() { return _outgoing_edges; }
        const std::vector<edge>& outgoing_edges() const { return _outgoing_edges; }

        const V& data() const { return _data; }
        V& data() { return _data; }

        size_t id() const { return _id; }
    public:
        bool operator==(const vertex& other) { return other._id == _id; }
        bool operator!=(const vertex& other) { return other._id != _id; }
    public:
        operator size_t() { return _id; }
    };
private:
    std::vector<vertex> _vertices;
public:
    graph(size_t vertices_count = 0)
    {
        std::vector<size_t> vids(vertices_count);
        std::iota(vids.begin(), vids.end(), size_t(0));

        std::transform(vids.begin(), vids.end(), std::back_inserter(_vertices), [](size_t i) {return vertex(i); });
    }
    template<class It>
    graph(size_t vertices_count, It data_it)
    {
        size_t i = 0;
        for (size_i = 0; i < vertices_count; i++)
        {
            _vertices.push_back(vertex(i, *data_it++));
        }
    }
    template<class It>
    graph(It data_b, It data_e)
    {
        size_t i = 0;
        std::transform(data_b, data_e, std::back_inserter(_vertices), [&i](const auto& data) {return vertex(i++, data); });
    }
public:
    vertex& add_vertex()
    {
        _vertices.push_back(vertex(_vertices.size()));
        return _vertices.back();
    }
    void remove_vertex(size_t vertex_id)
    {
        auto after_removed_vertex = _vertices.erase(_vertices.begin() + vertex_id);
        std::for_each(after_removed_vertex, _vertices.end(), [](vertex& v) {v._id = v._id - 1; });
    }
    void remove_vertex(const vertex& vertex)
    {
        auto vertex_to_remove = std::find(_vertices.begin(), _vertices.end(), vertex);
        auto after_removed_vertex = _vertices.erase(vertex_to_remove);
        std::for_each(after_removed_vertex, _vertices.end(), [](vertex& v) {v._id = v._id - 1; });
    }
    edge& add_directed_edge(size_t from_vertex_id, size_t to_vertex_id)
    {
        auto new_edge = edge(_vertices[from_vertex_id], _vertices[to_vertex_id]);
        _vertices[to_vertex_id].add_edge(new_edge);
        return _vertices[from_vertex_id].add_edge(new_edge);
    }
    std::pair<edge&, edge&> add_undirected_edge(size_t from_vertex_id, size_t to_vertex_id)
    {
        auto new_edge = edge(_vertices[from_vertex_id], _vertices[to_vertex_id]);
        auto new_redge = edge(_vertices[to_vertex_id], _vertices[from_vertex_id]);
        _vertices[to_vertex_id].add_edge(new_edge);
        _vertices[from_vertex_id].add_edge(new_redge);
        return std::pair(_vertices[from_vertex_id].add_edge(new_edge), _vertices[to_vertex_id].add_edge(new_redge));
    }
    void remove_directed_edge(size_t from_vertex_id, size_t to_vertex_id)
    {
        _vertices[to_vertex_id].remove_edge(from_vertex_id, to_vertex_id);
        _vertices[from_vertex_id].remove_edge(from_vertex_id, to_vertex_id);
    }
    void remove_undirected_edge(size_t from_vertex_id, size_t to_vertex_id)
    {
        _vertices[to_vertex_id].remove_edge(from_vertex_id, to_vertex_id);
        _vertices[from_vertex_id].remove_edge(from_vertex_id, to_vertex_id);
        _vertices[to_vertex_id].remove_edge(to_vertex_id, from_vertex_id);
        _vertices[from_vertex_id].remove_edge(to_vertex_id, from_vertex_id);
    }
    void remove_directed_edge(const edge& e)
    {
        remove_directed_edge(e.from(), e.to());
    }
    void remove_undirected_edge(const edge& e)
    {
        remove_undirected_edge(e.from(), e.to());
    }
public:
    std::vector<vertex>& vertices() { return _vertices; }
    const std::vector<vertex>& vertices() const { return _vertices; }
public:
    size_t size() const { return _vertices.size(); };
};

template<class V, class E>
inline bool graph<V, E>::edge::operator==(const edge& other)
{
    return from() == other.from() && to() == other.to();
}

template<class V, class E>
inline bool graph<V, E>::edge::operator!=(const edge& other)
{
    return from() != other.from() || to() != other.to();
}

struct empty {};

template<class V>
using visibility_graph = graph<V, empty>;

template<class T>
auto compute_visibility_graph(const std::vector<polygon<T>>& obstacles)
{
    using std::shared_ptr;
    using std::weak_ptr;
    using std::make_shared;
    using std::sort;
    using std::swap;
    using std::find;
    using std::set;

    struct vertex_info
    {
        point<T, 2> point;
        size_t obstacle_index;
        weak_ptr<vertex_info> prev_vertex;
        weak_ptr<vertex_info> next_vertex;
        size_t graph_vertex_index;
    };

    std::vector<shared_ptr<vertex_info>> G;

    for (size_t obstacle_index = 0; obstacle_index < obstacles.size(); obstacle_index++)
    {
        std::vector<shared_ptr<vertex_info>> obstacle_vertices;
        auto& obstacle = obstacles[obstacle_index];
        for (auto& vertex : obstacle)
        {
            obstacle_vertices.push_back(

                shared_ptr<vertex_info>(new vertex_info{
                    vertex,
                    obstacle_index,
                    weak_ptr<vertex_info>(),
                    weak_ptr<vertex_info>(),
                    0
                    })
            );
        }

        for (size_t vertex_index = 0; vertex_index < obstacle.size(); vertex_index++)
        {
            auto next_vertex_index = (vertex_index + 1) % obstacle.size();
            obstacle_vertices[vertex_index]->next_vertex = obstacle_vertices[next_vertex_index];
            obstacle_vertices[next_vertex_index]->prev_vertex = obstacle_vertices[vertex_index];
        }
        G.insert(G.end(), obstacle_vertices.begin(), obstacle_vertices.end());
    }

    for (size_t i = 0; i < G.size(); i++)
    {
        G[i]->graph_vertex_index = i;
    }

    visibility_graph<shared_ptr<vertex_info>> VG{ G.begin(), G.end() };

    if (VG.vertices().empty())
    {
        return VG;
    }

    auto visible_vertices = [&](const auto& p)
    {
        ray<T, 2> rotational_sweepline{ p->point, point<T,2>{ {p->point[0] + T(1),p->point[1]} } };
        rotational_sweepline[1] += rotational_sweepline.unit_creating_vector() * 10000;

        auto sorting_comparator = [&](const auto& l, const auto& r)
        {
            auto pl = line<T, 2>(p->point, l->point);
            auto pr = line<T, 2>(p->point, r->point);

            auto ltox = get_side_of_line(rotational_sweepline, l->point);
            auto rtox = get_side_of_line(rotational_sweepline, r->point);
            auto rtol = get_side_of_line(pl, r->point);

            auto ldist = (l->point - p->point).length();
            auto rdist = (r->point - p->point).length();

            if (ltox == side_of_line::right)
            {
                if (rtol == side_of_line::left && rtox == side_of_line::right)
                {
                    return false;
                }

                if (rtol == side_of_line::left && rtox == side_of_line::on)
                {
                    return false;
                }

                if (rtol == side_of_line::on && rtox == side_of_line::right)
                {
                    return ldist < rdist;
                }

                if (rtol == rtox)
                {
                    return true;
                }

                if (rtol == side_of_line::right && rtox == side_of_line::on)
                {
                    return true;
                }

                if (rtol == side_of_line::right && rtox == side_of_line::left)
                {
                    return true;
                }

                if (rtol == side_of_line::on && rtox == side_of_line::left)
                {
                    return true;
                }
            }

            if (ltox == side_of_line::left)
            {
                if (rtol == side_of_line::on && rtox == side_of_line::right)
                {
                    return false;
                }

                if (rtol == side_of_line::left && rtox == side_of_line::right)
                {
                    return false;
                }

                if (rtol == side_of_line::left && rtox == side_of_line::on)
                {
                    return false;
                }

                if (rtol == side_of_line::right && rtox == side_of_line::on)
                {
                    return false;
                }

                if (rtol == side_of_line::right && rtox == side_of_line::left)
                {
                    return true;
                }

                if (rtol == side_of_line::on && rtox == side_of_line::left)
                {
                    return ldist < rdist;
                }

                if (rtol == rtox)
                {
                    return false;
                }
            }

            if (ltox == side_of_line::on)
            {
                if (pl.unit_creating_vector()[0] > 0)
                {
                    if (rtol == side_of_line::on && rtox == side_of_line::on)
                    {
                        return ldist < rdist;
                    }

                    return true;
                }
                else
                {
                    if (rtol == side_of_line::on && rtox == side_of_line::on)
                    {
                        return ldist < rdist;
                    }

                    if (rtox == side_of_line::right)
                    {
                        return false;
                    }

                    return true;
                }
            }
        };

        swap(*find(G.begin(), G.end(), p), *G.rbegin());
        sort(G.begin(), G.end() - 1, sorting_comparator);

        auto tree_comparator = [&](const auto& l, const auto& r)
        {
            if (l == r)
            {
                return false;
            }

            auto intersectl = intersection(rotational_sweepline, l);
            auto intersectr = intersection(rotational_sweepline, r);

            size_t on_sweep = 0;

            if (get_side_of_line(rotational_sweepline, l[0]) == side_of_line::on ||
                get_side_of_line(rotational_sweepline, l[1]) == side_of_line::on ||
                !intersectl)
            {
                on_sweep++;

                if (!intersectl)
                {
                    if (get_side_of_line(rotational_sweepline, l[0]) == side_of_line::on)
                    {
                        intersectl = l[0];
                    }
                    else
                    {
                        intersectl = l[1];
                    }
                }
            }

            if (get_side_of_line(rotational_sweepline, r[0]) == side_of_line::on ||
                get_side_of_line(rotational_sweepline, r[1]) == side_of_line::on ||
                !intersectr)
            {
                on_sweep++;

                if (!intersectr)
                {
                    if (get_side_of_line(rotational_sweepline, r[0]) == side_of_line::on)
                    {
                        intersectr = r[0];
                    }
                    else
                    {
                        intersectr = r[1];
                    }
                }
            }

            if (on_sweep == 2)
            {
                auto same_pointl = l[1];
                auto outpointl = l[0];
                if (get_side_of_line(rotational_sweepline, l[0]) == side_of_line::on)
                {
                    outpointl = l[1];
                    same_pointl = l[0];
                }

                auto same_pointr = r[1];
                auto outpointr = r[0];
                if (get_side_of_line(rotational_sweepline, r[0]) == side_of_line::on)
                {
                    outpointr = r[1];
                    same_pointr = r[0];
                }

                if (same_pointl == same_pointr)
                {
                    return get_side_of_line(line<T, 2>(same_pointl, outpointr), outpointl) == side_of_line::right;
                }
            }

            auto vl = *intersectl - p->point;
            auto vr = *intersectr - p->point;

            auto distl = vl.dot(vl);
            auto distr = vr.dot(vr);

            return distl < distr;
        };

        set<segment<T, 2>, decltype(tree_comparator)> open_edges = set<segment<T, 2>, decltype(tree_comparator)>(tree_comparator);

        for (auto& obstacle : obstacles)
        {
            for (size_t vertex_index = 0; vertex_index < obstacle.size(); vertex_index++)
            {
                auto& p1 = obstacle[vertex_index];
                auto& p2 = obstacle[(vertex_index + 1) % obstacle.size()];
                segment<T, 2> edge{ p1,p2 };

                if (auto intersection_point = intersection(rotational_sweepline, edge))
                {
                    if (*intersection_point != p->point)
                    {
                        if (get_side_of_line(rotational_sweepline, p1) == side_of_line::on)
                        {
                            if (get_side_of_line(rotational_sweepline, p2) == side_of_line::left)
                            {
                                open_edges.insert(edge);
                            }
                        }
                        else if (get_side_of_line(rotational_sweepline, p2) == side_of_line::on)
                        {
                            if (get_side_of_line(rotational_sweepline, p1) == side_of_line::left)
                            {
                                open_edges.insert(segment<T, 2>{p2, p1});
                            }
                        }
                        else if (get_side_of_line(rotational_sweepline, p1) != side_of_line::right)
                        {
                            open_edges.insert(edge);
                        }
                        else
                        {
                            open_edges.insert(segment<T, 2>{p2, p1});
                        }
                    }
                }
            }
        }

        std::vector<shared_ptr<vertex_info>> W;

        shared_ptr<vertex_info> prev = nullptr;
        bool prev_visible = false;

        for (size_t i = 0; i < G.size(); i++)
        {
            auto& w = G[i];

            rotational_sweepline[1] = w->point;
            rotational_sweepline[1] += rotational_sweepline.unit_creating_vector() * 10000;
            
            auto& prevv = w->prev_vertex.lock()->point;
            auto& nextv = w->next_vertex.lock()->point;

            if (get_side_of_line(rotational_sweepline, prevv) == side_of_line::left)
            {
                if (!open_edges.empty())
                {
                    auto seg = segment<T, 2>{ prevv, w->point };
                    if (open_edges.erase(seg) == 0)
                    {
                        auto it = std::find(open_edges.begin(), open_edges.end(), seg);
                        if (it != open_edges.end())
                        {
                            open_edges.erase(it);
                        }
                    }
                }
            }

            if (get_side_of_line(rotational_sweepline, nextv) == side_of_line::left)
            {
                if (!open_edges.empty())
                {
                    auto seg = segment<T, 2>{ nextv, w->point };
                    if (open_edges.erase(seg) == 0)
                    {
                        auto it = std::find(open_edges.begin(), open_edges.end(), seg);
                        if (it != open_edges.end())
                        {
                            open_edges.erase(it);
                        }
                    }
                }
            }

            auto visible = [&](const auto& w) {
                auto pw = segment<T, 2>(p->point, w->point);
                auto p1s = pw[0] + pw.unit_creating_vector() * 0.01;
                auto p2s = pw[1] - pw.unit_creating_vector() * 0.01;

                if (intersects_interior(pw, obstacles[w->obstacle_index]) ||
                    contains(obstacles[w->obstacle_index], p1s) == polygon_point_classification::inside ||
                    contains(obstacles[w->obstacle_index], p2s) == polygon_point_classification::inside)
                {
                    return false;
                }

                if (!prev || !contains(pw, prev->point))
                {
                    if (open_edges.empty())
                    {
                        return true;
                    }
                    else
                    {
                        auto intpt = intersection(*open_edges.begin(), pw);

                        if (!intpt || *intpt == w->point)
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                }

                if (!prev_visible)
                {
                    return false;
                }

                auto lb_v = segment<T, 2>(prev->point, prev->point +
                    vector<T, 2>({ rotational_sweepline[1][1], -rotational_sweepline[1][0] }));
                auto ub_v = segment<T, 2>(w->point, w->point +
                    vector<T, 2>({ rotational_sweepline[1][1], -rotational_sweepline[1][0] }));

                auto lb = open_edges.lower_bound(lb_v);
                auto ub = open_edges.upper_bound(ub_v);

                if (lb != ub)
                {
                    if ((*lb)[0] == prev->point || (*lb)[1] == prev->point)
                    {
                        lb++;
                    }
                }

                return lb != ub;
            };
            bool w_visible = false;

            if (visible(w))
            {
                w_visible = true;
                W.push_back(w);
            }

            if (get_side_of_line(rotational_sweepline, nextv) == side_of_line::right)
            {
                open_edges.insert(segment<T, 2>{w->point, nextv});
            }

            if (get_side_of_line(rotational_sweepline, prevv) == side_of_line::right)
            {
                open_edges.insert(segment<T, 2>{w->point, prevv});
            }

            prev = w;
            prev_visible = w_visible;
        }

        return W;
    };

    for (auto& v : VG.vertices())
    {
        auto W = visible_vertices(v.data());

        for (auto& w : W)
        {
            VG.add_directed_edge(v.id(), w->graph_vertex_index);
        }
    }

    return VG;
}