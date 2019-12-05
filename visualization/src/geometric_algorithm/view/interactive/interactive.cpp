#include <geometric_algorithm/view/interactive/interactive.hpp>

interactive_view::interactive_view(QWidget* parent) :
    QWidget(parent)
{
    QHBoxLayout* root = new QHBoxLayout;
    setLayout(root);
    _tabs = new QTabWidget;
    root->addWidget(_tabs);
   
    // Input tab
    {
        QWidget* input_tab = new QWidget;

        _input = new polygon_input_view;
        QVBoxLayout* input_layout = new QVBoxLayout;
        QPushButton* compute_visibility_graph = new QPushButton("Compute visibility graph");
        auto font = compute_visibility_graph->font();
        font.setPointSize(16);
        compute_visibility_graph->setFont(font);
        input_layout->addWidget(_input);
        input_layout->addWidget(compute_visibility_graph);
        QObject::connect(compute_visibility_graph, &QPushButton::released, this, [&]() {
            std::vector<QPolygonF> polygons;

            auto polys = _input->get_polygons();
            for (auto& poly : polys)
            {
                polygons.push_back(poly);
            }

            _result->set_obstacles(polygons);

            std::vector<polygon<qreal>> obstacles;
            for (auto& obstacle : polygons)
            {
                polygon<qreal> p;
                for (auto& v : obstacle)
                {
                    p.push_back(point<qreal, 2>({ v.x(),v.y() }));
                }
                obstacles.push_back(p);
            }

            _step_by_step->set_steps(compute_visibility_graph_visualized(obstacles));
            });
        input_tab->setLayout(input_layout);
        _tabs->addTab(input_tab, "Input");
    }

    // Result tab
    {
        _result = new visibility_graph_result_view({});
        _tabs->addTab(_result, "Result");
    }

    // Step by step tab
    {
        _step_by_step = new stepbystep_view({});
        _tabs->addTab(_step_by_step, "Step by step");
    }
}
