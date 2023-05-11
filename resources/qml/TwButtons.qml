import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import com.dln.twtypes 1.0

RowLayout {
    QtObject {
        id: internal

        function handleValue(value) {
            return (value instanceof Function) ? value() : value
        }

        function handleEnum(idleValue, inProgressValue, pausedValue, finishedValue) {
            switch (_twController.readingStatus) {
            case TwTypes.Idle: return handleValue(idleValue)
            case TwTypes.InProgress: return handleValue(inProgressValue)
            case TwTypes.Paused: return handleValue(pausedValue)
            case TwTypes.Finished: return handleValue(finishedValue)
            default: return console.warn("wrong value")
            }
        }
    }

    Button {
        Layout.fillWidth: true

        text: internal.handleEnum(_twController.fileUrl.toString() ? qsTr("Open another file") : qsTr("Open file"),
                                  qsTr("Stop"),
                                  qsTr("Stop"),
                                  qsTr("Clear"))

        ToolTip.visible: hovered
        ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
        ToolTip.text: internal.handleEnum(qsTr("Open a file for analysis"),
                                          qsTr("Stop the analysis and clear the statistics"),
                                          qsTr("Stop the analysis and clear the statistics"),
                                          qsTr("Clear the statistics"))

        onClicked: internal.handleEnum(() => { TwDialogs.openFileDialog.openDialog((source) => { _twController.fileUrl = source }) },
                                       _twController.stop,
                                       _twController.stop,
                                       _twController.clear)
    }

    Button {
        Layout.fillWidth: true

        enabled: _twController.fileUrl.toString()
        text: internal.handleEnum(qsTr("Start"),
                                  qsTr("Pause"),
                                  qsTr("Resume"),
                                  qsTr("Restart"))

        ToolTip.visible: hovered
        ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
        ToolTip.text: internal.handleEnum(qsTr("Start the analysis"),
                                          qsTr("Pause the analysis"),
                                          qsTr("Resume the analysis"),
                                          qsTr("Restart the analysis"))

        onClicked: internal.handleEnum(_twController.start,
                                       _twController.pause,
                                       _twController.resume,
                                       _twController.start)
    }
}
