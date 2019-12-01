#include <geometric_algorithm/scene/element/near_circle.hpp>

near_circle_item::near_circle_item(const QPointF& point, const QColor& color, qreal radius, QGraphicsItem* parent):
    QGraphicsItem(parent),
    _point(point),
    _color(color),
    _radius(radius)
{
    setZValue(1);
    setAcceptHoverEvents(true);
}

QPainterPath get_path(const QPointF& point, qreal radius)
{
    QPainterPath path;
    path.addEllipse(point, radius, radius);
    return path;
}

QRectF near_circle_item::boundingRect() const
{
    return get_path(_point, _radius).boundingRect();
}

QPainterPath near_circle_item::shape() const
{
    return get_path(_point, _radius);
}

void near_circle_item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->save();

    painter->setBrush(Qt::NoBrush);

    if (option->state & QStyle::StateFlag::State_MouseOver)
    {
        painter->setPen(QPen(_color, 5));
    }
    else
    {
        painter->setPen(QPen(_color, 1));
    }

    painter->drawEllipse(_point, _radius, _radius);

    painter->restore();
}
