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
    Q_PROPERTY(QJsonObject  funds READ funds NOTIFY fundsChanged)
    Q_PROPERTY(ConState  state READ state NOTIFY stateChanged)
    QML_ELEMENT
public:
    Monitor(QObject *parent = nullptr);
    enum ConState {
        Disconnected = 0,
        Connected
    };
    Q_ENUM(ConState)
    ConState state(void){return state_;}
    void setState(ConState state_m){if(state_!=state_m){state_=state_m;emit stateChanged();}}
    QString addr(void)const
    {
        return qencoding::qbech32::Iota::encode(Node_Conection::rest_client->info()["protocol"].toObject()["bech32Hrp"].toString(),addr_);
    }

    void set_address(const std::string addr);
    void set_node(const std::string node_addr);
    void set_properties(const std::string &node_addr,const std::string &addr);

    static Monitor* get_monitor(void){return the_monitor;}
    QJsonObject funds(void)const{return funds_json;}

signals:
    void new_output(QString outid,QJsonObject var);
    void outputChanged(QString);
    void addr_changed();
    void fundsChanged();
    void restarted();
    void stateChanged();

private:
    ConState state_;
    void restart(void);
    void checkOutput(const Node_output &v);
    void folloup(QString outid);
    quint64 funds_;
    QJsonObject funds_json;
    qblocks::c_array addr_;
    static Monitor* the_monitor;
    QObject* receiver;
    Node_Conection * nodeconnection_;
    QHash<QString,quint64> hash;

};
