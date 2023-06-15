import QtQuick.Controls
import QtQuick
import QtQuick.Layouts
import BuyMeACoffe

ApplicationWindow {
    visible: true
    color:"#10141c"


    id:win
    property string addr:Message_model.monitor.addr
    FontLoader {
        id: webFont
        source: "qrc:/esterVtech.com/imports/BuyMeACoffe/qml/fonts/DeliciousHandrawn-Regular.ttf"
    }

    GridLayout
    {
        id:grid
        anchors.fill: parent
        columns: parent.width > 350 ? 2 : 1
        rows : parent.width > 350 ? 1 : 2

        MyAddressQr
        {
            id: qr_back
            Layout.minimumWidth: (grid.columns===2)?100:50
            Layout.maximumWidth: 150
            Layout.maximumHeight: width
            Layout.minimumHeight: width
            Layout.alignment: (grid.columns===2)?(Qt.AlignTop|Qt.AlignLeft):(Qt.AlignTop|Qt.AlignHCenter)
            Layout.fillHeight: true
            Layout.fillWidth: true
            address:win.addr
            color:"white"
            radius: 10
            visible: Message_model.monitor.state

        }

        ColumnLayout
        {
            id:right_
            Layout.minimumWidth: 100
            Layout.minimumHeight: 100
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: (grid.columns===2)?(Qt.AlignTop|Qt.AlignLeft):(Qt.AlignTop|Qt.AlignHCenter)
            spacing:0

            RowLayout
            {
                Layout.fillWidth: true
                Layout.fillHeight:  true
                Layout.maximumHeight: 40
                Layout.alignment:Qt.AlignCenter
                Text
                {
                    id:tamo_
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignCenter
                    text:(Message_model.monitor.state)?"<b>Total:   </b>":"Waiting for the node"
                    horizontalAlignment:(Message_model.monitor.state)?Text.AlignRight:Text.AlignHCenter
                    color: CustomStyle.frontColor1
                    font: webFont.font
                    fontSizeMode:Text.Fit
                }
                AmountText
                {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignCenter
                    jsob:Message_model.monitor.funds
                    horizontalAlignment:Text.AlignLeft
                    fontSizeMode:Text.Fit
                    font: webFont.font
                    visible:(Message_model.monitor.state)
                }

            }

            Rectangle
            {
                id:line
                Layout.fillWidth: true
                Layout.preferredHeight: 2
                Layout.maximumWidth: parent.width*0.9
                Layout.alignment: Qt.AlignCenter
                color: CustomStyle.frontColor2
            }


            Message_list
            {
                id:list_
                Layout.alignment: Qt.AlignTop
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }

    }

}

