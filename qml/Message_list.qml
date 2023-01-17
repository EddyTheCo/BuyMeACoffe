import QtQuick 2.0
import QtQuick.Controls
import BuyMeACoffe

Rectangle
{
    id:outs_

    required property int nelem
    color:"transparent"
    ListView {
        anchors.fill: outs_
        clip:true
        Message_model{
                    id:message_model
                    monitor:Monitor
                }
        model: message_model


        delegate: Message_delegate {
            height:outs_.height/nelem
            width:outs_.width
            dheight:outs_.height/nelem
            fheight:outs_.height
        }


    }

}

