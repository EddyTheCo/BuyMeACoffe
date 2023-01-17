import QtQuick.Controls
import BuyMeACoffe
import QtQuick
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    color:"transparent"
    width: 500;
    height:200;
    Rectangle
    {
        width: parent.width
        height: img.height*1.3
        anchors.centerIn: parent
        color:"#0f171e"
        border.color: "white"
        border.width: 1
        radius:10

        RowLayout
        {
            anchors.fill: parent
            spacing: 0

            Rectangle
            {
                id: qr_back
                Layout.preferredWidth: 150
                Layout.minimumWidth: 100
                Layout.maximumWidth: 200
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight
                Layout.maximumHeight: 400
                Layout.minimumHeight: 100
                Layout.fillHeight: true
                color:"white"
                Image {
                    id:img
                    anchors.centerIn:qr_back
                    sourceSize.width: ((qr_back.width<qr_back.height)?qr_back.width:qr_back.height)-5
                    source: "image://qrcodeblack/"+Monitor.addr
                    ToolTip
                    {
                        id:tooltip
                        visible: false
                        text:qsTr("Copy")
                    }
                }

                TextEdit{
                    id: textEdit
                    visible: false
                }
                MouseArea {
                    anchors.fill: img
                    hoverEnabled :true
                    onEntered: tooltip.visible=!tooltip.visible
                    onExited: tooltip.visible=!tooltip.visible
                    onClicked:
                    {
                        console.log("clicked",Monitor.addr)
                        textEdit.text = Monitor.addr
                        textEdit.selectAll()
                        textEdit.copy()
                    }
                }
            }

            Rectangle
            {
                id:right_
                color:"transparent"
                Layout.preferredWidth: 250
                Layout.minimumWidth: 200
                Layout.minimumHeight: 100
                Layout.maximumHeight: 400

                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop
                Text
                {
                    id:tamo_
                    anchors.top: right_.top
                    anchors.horizontalCenter: right_.horizontalCenter
                    color:"white"
                    text:"<b>Total:   </b>"+ Monitor.saldo
                    font.pointSize: 16
                    wrapMode:Text.WordWrap
                    width:right_.width*0.98
                    horizontalAlignment: TextEdit.AlignHCenter

                }
                Message_list
                {
                    id:list_
                    nelem:3
                    width:right_.width*0.95
                    anchors.top: tamo_.bottom
                    anchors.horizontalCenter: right_.horizontalCenter
                    height: (right_.height-tamo_.height)*0.98
                }
            }

        }

    }




}

