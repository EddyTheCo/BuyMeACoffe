#pragma once

#include<QObject>
#include<QString>
#include <QtQml/qqmlregistration.h>
#include"client/qclient.hpp"
#include"client/qclientMQTT.hpp"


class Monitor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString  addr READ addr NOTIFY addr_changed)
    Q_PROPERTY(QString  saldo READ saldo NOTIFY saldo_changed)

    QML_ELEMENT
    QML_SINGLETON
public:
    Monitor();
    QString addr(void)const{qDebug()<<"reading address "<<addr_;return addr_;}
    void set_address(const std::string addr);
    void set_node(const std::string node_addr);
    void set_properties(const std::string &node_addr,const std::string &addr);
    static Monitor* get_monitor(void){return (monitors.size())?monitors.back():nullptr;}
    void monitorSaldo(void);
    QString saldo(void)const{return saldo_str;};
signals:
    void new_output(QString message,QString amo);
    void addr_changed(QString addr);
    void node_addr_changed(void);
    void saldo_changed(void);
    void restarted(void);

private:
    void set_node_addr_wss(void);
    void set_saldo(QString saldo_m);
    void checkOutput(const qiota::Node_output & v);
    qiota::Client* iota_client_;
    qiota::ClientMqtt* iota_client_mqtt_;
    quint64 saldo_spent,saldo_;
    QString addr_,saldo_str;
    QUrl node_addr_,node_addr_wss_;
    static std::vector<Monitor*> monitors;

};
