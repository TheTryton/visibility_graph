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
    QSlider* _slider;
    std::vector<step> _steps;
    size_t _step_index;
public:
    stepbystep_view(const std::vector<step>& steps, QWidget* parent = nullptr);
};