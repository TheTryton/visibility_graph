#pragma once

#include <vector>
#include <algorithm>
#include <utility>

/* 
\class graph
\brief graph representation class

graph representation class implemented with adjacency list and with vertex (class V) and edge (class E) customizable data

*/
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