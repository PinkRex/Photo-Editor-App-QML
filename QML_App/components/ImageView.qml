import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion

Item {
    property alias imagePath: image.source
    property var controller
    property var logger
    property real imageScale: 1.0
    property bool imageLoaded: false
    property int toolbarMargin: 0

    id: root

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: image.implicitWidth * root.imageScale
        contentHeight: image.implicitHeight * root.imageScale
        clip: true
        interactive: false

        Image {
            id: image
            fillMode: Image.PreserveAspectFit
            source: controller.imageUrl
            width: implicitWidth * root.imageScale
            height: implicitHeight * root.imageScale

            onStatusChanged: {
                if (status === Image.Ready) {
                    root.imageLoaded = true
                    root.setInitialScaleAndCenter()
                }
            }
        }

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
        }

        ScrollBar.horizontal: ScrollBar {
            policy: ScrollBar.AsNeeded
        }
    }

    function setInitialScaleAndCenter() {
        if (!image.implicitWidth || !image.implicitHeight)
            return

        let scaleX = flickable.width / image.implicitWidth
        let scaleY = flickable.height / image.implicitHeight
        imageScale = Math.min(scaleX, scaleY)

        Qt.callLater(centerImage)
    }

    function centerImage() {
        flickable.contentX = (flickable.contentWidth - flickable.width) / 2
        flickable.contentY = (flickable.contentHeight - flickable.height) / 2
    }

    function zoomIn() {
        imageScale = Math.min(imageScale * 1.1, 5.0)
        Qt.callLater(centerImage)
        logger.pushAction("Zoom in image")
    }

    function zoomOut() {
        imageScale = Math.max(imageScale / 1.1, 0.2)
        Qt.callLater(centerImage)
        logger.pushAction("Zoom out image")
    }

    function resetScale() {
        setInitialScaleAndCenter()
    }

    function getDisplayedImageRect() {
        const imgW = image.paintedWidth;
        const imgH = image.paintedHeight;

        const x = (flickable.width - imgW) / 2;
        const y = (flickable.height - imgH) / 2 + root.toolbarMargin;
        return Qt.rect(x, y, imgW, imgH);
    }
}
