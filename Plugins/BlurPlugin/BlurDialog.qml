import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion

Rectangle {
    width: 900; height: 700
    color: "white"
    id: root

    Column {
        id: container
        anchors.fill: parent
        spacing: 10
        anchors.margins: 16

        Item {
            id: imageContainer
            width: parent.width
            height: parent.height - 30

            Image {
                id: previewImage
                source: "image://pluginprovider/input"
                anchors.centerIn: parent
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                cache: false
            }
        }

        Column {
            spacing: 10

            ComboBox {
                id: modeBox
                model: modeModel
                width: 150
                ListModel {
                    id: modeModel
                    ListElement { Keys: "Choose Mode ..." }
                    ListElement { Keys: "Gaussian" }
                    ListElement { Keys: "Box" }
                }

                onActivated: {
                    if (modeBox.currentText === "Gaussian") {
                        imageContainer.height = container.height - 150
                    } else if (modeBox.currentText === "Box") {
                        imageContainer.height = container.height - 150
                    } else {
                        imageContainer.height = container.height - 30
                    }

                    plugin.setMode(modeBox.currentText)
                }
            }

            Slider {
                id: separator
                opacity: 0
            }
        }

        // === Gaussian Mode Slider ===
        Column {
            spacing: 6
            visible: modeBox.currentText === "Gaussian"
            width: parent.width

            Text { text: "Blur Strength: " + gaussianSlider.value }
            Slider {
                id: gaussianSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 50
                stepSize: 1
                value: 0
                onValueChanged: {
                    plugin.updatePreview(value)
                }
            }
        }

        // === Box Mode Slider ===
        Column {
            spacing: 6
            visible: modeBox.currentText === "Box"
            width: parent.width

            Text { text: "Blur Strength: " + boxSlider.value }
            Slider {
                id: boxSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 50
                stepSize: 1
                value: 0
                onValueChanged: {
                    plugin.updatePreview(value)
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            visible: modeBox.currentText !== "Choose Mode ..."
            spacing: 10

            Button {
                text: "Apply"
                onClicked: plugin.onOkClicked(previewImage.source)
            }

            Button {
                text: "Cancel"
                onClicked: plugin.onCancelClicked()
            }

            Button {
                text: "Reset"
                onClicked: {
                    gaussianSlider.value = 0.0;
                    boxSlider.value = 0.0;
                    plugin.onResetClicked()
                }
            }
        }
    }

    function refreshPreview() {
        var current = previewImage.source;
        previewImage.source = "";
        previewImage.source = current;
    }
}
