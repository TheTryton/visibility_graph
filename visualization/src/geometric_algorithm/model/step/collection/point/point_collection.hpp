#pragma once

#include <geometric_algorithm/model/step/collection/collection.hpp>

class point_collection : public icollection
{
public:
    enum class point_shape
    {
        circle,
        square,
        star,
        cross,
        triangle
    };
    struct point_format
    {
        QColor color;
        point_shape shape;
        float width;
    };
private:
    point_format _format;
    std::vector<QPointF> _points;
public:
    point_collection(const std::vector<QPointF>& lines, const QColor& color = QColor(0, 0, 255), point_shape shape = point_shape::circle, float radius = 1.0f);
public:
    virtual void draw(QPainter& painter) const override;
};