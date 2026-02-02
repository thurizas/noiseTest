#include "noiseTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    noiseTest w(600,450);
    w.show();

    int nRes = a.exec();
    return nRes;
}
