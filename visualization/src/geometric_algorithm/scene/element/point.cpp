#include <geometric_algorithm/scene/element/point.hpp>

point_item::point_item(const QPointF& point, const QColor& color, float radius, size_t index, QGraphicsItem* parent) :
    QGraphicsItem(parent),
    _point(point),
    _color(color),
    _index(index),
    _radius(radius)
{
    setZValue(1);
}

QPointF& point_item::get_point()
{
    return _point;
}

QColor& point_item::get_color()
{
    return _color;
}

QPainterPath get_path(const QPointF& point, float radius, size_t index)
{
    QPainterPath path;
    path.addEllipse(point, radius, radius);
    if (index != 0)
    {
        path.addEllipse(point, radius + 12, radius + 12);
    }
    return path;
}

QRectF point_item::boundingRect() const
{
    return get_path(_point, _radius, _index).boundingRect();
}

QPainterPath point_item::shape() const
{
    return get_path(_point, _radius, _index);
}

void point_item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->save();

    painter->setBrush(_color);

    painter->setPen(_color);

    painter->drawEllipse(_point, _radius, _radius);

    painter->setBrush(QColor(255, 255, 255));

    painter->setPen(QColor(255, 255, 255));

    if (_index > 0)
    {
        painter->drawText(_point + QPointF(_radius, -_radius), QString::number(_index));
    }

    painter->restore();
}
