#include "application.hpp"
#include <QtWidgets>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    App my_app;
    my_app.run();

    return a.exec();
}