#ifndef JSON_H
#define JSON_H

#include <QJsonObject>
#include <QFile>

class Json {
private:
    QJsonObject object;
    QString path;
public:
    Json(QString _path) {
        object = load(_path);
        path = _path;
    }

    QJsonObject &data() {
        return object;
    }

    void save() {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "WhereIWas: cannot open file: " << path;
        }
        QJsonDocument doc(object);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }

    QJsonObject load(QString _path) {
        QFile file((_path));
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "WhereIWas: cannot open file: " << _path;
        }
        QByteArray bytes = file.readAll();
        file.close();
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(bytes, &parseError);
        return doc.object();
    }
};

#endif
