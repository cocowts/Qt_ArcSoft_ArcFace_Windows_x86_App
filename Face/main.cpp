#include "Wigdet.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Wigdet w;
    w.show();

    return a.exec();
}
