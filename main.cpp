#include <QApplication>
#include <QPointF>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
    //qWarning() << QPointF(12, 4) / 2;
    QApplication app(argc, argv);
    MainWindow *window = new MainWindow();
    window->show();
    return QApplication::exec();
}
