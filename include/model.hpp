#pragma once

#include <QAbstractListModel>
#include<QObject>
#include <QtQml/qqmlregistration.h>
#include <QAbstractItemModel>
#include <monitor.hpp>

class Message_box : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QJsonObject jsob READ jsob CONSTANT)

public:
    Message_box(QString outid_m,QJsonObject jsob_m):jsob_(jsob_m),outid_(outid_m)
    {};
    QJsonObject jsob()const{return jsob_;}

    const QString outid_;
private:
    const QJsonObject jsob_;


};

class Message_model : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(Monitor*  monitor READ monitor CONSTANT)
    QML_ELEMENT
    QML_SINGLETON

public:
    enum ModelRoles {
        jsobRole = Qt::UserRole + 1
    };
    int count() const;
    explicit Message_model(QObject *parent = nullptr);
    Monitor* monitor()const{return monitor_;}

    void add_message(QString outid,QJsonObject var);
    void rm_message(QString outid);
    void reset(void);


    int rowCount(const QModelIndex &p) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;



signals:
    void countChanged(int count);


private:
    int m_count;
    QList<Message_box*> m_messages;
    Monitor* monitor_;
};

