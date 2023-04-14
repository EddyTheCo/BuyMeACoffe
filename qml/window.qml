import QtQuick.Controls
import QtQuick
import QtQuick.Layouts
import BuyMeACoffe

ApplicationWindow {
    visible: true
    color:"transparent"
    id:win
    property string addr:Message_model.monitor.addr
    property var saldo:Message_model.monitor.saldo



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
            Layout.maximumWidth: 200
            Layout.maximumHeight: width
            Layout.minimumHeight: width
            Layout.alignment: (grid.columns===2)?(Qt.AlignTop|Qt.AlignLeft):(Qt.AlignTop|Qt.AlignHCenter)
            Layout.fillHeight: true
            Layout.fillWidth: true
            addr_:win.addr
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

                text:"<b>Total:   </b>"+ win.saldo.amount + " " + win.saldo.unit
                fontSizeMode:Text.Fit
                wrapMode:Text.WordWrap

                horizontalAlignment: TextEdit.AlignHCenter

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

