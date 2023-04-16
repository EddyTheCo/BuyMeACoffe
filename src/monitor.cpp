#include"monitor.hpp"
#include <QDebug>
#include<QJsonDocument>


#ifdef USE_EMSCRIPTEN
#include <emscripten/bind.h>
#endif
using namespace qiota::qblocks;
using namespace qiota;
using namespace qencoding::qbech32::Iota;

std::vector<Monitor*> Monitor::monitors=std::vector<Monitor*>();
Monitor::Monitor(QObject *parent):QObject(parent),receiver(nullptr),nodeconnection_(new Node_Conection(this)),saldo_(0)
{
    monitors.push_back(this);
    connect(this,&Monitor::addr_changed,this,&Monitor::restart);
    connect(nodeconnection_,&Node_Conection::stateChanged,this,[=](){
		    if(nodeconnection_->state()==Node_Conection::Connected)
		    {
		    	emit this->addr_changed();
		    }
		    });

}

void Monitor::restart(void)
{

    if(nodeconnection_->state()==Node_Conection::Connected&&!addr_.isNull())
    {
	saldo_=0;    
	emit saldo_changed();
	hash.clear();

        if(receiver)receiver->deleteLater();
        receiver=new QObject(this);

        auto info=Node_Conection::rest_client->get_api_core_v2_info();
        connect(info,&Node_info::finished,receiver,[=]( ){

            auto node_outputs_=new Node_outputs();
            connect(node_outputs_,&Node_outputs::finished,receiver,[=]( ){

                for(const auto& v:node_outputs_->outs_)
                {
                    checkOutput(v);
                }
                node_outputs_->deleteLater();


                auto resp=Node_Conection::mqtt_client->
                        get_outputs_unlock_condition_address("address/"+qencoding::qbech32::Iota::encode(info->bech32Hrp,addr_));

                auto lambda= [=](QJsonValue data){
                    const auto node_output=Node_output(data);
                    checkOutput(node_output);
                };
                connect(resp,&ResponseMqtt::returned,receiver,lambda);
                info->deleteLater();

            });
            Node_Conection::rest_client->get_outputs<qblocks::Output::Basic_typ>
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
    if(qnode_addr.isValid())
    {
        if(qnode_addr!=Node_Conection::rest_client->get_node_address())
        {
            Node_Conection::rest_client->set_node_address(qnode_addr);
        }
    }

}
QJsonObject amountString(const quint64& amount_)
{
    const auto baseToken=Node_Conection::rest_client->info()["baseToken"].toObject();
    const auto unit=baseToken["unit"].toString();
    const auto subunit=baseToken["subunit"].toString();
    const auto decimals=baseToken["decimals"].toInt();

    QJsonObject var;
    if(amount_>std::pow(10,decimals*0.8))
    {
        var.insert("unit",unit);
        var.insert("amount",QString::number(amount_*1.0/std::pow(10,decimals),'g', 5));
        return var;
    }
    else
    {
        var.insert("unit",subunit);
        var.insert("amount",QString::number(amount_,'g', 5));
        return var;
    }
}
QJsonObject Monitor::saldo()const
{
    return amountString(saldo_);
}

void Monitor::checkOutput(const Node_output &v)
{
    const auto eddaddr=qblocks::Address::from_(addr_);
    const auto outid=v.metadata().outputid_.toHexString();

    auto addr_bundle= new address_bundle(eddaddr);
    QString metadata;
    const auto  metfeau=v.output()->get_feature_(qblocks::Feature::Metadata_typ);
    if(metfeau)
    {
        auto metadata_feature=std::dynamic_pointer_cast<qblocks::Metadata_Feature>(metfeau);
        metadata=QString(metadata_feature->data());
    }
    std::vector<Node_output> outs{v};
    addr_bundle->consume_outputs(outs,0);

    if(addr_bundle->amount&&!hash.contains(outid))
    {

        folloup(outid);
        hash[outid]=addr_bundle->amount;
        saldo_+=addr_bundle->amount;
        emit saldo_changed();
        if(!metadata.isNull())
        {
            QJsonObject var;
            var.insert("message",metadata);
            var.insert("baseToken",amountString(addr_bundle->amount));
            emit new_output(outid,var);
        }


    }


}
void Monitor::folloup(QString outid)
{
    auto resp=Node_Conection::mqtt_client->get_outputs_outputId(outid);
    QObject::connect(resp,&ResponseMqtt::returned,receiver,[=](QJsonValue data)
    {

        const auto node_outputs_=Node_output(data);


        if(node_outputs_.metadata().is_spent_)
        {
            saldo_-=hash[outid];
            emit saldo_changed();
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
