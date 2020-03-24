#include "greenhouse.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GreenHouse w;
    qDebug() << "sizeof(GreenHouse)=" << sizeof (w);
    w.show();
    return a.exec();

}
