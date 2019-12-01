#pragma once

#include <QtWidgets>

class point_item : public QGraphicsItem
{
private:
    QPointF _point;
    QColor _color;
public:
    point_item(const QPointF& point, const QColor& color = QColor(255, 255, 255), QGraphicsItem* parent = nullptr);
public:
    QPointF&                get_point();
    QColor&                 get_color();
public:
    virtual QRectF          boundingRect() const override;
    virtual QPainterPath    shape() const override;
    virtual void            paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};
