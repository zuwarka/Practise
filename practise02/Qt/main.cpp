#include "startwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    startwindow w;
    w.setWindowTitle("Теплица Mark I");
    w.show();

    return a.exec();
}

