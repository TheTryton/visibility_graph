#include  <geometric_algorithm/view/input/polygon_input_view.hpp>
#include <geometric_algorithm/scene/scene.hpp>

polygon_input_view::polygon_input_view(QWidget* parent) :
    base_view(parent)
{
    setScene(new algorithm_input_scene());
    setMouseTracking(true);
}

QList<QPolygonF> polygon_input_view::get_polygons() const
{
    QList<QPolygonF> polygons;
    for (auto& item : scene()->items())
    {
        if (auto poly = dynamic_cast<polygon_item*>(item))
        {
            polygons.append(poly->get_polygon());
        }
    }
    return polygons;
}

void polygon_input_view::clear()
{
    _constructed_edge = std::nullopt;
    _constructed_edges.clear();
    _near_circle = nullptr;
    scene()->clear();
}

void polygon_input_view::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::RightButton)
    {
        if (auto poly = dynamic_cast<polygon_item*>(itemAt(event->pos())))
        {
            QMenu menu;
            menu.addAction("Remove polygon", [&]() {scene()->removeItem(poly); });
            menu.exec(event->globalPos());
        }
        else
        {
            auto scenePos = mapToScene(event->pos());

            if (!_constructed_edge)
            {
                _constructed_edge = new segment_item(QLineF(scenePos, scenePos));
                _constructed_edges.append(*_constructed_edge);

                _near_circle = new near_circle_item(scenePos, QColor(255, 255, 0), 15);

                scene()->addItem(*_constructed_edge);
                scene()->addItem(_near_circle);

                scene()->update();
            }
            else
            {
                auto item = itemAt(event->pos());
                if (item && item == _near_circle)
                {
                    if (_constructed_edges.size() <= 2)
                    {
                        for (auto& e : _constructed_edges)
                        {
                            scene()->removeItem(e);
                        }

                        scene()->removeItem(_near_circle);

                        _constructed_edges.clear();
                        _constructed_edge = std::nullopt;

                        scene()->update();
                    }
                    else
                    {
                        QPolygonF p;
                        for (auto& e : _constructed_edges)
                        {
                            p.append(e->get_segment().p1());
                            scene()->removeItem(e);
                        }
                        scene()->removeItem(_near_circle);
                        _constructed_edges.clear();
                        _constructed_edge = std::nullopt;

                        scene()->addItem(new polygon_item(p, QColor(255,0,0)));

                        scene()->update();
                    }
                }
                else
                {
                    (*_constructed_edge)->get_segment().setP2(scenePos);
                    _constructed_edge = new segment_item(QLineF(scenePos, scenePos));
                    _constructed_edges.append(*_constructed_edge);
                    scene()->addItem(*_constructed_edge);

                    scene()->update();
                }
            }
        }
    }
    else if (event->button() == Qt::MouseButton::LeftButton)
    {
        if (_constructed_edge)
        {
            for (auto& e : _constructed_edges)
            {
                scene()->removeItem(e);
            }

            scene()->removeItem(_near_circle);

            _constructed_edges.clear();
            _constructed_edge = std::nullopt;

            scene()->update();
        }
    }

    base_view::mousePressEvent(event);
}

void polygon_input_view::mouseMoveEvent(QMouseEvent* event)
{
    if (_constructed_edge)
    {
        auto scenePos = mapToScene(event->pos());
        (*_constructed_edge)->get_segment().setP2(scenePos);
        scene()->update();
    }

    base_view::mouseMoveEvent(event);
}

void polygon_input_view::mouseReleaseEvent(QMouseEvent* event)
{
    base_view::mouseReleaseEvent(event);
}
