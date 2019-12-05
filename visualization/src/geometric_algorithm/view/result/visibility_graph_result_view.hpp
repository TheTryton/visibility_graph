#pragma once

#include <geometric_algorithm/view/base/base_view.hpp>
#include <geometric_algorithm/scene/scene.hpp>
#include <vector>

class visibility_graph_result_view : public base_view
{
    Q_OBJECT
private:
    std::vector<QPolygonF> _obstacles;
public:
    visibility_graph_result_view(const std::vector<QPolygonF>& obstacles, QWidget* parent = nullptr);
public:
    void set_obstacles(const std::vector<QPolygonF>& obstacles);
};