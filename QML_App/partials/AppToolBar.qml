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
                    ToolButton {
                        icon.source: "qrc:/icons/open.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Open Image"
                        onClicked: appFileDialogs.chooseImageDialog.open()
                    }
                    ToolButton {
                        icon.source: "qrc:/icons/screenSnip.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Capture Current Screen"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                snipController.startSnip()
                            } else {
                                appDialogs.snipWarningDialog.open()
                            }
                        }
                    }
                    ToolButton {
                        icon.source: "qrc:/icons/saveAs.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Save File As ..."
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                appFileDialogs.saveImageDialog.open()
                            }
                        }
                    }
                    Rectangle {
                        width: 1
                        color: "#cccccc"
                        height: 20
                        Layout.alignment: Qt.AlignVCenter
                    }
                    ToolButton {
                        icon.source: "qrc:/icons/previous.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Previous Image"
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
                        icon.source: "qrc:/icons/next.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Next Image"
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
                        icon.source: "qrc:/icons/zoomIn.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Zoom In"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                appViews.imageViewArea.zoomIn()
                            }
                        }
                    }
                    ToolButton {
                        icon.source: "qrc:/icons/zoomOut.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Zoom Out"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                appViews.imageViewArea.zoomOut()
                            }
                        }
                    }
                    Rectangle {
                        width: 1
                        color: "#cccccc"
                        height: 20
                        Layout.alignment: Qt.AlignVCenter
                    }
                    ToolButton {
                        icon.source: "qrc:/icons/undo.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Undo Action"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                historyController.undo(appState.currentImage())
                            }
                        }
                    }
                    ToolButton {
                        icon.source: "qrc:/icons/redo.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Redo Action"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                historyController.redo(appState.currentImage())
                            }
                        }
                    }
                    ToolButton {
                        icon.source: "qrc:/icons/reset.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Reset Image"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                fileController.openImage(appState.currentPath);
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
                        icon.source: "qrc:/icons/rotateLeft.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Rotate Left 90°"
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
                        icon.source: "qrc:/icons/rotateRight.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Rotate Right 90°"
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
                        icon.source: "qrc:/icons/flipVertical.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Flip Vertical"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                appViews.imageViewArea.resetScale()
                                editController.flip(true)
                            }
                        }
                    }

                    ToolButton {
                        icon.source: "qrc:/icons/flipHorizontal.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Flip Horizontal"
                        onClicked: {
                            if (appState.getDefaultUrl() === appState.imageUrl) {
                                appDialogs.errorDialog.open()
                            } else {
                                appViews.imageViewArea.resetScale()
                                editController.flip(false)
                            }
                        }
                    }
                    Rectangle {
                        width: 1
                        color: "#cccccc"
                        height: 20
                        Layout.alignment: Qt.AlignVCenter
                    }
                    ToolButton {
                        icon.source: "qrc:/icons/resize.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Resize Image"
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
                        icon.source: "qrc:/icons/crop.png"
                        icon.width: 25
                        icon.height: 25
                        ToolTip.visible: hovered
                        ToolTip.text: "Crop Image"
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
