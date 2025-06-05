import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Fusion

Window {
    property bool dragging: false
    property point start: Qt.point(0, 0)
    property point end: Qt.point(0, 0)

    id: overlay
    width: Screen.width
    height: Screen.height
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Tool
    modality: Qt.ApplicationModal
    color: "#88000000"

    Rectangle {
        id: selectionRect
        visible: dragging
        border.color: "red"
        border.width: 2
        color: "#44ff0000"

        x: Math.min(start.x, end.x)
        y: Math.min(start.y, end.y)
        width: Math.abs(end.x - start.x)
        height: Math.abs(end.y - start.y)
    }

    MouseArea {
        anchors.fill: parent

        onPressed: (mouse) => {
            start = Qt.point(mouse.x, mouse.y)
            end = start
            overlay.dragging = true
        }
        onPositionChanged: (mouse) => {
            if (overlay.dragging)
                end = Qt.point(mouse.x, mouse.y)
        }
        onReleased: () => {
            overlay.dragging = false
            overlay.visible = false
            snipController.finishSnip(
                Math.min(start.x, end.x),
                Math.min(start.y, end.y),
                Math.abs(end.x - start.x),
                Math.abs(end.y - start.y)
            )
        }
    }
}
