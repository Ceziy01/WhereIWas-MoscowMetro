#include <QVBoxLayout>

#include "MainWindow.h"
#include "MapView.h"
#include "MapScene.h"

MainWindow::MainWindow() {
    resize(600, 480);
    MapScene *scene = new MapScene();
    MapView *view = new MapView(scene);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(view);
}