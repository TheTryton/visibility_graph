#pragma once

#include <geometric_algorithm/model/step/collection/collection_element/collection_element.hpp>

class line_element : public icollection_element
{
private:
    QLine _line;
    QColor _color;
    float _width;
public:
    line_element(const QPoint& point1, const QPoint& point2, const QColor& color = QColor(255, 0, 0), float width = 1.0f);
    line_element(const QLine& line, const QColor& color = QColor(255, 0, 0), float width = 1.0f);
public:
    virtual ~line_element() override = default;
public:
    virtual void draw(QPainter& painter) const override;
    virtual QRect bounding_rect() const override;
};