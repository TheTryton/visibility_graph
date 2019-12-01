#include <QtWidgets>
#include "visibility_graph.hpp"
#include <optional>
#include <numeric>
#include <iostream>
#include <stack>
#include <chrono>
#include <random>
#include "skip_list.hpp"

#include <geometric_algorithm/view/input/polygon_input_view.hpp>
#include <geometric_algorithm/view/result/visibility_graph_result_view.hpp>
#include <geometric_algorithm/view/stepbystep/stepbystep.hpp>

floating floating::max_value = floating(1000000000);
floating floating::min_value = floating(-100000000);

using namespace data_structures;
int main(int argc, char** argv)
{
	QApplication application(argc, argv);

    polygon_input_view view_input;
    view_input.show();
    application.exec();

    std::vector<QPolygonF> polygons;

    auto polys = view_input.get_polygons();
    for (auto& poly : polys)
    {
        polygons.push_back(poly);
    }
	
    visibility_graph_result_view view_result{polygons};
    view_result.show();

    stepbystep_view s{ {} };
    s.show();
	application.exec();

	return 0;
}