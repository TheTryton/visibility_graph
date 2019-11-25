#include <QtWidgets>
#include <visibility_graph.hpp>
#include <optional>
#include <numeric>
#include <iostream>
#include <stack>
#include <chrono>

std::optional<QPointF> intersect_line_line(const QLineF& line1, const QLineF& line2)
{
    auto p2p1 = line2.p1() - line1.p1();
    auto v1 = line1.p2() - line1.p1();
    auto v2 = line2.p2() - line2.p1();

    float W = v1.y() * v2.x() - v1.x() * v2.y();
    if (std::abs(W) < std::numeric_limits<float>::epsilon())
    {
        return std::nullopt;
    }
    float Wx = p2p1.y() * v2.x() - p2p1.x() * v2.y();
    float Wy = v1.y() * p2p1.x() - v1.x() * p2p1.y();

    float x = Wx / W;

    return v1 * x + line1.p1();
}

std::optional<QPointF> intersect_segment_segment(const QLineF& line1, const QLineF& line2)
{
    auto p2p1 = line2.p1() - line1.p1();
    auto v1 = line1.p2() - line1.p1();
    auto v2 = line2.p2() - line2.p1();

    float W = v1.y() * v2.x() - v1.x() * v2.y();
    if (std::abs(W) < std::numeric_limits<float>::epsilon())
    {
        return std::nullopt;
    }
    float Wx = p2p1.y() * v2.x() - p2p1.x() * v2.y();
    float Wy = v1.y() * p2p1.x() - v1.x() * p2p1.y();

    float x = Wx / W;
    float y = Wy / W;

    if (x < 0.0f || x > 1.0f)
    {
        return std::nullopt;
    }

    if (y < 0.0f || y > 1.0f)
    {
        return std::nullopt;
    }

    return v1 * x + line1.p1();
}

std::optional<QPointF> intersect_ray_ray(const QLineF& line1, const QLineF& line2)
{
    auto p2p1 = line2.p1() - line1.p1();
    auto v1 = line1.p2() - line1.p1();
    auto v2 = line2.p2() - line2.p1();

    float W = v1.y() * v2.x() - v1.x() * v2.y();
    if (std::abs(W) < std::numeric_limits<float>::epsilon())
    {
        return std::nullopt;
    }
    float Wx = p2p1.y() * v2.x() - p2p1.x() * v2.y();
    float Wy = v1.y() * p2p1.x() - v1.x() * p2p1.y();

    float x = Wx / W;
    float y = Wy / W;

    if (x < 0.0f)
    {
        return std::nullopt;
    }

    if (y < 0.0f)
    {
        return std::nullopt;
    }

    return v1 * x + line1.p1();
}

std::optional<QPointF> intersect_line_segment(const QLineF& line1, const QLineF& line2)
{
    auto p2p1 = line2.p1() - line1.p1();
    auto v1 = line1.p2() - line1.p1();
    auto v2 = line2.p2() - line2.p1();

    float W = v1.y() * v2.x() - v1.x() * v2.y();
    if (std::abs(W) < std::numeric_limits<float>::epsilon())
    {
        return std::nullopt;
    }
    float Wx = p2p1.y() * v2.x() - p2p1.x() * v2.y();
    float Wy = v1.y() * p2p1.x() - v1.x() * p2p1.y();

    float x = Wx / W;
    float y = Wy / W;

    if (y < 0.0f || y > 1.0f)
    {
        return std::nullopt;
    }

    return v1 * x + line1.p1();
}

std::optional<QPointF> intersect_ray_line(const QLineF& line1, const QLineF& line2)
{
    auto p2p1 = line2.p1() - line1.p1();
    auto v1 = line1.p2() - line1.p1();
    auto v2 = line2.p2() - line2.p1();

    float W = v1.y() * v2.x() - v1.x() * v2.y();
    if (std::abs(W) < std::numeric_limits<float>::epsilon())
    {
        return std::nullopt;
    }
    float Wx = p2p1.y() * v2.x() - p2p1.x() * v2.y();
    float Wy = v1.y() * p2p1.x() - v1.x() * p2p1.y();

    float x = Wx / W;

    if (x < 0.0f)
    {
        return std::nullopt;
    }

    return v1 * x + line1.p1();
}

std::optional<QPointF> intersect_ray_segment(const QLineF& line1, const QLineF& line2)
{
    auto p2p1 = line2.p1() - line1.p1();
    auto v1 = line1.p2() - line1.p1();
    auto v2 = line2.p2() - line2.p1();

    float W = v1.y() * v2.x() - v1.x() * v2.y();
    if (std::abs(W) < std::numeric_limits<float>::epsilon())
    {
        return std::nullopt;
    }
    float Wx = p2p1.y() * v2.x() - p2p1.x() * v2.y();
    float Wy = v1.y() * p2p1.x() - v1.x() * p2p1.y();

    float x = Wx / W;
    float y = Wy / W;

    if (x < 0.0f)
    {
        return std::nullopt;
    }

    if (y < 0.0f || y > 1.0f)
    {
        return std::nullopt;
    }

    return v1 * x + line1.p1();
}

std::optional<QPointF> project_on_line(const QLineF& line, const QPointF& point)
{
    auto v1 = line.p2() - line.p1();
    auto ptp1 = line.p1() - point;
    auto t = -QPointF::dotProduct(ptp1, v1) / QPointF::dotProduct(v1, v1);

    return v1 * t + line.p1();
}

std::optional<QPointF> project_on_ray(const QLineF& line, const QPointF& point)
{
    auto v1 = line.p2() - line.p1();
    auto ptp1 = point - line.p1();
    auto t = -QPointF::dotProduct(ptp1, v1) / QPointF::dotProduct(v1, v1);

    if (t < 0.0f)
    {
        return std::nullopt;
    }

    return v1 * t + line.p1();
}

std::optional<QPointF> project_on_segment(const QLineF& line, const QPointF& point)
{
    auto v1 = line.p2() - line.p1();
    auto ptp1 = point - line.p1();
    auto t = -QPointF::dotProduct(ptp1, v1) / QPointF::dotProduct(v1, v1);

    if (t < 0.0f || t > 1.0f)
    {
        return std::nullopt;
    }

    return v1 * t + line.p1();
}

float distance_to_line(const QLineF& line, const QPointF& point)
{
    return (point - *project_on_line(line, point)).manhattanLength();
}

float sign(QPointF p1, QPointF p2, QPointF p3)
{
    return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
}

bool point_in_triangle(QPointF pt, QPointF v1, QPointF v2, QPointF v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

enum class side_of_line
{
    left,
    on,
    right
};

side_of_line get_side_of_line(const QLineF& line, const QPointF& point)
{
    auto& a = line.p1();
    auto& b = line.p2();
    auto& c = point;

    auto v = (b.x() - a.x())*(c.y()-a.y())-(b.y()-a.y())*(c.x()-a.x());

    return v > 0.0f ? side_of_line::left : (v < 0.0f ? side_of_line::right : side_of_line::on);
}

std::list<QPointF> quick_hull(const QLineF& division_line, const std::vector<QPointF>& points_subset)
{
    if (points_subset.empty())
    {
        return std::list<QPointF>();
    }

    auto& p1 = division_line.p1();
    auto& p2 = division_line.p2();
    auto& p3 = *std::max_element(points_subset.begin(), points_subset.end(), [&division_line](const QPointF& l, const QPointF& r) {
        return distance_to_line(division_line,l) < distance_to_line(division_line, r);
        });

    auto line1 = QLineF(p1, p3);
    auto line2 = QLineF(p3, p2);

    std::vector<QPointF> left_to_line1;
    left_to_line1.reserve(points_subset.size());
    std::vector<QPointF> left_to_line2;
    left_to_line2.reserve(points_subset.size());

    for (auto& p : points_subset)
    {
        if (get_side_of_line(line1, p) == side_of_line::left)
        {
            left_to_line1.push_back(p);
        }
        else if (get_side_of_line(line2, p) == side_of_line::left)
        {
            left_to_line2.push_back(p);
        }
    }

    auto l1 = quick_hull(line1, left_to_line1);
    auto l2 = quick_hull(line2, left_to_line2);
    l1.push_back(p3);
    l1.insert(l1.end(), l2.begin(), l2.end());
    return l1;
}

QPolygonF quick_hull(const std::vector<QPointF>& points)
{
    auto [p1i,p2i] = std::minmax_element(points.begin(), points.end(), [](const QPointF& l, const QPointF& r) {return l.x() < r.x(); });
    auto p1 = *p1i;
    auto p2 = *p2i;

    auto line_up = QLineF(p1, p2);
    auto line_down = QLineF(p2, p1);
    std::vector<QPointF> points_up;
    points_up.reserve(points.size());
    std::vector<QPointF> points_down;
    points_up.reserve(points.size());
    for (auto& p : points)
    {
        auto side = get_side_of_line(line_up, p);

        if (side == side_of_line::left)
        {
            points_up.push_back(p);
        }
        else if (side == side_of_line::right)
        {
            points_down.push_back(p);
        }
    }

    auto lup = quick_hull(line_up, points_up);
    auto ldown = quick_hull(line_down, points_down);

    lup.push_back(p2);
    lup.insert(lup.end(), ldown.begin(), ldown.end());
    lup.push_back(p1);

    QPolygonF hull = QPolygonF(lup.size());
    std::copy(lup.begin(), lup.end(), hull.begin());
    return hull;
}

QPolygonF monotone_chain(const std::vector<QPointF>& points)
{
    std::vector<QPointF> sorted_points(points.size());
    std::copy(points.begin(), points.end(), sorted_points.begin());
    std::sort(sorted_points.begin(), sorted_points.end(), [](const QPointF& p1, const QPointF& p2) 
        {
            return p1.x() < p2.x() || (p1.x() == p2.x() && p1.y() < p2.y());
        }
    );

    std::vector<QPointF> H(points.size()*2);
    size_t k = 0;

    std::for_each(sorted_points.begin(), sorted_points.end(), [&](const QPointF& p)
        {
            while (k >= 2 && get_side_of_line(QLineF(H[k - 2], H[k - 1]), p) == side_of_line::left) k--;
            H[k++] = p;
        }
    );

    size_t t = k + 1;

    std::for_each(++sorted_points.rbegin(), sorted_points.rend(), [&](const QPointF& p)
        {
            while (k >= t && get_side_of_line(QLineF(H[k - 2], H[k - 1]), p) == side_of_line::left) k--;
            H[k++] = p;
        }
    );


    H.resize(k - 1);

    QPolygonF hull = QPolygonF(H.size());
    std::copy(H.begin(), H.end(), hull.begin());
    return hull;
}

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

            QLineF visibility_line = QLineF(v.data(), u.data());

            QList<QPointF> intersection_points;

            for (auto& e : edges)
            {
                if (auto p = intersect_ray_segment(visibility_line, e))
                {
                    intersection_points.append(*p);
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

class visualize_hull : public QWidget
{
private:
    std::vector<QPointF> _points;
public:
    visualize_hull(const std::vector<QPointF>& points, QWidget* parent = nullptr) :_points(points), QWidget(parent) {}
protected:
    void paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);

        for (auto& p : _points)
        {
            painter.setBrush(QColor(0, 0, 0));
            painter.setPen(QColor(0, 0, 0));
            painter.drawEllipse(p, 2, 2);
        }

        painter.setBrush(QColor(255, 0, 0));
        painter.setPen(QColor(255, 0, 0));
        auto poly = monotone_chain(_points);
        for (size_t i = 0; i < poly.size(); i++)
        {
            painter.drawLine(poly[i], poly[(i + 1) % poly.size()]);
        }
    }
};

class visualize : public QWidget
{
private:
    visibility_graph<QPointF> _graph;
    std::vector<QPolygonF> _polygons;
public:
    visualize(visibility_graph<QPointF>&& graph, const std::vector<QPolygonF>& polygons, QWidget* parent = nullptr) :_graph(graph), _polygons(polygons), QWidget(parent) {}
protected:
    void paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);

        for (auto& v : _graph.vertices())
        {
            painter.setBrush(QColor(0, 0, 0));
            painter.setPen(QColor(0, 0, 0));
            painter.drawEllipse(v.data(), 2, 2);

            for (auto& e : v.outgoing_edges())
            {
                painter.setBrush(QColor(0, 0, 255));
                painter.setPen(QColor(0, 0, 255));
                painter.drawLine(_graph.vertices()[e.from()].data(), _graph.vertices()[e.to()].data());
            }
        }

        for (auto& polygon : _polygons)
        {
            painter.setBrush(QColor(255, 0, 0));
            painter.setPen(QColor(255, 0, 0));
            painter.drawPolygon(polygon);
        }
    }
};

QPolygonF circle(const QPointF& point, float radius)
{
    constexpr size_t slices = 6;

    QPolygonF c;

    for (size_t i = 0; i < slices; i++)
    {
        float angle = 2 * M_PI / slices * i;
        c.append(QPointF(radius * cos(angle), radius * sin(angle)) + point);
    }

    return c;
}

std::vector<QPointF> circle_points(const QPointF& point, float radius)
{
    constexpr size_t slices = 8;

    std::vector<QPointF> c;

    for (size_t i = 0; i < slices; i++)
    {
        float angle = 2 * M_PI / slices * i;
        c.push_back(QPointF(radius * cos(angle), radius * sin(angle)) + point);
    }

    return c;
}


QPolygonF star(const QPointF& point, float radius)
{
    constexpr size_t points = 8;

    QPolygonF star;

    for (size_t i = 0; i < points * 2; i++)
    {
        float angle = 2 * M_PI / (points * 2) * i;
        float current_radius = radius * (i % 2 ? 0.4f : 1.0f);
        star.append(QPointF(current_radius * cos(angle), current_radius * sin(angle)) + point);
    }

    return star;
}

std::vector<QPointF> generate_range(const QRectF& range, size_t count)
{
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<float> x(range.left(), range.right());
    std::uniform_real_distribution<float> y(range.top(), range.bottom());
    std::vector<QPointF> points(count);

    for (auto& p : points)
    {
        p = QPointF(x(e2), y(e2));
    }

    return points;
}

int main(int argc, char** argv)
{
    auto test_set = generate_range(QRectF(100, 100, 200, 200), 100000000);

    {
        auto s = std::chrono::high_resolution_clock::now();
        auto result = quick_hull(test_set);
        auto e = std::chrono::high_resolution_clock::now();
        std::cout << "quick_hull" << std::endl;
        std::cout << (e - s).count() / 1e9 << "s" << std::endl;
        std::cout << result[0].x() << std::endl;
    }

    {
        auto s = std::chrono::high_resolution_clock::now();
        auto result = monotone_chain(test_set);
        auto e = std::chrono::high_resolution_clock::now();
        std::cout << "monotone_chain" << std::endl;
        std::cout << (e - s).count() / 1e9 << "s" << std::endl;
        std::cout << result[0].x() << std::endl;
    }


    std::cin.get();
    QApplication application(argc, argv);

    std::vector<QPolygonF> polygons =
    {
         QPolygonF(
                    {
                        QPointF(50,50),
                        QPointF(100,50),
                        QPointF(50,100)
                    }
                ),
                QPolygonF(
                    {
                        QPointF(200,200),
                        QPointF(250,200),
                        QPointF(200,250)
                    }
                ),
                QPolygonF(
                    {
                        QPointF(320,100),
                        QPointF(420,150),
                        QPointF(440,230),
                        QPointF(340,200)
                    }
                ),
                circle(QPointF(100,300), 25),
                star(QPointF(200,300), 25)
    };

    /*visualize v1(
        construct_visibility_graph(
            polygons
        ),
        polygons
    );

    v1.show();*/

    visualize_hull v2(generate_range(QRectF(100, 100, 200, 200), 10));
    v2.show();

    application.exec();

    return 0;
}