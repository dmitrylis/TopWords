import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: _twController.fileUrl.toString() ? qsTr("Top Words (%1)").arg(_twController.fileName(_twController.fileUrl))
                                            : qsTr("Top Words")

    menuBar: TwButtons {
        width: parent.width
    }

    TwBarChart {
        anchors.fill: parent
    }

    footer: TwProgressBar {
        width: parent.width
        value: _twController.readingProgress
    }
}
