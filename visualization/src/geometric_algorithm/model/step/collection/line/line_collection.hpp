#pragma once

#include <geometric_algorithm/model/step/collection/collection.hpp>

class line_collection : public icollection
{
public:
    struct line_format
    {
        QColor color;
        float width;
    };
private:
    line_format _format;
    std::vector<QLineF> _lines;
public:
    line_collection(const std::vector<QLineF>& lines, const QColor& color = QColor(0, 0, 255), float width = 1.0f);
public:
    virtual void draw(QPainter& painter) const override;
};