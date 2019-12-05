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
    _forward->setFont(font);
    _backward->setFont(font);
    _slider = new QSlider(Qt::Orientation::Horizontal);
    root->addWidget(_slider);
    _slider->setMaximum(static_cast<int>(_steps.size() - 1));
    _slider->setMinimum(0);
    _step_index = 0;
    if (!_steps.empty())
    {
        _steps[_step_index].scene->set_scene(_step_view);
        _step_description->setText(_steps[_step_index].description);
    }

    QObject::connect(_forward, &QPushButton::released, this, [&]() {
        if (!_steps.empty())
        {
            _step_index = (_step_index + 1) % _steps.size();
            _slider->setValue(static_cast<int>(_step_index));
        }
        });

    QObject::connect(_backward, &QPushButton::released, this, [&]() {
        if (!_steps.empty())
        {
            _step_index = _step_index == 0 ? _steps.size() - 1 : _step_index - 1;
            _slider->setValue(static_cast<int>(_step_index));
        }
        });

    QObject::connect(_slider, &QSlider::valueChanged, this, [&](int s) {
        _step_index = s;
        _steps[_step_index].scene->set_scene(_step_view);
        _step_description->setText(_steps[_step_index].description);
        });

    setLayout(root);
}

void stepbystep_view::set_steps(const std::vector<step>& steps)
{
    _steps = steps;
    _slider->setMaximum(static_cast<int>(_steps.size() - 1));
    _slider->setMinimum(0);
    _step_index = 0;
    if (!_steps.empty())
    {
        _steps[_step_index].scene->set_scene(_step_view);
        _step_description->setText(_steps[_step_index].description);
    }
}
