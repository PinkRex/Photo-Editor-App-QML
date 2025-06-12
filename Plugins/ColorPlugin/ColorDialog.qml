import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion

Rectangle {
    width: 900; height: 700
    color: "white"
    id: root

    Column {
        id: mainContainer
        anchors.fill: parent
        spacing: 10
        anchors.margins: 16

        Item {
            id: imageContainer
            width: mainContainer.width
            height: mainContainer.height - 30

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
                model: modeModel
                ListModel {
                    id: modeModel
                    ListElement { Keys: "Choose Mode ..." }
                    ListElement { Keys: "Manual RGBA" }
                    ListElement { Keys: "Auto White Balance" }
                }

                onActivated: {
                    if (modeBox.currentText === "Manual RGBA") {
                        imageContainer.height = mainContainer.height - 220
                    } else if (modeBox.currentText === "Auto White Balance") {
                        imageContainer.height = mainContainer.height - 180
                    } else {
                        imageContainer.height = mainContainer.height - 30
                    }

                    plugin.setMode(modeBox.currentText)
                }
            }

            Slider {
                opacity: 0
            }
        }

        // === Manual RGBA Mode ===
        Column {
            spacing: 6
            visible: modeBox.currentText === "Manual RGBA"
            width: parent.width

            Text { text: "Red Scale: " + manualRedScaleSlider.value }
            Slider {
                id: manualRedScaleSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 300
                stepSize: 5
                value: 100
                onValueChanged: {
                    plugin.manualRGBA(manualRedScaleSlider.value,
                                      manualGreenScaleSlider.value,
                                      manualBlueScaleSlider.value,
                                      100)
                }
            }

            Text { text: "Green Scale: " + manualGreenScaleSlider.value }
            Slider {
                id: manualGreenScaleSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 300
                stepSize: 5
                value: 100
                onValueChanged: {
                    plugin.manualRGBA(manualRedScaleSlider.value,
                                      manualGreenScaleSlider.value,
                                      manualBlueScaleSlider.value,
                                      100)
                }
            }

            Text { text: "Blue Scale: " + manualBlueScaleSlider.value }
            Slider {
                id: manualBlueScaleSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 300
                stepSize: 5
                value: 100
                onValueChanged: {
                    plugin.manualRGBA(manualRedScaleSlider.value,
                                      manualGreenScaleSlider.value,
                                      manualBlueScaleSlider.value,
                                      100)
                }
            }
        }

        // === Auto White Balance Mode ===
        Column {
            spacing: 6
            visible: modeBox.currentText === "Auto White Balance"
            width: parent.width

            Text { text: "Strength: " + autoBalanceStrengthSlider.value }
            Slider {
                id: autoBalanceStrengthSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 100
                stepSize: 5
                value: 0
                onValueChanged: {
                    plugin.autoWhiteBalance(
                                autoBalanceStrengthSlider.value,
                                autoBalanceGraySlider.value,
                                true)
                }
            }

            Text { text: "Strength: " + autoBalanceGraySlider.value }
            Slider {
                id: autoBalanceGraySlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 255
                stepSize: 5
                value: 0
                onValueChanged: {
                    plugin.autoWhiteBalance(
                                autoBalanceStrengthSlider.value,
                                autoBalanceGraySlider.value,
                                true)
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
                    manualRedScaleSlider.value = 100
                    manualGreenScaleSlider.value = 100
                    manualBlueScaleSlider.value = 100
                    autoBalanceStrengthSlider.value = 0
                    autoBalanceGraySlider.value = 0

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
