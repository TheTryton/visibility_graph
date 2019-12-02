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
    _backward->setMinimumHeight(100);
    _step_description = new QLabel();
    _forward = new QPushButton("Next step");
    _forward->setMinimumHeight(100);
    controls->addWidget(_backward);
    controls->addWidget(_step_description);
    controls->addWidget(_forward);
    auto font = _step_description->font();
    font.setPointSize(16);
    _step_description->setFont(font);
    _step_description->setWordWrap(true);

    _step_index = 0;
    if (!_steps.empty())
    {
        _step_view->setScene(steps[_step_index].scene);
        _step_description->setText(steps[_step_index].description);
    }

    QObject::connect(_forward, &QPushButton::released, this, [&]() {
        _step_index = (_step_index + 1)% _steps.size();
        _step_view->setScene(_steps[_step_index].scene);
        _step_description->setText(_steps[_step_index].description);
        });

    QObject::connect(_backward, &QPushButton::released, this, [&]() {
        _step_index = _step_index == 0 ? _steps.size() - 1 : _step_index - 1;
        _step_view->setScene(_steps[_step_index].scene);
        _step_description->setText(_steps[_step_index].description);
        });

    setLayout(root);
}
