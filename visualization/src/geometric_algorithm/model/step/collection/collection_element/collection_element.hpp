#pragma once

#include <QtWidgets>

class icollection_element
{
public:
    virtual ~icollection_element() = default;
public:
    virtual void draw(QPainter& painter) const = 0;
    virtual QRegion bounding_region() const = 0;
    virtual QRect bounding_rect() const = 0;
};