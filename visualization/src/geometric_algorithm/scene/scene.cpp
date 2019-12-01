#include <geometric_algorithm/scene/scene.hpp>
#include <geometric_algorithm/scene/element/point.hpp>
#include <geometric_algorithm/scene/element/segment.hpp>
#include <geometric_algorithm/scene/element/polygon.hpp>

algorithm_scene_base::algorithm_scene_base(QObject* parent) :
    QGraphicsScene(parent)
{
    setSceneRect(QRectF(-10000.0, -10000.0, 20000.0, 20000.0));
}

void algorithm_scene_base::drawBackground(QPainter* painter, const QRectF& rect)
{
    painter->fillRect(rect, QColor(72, 74, 82));

    float separation_thicker = 100.0f;
    float separation_thinner = 25.0f;

    QSizeF max_view_size = QSizeF(0, 0);

    for (auto view : views())
    {
        auto rect = view->mapToScene(view->viewport()->geometry()).boundingRect();
        max_view_size.setWidth(qMax(rect.width(), max_view_size.width()));
        max_view_size.setHeight(qMax(rect.height(), max_view_size.height()));
    }

    if (max_view_size.width() / separation_thicker > 20 && max_view_size.height() / separation_thicker > 20)
    {
        separation_thicker *= 4;
        separation_thinner *= 4;
    }
    else if (max_view_size.width() / separation_thicker < 5 && max_view_size.height() / separation_thicker < 5)
    {
        separation_thicker /= 4;
        separation_thinner /= 4;
    }

    QPointF start_thick = QPointF(qFloor(rect.left() / separation_thicker) * separation_thicker, qFloor(rect.top() / separation_thicker) * separation_thicker);
    QPointF start_thin = QPointF(qFloor(rect.left() / separation_thinner) * separation_thinner, qFloor(rect.top() / separation_thinner) * separation_thinner);

    QPen pen_thick;
    pen_thick.setWidthF(3);
    pen_thick.setColor(QColor(62, 69, 72, 168));
    painter->setPen(pen_thick);
    for (float current_thick_x = start_thick.x(); current_thick_x < rect.right(); current_thick_x += separation_thicker)
    {
        painter->drawLine(QPointF(current_thick_x, rect.top()), QPointF(current_thick_x, rect.bottom()));
    }
    for (float current_thick_y = start_thick.y(); current_thick_y < rect.bottom(); current_thick_y += separation_thicker)
    {
        painter->drawLine(QPointF(rect.left(), current_thick_y), QPointF(rect.right(), current_thick_y));
    }

    QPen pen_thin;
    pen_thick.setWidthF(1);
    pen_thick.setColor(QColor(60, 64, 72, 127));
    painter->setPen(pen_thick);
    for (float current_thin_x = start_thin.x(); current_thin_x < rect.right(); current_thin_x += separation_thinner)
    {
        painter->drawLine(QPointF(current_thin_x, rect.top()), QPointF(current_thin_x, rect.bottom()));
    }
    for (float current_thin_y = start_thick.y(); current_thin_y < rect.bottom(); current_thin_y += separation_thinner)
    {
        painter->drawLine(QPointF(rect.left(), current_thin_y), QPointF(rect.right(), current_thin_y));
    }
}

algorithm_input_scene::algorithm_input_scene(QObject* parent) :
    algorithm_scene_base(parent)
{
}

algorithm_scene::algorithm_scene(const QList<points_collection>& ptcolls, const QList<lines_collection>& lcolls, const QList<polygons_collection>& plcolls, QObject* parent) :
    algorithm_scene_base(parent)
{
    for (auto& pti : ptcolls)
    {
        for (auto& p : pti.points)
        {
            addItem(new point_item(p, pti.color));
        }
    }

    for (auto& lni : lcolls)
    {
        for (auto& l : lni.lines)
        {
            addItem(new segment_item(l, lni.color));
        }
    }

    for (auto& pli : plcolls)
    {
        for (auto& p : pli.polygons)
        {
            addItem(new polygon_item(p, pli.color));
        }
    }
}