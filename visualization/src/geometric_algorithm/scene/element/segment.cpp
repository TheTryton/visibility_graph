#include <geometric_algorithm/scene/element/segment.hpp>

segment_item::segment_item(const QLineF& segment, const QColor& color, QGraphicsItem* parent):
    QGraphicsItem(parent),
    _segment(segment),
    _color(color)
{
    setZValue(-1);
}

QLineF& segment_item::get_segment()
{
    return _segment;
}

QColor& segment_item::get_color()
{
    return _color;
}

QPainterPath get_path(const QLineF& line)
{
    QPainterPath path;
    path.moveTo(line.p1());
    path.lineTo(line.p2());
    return path;
}

QRectF segment_item::boundingRect() const
{
    return get_path(_segment).boundingRect();
}

QPainterPath segment_item::shape() const
{
    return get_path(_segment);
}

void segment_item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->save();

    painter->setBrush(_color);

    painter->setPen(_color);

    painter->drawLine(_segment);

    painter->restore();
}
