#include <geometric_algorithm/scene/element/segment.hpp>

segment_item::segment_item(const QLineF& segment, const QColor& color, float width, size_t index, QGraphicsItem* parent) :
    QGraphicsItem(parent),
    _segment(segment),
    _color(color),
    _width(width),
    _index(index)
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

QPainterPath get_path(const QLineF& line, float width, size_t index)
{
    QPainterPath path;
    path.moveTo(line.p1());
    path.lineTo(line.p2());
    QPainterPathStroker stroke;
    stroke.setWidth(width);
    auto stroked = stroke.createStroke(path);
    if (index != 0)
    {
        stroke.setWidth(width + 12);
        stroked = stroke.createStroke(path);
    }
    return stroked;
}

QRectF segment_item::boundingRect() const
{
    return get_path(_segment, _width, _index).boundingRect();
}

QPainterPath segment_item::shape() const
{
    return get_path(_segment, _width, _index);
}

void segment_item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->save();

    painter->setBrush(_color);

    painter->setPen(QPen(_color, _width));

    painter->drawLine(_segment);

    painter->setBrush(QColor(255, 255, 255));

    painter->setPen(QColor(255, 255, 255));

    if (_index > 0)
    {
        painter->drawText((_segment.p1() + _segment.p2()) / 2, QString::number(_index));
    }

    painter->restore();
}
