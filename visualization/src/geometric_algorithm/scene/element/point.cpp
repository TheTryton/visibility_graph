#include <geometric_algorithm/scene/element/point.hpp>

point_item::point_item(const QPointF& point, const QColor& color, QGraphicsItem* parent) :
    QGraphicsItem(parent),
    _point(point),
    _color(color)
{
    setZValue(0);
}

QPointF& point_item::get_point()
{
    return _point;
}

QColor& point_item::get_color()
{
    return _color;
}

QPainterPath get_path(const QPointF& point)
{
    QPainterPath path;
    path.addEllipse(point, 0.5f, 0.5f);
    return path;
}

QRectF point_item::boundingRect() const
{
    return get_path(_point).boundingRect();
}

QPainterPath point_item::shape() const
{
    return get_path(_point);
}

void point_item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->save();

    painter->setBrush(_color);

    painter->setPen(_color);

    painter->drawEllipse(_point, 0.5f, 0.5f);

    painter->restore();
}
