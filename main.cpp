#include <QApplication>
#include <QPointF>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "Russian");
    QApplication app(argc, argv);
    MainWindow *window = new MainWindow();
    window->show();
    return QApplication::exec();
}
