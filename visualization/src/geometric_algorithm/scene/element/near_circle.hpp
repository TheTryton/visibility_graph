#pragma once

#include <QtWidgets>

class near_circle_item : public QGraphicsItem
{
private:
    QPointF _point;
    QColor _color;
    qreal _radius;
public:
    near_circle_item(const QPointF& point, const QColor& color = QColor(255, 255, 0), qreal radius = 5.0, QGraphicsItem* parent = nullptr);
public:
    virtual QRectF          boundingRect() const override;
    virtual QPainterPath    shape() const override;
    virtual void            paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};
