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
struct vertex_data
{
    point<T, 2> p;
    const polygon<T>* poly;
    std::weak_ptr<vertex_data<T>> next;
    std::weak_ptr<vertex_data<T>> prev;
};

template<class T>
struct obstacle_edge
{
    segment<T, 2> edge;
    T dist_to_p;
    //bool operator<(const obstacle_edge& other) const
    //{
    //    return dist_to_p < other.dist_to_p;
    //}
    bool operator==(const obstacle_edge& other)
    {
        return edge == other.edge;
    }
    friend std::ostream& operator << (std::ostream& out, const obstacle_edge& e)
    {
        out << e.edge.p1() << " " << e.edge.p2();
        return out;

    }
};

template<class T>
using vertex_data_sptr = std::shared_ptr<vertex_data<T>>;

template<class T>
using vertex_data_wptr = std::weak_ptr<vertex_data<T>>;



template<class T>
floating distance_from_p(point<T, 2> p, point<T, 2> w)
{
    return floating((p - w).length());
}

template<class T>
bool visible(const vertex_data<T>& w, const segment<T, 2>& pw, const vertex_data<T>* wm1, bool wm1vis, skip_list<obstacle_edge<T>, floating>& edges_list)
{
    auto& obstacle = *w.poly;
    if (intersects_interior(pw, obstacle))
    {
        return false;
    }
    else if (wm1 && !contains(pw, wm1->p))
    {
        if (!edges_list.empty())
        {
            auto& e = edges_list.begin();
            if (e)
            {
                if (intersection((*e).edge, pw))
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
    }
    else if (!wm1vis)
    {
        return false;
    }
    else
    {
        auto p = pw.p1();
        auto wm1w = segment<T, 2>(wm1->p, w.p);
        if (edges_list.is_something_between(distance_from_p(p, wm1->p), distance_from_p(p, w.p)))return false;

        return true;
    }
}

template<class T>
T clockwise_angle(const point<T, 2>& p1, const point<T, 2>& p2, const point<T, 2>& p3)
{
    T dirp2p1 = std::atan2(p1[1] - p2[1], p1[0] - p2[0]);
    T dirp2p3 = std::atan2(p3[1] - p2[1], p3[0] - p2[0]);
    T anglep1p2p3 = dirp2p1 - dirp2p3;

    const T pi = std::acos(T(-1));
    if (anglep1p2p3 > pi) anglep1p2p3 -= 2 * pi;
    else if (anglep1p2p3 < -pi) anglep1p2p3 += 2 * pi;

    return anglep1p2p3 + pi;
}

template<class T, class VT>
auto visible_vertices(const VT& p, const std::vector<polygon<T>>& obstacles, const visibility_graph<vertex_data_sptr<T>>& vgraph)
{
    std::vector<std::pair<vertex_data_sptr<T>, size_t>> obstacle_vertices{ vgraph.vertices().size() };

    std::transform(vgraph.vertices().begin(), vgraph.vertices().end(), obstacle_vertices.begin(), [](const auto& v) {
        return std::make_pair(v.data(), v.id());
        });

    auto xline = line<T, 2>(point<T, 2>({ 0,0 }), point<T, 2>({ 1,0 }));
    auto compare_f = [&p, &xline](const std::pair<vertex_data_sptr<T>, size_t>& lp, const std::pair<vertex_data_sptr<T>, size_t>& rp)
    {
        auto& l = *lp.first;
        auto& r = *rp.first;

        auto pl = line<T, 2>(p.data()->p, l.p);
        auto pr = line<T, 2>(p.data()->p, r.p);

        auto ltox = get_side_of_line(xline, l.p);
        auto rtox = get_side_of_line(xline, r.p);
        auto rtol = get_side_of_line(pl, r.p);

        auto ldist = (l.p - p.data()->p).length();
        auto rdist = (r.p - p.data()->p).length();

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

    std::sort(obstacle_vertices.begin(), obstacle_vertices.end(), compare_f);

    point<T, 2> z{ {1,0} };
    ray<T, 2> rotational_sweepline = ray<T, 2>(xline + (p.data()->p - z));


    skip_list<obstacle_edge<T>, floating> edges_list([&rotational_sweepline](const obstacle_edge<T>& edge)->floating
        {
            line<T, 2> l = line<T, 2>(edge.edge.p1(), edge.edge.p2());
            std::optional<point<T, 2>> intersection_point = intersection(rotational_sweepline, l);
            //vector<T, 2> epsv = edge.edge.unit_creating_vector()*(std::numeric_limits<T>::epsilon()*1);
            if (intersection_point) return distance_from_p<T>(rotational_sweepline.p1(), *intersection_point);
            //throw new std::exception("Ray does not intersect given edge.");


        }
    );

    for (auto& obstacle : obstacles)
    {
        for (size_t i = 0; i < obstacle.size(); i++)
        {
            auto& p1 = obstacle[i];
            auto& p2 = obstacle[(i + 1) % obstacle.size()];
            segment<T, 2> edge{ p1,p2 };

            auto insersection_point_opt = intersection(rotational_sweepline, edge);
            if (insersection_point_opt)
            {
                auto& insersection_point = *insersection_point_opt;

                edges_list.insert(obstacle_edge<T>
                {
                    edge,
                        (p.data()->p - insersection_point).length()
                }
                );
            }

        }
    }

    using vertex_type = visibility_graph<vertex_data<T>>::vertex;

    std::vector<size_t> W;
    vertex_data<T>* wm1 = nullptr;
    bool wm1vis = false;
    for (size_t i = 0; i < obstacle_vertices.size(); i++)
    {

        auto& wi = obstacle_vertices[i];
        if (wi.first->p == p.data()->p)continue;
        auto pw = segment<T, 2>(p.data()->p, wi.first->p);
        rotational_sweepline = ray<T, 2>(pw);
        if (visible(*wi.first, pw, wm1, wm1vis, edges_list))
        {
            wm1vis = true;
            W.push_back(wi.second);
        }
        else
        {
            wm1vis = false;
        }

        auto nextedge = segment<T, 2>(wi.first->p, wi.first->next.lock()->p);
        auto prevedge = segment<T, 2>(wi.first->prev.lock()->p, wi.first->p);



        rotational_sweepline.rotate_clockwisely(epsilon<T>);

        if (get_side_of_line(rotational_sweepline, nextedge[1]) == side_of_line::left)
        {
            edges_list.remove(obstacle_edge<T>
            {
                nextedge,
                    (nextedge[0] - p.data()->p).length()
            }
            );
        }


        if (get_side_of_line(rotational_sweepline, prevedge[0]) == side_of_line::left)
        {
            edges_list.remove(obstacle_edge<T>
            {
                prevedge,
                    (prevedge[1] - p.data()->p).length()
            }
            );
        }



        rotational_sweepline.rotate_clockwisely(-2 * epsilon<T>);

        if (get_side_of_line(rotational_sweepline, nextedge[1]) == side_of_line::right)
        {
            edges_list.insert(
                obstacle_edge<T>
            {
                nextedge,
                    (nextedge[0] - p.data()->p).length()
            }
            );
        }



        if (get_side_of_line(rotational_sweepline, prevedge[0]) == side_of_line::right)
        {
            edges_list.insert(
                obstacle_edge<T>
            {
                prevedge,
                    (prevedge[1] - p.data()->p).length()
            }
            );
        }

        wm1 = wi.first.get();
    }
    return W;
}

template<class T>
auto compute_visibility_graph(const std::vector<polygon<T>>& obstacles)
{
    std::vector<vertex_data_sptr<T>> all_vertices;
    for (auto& obstacle : obstacles)
    {
        std::vector<vertex_data_sptr<T>> polygon_vertices;
        for (size_t i = 0; i < obstacle.size(); i++)
        {
            auto& vertex = obstacle[i];
            polygon_vertices.emplace_back(
                new vertex_data<T>{ vertex, &obstacle, std::weak_ptr<vertex_data<T>>(), std::weak_ptr<vertex_data<T>>() }
            );
        }

        for (size_t i = 1; i < polygon_vertices.size() + 1; i++)
        {
            polygon_vertices[(i + 1) % polygon_vertices.size()]->prev = polygon_vertices[i % polygon_vertices.size()];
            polygon_vertices[(i + 1) % polygon_vertices.size()]->next = polygon_vertices[(i + 2) % polygon_vertices.size()];
        }

        all_vertices.insert(all_vertices.begin(), polygon_vertices.begin(), polygon_vertices.end());
    }

    visibility_graph<vertex_data_sptr<T>> vgraph{ all_vertices.begin(), all_vertices.end() };


    for (auto& v : vgraph.vertices())
    {
        auto W = visible_vertices(v, obstacles, vgraph);
        for (auto& w : W)
        {
            vgraph.add_directed_edge(v.id(), w);
        }
    }

    return vgraph;
}

template<class T>
auto create_visibility_graph(const std::vector<polygon<T>>& obstacles)
{
    using std::shared_ptr;
    using std::weak_ptr;
    using std::make_shared;

    struct vertex_info
    {
        point<T, 2> point;
        size_t obstacle_index;
        weak_ptr<vertex_info> prev_vertex;
        weak_ptr<vertex_info> next_vertex;
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
                    weak_ptr<vertex_info>()
                })
            );
        }

        for (size_t vertex_index = 0; vertex_index < obstacle.size(); vertex_index++)
        {
            auto prev_vertex_index = vertex_index == 0 ? obstacle.size() - 1 : vertex_index - 1;
            auto next_vertex_index = (vertex_index + 1) % obstacle.size();
            obstacle_vertices[vertex_index]->prev_vertex = obstacle_vertices[prev_vertex_index];
            obstacle_vertices[vertex_index]->next_vertex = obstacle_vertices[next_vertex_index];
        }
        G.insert(G.begin(), obstacle_vertices.begin(), obstacle_vertices.end());
    }

    visibility_graph<shared_ptr<vertex_info>> VG{ G.begin(), G.end()};

    auto angle = [](const auto& center, const auto& point) -> T {
        auto dx = point[0] - center[0];
        auto dy = point[1] - center[1];
        if (std::abs(dx) < epsilon<T>)
        {
            if (dy < 0)
            {
                return M_PI * 3 / 2;
            }
            else
            {
                return M_PI / 2;
            }
        }
        if (std::abs(dy) < epsilon<T>)
        {
            if (dx < 0)
            {
                return M_PI;
            }
            else
            {
                return 0;
            }
        }
        if (dx < 0)
        {
            return M_PI + atan(dy / dx);
        }
        if (dy < 0)
        {
            return 2 * M_PI + atan(dy / dx);
        }
        return atan(dy / dx);
    };

    auto visible_vertices = [&](const auto& p)
    {
        using std::sort;

        sort(G.begin(), G.end(), [&](const auto& l, const auto& r) {
            auto anglel = angle(p->point, l->point);
            auto angler = angle(p->point, r->point);
            if (anglel < angler) return true;
            if (anglel > angler) return false;
            return (l->point - p->point).length() < (r->point - p->point).length();
            });

        ray<T, 2> rotational_sweepline{ point<T,2>{ {0,0} }, point<T,2>{ {std::numeric_limits<T>::max(),0} } };

        auto tree_comparator = [&](const auto& l, const auto& r)
        {
            auto intersectl = intersection(rotational_sweepline, l);
            auto intersectr = intersection(rotational_sweepline, r);
            auto shifted_intersectl = *intersectl;// +l.unit_creating_vector() * epsilon<T>;
            auto shifted_intersectr = *intersectr;// +r.unit_creating_vector() * epsilon<T>;
            return (shifted_intersectl - p->point).length() < (shifted_intersectr - p->point).length();
        };

        using std::set;

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
                    if (get_side_of_line(rotational_sweepline, p1) != side_of_line::right)
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

        using std::pair;
        std::vector<pair<shared_ptr<vertex_info>,size_t>> W;

        for (size_t i = 0; i < VG.vertices().size(); i++)
        {
            auto& w = VG.vertices()[i];
            if (p->point == w.data()->point)
                continue;
            rotational_sweepline = ray<T, 2>(p->point, w.data()->point);

            auto visible = [&](const auto& w) {
                auto pw = segment<T, 2>(p->point, w.data()->point);
                if (open_edges.empty())
                {
                    return true;
                }
                else if (!intersection(*open_edges.begin(), pw))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            };
            if (visible(w))
            {
                W.push_back(std::make_pair(w.data(),i));
            }

            auto& prevv = w.data()->prev_vertex.lock()->point;
            auto& nextv = w.data()->next_vertex.lock()->point;

            if (get_side_of_line(rotational_sweepline, nextv) == side_of_line::right)
            {
                open_edges.insert(segment<T, 2>{w.data()->point, nextv});
            }

            if (get_side_of_line(rotational_sweepline, prevv) == side_of_line::right)
            {
                open_edges.insert(segment<T, 2>{w.data()->point, prevv});
            }

            if (get_side_of_line(rotational_sweepline, nextv) == side_of_line::left)
            {
                open_edges.erase(segment<T, 2>{nextv, w.data()->point});
            }

            if (get_side_of_line(rotational_sweepline, prevv) == side_of_line::left)
            {
                open_edges.erase(segment<T, 2>{prevv, w.data()->point});
            }
        }

        return W;
    };

    for (auto& v : VG.vertices())
    {
        auto W = visible_vertices(v.data());
        for (auto& w : W)
        {
            VG.add_directed_edge(v.id(), w.second);
        }
    }

    return VG;
}
