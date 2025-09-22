#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsScene>
#include <QPointF>
#include <QColor>
#include <fstream>
#include <QJsonArray>
#include <QMap>

#include "BranchLabel.h"
#include "Json.h"
#include "MapBranch.h"
#include "MapStation.h"
#include "DotConnector.h"
#include "Connector.h"

class MapScene : public QGraphicsScene {
public:
    using QGraphicsScene::QGraphicsScene;
    MapScene() {
        setSceneRect(0, 0, 1210, 1680);
        int dx = 635;
        int dy = 800;
        Json *branches_json = new Json(":map_data/branches.json");
        Json *stations_json = new Json(":map_data/stations.json");
        Json *dot_connectors_json = new Json(":map_data/dot_connectors.json");
        Json *connectors_json = new Json(":map_data/connectors.json");
        Json *branch_labels_json = new Json(":map_data/branch_labels.json");
        Json *i_was_json = new Json("i_was.json");
        Json *const_color = new Json(":map_data/colors.json");



        QObject::connect(qApp, &QCoreApplication::aboutToQuit, [i_was_json]() {
            i_was_json->save();
        });

        //Цвета
        QJsonObject ccjvr = const_color->data();
        QMap<QString, QColor> clr_c;

        for (auto it = ccjvr.begin(); it != ccjvr.end(); ++it) {
            if (it.value().isArray()) {
                QJsonArray arr = it.value().toArray();
                if (arr.size() >= 3) {
                    int r = arr.at(0).toInt();
                    int g = arr.at(1).toInt();
                    int b = arr.at(2).toInt();
                    clr_c.insert(it.key(), QColor(r, g, b));
                }
            }
        }

        //Имена Веток
        for (QJsonValueRef label : branch_labels_json->data()) {
            QJsonArray pair = label.toObject()["pos"].toArray();
            QPointF pos;
            if (pair.size() == 2) {
                double x = pair[0].toDouble() + dx;
                double y = pair[1].toDouble() + dy;
                pos = QPointF(x, y);
            }

            QString col = label.toObject()["color"].toString();
            QString name = label.toObject()["name"].toString();
            bool mcd = label.toObject()["mcd"].toBool();

            BranchLabel* item = new BranchLabel(pos, clr_c[col], name,mcd);
            addItem(item);
        }

            //Ветки
            for (QJsonValueRef branch : branches_json->data()) {
                QVector<QPointF> points;
                for (const QJsonValue& val : branch.toObject()["points"].toArray()) {
                    QJsonArray pair = val.toArray();
                    if (pair.size() == 2) {
                        double x = pair[0].toDouble() + dx;
                        double y = pair[1].toDouble() + dy;
                        points.append(QPointF(x, y));
                    }
                }

                QString col = branch.toObject()["color"].toString();
                bool mcd = branch.toObject()["mcd"].toBool();

                MapBranch* item = new MapBranch(points, clr_c[col], mcd);
                addItem(item);
            }

            //Точковые коннекторы
            for (QJsonValueRef connector : dot_connectors_json->data()) {
                QJsonArray pair = connector.toObject()["start"].toArray();
                QPointF start;
                if (pair.size() == 2) {
                    double x = pair[0].toDouble() + dx;
                    double y = pair[1].toDouble() + dy;
                    start = QPointF(x, y);
                }

                QJsonArray pair1 = connector.toObject()["end"].toArray();
                QPointF end;
                if (pair1.size() == 2) {
                    double x = pair1[0].toDouble() + dx;
                    double y = pair1[1].toDouble() + dy;
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
                    double x = pair[0].toDouble() + dx;
                    double y = pair[1].toDouble() + dy;
                    start = QPointF(x, y);
                }

                QJsonArray pair1 = connector.toObject()["end"].toArray();
                QPointF end;
                if (pair1.size() == 2) {
                    double x = pair1[0].toDouble() + dx;
                    double y = pair1[1].toDouble() + dy;
                    end = QPointF(x, y);
                }
                QJsonArray pair2 = connector.toObject()["mid"].toArray();
                QPointF mid;
                if (pair2.size() == 2) {
                    double x = pair2[0].toDouble() + dx;
                    double y = pair2[1].toDouble() + dy;
                    mid = QPointF(x, y);
                }

                QString col1 = connector.toObject()["color1"].toString();
                QString col2 = connector.toObject()["color2"].toString();
                bool arc = connector.toObject()["arc"].toBool();

                Connector* item = new Connector(start, end, clr_c[col1], clr_c[col2], arc, mid);
                addItem(item);
            }

            //Станции
            for (QJsonValueRef station : stations_json->data()) {
                QJsonArray pair = station.toObject()["pos"].toArray();
                QPointF pos;
                if (pair.size() == 2) {
                    double x = pair[0].toDouble() + dx;
                    double y = pair[1].toDouble() + dy;
                    pos = QPointF(x, y);
                }

                QJsonArray pair1 = station.toObject()["label_pos"].toArray();
                QPointF label_pos;
                if (pair1.size() == 2) {
                    double x = pair1[0].toDouble();
                    double y = pair1[1].toDouble();
                    label_pos = QPointF(x, y) + pos;
                }

                QVector<QColor> colors;
                for (const QJsonValue& val : station.toObject()["color"].toArray()) {
                    QString col = val.toString();
                    colors.append(clr_c[col]);
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