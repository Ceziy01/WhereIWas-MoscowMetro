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

        //path.moveTo(points[0]);

        //for (int i = 1; i < points.size(); i++) {
        //    path.lineTo(points[i]);
        //}
        path.moveTo(points[0]);

        for (int i = 1; i < points.size() - 1; ++i) {
            QPointF mid = (points[i] + points[i + 1]) / 2.0;
            path.quadTo(points[i], mid);
        }

        path.lineTo(points.back());
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

    void addCatmullRom(QPainterPath &path, const QVector<QPointF> &points) {
        if (points.size() < 2) return;

        path.moveTo(points[0]);

        for (int i = 0; i < points.size() - 1; i++) {
            QPointF p0 = (i > 0) ? points[i - 1] : points[i];
            QPointF p1 = points[i];
            QPointF p2 = points[i + 1];
            QPointF p3 = (i < points.size() - 2) ? points[i + 2] : p2;

            double t = 0.5;

            QPointF c1 = p1 + (p2 - p0) * t / 3.0;
            QPointF c2 = p2 - (p3 - p1) * t / 3.0;

            path.cubicTo(c1, c2, p2);
        }
    }
};

#endif
