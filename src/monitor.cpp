#include"monitor.hpp"
#include <QDebug>
#include<QJsonDocument>


#ifdef USE_EMSCRIPTEN
#include <emscripten/bind.h>
#endif
using namespace qiota::qblocks;
using namespace qiota;
using namespace qencoding::qbech32::Iota;

Monitor* Monitor::the_monitor= nullptr;
Monitor::Monitor(QObject *parent):QObject(parent),receiver(new QObject(this)),
    state_(Disconnected),nodeconnection_(new Node_Conection(this)),
    funds_(0)
{
    the_monitor=this;

    connect(this,&Monitor::addr_changed,this,&Monitor::restart);
    connect(nodeconnection_,&Node_Conection::stateChanged,this,[=](){
        emit this->addr_changed();
    });

}

void Monitor::restart(void)
{
    setState(Disconnected);
    if(nodeconnection_->state()==Node_Conection::Connected&&!addr_.isNull())
    {
        setState(Connected);
        funds_=0;
        hash.clear();

        if(receiver)receiver->deleteLater();
        receiver=new QObject(this);

        auto info=nodeconnection_->rest_client->get_api_core_v2_info();
        connect(info,&Node_info::finished,receiver,[=]( ){
            funds_json=info->amount_json(funds_);
            emit fundsChanged();

            auto node_outputs_=new Node_outputs();
            connect(node_outputs_,&Node_outputs::finished,receiver,[=]( ){

                for(const auto& v:node_outputs_->outs_)
                {
                    checkOutput(v);
                }
                node_outputs_->deleteLater();

                auto resp=nodeconnection_->mqtt_client->
                        get_outputs_unlock_condition_address("address/"+qencoding::qbech32::Iota::encode(info->bech32Hrp,addr_));

                auto lambda= [=](QJsonValue data){

                    const auto node_output=Node_output(data);
                    checkOutput(node_output);
                };

                connect(resp,&ResponseMqtt::returned,receiver,lambda);

                info->deleteLater();

            });
            nodeconnection_->rest_client->get_outputs<Output::Basic_typ>
                    (node_outputs_,"address="+qencoding::qbech32::Iota::encode(info->bech32Hrp,addr_));

        });
        emit restarted();
    }
}

void Monitor::set_address(const std::string addr)
{
    const auto qaddr=QString::fromStdString(addr);

    const auto addr_pair=qencoding::qbech32::Iota::decode(qaddr);
    if(addr_pair.second.size()&&addr_pair.second!=addr_)
    {
        addr_=addr_pair.second;
        emit addr_changed();
    }

}

void Monitor::set_node(const std::string node_addr)
{
    const auto qnode_addr=QUrl(QString::fromStdString(node_addr));

    nodeconnection_->set_naddr(qnode_addr);

}
void Monitor::checkOutput(const Node_output &v)
{
    const auto eddaddr=Address::from_array(addr_);
    const auto outid=v.metadata().outputid_.toHexString();

    auto addr_bundle= address_bundle(eddaddr);
    QString metadata;
    const auto  metfeau=v.output()->get_feature_(Feature::Metadata_typ);
    if(metfeau)
    {
        auto metadata_feature=std::static_pointer_cast<const Metadata_Feature>(metfeau);
        metadata=QString(metadata_feature->data());
    }
    std::vector<Node_output> outs{v};
    addr_bundle.consume_outputs(outs,0);
    if(addr_bundle.amount&&!hash.contains(outid))
    {
        folloup(outid);
        hash[outid]=addr_bundle.amount;
        funds_+=addr_bundle.amount;
        auto info=nodeconnection_->rest_client->get_api_core_v2_info();
        QObject::connect(info,&Node_info::finished,receiver,[=]( ){
            funds_json=info->amount_json(this->funds_);
            emit fundsChanged();

            if(!metadata.isNull())
            {
                QJsonObject var;
                var.insert("message",metadata);
                var.insert("baseToken",info->amount_json(addr_bundle.amount));
                emit new_output(outid,var);
            }
            info->deleteLater();
        });
    }
}
void Monitor::folloup(QString outid)
{
    auto resp=nodeconnection_->mqtt_client->get_outputs_outputId(outid);
    QObject::connect(resp,&ResponseMqtt::returned,receiver,[=](QJsonValue data)
    {

        const auto node_outputs_=Node_output(data);


        if(node_outputs_.metadata().is_spent_)
        {
            funds_-=hash[outid];
            auto info=nodeconnection_->rest_client->get_api_core_v2_info();
            QObject::connect(info,&Node_info::finished,receiver,[=]( ){
                funds_json=info->amount_json(this->funds_);
                emit fundsChanged();
                info->deleteLater();
            });
            hash.remove(outid);
            emit outputChanged(outid);
            resp->deleteLater();
        }
    });


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
