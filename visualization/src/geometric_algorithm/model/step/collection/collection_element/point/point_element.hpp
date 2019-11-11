#pragma once

#include <geometric_algorithm/model/step/collection/collection_element/collection_element.hpp>

enum point_marker
{
    circle,
    square,
    star,
    cross,
    triangle
};

class point_element : public icollection_element
{
private:
    QPoint _point;
    QColor _color;
    point_marker _marker;
    float _radius;
public:
    point_element(const QPoint& point, const QColor& color = QColor(255, 0, 0), point_marker marker = circle, float radius = 1.0f);
public:
    virtual ~point_element() override = default;
public:
    virtual void draw(QPainter& painter) const override;
    virtual QRect bounding_rect() const override;
};