import QtQuick 2.15

Rectangle {
    property alias text: textItem.text

    implicitWidth: textItem.width + 10
    implicitHeight: textItem.height + 2
    radius: 2
    color: Qt.rgba(1.0, 1.0, 1.0, 0.6)

    Text {
        id: textItem

        anchors.centerIn: parent
    }
}
