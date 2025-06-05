import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion

Item {
    property alias cropRect: cropRect
    property bool cropping: false
    property var controller

    id: root
    width: 800
    height: 600

    Rectangle {
        id: imageContainer
        Rectangle {
            id: cropRect
            visible: cropping
            border.color: "red"
            border.width: 2
            color: "transparent"

            property real limitX: 0
            property real limitY: 0
            property real limitWidth: 0
            property real limitHeight: 0

            MouseArea {
                anchors.fill: parent
                drag.target: cropRect
                drag.axis: Drag.XAndYAxis
                cursorShape: Qt.SizeAllCursor

                onReleased: {
                    cropRect.x = Math.max(cropRect.limitX, Math.min(cropRect.x, cropRect.limitX + cropRect.limitWidth - cropRect.width));
                    cropRect.y = Math.max(cropRect.limitY, Math.min(cropRect.y, cropRect.limitY + cropRect.limitHeight - cropRect.height));
                }
            }

            Repeater {
                model: [
                    { x: 0, y: 0, cursor: Qt.SizeFDiagCursor },
                    { x: 1, y: 0, cursor: Qt.SizeBDiagCursor },
                    { x: 0, y: 1, cursor: Qt.SizeBDiagCursor },
                    { x: 1, y: 1, cursor: Qt.SizeFDiagCursor }
                ]
                delegate: Rectangle {
                    width: 12
                    height: 12
                    color: "white"
                    radius: 6
                    border.color: "black"
                    border.width: 1

                    x: modelData.x === 0 ? 0 : cropRect.width - width
                    y: modelData.y === 0 ? 0 : cropRect.height - height

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: modelData.cursor
                    }

                    DragHandler {
                        id: resizeDrag
                        cursorShape: modelData.cursor

                        property real startX
                        property real startY
                        property real startW
                        property real startH

                        onActiveChanged: {
                            if (active) {
                                startX = cropRect.x
                                startY = cropRect.y
                                startW = cropRect.width
                                startH = cropRect.height
                            }
                        }

                        onTranslationChanged: {
                            const dx = resizeDrag.translation.x
                            const dy = resizeDrag.translation.y
                            const minSize = 50

                            var newX = startX
                            var newY = startY
                            var newW = startW
                            var newH = startH

                            if (modelData.x === 0) {
                                newX = startX + dx
                                newW = startW - dx
                            } else {
                                newW = startW + dx
                            }

                            if (modelData.y === 0) {
                                newY = startY + dy
                                newH = startH - dy
                            } else {
                                newH = startH + dy
                            }

                            newX = Math.max(cropRect.limitX, newX)
                            newY = Math.max(cropRect.limitY, newY)

                            newW = Math.min(newW, cropRect.limitX + cropRect.limitWidth - newX)
                            newH = Math.min(newH, cropRect.limitY + cropRect.limitHeight - newY)

                            if (newW >= minSize) {
                                cropRect.x = newX
                                cropRect.width = newW
                            }

                            if (newH >= minSize) {
                                cropRect.y = newY
                                cropRect.height = newH
                            }
                        }
                    }
                }
            }
        }
    }

    function setStartCoordinate(x, y, width, height) {
        cropRect.x = x;
        cropRect.y = y;
        cropRect.width = width;
        cropRect.height = height;

        cropRect.limitX = x;
        cropRect.limitY = y;
        cropRect.limitWidth = width;
        cropRect.limitHeight = height;
    }

    function getCropArea() {
        return Qt.rect(cropRect.x, cropRect.y, cropRect.width, cropRect.height);
    }
}
