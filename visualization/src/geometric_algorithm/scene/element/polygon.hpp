#pragma once

#include <QtWidgets>

class polygon_item : public QGraphicsItem
{
private:
    QPolygonF _polygon;
    QColor _color;
public:
    polygon_item(const QPolygonF& polygon, const QColor& color, QGraphicsItem* parent = nullptr);
public:
    QColor&                 get_color();
    QPolygonF&              get_polygon();
public:
    virtual QRectF          boundingRect() const override;
    virtual QPainterPath    shape() const override;
    virtual void            paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};
