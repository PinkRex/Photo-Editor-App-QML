import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Dialogs
import "components"
import "partials"

ApplicationWindow {
    id: mainWindow
    width: 1100
    height: 700
    visible: true
    title: qsTr("Photo Editor")

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
        property string previousShortcut:   "\t                Left Arrow"
        property string nextShortcut:       "\t              Right Arrow"

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
                onTriggered: snipController.startSnip()
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
                text: qsTr("Exit") + menuBar.exitShortcut
                onTriggered: Qt.quit()
                Shortcut {
                    sequence: "Shift+Escape"
                    onActivated: Qt.quit()
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
            MenuItem {
                text: qsTr("Previous") + menuBar.previousShortcut
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
                text: qsTr("Next") + menuBar.nextShortcut
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
                            appViews.cropView.cropping = true;
                            appViews.cropView.setStartCoordinate(rect.x, rect.y, rect.width, rect.height);
                            statusController.setGuideText("Adjust the selection rectangle to define the crop area, then press 'Crop' to crop the image.");
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
            MenuItem { text: qsTr("About"); onTriggered: appDialogs.aboutDialog.open() }
        }
    }

    AppToolBar {
        id: appToolBar
        appFileDialogs: appFileDialogs
        appDialogs: appDialogs
        appViews: appViews
    }

    AppViews {
        id: appViews
        anchors.fill: parent
    }

    PluginsManager {
        id: pluginsManager
    }

    SnipOverlay {
        id: snipOverlay
        visible: false
    }

    footer: Label {
        id: statusBar
        text: statusController ? statusController.statusText : ""
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.italic: true
        height: 40
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
