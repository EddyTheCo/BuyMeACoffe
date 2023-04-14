#pragma once

#include<QObject>
#include<QString>
#include <QtQml/qqmlregistration.h>

#include<account.hpp>
#include"nodeConnection.hpp"
#include<QHash>

class Monitor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString  addr READ addr NOTIFY addr_changed)
    Q_PROPERTY(QJsonObject  saldo READ saldo NOTIFY saldo_changed)
    QML_ELEMENT
public:
    Monitor(QObject *parent = nullptr);

    QString addr(void)const
    {
        qDebug()<<"add:"<<Node_Conection::rest_client->info()["protocol"].toObject()["bech32Hrp"];
        return qencoding::qbech32::Iota::encode(Node_Conection::rest_client->info()["protocol"].toObject()["bech32Hrp"].toString(),addr_);
    }
    QJsonObject saldo()const;
    void set_address(const std::string addr);
    void set_node(const std::string node_addr);
    void set_properties(const std::string &node_addr,const std::string &addr);

    static Monitor* get_monitor(void){return (monitors.size())?monitors.back():nullptr;}


signals:
    void new_output(QString outid,QJsonObject var);
    void outputChanged(QString);
    void addr_changed();
    void saldo_changed();
    void restarted();

private:
    void restart(void);
    void checkOutput(const Node_output &v);
    void folloup(QString outid);
    quint64 saldo_;
    qblocks::c_array addr_;
    static std::vector<Monitor*> monitors;
    QObject* receiver;
    Node_Conection * nodeconnection_;
    QHash<QString,quint64> hash;

};
