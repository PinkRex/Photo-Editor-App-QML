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
            ComboBox {
                id: modeBox
                width: 150
                model: modeModel
                ListModel {
                    id: modeModel
                    ListElement { Keys: "Choose Mode ..." }
                    ListElement { Keys: "Linear" }
                    ListElement { Keys: "CLAHE" }
                }

                onActivated: {
                    if (modeBox.currentText === "Linear") {
                        imageContainer.height = container.height - 180
                    } else if (modeBox.currentText === "CLAHE") {
                        imageContainer.height = container.height - 180
                    } else {
                        imageContainer.height = container.height - 30
                    }

                    plugin.setMode(currentText)
                }
            }

            Slider {
                opacity: 0
            }
        }

        Column {
            spacing: 6
            visible: modeBox.currentText === "Linear"
            width: parent.width

            Text { text: "Contrast: " + contrastSlider.value.toFixed(2) }
            Slider {
                id: contrastSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 500
                stepSize: 5
                value: 100
                onValueChanged: {
                    plugin.updatePreview(value, brightnessSlider.value)
                }
            }

            Text { text: "Brightness: " + brightnessSlider.value.toFixed(2) }
            Slider {
                id: brightnessSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 200
                stepSize: 5
                value: 100
                onValueChanged: {
                    plugin.updatePreview(contrastSlider.value, value)
                }
            }
        }

        // === CLAHE Mode Sliders ===
        Column {
            spacing: 6
            visible: modeBox.currentText === "CLAHE"
            width: parent.width

            Text { text: "Clip Limit: " + clipLimitSlider.value.toFixed(2) }
            Slider {
                id: clipLimitSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 1.0; to: 40.0
                value: 4.0
                stepSize: 0.1
                onValueChanged: {
                    plugin.setClipLimit(value)
                    plugin.updatePreview(contrastSlider.value, brightnessSlider.value)
                }
            }

            Text { text: "Tile Grid Size: " + tileGridSlider.value }
            Slider {
                id: tileGridSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 4; to: 32
                value: 8
                stepSize: 1
                onValueChanged: {
                    plugin.setTileGridSize(value)
                    plugin.updatePreview(contrastSlider.value, brightnessSlider.value)
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            visible: modeBox.currentText !== "Choose Mode ..."
            spacing: 10

            Button {
                text: "Apply"
                onClicked: {
                    plugin.onOkClicked(previewImage.source)
                }
            }

            Button {
                text: "Cancel"
                onClicked: plugin.onCancelClicked()
            }

            Button {
                text: "Reset"
                onClicked: {
                    contrastSlider.value = 100;
                    brightnessSlider.value = 100;
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
