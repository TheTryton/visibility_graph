#pragma once

#include <geometric_algorithm/view/input/polygon_input_view.hpp>
#include <geometric_algorithm/view/stepbystep/stepbystep.hpp>
#include <geometric_algorithm/view/result/visibility_graph_result_view.hpp>

class interactive_view : public QWidget
{
    Q_OBJECT
private:
    polygon_input_view* _input;
    visibility_graph_result_view* _result;
    stepbystep_view* _step_by_step;

    QTabWidget* _tabs;
public:
    interactive_view(QWidget* parent = nullptr);
};