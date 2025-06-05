import QtQuick
import QtQuick.Controls.Fusion

Item {
    property var appDialogs
    property var snipOverlay

    id: appConnections

    Connections {
        target: viewController
        function onShowLastImageDialog() {
            appDialogs.lastImageDialog.open();
        }

        function onShowFirstImageDialog() {
            appDialogs.firstImageDialog.open();
        }
    }

    Connections {
        target: snipController
        function onShowOverlay() {
            snipOverlay.visible = !snipOverlay.visible
        }
    }

    Connections {
        target: pluginController
        function onShowConfirmDialog() {
            appDialogs.pluginConfirmDialog.open()
        }
    }

    Connections {
        target: pluginController
        function onShowDisabledDialog() {
            appDialogs.disabledDialog.open()
        }
    }
}
