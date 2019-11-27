#include "point_element.hpp"

point_element::point_element(const QPoint& point, const QColor& color, point_marker marker, float radius) :
    _point(point),
    _color(color),
    _marker(marker),
    _radius(radius)
{
}

QPolygon constructStar(const QPoint& point, float radius)
{
    constexpr size_t points = 4;

    QPolygon star;

    for (size_t i = 0; i < points*2; i++)
    {
        float angle = 2 * M_PI / (points * 2) * i;
        float current_radius = radius * (i % 2 ? 0.25f : 1.0f);
        QPoint p = QPoint(current_radius * cos(angle), current_radius * sin(angle));
        star.append(p);
    }

    return star;
}

QPainterPath constructCross(const QPoint& point, float radius)
{
    constexpr size_t points = 4;

    QPainterPath cross;

    cross.addRect(QRect(point - QPoint(radius * 0.125f, radius), point + QPoint(radius * 0.125f, radius)));
    cross.addRect(QRect(point - QPoint(radius, radius * 0.125f), point + QPoint(radius, radius * 0.125f)));

    return cross;
}

QPolygon constructTriangle(const QPoint& point, float radius)
{
    constexpr size_t points = 4;

    QPolygon triangle;

    QMatrix4x4 rotation;
    rotation.setToIdentity();
    rotation.rotate(120, QVector3D(0, 0, 1));

    triangle.append(QPoint(point - QPoint(0, radius)));
    auto rotated120 = QVector2D(QVector3D(0, radius, 0) * rotation);
    auto rotated240 = QVector2D(QVector3D(0, radius, 0) * rotation * rotation);
    triangle.append(QPoint(point - QPoint(rotated120.x(), rotated120.y())));
    triangle.append(QPoint(point - QPoint(rotated240.x(), rotated240.y())));

    return triangle;
}

void point_element::draw(QPainter& painter) const
{
    painter.save();

    switch (_marker)
    {
    case circle:
        painter.setBrush(_color);
        painter.setPen(_color);
        painter.drawEllipse(_point, static_cast<int>(_radius), static_cast<int>(_radius));
        break;
    case square:
        painter.setBrush(_color);
        painter.setPen(_color);
        painter.fillRect(QRect(_point - QPoint(_radius,_radius), _point + QPoint(_radius, _radius)), QBrush(_color));
        break;
    case star:
        painter.setBrush(_color);
        painter.setPen(_color);
        painter.drawPolygon(constructStar(_point, _radius));
        break;
    case cross:
        painter.setBrush(_color);
        painter.setPen(_color);
        painter.drawPath(constructCross(_point, _radius));
        break;
    case triangle:
        painter.setBrush(_color);
        painter.setPen(_color);
        painter.drawPolygon(constructTriangle(_point, _radius));
        break;
    }

    painter.restore();
}

QRect point_element::bounding_rect() const
{
    QPainterPath path;
    switch (_marker)
    {
    case circle:
        path.addEllipse(_point, static_cast<int>(_radius), static_cast<int>(_radius));
        return path.boundingRect().toRect();
    case square:
        path.addRect(QRect(_point - QPoint(_radius, _radius), _point + QPoint(_radius, _radius)));
        return path.boundingRect().toRect();
    case star:
        path.addPolygon(constructStar(_point, _radius));
        return path.boundingRect().toRect();
    case cross:
        path.addPath(constructCross(_point, _radius));
        return path.boundingRect().toRect();
    case triangle:
        path.addPolygon(constructTriangle(_point, _radius));
        return path.boundingRect().toRect();
    }

    return QRect();
}