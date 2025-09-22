#include <QGraphicsItem>
#include <QPen>
#include <QApplication>
#include <qpainter.h>
#include <QLinearGradient>

#ifndef CONNECTOR_H
#define CONNECTOR_H

class Connector : public QGraphicsPathItem {

private:
    QColor color1, color2;
    QPointF start, mid, end;
    bool arc;
    QPainterPath path;

public:
    Connector(const QPointF &_start, const QPointF &_end, const QColor &_color1, const QColor &_color2, const bool &_arc, const QPointF &_mid)
        : start(_start), end(_end), color1(_color1), color2(_color2), arc(_arc), mid(_mid) {

        if (arc) {
            path.moveTo(start);

            // Вычисляем направляющие векторы
            QPointF vec1 = mid - start;
            QPointF vec2 = end - mid;

            // Контрольные точки делаем более длинными для плавности
            QPointF ctrl1 = start + vec1 * 0.5;  // Увеличили с 0.7 до 0.8
            QPointF ctrl2 = end - vec2 * 0.5;  // Увеличили с 0.7 до 0.8

            path.cubicTo(ctrl1, ctrl2, end);
        } else {
            path.moveTo(start);
            path.lineTo(end);
        }
    }

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override {
        QPointF dir = end - start;
        qreal length = std::sqrt(dir.x() * dir.x() + dir.y() * dir.y());
        QPointF unit = dir / length;

        QPointF offset = unit * 5;
        QPointF newP1 = start + offset;
        QPointF newP2 = end - offset;
        QLinearGradient gradient(newP1, newP2);
        gradient.setColorAt(0, color1);
        gradient.setColorAt(1, color2);
        painter->setPen(QPen(QBrush(gradient), 4));
        painter->drawPath(path);
    }

    QRectF boundingRect() const override {
        return path.boundingRect();
    }
};

#endif
