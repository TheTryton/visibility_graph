#pragma once

#include <functions.hpp>

#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <memory>

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
        vector<size_t> vids(vertices_count);
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
    float dist_to_p;
    bool operator<(const obstacle_edge& other) const
    {
        return dist_to_p < other.dist_to_p;
    }
    bool operator==(const obstacle_edge& other) const
    {
        return edge < other.edge;
    }
};

template<class T>
using vertex_data_sptr = std::shared_ptr<vertex_data<T>>;

template<class T>
using vertex_data_wptr = std::weak_ptr<vertex_data<T>>;

template<class T>
bool visible(const vertex_data<T>& w, const segment<T, 2>& pw, const vertex_data<T>* wm1, bool wm1vis, const std::set<obstacle_edge<T>>& Tree)
{
    auto& obstacle = *w.poly;
    if (intersects_interior(pw, obstacle))
    {
        return false;
    }
    else if (wm1 && contains(pw, wm1->p))
    {
        if (!Tree.empty())
        {
            auto& e = *Tree.begin();
            if (intersection(e.edge, pw))
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    else if (!wm1vis)
    {
        return false;
    }
    else
    {
        auto wm1w = segment<T, 2>(wm1->p, w.p);
        for (auto& e : Tree)
        {
            if (intersection(e.edge, wm1w))
            {
                return false;
            }
        }

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
    std::vector<std::pair<vertex_data_sptr<T>, size_t>> obstacle_vertices{vgraph.vertices().size()};

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

    point<T, 2> z{ {0,0} };
    ray<T,2> rotational_sweepline = ray<T, 2>(xline + (p.data()->p - z));

    std::set<obstacle_edge<T>> Tree;
    for (auto& obstacle : obstacles)
    {
        for (size_t i = 0; i < obstacle.size(); i++)
        {
            auto& p1 = obstacle[i];
            auto& p2 = obstacle[(i+1) % obstacle.size()];
            segment<T, 2> edge{ p1,p2 };

            auto insersection_point_opt = intersection(rotational_sweepline, edge);
            if (insersection_point_opt)
            {
                auto& insersection_point = *insersection_point_opt;

                Tree.insert(obstacle_edge<T>
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
        auto pw = segment<T, 2>(p.data()->p, wi.first->p);
        if (visible(*wi.first, pw, wm1, wm1vis, Tree))
        {
            wm1vis = true;
            W.push_back(wi.second);

            rotational_sweepline = ray<T, 2>(pw);

            auto nextedge = segment<T, 2>(wi.first->p, wi.first->next.lock()->p);
            auto prevedge = segment<T, 2>(wi.first->prev.lock()->p, wi.first->p);

            if (get_side_of_line(rotational_sweepline, nextedge[1]) == side_of_line::right)
            {
                Tree.insert(
                    obstacle_edge<T>
                    {
                        nextedge,
                        (nextedge[1] - p.data()->p).length()
                    }
                );
            }
            else
            {
                Tree.erase(obstacle_edge<T>
                    {
                        nextedge,
                        (nextedge[1] - p.data()->p).length()
                    }
                );
            }

            if (get_side_of_line(rotational_sweepline, prevedge[0]) == side_of_line::right)
            {
                Tree.insert(
                    obstacle_edge<T>
                    {
                        prevedge,
                        (prevedge[0] - p.data()->p).length()
                    }
                );
            }
            else
            {
                Tree.erase(obstacle_edge<T>
                    {
                        prevedge,
                        (prevedge[0] - p.data()->p).length()
                    }
                );
            }
        }
        else
        {
            wm1vis = false;
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
                new vertex_data<T>{ vertex, & obstacle, std::weak_ptr<vertex_data<T>>(), std::weak_ptr<vertex_data<T>>() }
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