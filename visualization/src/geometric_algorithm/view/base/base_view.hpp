#pragma once

#include <QtWidgets>

class base_view : public QGraphicsView
{
    Q_OBJECT
private:
    qreal                       _current_zoom = 1.0;
    qreal                       _target_zoom = 1.0;
    QVariantAnimation           _zoom_animation;

    constexpr static qreal      _min_zoom = 0.125;
    constexpr static qreal      _max_zoom = 15.0;
public:
    base_view(QWidget* parent = nullptr);
protected:
    virtual void                wheelEvent(QWheelEvent* event) override;
};