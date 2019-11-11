#include <geometric_algorithm/model/step/step.hpp>

step::step(const std::vector<icollection*> collections, const QString& description):
    _collections(collections),
    _description(description)
{
}

void step::draw(QPainter& painter) const
{
    painter.save();

    for (const auto& collection : _collections)
    {
        collection->draw(painter);
    }

    painter.restore();
}