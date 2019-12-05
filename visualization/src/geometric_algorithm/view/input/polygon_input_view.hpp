#pragma once

#include <geometric_algorithm/view/base/base_view.hpp>
#include <geometric_algorithm/scene/element/point.hpp>
#include <geometric_algorithm/scene/element/segment.hpp>
#include <geometric_algorithm/scene/element/polygon.hpp>
#include <geometric_algorithm/scene/element/near_circle.hpp>
#include <optional>

class polygon_input_view : public base_view
{
    Q_OBJECT
private:
    std::optional<segment_item*> _constructed_edge;
    QList<segment_item*> _constructed_edges;
    near_circle_item* _near_circle;
public:
    polygon_input_view(QWidget* parent = nullptr);
public:
    QList<QPolygonF> get_polygons() const;
    void clear();
protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
};