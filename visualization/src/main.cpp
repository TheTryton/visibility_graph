#include <QtWidgets>
#include "visibility_graph.hpp"
#include <optional>
#include <numeric>
#include <iostream>
#include <stack>
#include <chrono>
#include <random>
#include "skip_list.hpp"

#include <geometric_algorithm/view/input/polygon_input_view.hpp>
#include <geometric_algorithm/view/result/visibility_graph_result_view.hpp>
#include <geometric_algorithm/view/stepbystep/stepbystep.hpp>

floating floating::max_value = floating(1000000000);
floating floating::min_value = floating(-100000000);

template<class T>
auto create_visibility_graph_visualized(const std::vector<polygon<T>>& obstacles)
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

    std::vector<step> steps;

    polygons_collection all_polys;
    all_polys.color = QColor(255, 0, 0);
    for (auto& obstacle : obstacles)
    {
        QPolygonF poly;
        for (auto& pt : obstacle)
        {
            poly.push_back(QPointF(pt[0], pt[1]));
        }
        all_polys.polygons.push_back(poly);
    }
    steps.push_back(
        step
        {
            new algorithm_scene({},{},{all_polys}),
            "S <- disjoint polygonal obstacles"
        }
    );
    points_collection all_pts;
    all_pts.color = QColor(0, 0, 255);
    std::transform(G.begin(), G.end(), std::back_inserter(all_pts.points), [](const auto& p) {return QPointF(p->point[0], p->point[1]); }); 
    steps.push_back(
        step
        {
            new algorithm_scene({all_pts},{},{all_polys}),
            "G <- all vertices of obstacles S"
        }
    );

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

        points_collection sorted_pts;
        sorted_pts.color = QColor(0, 0, 255);
        sorted_pts.indexed = true;
        std::transform(G.begin(), G.end(), std::back_inserter(sorted_pts.points), [](const auto& p) {return QPointF(p->point[0], p->point[1]); });
        points_collection current_point;
        current_point.color = QColor(255, 0, 255);
        current_point.points = { QPointF(p->point[0],p->point[1]) };
        steps.push_back(
            step
            {
                new algorithm_scene({sorted_pts, current_point},{},{all_polys}),
                "Sort vertices according to clockwise angle that ray pw makes with positive x-axis"
            }
        );

        ray<T, 2> rotational_sweepline{ p->point, point<T,2>{ {p->point[0] + T(1),p->point[1]} } };
        rotational_sweepline[1] += rotational_sweepline.unit_creating_vector() * 10000;

        auto angle2 = [](const auto& pa, const auto& pb, const auto& pc) -> T {
            auto a = pow(pc[0] - pb[0], T(2)) + pow(pc[1] - pb[1], T(2));
            auto b = pow(pc[0] - pa[0], T(2)) + pow(pc[1] - pa[1], T(2));
            auto c = pow(pb[0] - pa[0], T(2)) + pow(pb[1] - pa[1], T(2));
            auto cos_value = (a + b - c) / (2 * sqrt(a) * sqrt(c));
            return acos(int(cos_value * 1e10) / 1e10);
        };

        auto tree_comparator = [&](const auto& l, const auto& r)
        {
            auto intersectl = intersection(rotational_sweepline, l);
            auto intersectr = intersection(rotational_sweepline, r);

            size_t on_sweep = 0;

            if (get_side_of_line(rotational_sweepline, l[0]) == side_of_line::on ||
                get_side_of_line(rotational_sweepline, l[1]) == side_of_line::on ||
                !intersectl)
            {
                on_sweep++;

                if (get_side_of_line(rotational_sweepline, l[0]) == side_of_line::on)
                {
                    intersectl = l[0];
                }
                else
                {
                    intersectl = l[1];
                }
            }

            if (get_side_of_line(rotational_sweepline, r[0]) == side_of_line::on ||
                get_side_of_line(rotational_sweepline, r[1]) == side_of_line::on ||
                !intersectr)
            {
                on_sweep++;

                if (get_side_of_line(rotational_sweepline, r[0]) == side_of_line::on)
                {
                    intersectr = r[0];
                }
                else
                {
                    intersectr = r[1];
                }
            }

            if (on_sweep == 2)
            {
                auto same_point = l[1];
                auto outpointl = l[0];
                if (get_side_of_line(rotational_sweepline, l[0]) == side_of_line::on)
                {
                    outpointl = l[1];
                    same_point = l[0];
                }

                auto outpointr = r[0];
                if (get_side_of_line(rotational_sweepline, r[0]) == side_of_line::on)
                {
                    outpointr = r[1];
                }

                return get_side_of_line(line<T, 2>(same_point, outpointr), outpointl) == side_of_line::right;
            }

            auto vl = *intersectl - p->point;
            auto vr = *intersectr - p->point;

            auto distl = vl.dot(vl);
            auto distr = vr.dot(vr);

            return distl < distr;
        };

        class tree
        {
        private:
            std::vector<segment<T, 2>> edges;
        public:
            void insert(const ray<T, 2>& sweep_line, const segment<T, 2>& edge)
            {
                edges.insert(
                    std::upper_bound(edges.begin(), edges.end(), edge, [&](const auto& l, const auto& r) {
                        return less_than(sweep_line, l, r);
                        }),
                    edge);
            }
            void erase(const ray<T, 2>& sweep_line, const segment<T, 2>& edge)
            {
                auto i = std::lower_bound(edges.begin(), edges.end(), edge, [&](const auto& l, const auto& r) {
                    return less_than(sweep_line, l, r);
                    });
                if (*i == edge)
                {
                    edges.erase(i);
                }
            }
            auto begin() const
            {
                return edges.begin();
            }
            auto end() const
            {
                return edges.end();
            }
            bool empty() const
            {
                return edges.empty();
            }
            bool less_than(const ray<T, 2>& sweep_line, const segment<T, 2>& l, const segment<T, 2>& r)
            {
                if (l == r)
                {
                    return false;
                }

                auto intersectl = intersection(sweep_line, l);
                auto intersectr = intersection(sweep_line, r);

                if (*intersectl == *intersectr)
                {
                    auto same_point = l[0];
                    if (r[0] != same_point && r[1] != same_point)
                    {
                        same_point = l[1];
                    }

                    auto outpointl = l[0];
                    if (outpointl == same_point)
                    {
                        outpointl = l[1];
                    }

                    auto outpointr = r[0];
                    if (outpointr == same_point)
                    {
                        outpointr = r[1];
                    }

                    return get_side_of_line(line<T, 2>(*intersectr, outpointr), outpointl) == side_of_line::right;
                }

                auto vl = *intersectl - sweep_line[0];
                auto vr = *intersectr - sweep_line[0];

                auto distl = vl.dot(vl);
                auto distr = vr.dot(vr);

                return distl < distr;
            }
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
                    if (*intersection_point != p->point)
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
        }

        lines_collection sweepline;
        sweepline.color = QColor(255, 0, 255);
        sweepline.width = 2.0f;
        sweepline.lines = { QLineF(QPointF(rotational_sweepline[0][0], rotational_sweepline[0][1]), QPointF(rotational_sweepline[1][0], rotational_sweepline[1][1])) };

        lines_collection begin_edges;
        begin_edges.color = QColor(0, 0, 255);
        begin_edges.width = 3.0f;
        begin_edges.indexed = true;
        std::transform(open_edges.begin(), open_edges.end(), std::back_inserter(begin_edges.lines), [](const auto& e) {return QLineF(QPointF(e[0][0], e[0][1]), QPointF(e[1][0], e[1][1])); });
        steps.push_back(
            step
            {
                new algorithm_scene({sorted_pts, current_point},{sweepline, begin_edges},{all_polys}),
                "Insert intersecting edges into tree (ones that intersect positive x-axis)"
            }
        );

        using std::pair;
        std::vector<shared_ptr<vertex_info>> W;

        shared_ptr<vertex_info> prev = nullptr;
        bool prev_visible = false;

        for (size_t i = 0; i < G.size(); i++)
        {
            auto& w = G[i];
            if (p->point == w->point)
                continue;
            rotational_sweepline[1] = w->point;
            rotational_sweepline[1] += rotational_sweepline.unit_creating_vector() * 10000;
            sweepline.color = QColor(255, 0, 255);
            sweepline.width = 2.0f;
            sweepline.lines = { QLineF(QPointF(rotational_sweepline[0][0], rotational_sweepline[0][1]), QPointF(rotational_sweepline[1][0], rotational_sweepline[1][1])) };
            points_collection point_w;
            point_w.color = QColor(255, 255, 0);
            point_w.points = { QPointF(w->point[0],w->point[1]) };

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

                bool is_visible = false;

                if (p->obstacle_index != w->obstacle_index)
                {
                    if (intersects_interior(pw, obstacles[w->obstacle_index]))
                    {
                        return false;
                    }

                    if (intersects_interior(pw, obstacles[p->obstacle_index]))
                    {
                        return false;
                    }
                }
                else
                {
                    if (prevv != p->point && nextv != p->point)
                    {
                        auto p1s = pw[0] + pw.unit_creating_vector() * 0.01;
                        auto p2s = pw[1] - pw.unit_creating_vector() * 0.01;

                        if (intersects_interior(pw, obstacles[w->obstacle_index]) ||
                            contains(obstacles[w->obstacle_index], p1s) == polygon_point_classification::inside ||
                            contains(obstacles[w->obstacle_index], p2s) == polygon_point_classification::inside)
                        {
                            return false;
                        }
                    }
                }

                if (open_edges.empty())
                {
                    return true;
                }

                auto intpt = intersection(*open_edges.begin(), pw);

                if (!intpt || *intpt == w->point)
                {
                    return true;
                }

                return false;
            };
            bool w_visible = false;
            
            if (visible(w))
            {
                w_visible = true;
                W.push_back(w);
            }

            steps.push_back(
                step
                {
                    new algorithm_scene({sorted_pts, current_point, point_w},{sweepline, begin_edges},{all_polys}),
                    QString("Point w is ") + (w_visible ? "visible" : "not visible") + " from point p"
                }
            );

            points_collection compared_point;
            compared_point.color = QColor(0, 255, 0);
            compared_point.points = { QPointF(nextv[0],nextv[1]) };
            steps.push_back(
                step
                {
                    new algorithm_scene({sorted_pts, current_point, point_w, compared_point},{sweepline, begin_edges},{all_polys}),
                    "Checking first edge relative side to sweepline"
                }
            );

            auto result_side = get_side_of_line(rotational_sweepline, nextv);
            auto rstext = result_side == side_of_line::left ? "left" : (result_side == side_of_line::right) ? "right" : "on";
            steps.push_back(
                step
                {
                    new algorithm_scene({sorted_pts, current_point, point_w, compared_point},{sweepline, begin_edges},{all_polys}),
                    QString("Edge side relative to sweepline: ") + rstext
                }
            );

            if (get_side_of_line(rotational_sweepline, nextv) == side_of_line::right)
            {
                open_edges.insert(segment<T, 2>{w->point, nextv});
            }
            

            compared_point.color = QColor(0, 255, 0);
            compared_point.points = { QPointF(prevv[0],prevv[1]) };
            steps.push_back(
                step
                {
                    new algorithm_scene({sorted_pts, current_point, point_w, compared_point},{sweepline, begin_edges},{all_polys}),
                    "Checking second edge relative side to sweepline"
                }
            );

            result_side = get_side_of_line(rotational_sweepline, prevv);
            rstext = result_side == side_of_line::left ? "left" : (result_side == side_of_line::right) ? "right" : "on";
            steps.push_back(
                step
                {
                    new algorithm_scene({sorted_pts, current_point, point_w, compared_point},{sweepline, begin_edges},{all_polys}),
                    QString("Edge side relative to sweepline: ")+rstext
                }
            );

            if (get_side_of_line(rotational_sweepline, prevv) == side_of_line::right)
            {
                open_edges.insert(segment<T, 2>{w->point, prevv});
            }
            

            begin_edges.lines.clear();
            std::transform(open_edges.begin(), open_edges.end(), std::back_inserter(begin_edges.lines), [](const auto& e) {return QLineF(QPointF(e[0][0], e[0][1]), QPointF(e[1][0], e[1][1])); });
            
            steps.push_back(
                step
                {
                    new algorithm_scene({sorted_pts, current_point, point_w},{sweepline, begin_edges},{all_polys}),
                    "Insert delete on point"
                }
            );

            prev = w;
            prev_visible = w_visible;
        }

        return W;
    };

    for (auto& v : VG.vertices())
    {
        points_collection current_point;
        current_point.color = QColor(255, 0, 255);
        current_point.points = { QPointF(v.data()->point[0],v.data()->point[1]) };
        steps.push_back(
            step
            {
                new algorithm_scene({all_pts, current_point},{},{all_polys}),
                "foreach vertex p in VG retrieve all visible vertices"
            }
        );

        auto W = visible_vertices(v.data());

        for (auto& w : W)
        {
            VG.add_directed_edge(v.id(), w->graph_vertex_index);
        }

        lines_collection edges_W;
        edges_W.color = QColor(0, 0, 255);
        edges_W.width = 3.0f;
        std::transform(W.begin(), W.end(), std::back_inserter(edges_W.lines),
            [&](const auto& w) {
                return QLineF(
                    QPointF(v.data()->point[0], v.data()->point[1]), 
                    QPointF(VG.vertices()[w->graph_vertex_index].data()->point[0], VG.vertices()[w->graph_vertex_index].data()->point[1])
                );
            }
        );

        steps.push_back(
            step
            {
                new algorithm_scene({all_pts, current_point},{edges_W},{all_polys}),
                "All points visible from point p"
            }
        );

        lines_collection edges_all;
        edges_all.color = QColor(0, 0, 255);
        edges_all.width = 3.0f;

        for (auto& v : VG.vertices())
        {
            for (auto& e : v.outgoing_edges())
            {
                auto& p1 = v.data();
                auto& p2 = VG.vertices()[e.to()].data();
                edges_all.lines.append(QLineF(QPointF(p1->point[0], p1->point[1]), QPointF(p2->point[0], p2->point[1])));
            }
        }

        steps.push_back(
            step
            {
                new algorithm_scene({all_pts, current_point},{edges_all},{all_polys}),
                "Current state of visibility graph"
            }
        );
    }

    lines_collection edges_all;
    edges_all.color = QColor(0, 0, 255);
    edges_all.width = 3.0f;

    for (auto& v : VG.vertices())
    {
        for (auto& e : v.outgoing_edges())
        {
            auto& p1 = v.data();
            auto& p2 = VG.vertices()[e.to()].data();
            edges_all.lines.append(QLineF(QPointF(p1->point[0], p1->point[1]), QPointF(p2->point[0], p2->point[1])));
        }
    }

    steps.push_back(
        step
        {
            new algorithm_scene({all_pts},{edges_all},{all_polys}),
            "Visibility graph"
        }
    );

    return steps;
}

using namespace data_structures;
#include <iostream>
int main(int argc, char** argv)
{
    segment<float, 2> s1{ point<float,2>{ {2000,5000} },point<float,2>{ {0,0} } };
    segment<float, 2> s2{ point<float,2>{ {-253,-223} },point<float,2>{ {2.54,2.33} } };

    auto inter1 = intersection(s1, s2);

    std::cout << (*inter1)[0] << ' ' << (*inter1)[1] << std::endl;

    QApplication application(argc, argv);

    polygon_input_view view_input;
    view_input.show();
    application.exec();

    std::vector<QPolygonF> polygons;

    auto polys = view_input.get_polygons();
    for (auto& poly : polys)
    {
        polygons.push_back(poly);
    }
	
    std::vector<polygon<qreal>> obstacles;
    for (auto& obstacle : polygons)
    {
        polygon<qreal> p;
        for (auto& v : obstacle)
        {
            p.push_back(point<qreal, 2>({ v.x(),v.y() }));
        }
        obstacles.push_back(p);
    }

    stepbystep_view s{ create_visibility_graph_visualized(obstacles) };
    s.show();
	application.exec();

	return 0;
}