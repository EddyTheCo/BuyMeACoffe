#pragma once

#include <QAbstractListModel>
#include<QObject>
#include <QtQml/qqmlregistration.h>
#include <QAbstractItemModel>
#include <client.hpp>

class Message_box : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString metadata READ metadata CONSTANT)
    Q_PROPERTY(QString amount READ amount CONSTANT)

public:
    Message_box(QString metadata_m,QString amount_m):metadata_(metadata_m),amount_(amount_m)
    {qDebug()<<"creting message box with metadata:"<<metadata_;
     qDebug()<<"with amount:"<<amount_;
    };
    QString metadata(void)const{return metadata_;};
    QString amount(void)const{return amount_;};

private:
    QString metadata_,amount_;

};

class Message_model : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(Monitor* monitor  READ monitor  WRITE set_monitor NOTIFY monitor_changed)

public:
    enum ModelRoles {
        metadataRole = Qt::UserRole + 1, amountRole
    };
    int count() const;
    explicit Message_model(QObject *parent = nullptr);

    void add_message(const QString message,const QString amo);
    void reset(void);

    Monitor* monitor(void)const{return monitor_;}
    void set_monitor(Monitor* monitor_m){monitor_=monitor_m;emit monitor_changed();}

    int rowCount(const QModelIndex &p) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;



signals:
    void countChanged(int count);
    void monitor_changed(void);

private:
    int m_count;
    QList<Message_box*> m_messages;
    Monitor* monitor_;
};

