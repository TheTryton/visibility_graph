#pragma once

#include <geometric_algorithm/view/base/base_view.hpp>
#include <geometric_algorithm/scene/scene.hpp>
#include <visibility_graph.hpp>

struct step
{
    algorithm_scene* scene;
    QString description;
};

class stepbystep_view : public QWidget
{
    Q_OBJECT
private:
    base_view* _step_view;
    QPushButton* _forward;
    QPushButton* _backward;
    QLabel* _step_description;
    QSlider* _slider;
    std::vector<step> _steps;
    size_t _step_index;
public:
    stepbystep_view(const std::vector<step>& steps, QWidget* parent = nullptr);
public:
    void set_steps(const std::vector<step>& steps);
};

template<class T>
auto compute_visibility_graph_visualized(const std::vector<polygon<T>>& obstacles)
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

    std::vector<step> steps;

    if (VG.vertices().empty())
    {
        return steps;
    }

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

        ray<T, 2> rotational_sweepline{ p->point, point<T,2>{ {p->point[0] + T(1),p->point[1]} } };
        rotational_sweepline[1] += rotational_sweepline.unit_creating_vector() * 10000;

        auto compare_f = [&](const auto& l, const auto& r)
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

        std::swap(*std::find(G.begin(), G.end(), p), *G.rbegin());
        sort(G.begin(), G.end() - 1, compare_f);

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

        for (size_t i = 0; i < G.size() - 1; i++)
        {
            auto& w = G[i];

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
                    QString("Edge side relative to sweepline: ") + rstext
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