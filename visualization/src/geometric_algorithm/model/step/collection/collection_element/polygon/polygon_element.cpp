#include <geometric_algorithm/model/step/collection/collection_element/polygon/polygon_element.hpp>

polygon_element::polygon_element(const QVector<QPoint>& points, const QColor& color, polygon_representation representation) :
    _polygon(points),
    _color(color),
    _representation(representation)
{
}

polygon_element::polygon_element(const QPolygon& polygon, const QColor& color, polygon_representation representation) :
    _polygon(polygon),
    _color(color),
    _representation(representation)
{
}

void polygon_element::draw(QPainter& painter) const
{
    painter.save();

    painter.setPen(_color);
    painter.setBrush(_color);

    switch (_representation)
    {
    case wireframe:
        painter.drawPolyline(_polygon);
        break;
    case fill:
        painter.drawPolygon(_polygon);
        break;
    }

    painter.restore();
}

QRect polygon_element::bounding_rect() const
{
    return _polygon.boundingRect();
}
