#include <geometric_algorithm/scene/element/polygon.hpp>

polygon_item::polygon_item(const QPolygonF& polygon, const QColor& color, QGraphicsItem* parent) :
    QGraphicsItem(parent),
    _polygon(polygon),
    _color(color)
{
    setZValue(0);
}

QPolygonF& polygon_item::get_polygon()
{
    return _polygon;
}

QColor& polygon_item::get_color()
{
    return _color;
}

QPainterPath get_path(const QPolygonF& polygon)
{
    QPainterPath path;
    path.addPolygon(polygon);
    return path;
}

QRectF polygon_item::boundingRect() const
{
    return _polygon.boundingRect();
}

QPainterPath polygon_item::shape() const
{
    return get_path(_polygon);
}

void polygon_item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->save();

    painter->setBrush(_color);

    painter->setPen(_color);

    painter->drawPolygon(_polygon);

    painter->restore();
}
