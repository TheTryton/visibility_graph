#include <geometric_algorithm/model/step/collection/collection_element/line/line_element.hpp>

line_element::line_element(const QPoint& point1, const QPoint& point2, const QColor& color, float width) :
    _line(point1, point2),
    _color(color),
    _width(width)
{
}

line_element::line_element(const QLine& line, const QColor& color, float width) :
    _line(line),
    _color(color),
    _width(width)
{
}

void line_element::draw(QPainter& painter) const
{
    painter.save();

    QPainterPath path;
    path.moveTo(_line.p1());
    path.lineTo(_line.p2());

    QPainterPathStroker stroker;
    path = stroker.createStroke(path);

    painter.setPen(_color);
    painter.setBrush(_color);
    painter.drawPath(path);

    painter.restore();
}

QRect line_element::bounding_rect() const
{
    QPainterPath path;
    path.moveTo(_line.p1());
    path.lineTo(_line.p2());

    QPainterPathStroker stroker;
    path = stroker.createStroke(path);

    return path.boundingRect().toRect();
}
