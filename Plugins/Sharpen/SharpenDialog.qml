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
            height: parent.height - 220

            Image {
                id: previewImage
                source: "image://pluginprovider/input"
                anchors.centerIn: parent
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                cache: false
            }
        }

        ComboBox {
            id: modeBox
            width: 150
            model: modeModel
            ListModel {
                id: modeModel
                ListElement { Keys: "Choose Preset ..." }
                ListElement { Keys: "Soft" }
                ListElement { Keys: "Medium" }
                ListElement { Keys: "Strong" }
            }

            onActivated: {
                if (modeBox.currentText === "Soft") {
                    radiusSlider.value = 1;
                    amountSlider.value = 5;
                    thresholdSlider.value = 5;
                }
                if (modeBox.currentText === "Medium") {
                    radiusSlider.value = 2;
                    amountSlider.value = 10;
                    thresholdSlider.value = 3;
                }
                if (modeBox.currentText === "Strong") {
                    radiusSlider.value = 3;
                    amountSlider.value = 15;
                    thresholdSlider.value = 1;
                }
            }
        }

        Column {
            spacing: 6
            width: parent.width

            // === Radius Slider ===
            Text { text: "Radius: " + radiusSlider.value}
            Slider {
                id: radiusSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 10
                stepSize: 1
                value: 0
                onValueChanged: {
                    plugin.updatePreview(value, amountSlider.value, thresholdSlider.value)
                }
            }

            // === Amount Slider ===
            Text { text: "Amount: " + amountSlider.value }
            Slider {
                id: amountSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 30
                stepSize: 1
                value: 0
                onValueChanged: {
                    plugin.updatePreview(radiusSlider.value, value, thresholdSlider.value)
                }
            }

            // === Threshold Slider ===
            Text { text: "Threshold: " + thresholdSlider.value }
            Slider {
                id: thresholdSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 50
                stepSize: 1
                value: 0
                onValueChanged: {
                    plugin.updatePreview(radiusSlider.value, amountSlider.value, value)
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            Button {
                text: "OK"
                onClicked: plugin.onOkClicked(previewImage.source)
            }

            Button {
                text: "Cancel"
                onClicked: plugin.onCancelClicked()
            }

            Button {
                text: "Reset"
                onClicked: {
                    radiusSlider.value = 0;
                    amountSlider.value = 0;
                    thresholdSlider.value = 0;
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
