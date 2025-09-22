#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWheelEvent>
#include <QGraphicsView>

class MapView : public QGraphicsView {
private:
    const double scaleStep = 1.15;
    const double maxScale = 5.0;
    const double minScale = 0.7;
    double currentScaleFactor;
public:
    using QGraphicsView::QGraphicsView;
    MapView(QGraphicsScene *scene, QWidget *parent = nullptr) {
        setAlignment( Qt::AlignLeft | Qt::AlignTop );
        setScene(scene);
        setRenderHint(QPainter::Antialiasing);
        setDragMode(QGraphicsView::ScrollHandDrag);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setCursor(Qt::CursorShape::BlankCursor);
    }
protected:
    void wheelEvent(QWheelEvent *event) override {
        double currentScale = transform().m11();
        if (event->angleDelta().y() > 0 && currentScale < maxScale) {
            scale(scaleStep, scaleStep);
        } else if (event->angleDelta().y() < 0 && currentScale > minScale) {
            scale(1.0 / scaleStep, 1.0 / scaleStep);
        }
        event->accept();
    }
};

#endif