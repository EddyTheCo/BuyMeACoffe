#include"client.hpp"
#include <QDebug>
#include<QJsonDocument>
#include"encoding/qbech32.hpp"
#include<QDateTime>

#ifdef USE_EMSCRIPTEN
#include <emscripten/bind.h>
#endif
using namespace qiota::qblocks;
using namespace qiota;
using namespace qencoding::qbech32::Iota;

std::vector<Monitor*> Monitor::monitors=std::vector<Monitor*>();
Monitor::Monitor():node_addr_(""),
    addr_(""),
    node_addr_wss_(""),saldo_(0),saldo_spent(0),
    saldo_str("")
{
    iota_client_=new qiota::Client(node_addr_);
    iota_client_mqtt_=new qiota::ClientMqtt(node_addr_wss_);

    monitors.push_back(this);

    connect(this,&Monitor::addr_changed,this,&Monitor::monitorSaldo);

    connect(this,&Monitor::node_addr_changed,this,&Monitor::monitorSaldo);

    //set_node("https://api.testnet.shimmer.network");
    //set_address("rms1qrzgmpr0lzvqxzu73qakkvg7v2qd2lngkaf64w256c76vepag4sqs27e25s");

}

void Monitor::set_address(const std::string addr)
{
    const auto qaddr=QString::fromStdString(addr);
    if(qaddr!=addr_)
    {
        const auto addr_pair=qencoding::qbech32::Iota::decode(qaddr);
        if(addr_pair.second.size())
        {
            auto info=iota_client_->get_api_core_v2_info();
            QObject::connect(info,&Node_info::finished,iota_client_,[=]( ){

                if(info->bech32Hrp==addr_pair.first)
                {
                    addr_=qaddr;
                    emit addr_changed(addr_);
                }
                info->deleteLater();
            });
        }
    }
}
void Monitor::set_node_addr_wss(void)
{
    node_addr_wss_=node_addr_;
    node_addr_wss_.setScheme("wss");
    node_addr_wss_.setPort(443);
    node_addr_wss_.setPath("/api/mqtt/v1");
    qDebug()<<"set wss: "<<node_addr_wss_;
}
void Monitor::set_node(const std::string node_addr)
{
    const auto qnode_addr=QUrl(QString::fromStdString(node_addr));
    if(qnode_addr.isValid())
    {
        if(qnode_addr!=node_addr_)
        {
            node_addr_=qnode_addr;
            set_node_addr_wss();
            iota_client_mqtt_->set_node_address(node_addr_wss_);
            iota_client_->set_node_address(node_addr_);
            emit node_addr_changed();
        }
    }

}
QString amountString(const quint64& amount_,const Node_info* info)
{
    if(amount_>std::pow(10,info->decimals/2))
    {
        return QString::number(amount_*1.0/std::pow(10,info->decimals),'g',info->decimals/2)+"\n"+info->unit;
    }
    else
    {
        return QString::number(amount_)+"\n"+info->subunit;
    }
}
void Monitor::checkOutput(const qiota::Node_output & v)
{
    const auto cday=QDateTime::currentDateTime().toSecsSinceEpoch();

    qDebug()<<"v.metadata().is_spent_:"<<v.metadata().is_spent_;
    if(v.output()->type_m==qblocks::Output::Basic_typ)
    {
        const auto basic_output_=std::dynamic_pointer_cast<qblocks::Basic_Output>(v.output());

        const auto  stor_unlock=basic_output_->get_unlock_(qblocks::Unlock_Condition::Storage_Deposit_Return_typ);
        quint64 ret_amount=0;
        if(stor_unlock)
        {
            const auto sdruc=std::dynamic_pointer_cast<qblocks::Storage_Deposit_Return_Unlock_Condition>(stor_unlock);
            ret_amount=sdruc->return_amount();
        }
        qDebug()<<"ret_amount:"<<ret_amount;
        const auto expir=basic_output_->get_unlock_(qblocks::Unlock_Condition::Expiration_typ);
        if(expir)
        {
            const auto expiration_cond=std::dynamic_pointer_cast<qblocks::Expiration_Unlock_Condition>(expir);
            const auto unix_time=expiration_cond->unix_time();
            if(unix_time<cday)
            {
                qDebug()<<"Expir unix_time>=cday:";
                qDebug()<<"unix_time:"<<unix_time;
                qDebug()<<"cday:"<<cday;
                return;
            }

        }
        const auto time_lock=basic_output_->get_unlock_(qblocks::Unlock_Condition::Timelock_typ);
        if(time_lock)
        {
            const auto time_lock_cond=std::dynamic_pointer_cast<qblocks::Timelock_Unlock_Condition>(time_lock);
            const auto unix_time=time_lock_cond->unix_time();
            if(cday<unix_time)
            {
                qDebug()<<"time_lock cday<unix_time:";
                return;
            }

        }

        auto info=iota_client_->get_api_core_v2_info();
        connect(info,&Node_info::finished,iota_client_,[=]( ){
            if(v.metadata().is_spent_)
            {
                saldo_spent+=basic_output_->amount()-ret_amount;
                qDebug()<<"saldo_spent+:"<<basic_output_->amount()-ret_amount;
            }
            else
            {
                const auto plus=basic_output_->amount()-ret_amount;
                saldo_+=plus;
                qDebug()<<"plus:"<<plus;
                const auto  metfeau=basic_output_->get_feature_(qblocks::Feature::Metadata_typ);

                if(metfeau)
                {
                    auto metadata_feature=std::dynamic_pointer_cast<qblocks::Metadata_Feature>(metfeau);
                    auto metadata=QString(metadata_feature->data());
                    if(metadata.size())
                    {
                        qDebug()<<"metadata:"<<metadata;
                        qDebug()<<"metadata->data:"<<metadata_feature->data();
                        QString amo=amountString(plus,info);
                        emit new_output(metadata,amo);

                    }

                }
            }
            qDebug()<<"set_saldo:"<<amountString(saldo_-saldo_spent,info);
            set_saldo(amountString(saldo_-saldo_spent,info));
            info->deleteLater();
        });
    }
}
void Monitor::set_saldo(QString saldo_m)
{
    if(saldo_str!=saldo_m)
    {
        saldo_str=saldo_m;
        emit saldo_changed();
    }
}
void Monitor::monitorSaldo(void)
{
    if(node_addr_.isValid()&&addr_.size())
    {
        auto node_outputs_=new Node_outputs();
        iota_client_->get_basic_outputs(node_outputs_,"address="+addr_);

        QObject::connect(node_outputs_,&Node_outputs::finished,iota_client_,[=]( ){
            emit restarted();
            saldo_=0;
            saldo_spent=0;
            for(const auto& v:node_outputs_->outs_)
            {
                checkOutput(v);
            }
            node_outputs_->deleteLater();


            if(iota_client_mqtt_->state()==QMqttClient::Connected)
            {
                qDebug()<<"mqtt get outputs";
                auto resp=iota_client_mqtt_->
                        get_outputs_unlock_condition_address("address/"+addr_);
                connect(resp,&ResponseMqtt::returned,this,[=](QJsonValue data){
                    const auto node_output=Node_output(data);
                    qDebug()<<"got saldo";
                    checkOutput(node_output);
                });
                auto resp2=iota_client_mqtt_->
                        get_outputs_unlock_condition_address("address/"+addr_+"/spent");

                connect(resp2,&ResponseMqtt::returned,this,[=](QJsonValue data){
                    qDebug()<<"got spent";
                    const auto node_output=Node_output(data);
                    checkOutput(node_output);
                });
            }

        });
    }
}
void Monitor::set_properties(const std::string &node_addr,const std::string &addr)
{
    set_node(node_addr);
    set_address(addr);
}

#ifdef USE_EMSCRIPTEN
EMSCRIPTEN_BINDINGS(Monitor) {
    emscripten::class_<Monitor>("Monitor")
            .function("set_properties", &Monitor::set_properties)
            .class_function("get_monitor", &Monitor::get_monitor, emscripten::allow_raw_pointers());
}
#endif
