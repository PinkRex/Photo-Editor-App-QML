import QtQuick
import QtQuick.Controls.Fusion
import "../components"

Item {
    property alias errorDialog: errorDialog
    property alias lastImageDialog: lastImageDialog
    property alias firstImageDialog: firstImageDialog
    property alias aboutDialog: aboutDialog
    property alias resizeDialog: resizeDialog
    property alias pluginConfirmDialog: pluginConfirmDialog
    property alias disabledDialog: disabledDialog
    property alias cropGuideDialog: cropGuideDialog
    property alias snipWarningDialog: snipWarningDialog
    property alias copySuccessDialog: copySuccessDialog
    property alias exportLogSuccessDialog: exportLogSuccessDialog
    property alias logListEmptyDialog: logListEmptyDialog
    property alias saveSuccessDialog: saveSuccessDialog

    property string exportText: "Export Success."
    property string saveText: "Image saved successfully."

    id: appDialogs

    CustomDialog {
        id: errorDialog
        text: "No image is currently open."
    }

    CustomDialog {
        id: saveSuccessDialog
        text: saveText
        customWidth: 500
    }

    CustomDialog {
        id: copySuccessDialog
        text: "Logs successfully copied to clipboard."
        customWidth: 350
    }

    CustomDialog {
        id: logListEmptyDialog
        text: "Log list is empty."
    }

    CustomDialog {
        id: exportLogSuccessDialog
        text: exportText
        customWidth: 500
    }

    CustomDialog {
        id: disabledDialog
        text: "This plugin is currently disabled."
    }

    CustomDialog {
        id: pluginConfirmDialog
        text: "The original image has been modified. Do you want to overwrite it from the plugin?"
        customWidth: 400
        customHeight: 180
        isPluginConfirm: true
    }

    CustomDialog {
        id: snipWarningDialog
        text: "There is currently an image open. Proceeding with a screenshot<br><br>" +
              "will discard the current image. Are you sure you want to continue?"
        customWidth: 550
        customHeight: 180
        isSnipWarning: true
    }

    CustomDialog {
        id: cropGuideDialog
        text: "Adjust the selection rectangle to define the crop area,<br><br>" +
              "then press 'Crop' again to crop the image."
        customWidth: 450
    }

    CustomDialog {
        id: lastImageDialog
        customWidth: 350
        text: "This is the last image in the folder.<br><br>" +
              "There are no more images to display."
    }

    CustomDialog {
        id: firstImageDialog
        customWidth: 350
        text: "This is the first image in the folder.<br><br>" +
              "There are no previous images to display."
    }

    CustomDialog {
        id: aboutDialog
        customWidth: 500
        customHeight: 230
        text: "<b>Photo Editor</b><br>" +
              "Developed by Nguyễn Minh Đức<br><br>" +
              "This application was developed as a mini project for the VDT2025 program.<br>" +
              "Github Repository: <a href='https://github.com/PinkRex/Photo-Editor-App-QML'>Pinkrex/PhotoEditor</a>"
    }

    ResizeDialog {
        id: resizeDialog
        onOpened: {
            resizeDialog.widthValue = statusController.getWidth()
            resizeDialog.heightValue = statusController.getHeight()
        }
        onAccepted: function (w, h) {
            editController.resize(w, h)
        }
    }
}
