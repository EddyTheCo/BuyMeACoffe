import QtQuick.Controls
import QtQuick
import QtQuick.Layouts
import BuyMeACoffe

ApplicationWindow {
    visible: true
    color:"#10141c"

    id:win
    property string addr:Message_model.monitor.addr
    property var saldo:Message_model.monitor.saldo
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
            addr_:win.addr
            color:"white"
            radius: 10

        }

        ColumnLayout
        {
            id:right_
            Layout.minimumWidth: 50
            Layout.minimumHeight: 50
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: (grid.columns===2)?(Qt.AlignTop|Qt.AlignLeft):(Qt.AlignTop|Qt.AlignHCenter)
            spacing:0
            Text
            {
                id:tamo_
                Layout.minimumWidth: 75
                Layout.minimumHeight: 35
                Layout.maximumHeight: 50
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop
                text:"<b>Total:   </b> <font color=\"#1998ff\">"+ win.saldo.amount  +" "+win.saldo.unit +"</font>"
                fontSizeMode:Text.Fit
                wrapMode:Text.WordWrap
                color:"white"
                font: webFont.font
                horizontalAlignment: TextEdit.AlignHCenter
                Rectangle
                {
                    id:line
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    width:parent.width*0.9
                    height:2
                    color:"#1998ff"
                }

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

