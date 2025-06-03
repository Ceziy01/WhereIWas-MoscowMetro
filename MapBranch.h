#include <QGraphicsItem>
#include <QPen>
#include <QApplication>

#ifndef MAPBRANCH_H
#define MAPBRANCH_H

class MapBranch : public QGraphicsPathItem {

private:
    QPainterPath path;
    QPen pen;
    bool mcd_branch;

public:
    MapBranch(QVector<QPointF> &points, QColor &color, bool mcd) {
        pen.setColor(color);
        pen.setWidth(4);
        mcd_branch = mcd;

        path.moveTo(points[0]);

        for (int i = 1; i < points.size(); i++) {
            path.lineTo(points[i]);
        }
    }

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override {
        painter->setPen(pen);
        painter->drawPath(path);
        if (mcd_branch) {
            QPalette palette = QApplication::palette();
            painter->setPen(QPen(palette.color(QPalette::Base), 2));
            painter->drawPath(path);
        }
    }

    QRectF boundingRect() const override {
        return path.boundingRect();
    }
};

#endif
