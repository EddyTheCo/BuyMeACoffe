import QtQuick 2.0
import QtQuick.Controls
import BuyMeACoffe


ListView {
    id:outs_
    spacing : 10
    clip:true

    model: Message_model

    delegate: Message_delegate {
        width: outs_.width
    }


}



