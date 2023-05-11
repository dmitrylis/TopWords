import QtQuick 2.15
import QtQuick.Controls 2.15

import com.dln.twtypes 1.0

ListView { // It could be just a Row with a Repeater
    id: root

    QtObject {
        id: internal

        function randomColor(seed) { // better to move to C++ side
            let color = Math.floor((Math.abs(Math.sin(seed) * 16777215))).toString(16)
            while (color.length < 6) {
                color = '0' + color
            }
            return  '#' + color
        }
    }

    implicitWidth: 300
    implicitHeight: 300
    interactive: false
    orientation: ListView.Horizontal

    model: _twController.wordsModel

    delegate: Item {
        width: ListView.view.width / ListView.view.count
        height: ListView.view.height

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            height: parent.height * model.CountRole / _twController.wordsModel.maxWordCount
            color: internal.randomColor(model.index + 1)
            opacity: ma.containsMouse ? 0.6 : 1.0

            MouseArea {
                id: ma

                anchors.fill: parent
                hoverEnabled: true

                ToolTip.visible: containsMouse
                ToolTip.text: "Word: \"%1\", count: %2".arg(model.WordRole).arg(model.CountRole)
            }
        }

        TwContrastLabel {
            anchors {
                left: parent.horizontalCenter
                bottom: parent.bottom
            }
            transformOrigin: Item.Left
            rotation: -90
            text: "%1 : %2".arg(model.WordRole).arg(model.CountRole)
            visible: height < parent.width
        }
    }

    Text {
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.Wrap

        text: {
            if (!_twController.fileUrl.toString()) {
                return qsTr("Click \"Open\" button in order to open a file for the analysis")
            }

            if (_twController.readingStatus !== TwTypes.InProgress) {
                return qsTr("Click \"Start\" button in order to analyze \"%1\" file").arg(_twController.fileName(_twController.fileUrl))
            }

            return ""
        }
        visible: root.count === 0
    }
}
