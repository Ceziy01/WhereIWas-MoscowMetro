#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsScene>
#include <QPointF>
#include <QColor>
#include <fstream>
#include <QJsonArray>

#include "Json.h"
#include "MapBranch.h"
#include "MapStation.h"
#include "DotConnector.h"
#include "Connector.h"

class MapScene : public QGraphicsScene {
public:
    using QGraphicsScene::QGraphicsScene;
    MapScene() {
        setSceneRect(0, 0, 1050, 1330);
        Json *branches_json = new Json(":map_data/branches.json");
        Json *stations_json = new Json(":map_data/stations.json");
        Json *dot_connectors_json = new Json(":map_data/dot_connectors.json");
        Json *connectors_json = new Json(":map_data/connectors.json");
        Json *i_was_json = new Json("i_was.json");

        QObject::connect(qApp, &QCoreApplication::aboutToQuit, [i_was_json]() {
            i_was_json->save();
        });

        //Ветки
        for (QJsonValueRef branch : branches_json->data()) {
            QVector<QPointF> points;
            for (const QJsonValue& val : branch.toObject()["points"].toArray()) {
                QJsonArray pair = val.toArray();
                if (pair.size() == 2) {
                    double x = pair[0].toDouble() + 500;
                    double y = pair[1].toDouble() + 640;
                    points.append(QPointF(x, y));
                }
            }

            QJsonArray col_arr = branch.toObject()["color"].toArray();
            QColor color = QColor(col_arr[0].toInt(), col_arr[1].toInt(), col_arr[2].toInt());

            bool mcd = branch.toObject()["mcd"].toBool();

            MapBranch* item = new MapBranch(points, color, mcd);
            addItem(item);
        }

        //Точковые коннекторы
        for (QJsonValueRef connector : dot_connectors_json->data()) {
            QJsonArray pair = connector.toObject()["start"].toArray();
            QPointF start;
            if (pair.size() == 2) {
                double x = pair[0].toDouble() + 500;
                double y = pair[1].toDouble() + 640;
                start = QPointF(x, y);
            }

            QJsonArray pair1 = connector.toObject()["end"].toArray();
            QPointF end;
            if (pair1.size() == 2) {
                double x = pair1[0].toDouble() + 500;
                double y = pair1[1].toDouble() + 640;
                end = QPointF(x, y);
            }

            DotConnector* item = new DotConnector(start, end);
            addItem(item);
        }

        //Прямые Коннекторы
        for (QJsonValueRef connector : connectors_json->data()) {
            QJsonArray pair = connector.toObject()["start"].toArray();
            QPointF start;
            if (pair.size() == 2) {
                double x = pair[0].toDouble() + 500;
                double y = pair[1].toDouble() + 640;
                start = QPointF(x, y);
            }

            QJsonArray pair1 = connector.toObject()["end"].toArray();
            QPointF end;
            if (pair1.size() == 2) {
                double x = pair1[0].toDouble() + 500;
                double y = pair1[1].toDouble() + 640;
                end = QPointF(x, y);
            }
            QJsonArray pair2 = connector.toObject()["mid"].toArray();
            QPointF mid;
            if (pair2.size() == 2) {
                double x = pair2[0].toDouble() + 500;
                double y = pair2[1].toDouble() + 640;
                mid = QPointF(x, y);
            }

            QJsonArray col_arr1 = connector.toObject()["color1"].toArray();
            QColor color1 = QColor(col_arr1[0].toInt(), col_arr1[1].toInt(), col_arr1[2].toInt());
            QJsonArray col_arr2 = connector.toObject()["color2"].toArray();
            QColor color2 = QColor(col_arr2[0].toInt(), col_arr2[1].toInt(), col_arr2[2].toInt());
            bool arc = connector.toObject()["arc"].toBool();

            Connector* item = new Connector(start, end, color1, color2, arc, mid);
            addItem(item);
        }

        //Станции
        for (QJsonValueRef station : stations_json->data()) {
            QJsonArray pair = station.toObject()["pos"].toArray();
            QPointF pos;
            if (pair.size() == 2) {
                double x = pair[0].toDouble() + 500;
                double y = pair[1].toDouble() + 640;
                pos = QPointF(x, y);
            }

            QJsonArray pair1 = station.toObject()["label_pos"].toArray();
            QPointF label_pos;
            if (pair1.size() == 2) {
                double x = pair1[0].toDouble() + 500;
                double y = pair1[1].toDouble() + 640;
                label_pos = QPointF(x, y);
            }

            QVector<QColor> colors;
            for (const QJsonValue& val : station.toObject()["color"].toArray()) {
                QJsonArray triple = val.toArray();
                if (triple.size() == 3) {
                    int r = triple[0].toInt();
                    int g = triple[1].toInt();
                    int b = triple[2].toInt();
                    colors.append(QColor(r, g, b));
                }
            }

            QString name = station.toObject()["name"].toString();

            int width = station.toObject()["width"].toInt() * 1.7;
            int hyp = station.toObject()["label_hyp"].toInt();

            bool i_was_b = i_was_json->data()[name].toBool();
            bool display_name = station.toObject()["display_name"].toBool();

            MapStation* item = new MapStation(pos, colors, width, label_pos, name, display_name, hyp, i_was_b, i_was_json);
            addItem(item);
        }
    }
};

#endif