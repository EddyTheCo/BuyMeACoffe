import QtQuick 2.0
import QtQuick.Layouts
import QtQuick.Controls
import BuyMeACoffe



RowLayout
{
    id:root_box
    required property var jsob
    spacing: 20

    FontLoader {
            id: webFont
            source: "qrc:/esterVtech.com/imports/BuyMeACoffe/qml/fonts/DeliciousHandrawn-Regular.ttf"
        }

    Text
    {
        id:met
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignCenter

        text:root_box.jsob.message
        wrapMode:Text.Wrap
        fontSizeMode:Text.Fit
        font: webFont.font
        color:"white"
        horizontalAlignment: TextEdit.AlignLeft
        onLinkActivated: (link) => Qt.openUrlExternally(link)
        textFormat:Text.RichText

    }

    Text
    {
        id:amo_
        Layout.maximumWidth: 100
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignCenter
        text:root_box.jsob.baseToken.amount+" "+root_box.jsob.baseToken.unit
        fontSizeMode:Text.Fit
        font: webFont.font
        color:"#1998ff"
        horizontalAlignment: TextEdit.AlignRight
        wrapMode:Text.WordWrap
    }
}
