#pragma once

#include <geometric_algorithm/view/base/base_view.hpp>
#include <geometric_algorithm/scene/scene.hpp>

struct step
{
    algorithm_scene* scene;
    QString description;
};

class stepbystep_view : public QWidget
{
    Q_OBJECT
private:
    base_view* _step_view;
    QPushButton* _forward;
    QPushButton* _backward;
    QLabel* _step_description;
    std::vector<step> _steps;
    size_t step_index = 0;
public:
    stepbystep_view(const std::vector<step>& steps, QWidget* parent = nullptr);
};