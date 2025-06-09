import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion

Popup {
    property string text: "Something went wrong."
    property double customWidth: 300
    property double customHeight: 140
    property bool isPluginConfirm: false
    property bool isSnipWarning: false

    id: dialog
    anchors.centerIn: parent
    width: dialog.customWidth
    height: dialog.customHeight
    modal: true
    focus: true

    Overlay.modal: Rectangle {
        color: "#9B9B9BA0"
    }

    background: Rectangle {
        anchors.fill: parent
        color: "white"
        radius: 12
        border.color: "#ccc"
        border.width: 1
        layer.enabled: true
    }

    Column {
        anchors.centerIn: parent
        spacing: 20
        width: parent.width * 0.9

        Text {
            textFormat: Text.RichText
            text: dialog.text
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width
            font.pixelSize: 16
            onLinkActivated: function(link) {
                Qt.openUrlExternally(link)
            }
        }

        Button {
            text: "OK"
            visible: !isPluginConfirm && !isSnipWarning
            width: parent.width * 0.5
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: dialog.close()
        }

        Row {
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter
            visible: isPluginConfirm
            Button {
                text: "Confirm"
                width: dialog.width * 0.4
                height: 40
                onClicked: {
                    appState.setCurrentImage(appState.pluginImage())
                    dialog.close()
                }
            }
            Button {
                text: "Cancel"
                width: dialog.width * 0.4
                height: 40
                onClicked: dialog.close()
            }
        }

        Row {
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter
            visible: isSnipWarning
            Button {
                text: "Proceed"
                width: dialog.width * 0.4
                height: 40
                onClicked: {
                    dialog.close()
                    snipController.startSnip()
                }
            }
            Button {
                text: "Cancel"
                width: dialog.width * 0.4
                height: 40
                onClicked: dialog.close()
            }
        }
    }
}
