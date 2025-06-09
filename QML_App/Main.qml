import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Dialogs
import "components"
import "partials"

ApplicationWindow {
    id: mainWindow
    width: 1400
    height: 750
    visible: true
    title: qsTr("Photo Editor")
    flags: Qt.Window |
           Qt.CustomizeWindowHint |
           Qt.WindowTitleHint |
           Qt.WindowCloseButtonHint


    Component.onCompleted: {
        let screen = mainWindow.screen;
        let screenWidth = screen.width;
        let screenHeight = screen.height;
        mainWindow.x = (screenWidth - mainWindow.width) / 2;
        mainWindow.y = (screenHeight - mainWindow.height) / 2;
    }

    menuBar: MenuBar {
        id: menuBar
        spacing: 1

        property string openShortcut:       "\t                         Ctrl+O"
        property string screenSnipShortcut: "\t              Ctrl+Shift+S"
        property string saveAsShortcut:     "\t                         Ctrl+S"
        property string exitShortcut:       "\t            Shift+Escape"

        property string zoomInShortcut:     "\t                         Ctrl++"
        property string zoomOutShortcut:    "\t                         Ctrl+-"
        property string undoShortcut:       "\t                         Ctrl+Z"
        property string redoShortcut:       "\t             Ctrl+Shift+Z"
        property string previousShortcut:   "         Left Arrow"
        property string nextShortcut:       "\t             Right Arrow"

        Menu {
            id: fileMenu
            title: qsTr("File")
            width: 230

            MenuItem {
                text: qsTr("Open") + menuBar.openShortcut
                onTriggered: appFileDialogs.chooseImageDialog.open()
                Shortcut {
                    sequence: "Ctrl+O"
                    onActivated: appFileDialogs.chooseImageDialog.open()
                }
            }
            MenuItem {
                text: qsTr("Screen Snip") + menuBar.screenSnipShortcut
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        snipController.startSnip()
                    } else {
                        appDialogs.snipWarningDialog.open()
                    }
                }
                Shortcut {
                    sequence: "Ctrl+Shift+S"
                    onActivated: snipController.startSnip()
                }
            }
            MenuItem {
                text: qsTr("Save As") + menuBar.saveAsShortcut
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        appDialogs.errorDialog.open()
                    } else {
                        appFileDialogs.saveImageDialog.open()
                    }
                }
                Shortcut {
                    sequence: "Ctrl+S"
                    onActivated: {
                        if (appState.getDefaultUrl() === appState.imageUrl) {
                            appDialogs.errorDialog.open()
                        } else {
                            appFileDialogs.saveImageDialog.open()
                        }
                    }
                }
            }
            MenuItem {
                text: qsTr("Previous Image") + menuBar.previousShortcut
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        appDialogs.errorDialog.open()
                    } else {
                        historyController.clearAll()
                        viewController.previousImage()
                    }
                }
                Shortcut {
                    sequence: "Left"
                    onActivated: {
                        if (appState.getDefaultUrl() === appState.imageUrl) {
                            appDialogs.errorDialog.open()
                        } else {
                            historyController.clearAll()
                            viewController.previousImage()
                        }
                    }
                }
            }
            MenuItem {
                text: qsTr("Next Image") + menuBar.nextShortcut
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        appDialogs.errorDialog.open()
                    } else {
                        viewController.nextImage()
                    }
                }
                Shortcut {
                    sequence: "Right"
                    onActivated: {
                        if (appState.getDefaultUrl() === appState.imageUrl) {
                            appDialogs.errorDialog.open()
                        } else {
                            viewController.nextImage()
                        }
                    }
                }
            }
        }
        Menu {
            id: viewMenu
            title: qsTr("View")
            width: 230

            MenuItem {
                text: qsTr("Zoom In") + menuBar.zoomInShortcut
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        appDialogs.errorDialog.open()
                    } else {
                        appViews.imageViewArea.zoomIn()
                    }
                }
                Shortcut {
                    sequences: ["Ctrl+=", "Ctrl++"]
                    onActivated: {
                        if (appState.getDefaultUrl() === appState.imageUrl) {
                            appDialogs.errorDialog.open()
                        } else {
                            appViews.imageViewArea.zoomIn()
                        }
                    }
                }
            }
            MenuItem {
                text: qsTr("Zoom Out") + menuBar.zoomOutShortcut
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        appDialogs.errorDialog.open()
                    } else {
                        appViews.imageViewArea.zoomOut()
                    }
                }
                Shortcut {
                    sequence: "Ctrl+-"
                    onActivated: {
                        if (appState.getDefaultUrl() === appState.imageUrl) {
                            appDialogs.errorDialog.open()
                        } else {
                            appViews.imageViewArea.zoomOut()
                        }
                    }
                }
            }
            MenuItem {
                text: qsTr("Undo") + menuBar.undoShortcut
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        appDialogs.errorDialog.open()
                    } else {
                        historyController.undo(appState.currentImage())
                    }
                }
                Shortcut {
                    sequence: "Ctrl+Z"
                    onActivated: {
                        if (appState.getDefaultUrl() === appState.imageUrl) {
                            appDialogs.errorDialog.open()
                        } else {
                            historyController.undo(appState.currentImage())
                        }
                    }
                }
            }
            MenuItem {
                text: qsTr("Redo") + menuBar.redoShortcut
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        appDialogs.errorDialog.open()
                    } else {
                        historyController.redo(appState.currentImage())
                    }
                }
                Shortcut {
                    sequence: "Ctrl+Shift+Z"
                    onActivated: {
                        if (appState.getDefaultUrl() === appState.imageUrl) {
                            appDialogs.errorDialog.open()
                        } else {
                            historyController.redo(appState.currentImage())
                        }
                    }
                }
            }
        }
        Menu {
            id: editMenu
            title: qsTr("Edit")
            MenuItem {
                text: qsTr("Rotate Left")
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        appDialogs.errorDialog.open()
                    } else {
                        appViews.imageViewArea.resetScale()
                        editController.rotate(90.0)
                    }
                }
            }
            MenuItem {
                text: qsTr("Rotate Right")
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        appDialogs.errorDialog.open()
                    } else {
                        appViews.imageViewArea.resetScale()
                        editController.rotate(-90.0)
                    }
                }
            }
            MenuItem {
                text: qsTr("Flip Vertical")
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        appDialogs.errorDialog.open()
                    } else {
                        appViews.imageViewArea.resetScale()
                        editController.flip(true)
                    }
                }
            }
            MenuItem {
                text: qsTr("Flip Horizontal")
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        appDialogs.errorDialog.open()
                    } else {
                        appViews.imageViewArea.resetScale()
                        editController.flip(false)
                    }
                }
            }
            MenuItem {
                text: qsTr("Resize")
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        appDialogs.errorDialog.open()
                    } else {
                        appViews.imageViewArea.resetScale()
                        appDialogs.resizeDialog.open()
                    }
                }
            }
            MenuItem {
                text: qsTr("Crop")
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        appDialogs.errorDialog.open()
                    } else {
                        appViews.imageViewArea.resetScale()
                        let rect = appViews.imageViewArea.getDisplayedImageRect();
                        let viewWidth = rect.width;
                        let viewHeight = rect.height;1
                        let startX = rect.x;
                        let startY = rect.y;

                        if (!appViews.cropView.cropping) {
                            appDialogs.cropGuideDialog.open();
                            appViews.cropView.cropping = true;
                            appViews.cropView.setStartCoordinate(rect.x, rect.y, rect.width, rect.height);
                            statusController.setGuideText("Adjust the selection rectangle to define the crop area, then press 'Crop' again to crop the image.");
                        } else {
                            appViews.cropView.cropping = false;
                            let crop = appViews.cropView.getCropArea();
                            editController.crop(crop.x, crop.y, crop.width, crop.height, startX, startY, viewWidth, viewHeight);
                        }
                    }
                }
            }
            MenuItem {
                text: qsTr("Reset")
                onTriggered: {
                    if (appState.getDefaultUrl() === appState.imageUrl) {
                        appDialogs.errorDialog.open()
                    } else {
                        fileController.openImage(appState.currentPath);
                    }
                }
            }
        }
        Menu {
            id: pluginMenu
            title: qsTr("Plugins")
            MenuItem {
                text: qsTr("Load Plugins")
                onTriggered: appFileDialogs.chooseDllsDialog.open()
            }
            MenuItem {
                text: qsTr("Plugins Manager")
                onTriggered: {
                    pluginsManager.pluginNames = pluginController.pluginModel.pluginNames()
                    pluginsManager.selectedIndexes = []
                    pluginsManager.pluginStates = pluginController.pluginModel.pluginStates()
                    pluginsManager.open()
                }
            }
        }
        Menu {
            id: helpMenu
            title: qsTr("Help")
            MenuItem {
                text: qsTr("Toggle Logger")
                onTriggered: {
                    if (rightColumn.opacity === 1) {
                        rightColumn.opacity = 0
                    } else {
                        rightColumn.opacity = 1
                    }
                }
            }
            MenuItem { text: qsTr("About"); onTriggered: appDialogs.aboutDialog.open() }
        }
        Menu {
            id: exitMenu
            title: qsTr("Exit")
            width: 230
            MenuItem {
                text: qsTr("Exit") + menuBar.exitShortcut
                onTriggered: Qt.quit()
                Shortcut {
                    sequence: "Shift+Escape"
                    onActivated: Qt.quit()
                }
            }
        }
    }

    RowLayout {
        id: mainContainer
        anchors.fill: parent

        ColumnLayout {
            id: leftColumn
            Layout.preferredWidth: parent.width * 0.8
            Layout.fillHeight: true

            AppToolBar {
                id: appToolBar
                appFileDialogs: appFileDialogs
                appDialogs: appDialogs
                appViews: appViews
            }

            AppViews {
                id: appViews
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            // footer
            Label {
                id: statusBar
                text: statusController ? statusController.statusText : ""
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.italic: true
                Layout.preferredHeight: 40
                Layout.fillWidth: true
            }
        }

        Rectangle {
            width: 1
            color: "#cccccc"
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignVCenter
        }

        ColumnLayout {
            id: rightColumn
            Layout.preferredWidth: parent.width * 0.2
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rightMargin: 6

            Text {
                textFormat: Text.RichText
                text: "<span style='font-size:25px;'>📜</span> <span style='font-size:16px; font-weight:bold;'>Action Logger</span>"
                padding: 6
            }

            Rectangle {
                id: actionLoggerArea
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignTop
                        border.color: "#cccccc"
                        clip: true

                        ListView {
                            id: logList
                            width: parent.width
                            height: parent.height
                            model: actionLog.model

                            onCountChanged: {
                                logList.positionViewAtBeginning()
                            }

                            delegate: Item {
                                width: ListView.view.width
                                height: contentItem.implicitHeight + 10

                                Rectangle {
                                    anchors.fill: parent
                                    color: "transparent"

                                    Column {
                                        id: contentItem
                                        spacing: 4
                                        anchors.margins: 8
                                        anchors.fill: parent

                                        Text {
                                            text: "[" + timestamp + "]"
                                            font.pixelSize: 14
                                            color: "#888"
                                        }

                                        Text {
                                            text: action
                                            font.pixelSize: 14
                                            wrapMode: Text.Wrap
                                            width: parent.width
                                        }
                                    }
                                }
                            }
                        }
                    }

                    RowLayout {
                        Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.bottomMargin: 8
                        spacing: 10

                        Button {
                            text: "Clear"
                            Layout.fillWidth: true
                            onClicked: {
                                actionLog.clearLogs()
                            }
                        }

                        Button {
                            text: "Copy"
                            Layout.fillWidth: true
                            onClicked: {
                                actionLog.copyLogs()
                            }
                        }

                        Button {
                            text: "Export"
                            Layout.fillWidth: true
                            onClicked: {
                                actionLog.exportLogs()
                            }
                        }
                    }
                }
            }
        }
    }

    PluginsManager {
        id: pluginsManager
    }

    SnipOverlay {
        id: snipOverlay
        visible: false
    }

    AppConnections {
        id: appConnections
        appDialogs: appDialogs
        snipOverlay: snipOverlay
    }

    AppDialogs {
        id: appDialogs
        anchors.fill: parent
    }

    AppFileDialogs {
        id: appFileDialogs
        anchors.fill: parent
    }
}
