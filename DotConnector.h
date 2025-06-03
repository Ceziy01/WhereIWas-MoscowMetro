#include <QGraphicsItem>

#ifndef DOTCONNECTOR_H
#define DOTCONNECTOR_H

class DotConnector : public QGraphicsItem {
private:
    int radius = 1;
    int step = 4;
    QColor color = Qt::lightGray;
    QPointF start;
    QPointF end;

public:
    DotConnector(const QPointF &_start, const QPointF &_end)
        : start(_start), end(_end)
    {
        if (step <= 0) step = 1;
    }

    QRectF boundingRect() const override {
        qreal minX = std::min(start.x(), end.x()) - radius;
        qreal minY = std::min(start.y(), end.y()) - radius;
        qreal maxX = std::max(start.x(), end.x()) + radius;
        qreal maxY = std::max(start.y(), end.y()) + radius;
        return QRectF(minX, minY, maxX - minX, maxY - minY);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override {
        QPointF delta = end - start;
        qreal length = sqrt(delta.x() * delta.x() + delta.y() * delta.y());
        if (length <= 0) return;

        int dotCount = qRound(length / step);
        if (dotCount <= 0) dotCount = 1;

        painter->setBrush(color);
        painter->setPen(Qt::NoPen);

        for (int i = 0; i <= dotCount; ++i) {
            qreal ratio = static_cast<qreal>(i) / dotCount;
            QPointF dotPos = start + ratio * delta;
            painter->drawEllipse(dotPos, radius, radius);
        }

        if (dotCount < 5) {
            painter->drawEllipse(QPointF((start.x() + end.x()) / 2, (start.y() + end.y()) / 2), radius, radius);
        }
    }
};

#endif