import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls.Fusion

Item {
    property alias chooseImageDialog: chooseImageDialog
    property alias chooseDllsDialog: chooseDllsDialog
    property alias saveImageDialog: saveImageDialog

    id: appFileDialogs

    FileDialog {
        id: chooseImageDialog
        title: "Open Image"
        fileMode: FileDialog.OpenFile
        modality: Qt.ApplicationModal
        nameFilters: ["Images file (*.jpg *.png)"]

        onAccepted: {
            const localPath = chooseImageDialog.selectedFile.toString().replace("file://", "");
            fileController.openImage(localPath);
        }
    }

    FileDialog {
        id: chooseDllsDialog
        title: "Load Plugins"
        fileMode: FileDialog.OpenFiles
        modality: Qt.ApplicationModal
        nameFilters: [".So files (*.so)"]

        onAccepted: {
            const filePaths = chooseDllsDialog.selectedFiles.map(f => f.toString().replace("file://", ""));
            pluginController.loadPluginsManually(filePaths);
        }
    }

    FileDialog {
        id: saveImageDialog
        title: "Save Image As"
        fileMode: FileDialog.SaveFile
        modality: Qt.ApplicationModal
        nameFilters: ["Images file (*.jpg *.png)"]

        onAccepted: {
            const localPath = saveImageDialog.selectedFile.toString().replace("file://", "");
            fileController.saveImageAs(localPath);
        }
    }
}
