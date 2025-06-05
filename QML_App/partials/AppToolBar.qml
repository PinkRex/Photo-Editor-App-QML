import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts

Item {
    property var appFileDialogs
    property var appDialogs
    property var appViews

    id: appToolBar

    Rectangle {
        id: toolBar
        implicitHeight: childrenRect.height

        ColumnLayout {
            RowLayout {
                RowLayout {
                    id: fileToolbar
                    spacing: 5
                    Layout.leftMargin: 5
                    ToolButton { text: "Open"; onClicked: appFileDialogs.chooseImageDialog.open() }
                    ToolButton {
                        text: "Screen Snip"
                        onClicked: {
                            snipController.startSnip()
                        }
                    }
                    ToolButton {
                        text: "Save As"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                appFileDialogs.saveImageDialog.open()
                            }
                        }
                    }
                    ToolButton {
                        text: "Previous Image"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                historyController.clearAll()
                                viewController.previousImage()
                            }
                        }
                    }
                    ToolButton {
                        text: "Next Image"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                historyController.clearAll()
                                viewController.nextImage()
                            }
                        }
                    }
                }
                Rectangle {
                    width: 1
                    color: "#cccccc"
                    height: 20
                    Layout.alignment: Qt.AlignVCenter
                }
                RowLayout {
                    id: viewToolbar
                    spacing: 5
                    ToolButton {
                        text: "Zoom In"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                appViews.imageViewArea.zoomIn()
                            }
                        }
                    }
                    ToolButton {
                        text: "Zoom Out"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                appViews.imageViewArea.zoomOut()
                            }
                        }
                    }
                    ToolButton {
                        text: "Undo"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                historyController.undo(appState.currentImage())
                            }
                        }
                    }
                    ToolButton {
                        text: "Redo"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                historyController.redo(appState.currentImage())
                            }
                        }
                    }
                }
                Rectangle {
                    width: 1
                    color: "#cccccc"
                    height: 20
                    Layout.alignment: Qt.AlignVCenter
                }
                RowLayout {
                    id: editToolBar
                    spacing: 5
                    ToolButton {
                        text: "Rotate Left"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                appViews.imageViewArea.resetScale()
                                editController.rotate(90.0)
                            }
                        }
                    }
                    ToolButton {
                        text: "Rotate Right"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                appViews.imageViewArea.resetScale()
                                editController.rotate(-90.0)
                            }
                        }
                    }
                    ToolButton {
                        text: "Resize"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                appViews.imageViewArea.resetScale()
                                appDialogs.resizeDialog.open()
                            }
                        }
                    }
                    ToolButton {
                        text: "Crop"
                        onClicked: {
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
                    ToolButton {
                        text: "Reset"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                fileController.openImage(appState.currentPath);
                            }
                        }
                    }
                }
            }

            RowLayout {
                id: pluginToolBar
                spacing: 5
                height: 28
                Layout.leftMargin: 5
                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    orientation: ListView.Horizontal
                    model: pluginController ? pluginController.pluginModel : null
                    delegate: ToolButton {
                        text: model.name
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                pluginController.performPlugin(index)
                            }
                        }
                    }
                }
            }
        }
    }
}
