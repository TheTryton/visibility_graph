#pragma once

#include <QtWidgets>

class segment_item : public QGraphicsItem
{
private:
    QLineF _segment;
    QColor _color;
public:
    segment_item(const QLineF& segment, const QColor& color = QColor(0, 255, 0), QGraphicsItem* parent = nullptr);
public:
    QLineF&                 get_segment();
    QColor&                 get_color();
public:
    virtual QRectF          boundingRect() const override;
    virtual QPainterPath    shape() const override;
    virtual void            paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};
