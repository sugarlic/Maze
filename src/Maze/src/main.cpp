#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    s21::Maze *maze = new s21::Maze();
    MainWindow w(nullptr, maze);
    w.show();
    return a.exec();
}
