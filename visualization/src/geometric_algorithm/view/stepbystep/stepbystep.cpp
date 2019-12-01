#include <geometric_algorithm/view/stepbystep/stepbystep.hpp>

stepbystep_view::stepbystep_view(const std::vector<step>& steps, QWidget* parent):
    QWidget(parent),
    _steps(steps)
{
    QVBoxLayout* root = new QVBoxLayout();
    _step_view = new base_view;
    root->addWidget(_step_view);
    QHBoxLayout* controls = new QHBoxLayout();
    root->addLayout(controls);
    _backward = new QPushButton("Previous step");
    _step_description = new QLabel();
    _forward = new QPushButton("Next step");
    controls->addWidget(_backward);
    controls->addWidget(_step_description);
    controls->addWidget(_forward);

    setLayout(root);
}
