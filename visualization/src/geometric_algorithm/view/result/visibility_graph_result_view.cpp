#include <geometric_algorithm/view/result/visibility_graph_result_view.hpp>
#include <geometric_algorithm/scene/scene.hpp>
#include <geometric_algorithm/scene/element/point.hpp>
#include <geometric_algorithm/scene/element/segment.hpp>
#include <geometric_algorithm/scene/element/polygon.hpp>
#include <math/math.hpp>
#include <visibility_graph.hpp>

using namespace math;

visibility_graph<QPointF> construct_visibility_graph(const std::vector<QPolygonF>& polygons)
{
    size_t vertices_count = std::accumulate(polygons.begin(), polygons.end(), size_t(0), [](size_t l, const QPolygonF& r) {return l + r.size(); });
    std::vector<QPointF> vertices(vertices_count);
    auto itv = vertices.begin();
    for (auto& polygon : polygons)
    {
        itv = std::copy(polygon.begin(), polygon.end(), itv);
    }
    std::vector<QLineF> edges(vertices_count);
    auto ite = edges.begin();
    for (auto& polygon : polygons)
    {
        for (size_t i = 0; i < polygon.size() - 1; i++)
        {
            *ite++ = QLineF(polygon[i], polygon[i + 1]);
        }
        *ite++ = QLineF(polygon.back(), polygon.front());
    }

    visibility_graph<QPointF> vgraph = visibility_graph<QPointF>(vertices.begin(), vertices.end());

    for (auto& v : vgraph.vertices())
    {
        for (auto& u : vgraph.vertices())
        {
            if (v == u)
                continue;

            auto visibility_ray = ray<qreal, 2>(point<qreal, 2>({ v.data().x(), v.data().y() }), point<qreal, 2>({ u.data().x(), u.data().y() }));

            QList<QPointF> intersection_points;

            for (auto& edge : edges)
            {
                auto e = segment<qreal, 2>(point<qreal, 2>({ edge.p1().x(), edge.p1().y() }), point<qreal, 2>({ edge.p2().x(), edge.p2().y() }));
                if (auto p = intersection(visibility_ray, e))
                {
                    intersection_points.append(QPointF((*p)[0], (*p)[1]));
                }
            }

            std::sort(intersection_points.begin(), intersection_points.end(), [v](const QPointF& p1, const QPointF& p2) {return (p1 - v.data()).manhattanLength() < (p2 - v.data()).manhattanLength(); });
            intersection_points.erase(std::unique(intersection_points.begin(), intersection_points.end()), intersection_points.end());
            intersection_points.pop_front();

            if (!intersection_points.empty())
            {
                if (intersection_points.front() == u.data())
                {
                    vgraph.add_directed_edge(v.id(), u.id());
                }
            }
        }
    }

    return vgraph;
}

visibility_graph_result_view::visibility_graph_result_view(const std::vector<QPolygonF>& obstacles, QWidget* parent) :
    base_view(parent),
    _obstacles(obstacles)
{
    std::vector<polygon<qreal>> polys;
    for (auto& obstacle : _obstacles)
    {
        polygon<qreal> p;
        for (auto& v : obstacle)
        {
            p.push_back(point<qreal, 2>({ v.x(),v.y() }));
        }
        polys.push_back(p);
    }
    auto vgraph = create_visibility_graph(polys);
    QList<QLineF> lines;

    for (auto& v : vgraph.vertices())
    {
        for (auto& e : v.outgoing_edges())
        {
            auto& p1 = v.data();
            auto& p2 = vgraph.vertices()[e.to()].data();
            lines.append(QLineF(QPointF(p1->point[0],p1->point[1]), QPointF(p2->point[0], p2->point[1])));
        }
    }

    setScene(
        new algorithm_scene(
            { points_collection{ {},QColor() } },
            { lines_collection{ lines, QColor(0,0,255) } },
            { polygons_collection{ QList<QPolygonF>::fromVector(QVector<QPolygonF>::fromStdVector(obstacles)), QColor(255, 0, 0) } }
        )
    );
}
