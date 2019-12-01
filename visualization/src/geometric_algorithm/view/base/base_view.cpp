#include <geometric_algorithm/view/base/base_view.hpp>

base_view::base_view(QWidget* parent) :
    QGraphicsView(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
    setCursor(Qt::CursorShape::ArrowCursor);
    setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::FullViewportUpdate);

    QObject::connect(&_zoom_animation, &QVariantAnimation::valueChanged, this, [this](const QVariant& v) {
        auto factor = v.toDouble() / this->_current_zoom;
        scale(factor, factor);
        this->_current_zoom = v.toDouble();
        });
}

void base_view::wheelEvent(QWheelEvent* event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    double angle = event->angleDelta().y();
    double factor = qPow(1.0015, angle);

    _zoom_animation.stop();
    _target_zoom = qMax(qMin(_target_zoom * factor, _max_zoom), _min_zoom);
    _zoom_animation.setStartValue(_current_zoom);
    _zoom_animation.setEndValue(_target_zoom);
    _zoom_animation.setDuration(100);
    _zoom_animation.start();
}