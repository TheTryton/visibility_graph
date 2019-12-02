#include <geometric_algorithm/scene/element/segment.hpp>

segment_item::segment_item(const QLineF& segment, const QColor& color, float width, QGraphicsItem* parent) :
    QGraphicsItem(parent),
    _segment(segment),
    _color(color),
    _width(width)
{
    setZValue(0.5);
}

QLineF& segment_item::get_segment()
{
    return _segment;
}

QColor& segment_item::get_color()
{
    return _color;
}

QPainterPath get_path(const QLineF& line, float width)
{
    QPainterPath path;
    path.moveTo(line.p1());
    path.lineTo(line.p2());
    QPainterPathStroker stroke;
    stroke.setWidth(width);
    return stroke.createStroke(path);
}

QRectF segment_item::boundingRect() const
{
    return get_path(_segment, _width).boundingRect();
}

QPainterPath segment_item::shape() const
{
    return get_path(_segment, _width);
}

void segment_item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->save();

    painter->setBrush(_color);

    painter->setPen(QPen(_color, _width));

    painter->drawLine(_segment);

    painter->restore();
}
