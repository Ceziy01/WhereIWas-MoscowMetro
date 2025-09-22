#ifndef BRANCHLABEL_H
#define BRANCHLABEL_H

#include <QApplication>
#include <QFontMetricsF>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <Qpainter>

#include "Json.h"

class BranchLabel : public QGraphicsItem {

private:
    QColor color;
    bool mcd;
    QString name;
    QPointF pos;
    int width;

public:
    BranchLabel(QPointF _pos, QColor _color, const QString &_name, bool _mcd) {
        color = _color;
        pos = _pos;
        name = _name;
        mcd = _mcd;
        width = 14;
    }

protected:
    QRectF boundingRect() const override {
        return QRectF(pos.x() - width / 2, pos.y() - width / 2, width, width);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setPen(Qt::NoPen);
        painter->setBrush(color);

        if (mcd) {
            QPolygonF parallelogram;
            QPointF p1(pos.x() - 6, pos.y() - 5);
            QPointF p2(pos.x() + 9, pos.y() - 5);
            QPointF p3(pos.x() + 6, pos.y() + 5);
            QPointF p4(pos.x() - 9, pos.y() + 5);
            parallelogram << p1 << p2 << p3 << p4;
            painter->drawPolygon(parallelogram);
        } else {
            painter->drawEllipse(QRectF(pos.x() - width / 2, pos.y() - width / 2, width, width));
        }

        int fs = name.length() < 3 ? 6 : 4;
        QFont font("Arial", fs);
        font.setBold(true);
        painter->setFont(font);
        QFontMetricsF met(font);
        QRectF text_rect = met.boundingRect(name);
        text_rect.moveCenter(pos);
        painter->setPen(Qt::white);
        painter->drawText(text_rect, Qt::AlignCenter, name);
    }
};

#endif