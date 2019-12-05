#include <geometric_algorithm/view/interactive/interactive.hpp>

int main(int argc, char** argv)
{
    QApplication application(argc, argv);

    interactive_view view;
    view.show();

    application.exec();

    return 0;
}

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
    return main(0, nullptr);
}
