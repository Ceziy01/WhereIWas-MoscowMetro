#ifndef MAPSTATION_H
#define MAPSTATION_H

#include <QApplication>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <Qpainter>
#include <QPalette>

#include "Json.h"

class MapStation : public QGraphicsItem {

private:
    QPalette palette = QApplication::palette();
    QVector<QColor> colors;
    QColor hollow_color = palette.color(QPalette::Base);
    bool mcd_branch;
    bool i_was = false;
    bool display_name;
    int width;
    int hyp;
    QString name;
    QPointF pos;
    QPointF label_pos;
    Json *i_was_json;

    QBrush createBrush() {
        QConicalGradient gradient(pos, 0);
        int len = colors.length();
        for (int i = 0; i < len; i++) {
            gradient.setColorAt(double(i) / len, colors[i]);
        }
        gradient.setColorAt(1.0, colors[0]);
        return QBrush(gradient);
    }

public:
    MapStation(QPointF &_pos, QVector<QColor> &_colors, int _width, QPointF &_label_pos, QString &_name, bool _display_name, int &_hyp, bool &_i_was, Json *_i_was_json): i_was_json(_i_was_json) {
        colors = _colors;
        pos = _pos;
        width = _width;
        label_pos = _label_pos;
        name = _name;
        hyp = _hyp;
        i_was = _i_was;
        *i_was_json = *_i_was_json;
        display_name = _display_name;

        hollow_color = i_was ? Qt::green : palette.color(QPalette::Base);
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        if(event->button() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier) {
            hollow_color = i_was ? palette.color(QPalette::Base) : Qt::green;
            i_was = !i_was;
            i_was_json->data()[name] = i_was;
            update();
        } else {
            QGraphicsItem::mousePressEvent(event);
        }
    }

    QRectF boundingRect() const override {
        return QRectF(pos.x() - width / 2, pos.y() - width / 2, width, width);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setPen(Qt::NoPen);
        painter->setBrush(createBrush());
        painter->drawEllipse(QRectF(pos.x() - width / 2, pos.y() - width / 2, width, width));
        painter->setBrush(hollow_color);
        int hw = width - 4;
        painter->drawEllipse(QRectF(pos.x() - (hw / 2), pos.y() - (hw / 2), hw, hw));

        QFont font = painter->font();
        font.setPointSize(6);
        painter->setFont(font);

        if (display_name) {
            QString dname = QString(hyp > 0 ? name.left(hyp) : name);

            if (hyp > 0) {
                painter->setPen(palette.color(QPalette::Text));
                QString d1name = QString(name.right(name.length() - hyp));
                dname = d1name.startsWith(" ") ? dname : (dname.endsWith("-") ? dname : dname + QString("-"));
                d1name = d1name.startsWith(" ") ? d1name.right(d1name.length() - 1) : d1name;
                painter->drawText(label_pos + QPointF(0, 8), d1name);
            }

            painter->setPen(palette.color(QPalette::Text));
            painter->drawText(label_pos, dname);

            painter->setPen(Qt::NoPen);
            painter->setBrush(colors[0]);
        }
    }
};

#endif