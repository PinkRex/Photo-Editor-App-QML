import QtQuick
import QtQuick.Controls.Fusion
import "../components"

Item {
    property alias imageViewArea: imageViewArea
    property alias cropView: cropView
    property alias imageDropArea: imageDropArea

    id: appViews

    ImageView {
        id: imageViewArea
        anchors.fill: parent
        anchors.topMargin: 67
        toolbarMargin: 67
        controller: appState
        logger: actionLog
    }

    CropView {
        id: cropView
        controller: appState
        cropping: false
    }

    DropArea {
        id: imageDropArea
        anchors.fill: parent
        onEntered: function(drag) {
            drag.accepted = drag.hasUrls
        }

        onDropped: function(drop) {
            for (let i = 0; i < drop.urls.length; i++) {
                const localPath = drop.urls[i].toString().replace("file://", "");
                fileController.openImage(localPath);
            }
        }
    }
}
