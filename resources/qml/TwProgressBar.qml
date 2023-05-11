import QtQuick 2.15
import QtQuick.Templates 2.15 as T

T.ProgressBar {
    id: root

    implicitWidth: 200
    implicitHeight: 28

    background: Rectangle {
        color: "white"
    }

    contentItem: Item {
        Rectangle {
            width: parent.width * root.position
            height: parent.height
            color: root.position === 0.0 ? "white" : "green"
        }

        TwContrastLabel {
            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
                rightMargin: 5
            }
            text: Math.round(root.position * 100) + "%"
        }
    }
}
