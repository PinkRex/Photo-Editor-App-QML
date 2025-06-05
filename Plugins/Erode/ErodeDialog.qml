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
                model: modeModel
                width: 150
                ListModel {
                    id: modeModel
                    ListElement { Keys: "Choose Mode ..." }
                    ListElement { Keys: "Rect" }
                    ListElement { Keys: "Ellipse" }
                    ListElement { Keys: "Cross" }
                }

                onActivated: {
                    if (modeBox.currentText === "Rect") {
                        imageContainer.height = container.height - 180
                    } else if (modeBox.currentText === "Ellipse") {
                        imageContainer.height = container.height - 180
                    } else if (modeBox.currentText === "Cross") {
                        imageContainer.height = container.height - 180
                    } else {
                        imageContainer.height = container.height - 30
                    }

                    plugin.setMode(modeBox.currentText)
                }
            }

            Slider {
                opacity: 0
            }
        }

        // === Rect Mode Slider ===
        Column {
            spacing: 6
            visible: modeBox.currentText === "Rect"
            width: parent.width

            Text { text: "Kernel Size: " + rectKernelSlider.value }
            Slider {
                id: rectKernelSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 31
                stepSize: 1
                value: 0
                onValueChanged: {
                    plugin.updatePreview(value, rectIterationSlider.value)
                }
            }

            Text { text: "Iterations: " + rectIterationSlider.value }
            Slider {
                id: rectIterationSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 10
                stepSize: 1
                value: 0
                onValueChanged: {
                    plugin.updatePreview(rectKernelSlider.value, value)
                }
            }
        }

        // === Ellipse Mode Slider ===
        Column {
            spacing: 6
            visible: modeBox.currentText === "Ellipse"
            width: parent.width

            Text { text: "Kernel Size: " + ellipseKernelSlider.value }
            Slider {
                id: ellipseKernelSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 31
                stepSize: 1
                value: 0
                onValueChanged: {
                    plugin.updatePreview(value, ellipseIterationSlider.value)
                }
            }

            Text { text: "Iterations: " + ellipseIterationSlider.value }
            Slider {
                id: ellipseIterationSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 10
                stepSize: 1
                value: 0
                onValueChanged: {
                    plugin.updatePreview(ellipseKernelSlider.value, value)
                }
            }
        }

        // === Cross Mode Slider ===
        Column {
            spacing: 6
            visible: modeBox.currentText === "Cross"
            width: parent.width

            Text { text: "Kernel Size: " + crossKernelSlider.value }
            Slider {
                id: crossKernelSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 31
                stepSize: 1
                value: 0
                onValueChanged: {
                    plugin.updatePreview(value, crossIterationSlider.value)
                }
            }

            Text { text: "Iterations: " + crossIterationSlider.value }
            Slider {
                id: crossIterationSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 10
                stepSize: 1
                value: 0
                onValueChanged: {
                    plugin.updatePreview(crossKernelSlider.value, value)
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            visible: modeBox.currentText !== "Choose Mode ..."
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
                    rectKernelSlider.value = 0
                    rectIterationSlider.value = 0
                    ellipseKernelSlider.value = 0
                    ellipseIterationSlider.value = 0
                    crossKernelSlider.value = 0
                    crossIterationSlider.value = 0
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
