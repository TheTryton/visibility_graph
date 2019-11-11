#pragma once

#include <geometric_algorithm/model/step/collection/collection.hpp>

class step
{
    std::vector<icollection*> _collections;
    QString _description;
public:
    step(const std::vector<icollection*> collections, const QString& description);
public:
    void draw(QPainter& painter) const;
};