pragma Singleton

import QtQuick 2.15
import QtQuick.Dialogs 1.3 as Dialogs1

QtObject {
    property Dialogs1.FileDialog openFileDialog: Dialogs1.FileDialog {
        property var acceptAction: (source) => {}

        function openDialog(acceptAction) {
            openFileDialog.acceptAction = acceptAction
            open()
        }

        title: "Please choose a txt file"
        folder: shortcuts.documents
        nameFilters: [ "Text files (*.txt)" ]
        modality: Qt.ApplicationModal

        onAccepted: {
            const files = fileUrls
            if (files.length > 0) {
                openFileDialog.acceptAction(files[0])
            }
        }
    }
}
