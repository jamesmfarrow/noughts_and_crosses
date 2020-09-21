#include "mainwindow.h"

#include <QApplication>
#include "tictactoe.hpp"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();

    controller c;

    return a.exec();
}
