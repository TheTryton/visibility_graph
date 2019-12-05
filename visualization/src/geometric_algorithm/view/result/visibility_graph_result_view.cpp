#include <geometric_algorithm/view/result/visibility_graph_result_view.hpp>
#include <geometric_algorithm/scene/element/point.hpp>
#include <geometric_algorithm/scene/element/segment.hpp>
#include <geometric_algorithm/scene/element/polygon.hpp>
#include <math/math.hpp>
#include <visibility_graph.hpp>
#include "visibility_graph_result_view.hpp"

using namespace math;

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

    auto vgraph = compute_visibility_graph(polys);
    QList<QLineF> lines;
    QList<QPointF> points;

    for (auto& v : vgraph.vertices())
    {
        points.append(QPointF(v.data()->point[0], v.data()->point[1]));
        for (auto& e : v.outgoing_edges())
        {
            auto& p1 = v.data();
            auto& p2 = vgraph.vertices()[e.to()].data();
            lines.append(QLineF(QPointF(p1->point[0],p1->point[1]), QPointF(p2->point[0], p2->point[1])));
        }
    }

    points_collection pc;
    pc.points = points;
    pc.color = QColor(0, 0, 255);

    lines_collection lc;
    lc.lines = lines;
    lc.color = QColor(0, 0, 255);
    lc.width = 3.0f;

    polygons_collection plc;
    plc.polygons = QList<QPolygonF>::fromVector(QVector<QPolygonF>::fromStdVector(obstacles));
    plc.color = QColor(255, 0, 0);

    auto scene = algorithm_scene(
        { pc },
        { lc },
        { plc }
    );

    scene.set_scene(this);
}

void visibility_graph_result_view::set_obstacles(const std::vector<QPolygonF>& obstacles)
{
    _obstacles = obstacles;

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

    auto vgraph = compute_visibility_graph(polys);
    QList<QLineF> lines;
    QList<QPointF> points;

    for (auto& v : vgraph.vertices())
    {
        points.append(QPointF(v.data()->point[0], v.data()->point[1]));
        for (auto& e : v.outgoing_edges())
        {
            auto& p1 = v.data();
            auto& p2 = vgraph.vertices()[e.to()].data();
            lines.append(QLineF(QPointF(p1->point[0], p1->point[1]), QPointF(p2->point[0], p2->point[1])));
        }
    }

    points_collection pc;
    pc.points = points;
    pc.color = QColor(0, 0, 255);

    lines_collection lc;
    lc.lines = lines;
    lc.color = QColor(0, 0, 255);
    lc.width = 3.0f;

    polygons_collection plc;
    plc.polygons = QList<QPolygonF>::fromVector(QVector<QPolygonF>::fromStdVector(obstacles));
    plc.color = QColor(255, 0, 0);

    auto scene = algorithm_scene(
        { pc },
        { lc },
        { plc }
    );

    scene.set_scene(this);
}
