import QtQuick 2.0
import QtQuick.Layouts
import QtQuick.Controls

Rectangle
{
    id:root_box
    color:"transparent"
    required property string metadata
    required property string amount
    required property int dheight
    required property int fheight
    property bool fullwindow:false


    MouseArea {
        anchors.fill: parent

        onClicked:
        {
            root_box.fullwindow=!root_box.fullwindow
            root_box.height=(root_box.fullwindow)?root_box.fheight:root_box.dheight
        }
    }
    RowLayout
    {

        spacing: 0
        anchors.fill: root_box

        Rectangle
        {
            id:box_
            Layout.preferredWidth: 200
            Layout.minimumWidth: 100
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft
            color:"transparent"

                Text
                {
                    anchors.bottom: box_.bottom
                    anchors.horizontalCenter: box_.horizontalCenter
                    height:box_.height
                    width:box_.width*0.9
                    clip:true
                    id:met
                    color:"white"
                    text:root_box.metadata
                     horizontalAlignment: TextEdit.AlignJustify
                    font.pointSize: 10+((box_.width<box_.height)?box_.width:box_.height)*0.02
                    wrapMode:Text.WordWrap
                    onLinkActivated: (link) => Qt.openUrlExternally(link)
                }



        }

        Rectangle
        {
            id:box2
            Layout.preferredWidth: 75
            Layout.minimumWidth: 0
            Layout.maximumWidth: 100
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft
            color:"transparent"
            Text
            {
                anchors.centerIn: parent
                elide:Text.ElideRight
                id:amo_
                color:"white"
                text:root_box.amount
                font.pointSize: 7+((box2.width<box_.height)?box2.width:box2.height)*0.1
                wrapMode:Text.WordWrap
                width:parent.width*0.98
                horizontalAlignment: TextEdit.AlignRight
            }
        }




    }
    Rectangle
    {
        id:line_
        width:parent.width*0.95
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        height:1
        color:"white"
    }
}
