import QtQuick 2.0
import QtQuick.Layouts
import QtQuick.Controls




RowLayout
{
    id:root_box
    required property var jsob
    spacing: 20

    Text
    {
        id:met
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignCenter

        text:root_box.jsob.message
        wrapMode:Text.Wrap
        fontSizeMode:Text.Fit
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
        horizontalAlignment: TextEdit.AlignRight
        wrapMode:Text.WordWrap
    }
}
