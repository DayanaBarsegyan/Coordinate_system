#include "coordsystem.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    CoordSystem* s = new CoordSystem;
    s->show();
    return app.exec();
}
