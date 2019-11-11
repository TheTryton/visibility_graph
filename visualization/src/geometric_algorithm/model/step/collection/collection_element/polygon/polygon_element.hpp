#pragma once

#include <geometric_algorithm/model/step/collection/collection_element/collection_element.hpp>

enum polygon_representation
{
    wireframe,
    fill
};

class polygon_element : public icollection_element
{
private:
    QPolygon _polygon;
    QColor _color;
    polygon_representation _representation;
public:
    polygon_element(const QVector<QPoint>& points, const QColor& color = QColor(255, 0, 0), polygon_representation representation = polygon_representation::wireframe);
    polygon_element(const QPolygon& polygon, const QColor& color = QColor(255, 0, 0), polygon_representation representation = polygon_representation::wireframe);
public:
    virtual ~polygon_element() override = default;
public:
    virtual void draw(QPainter& painter) const override;
    virtual QRect bounding_rect() const override;
};