#pragma once

#include <QtWidgets>

class icollection
{
public:
    virtual void draw(QPainter& painter) const = 0;
};