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
                width: 150
                ListModel {
                    id: modeModel
                    ListElement { Keys: "Choose Mode ..." }
                    ListElement { Keys: "Classic" }
                    ListElement { Keys: "Custom Edge" }
                    ListElement { Keys: "Toon Shading" }
                    ListElement { Keys: "Comic Book" }
                }

                onActivated: {
                    if (modeBox.currentText === "Classic") {
                        imageContainer.height = mainContainer.height - 180
                    } else if (modeBox.currentText === "Custom Edge") {
                        imageContainer.height = mainContainer.height - 300
                    } else if (modeBox.currentText === "Toon Shading") {
                        imageContainer.height = mainContainer.height - 180
                    } else if (modeBox.currentText === "Comic Book") {
                        imageContainer.height = mainContainer.height - 220
                    } else {
                        imageContainer.height = mainContainer.height - 30
                    }

                    plugin.setMode(modeBox.currentText)
                }
            }

            Slider {
                id: separator
                opacity: 0
            }
        }

        // === Classic Mode ===
        Column {
            spacing: 6
            visible: modeBox.currentText === "Classic"
            width: parent.width

            Text { text: "Smoothness: " + classicSmoothnessSlider.value }
            Slider {
                id: classicSmoothnessSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 1; to: 10
                stepSize: 1
                value: 1
                onValueChanged: {
                    plugin.classicCartoon(value, classicEdgeSlider.value)
                }
            }

            Text { text: "Edge Strength: " + classicEdgeSlider.value }
            Slider {
                id: classicEdgeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 1; to: 10
                stepSize: 1
                value: 1
                onValueChanged: {
                    plugin.classicCartoon(classicSmoothnessSlider.value, value)
                }
            }
        }

        // === Custom Edge Mode ===
        Column {
            spacing: 6
            visible: modeBox.currentText === "Custom Edge"
            width: parent.width

            Text { text: "Smoothness: " + customEdgeSmoothnessSlider.value }
            Slider {
                id: customEdgeSmoothnessSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 1; to: 10
                stepSize: 1
                value: 1
                onValueChanged: {
                    plugin.customEdge(
                                customEdgeSmoothnessSlider.value,
                                customEdgeThicknessSlider.value,
                                customEdgeRedSlider.value,
                                customEdgeGreenSlider.value,
                                customEdgeBlueSlider.value)
                }
            }

            Text { text: "Edge Thickness: " + customEdgeThicknessSlider.value }
            Slider {
                id: customEdgeThicknessSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 1; to: 5
                stepSize: 1
                value: 1
                onValueChanged: {
                    plugin.customEdge(
                                customEdgeSmoothnessSlider.value,
                                customEdgeThicknessSlider.value,
                                customEdgeRedSlider.value,
                                customEdgeGreenSlider.value,
                                customEdgeBlueSlider.value)
                }
            }

            Text { text: "Edge Color 'R': " + customEdgeRedSlider.value }
            Slider {
                id: customEdgeRedSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 255
                stepSize: 5
                value: 0
                onValueChanged: {
                    plugin.customEdge(
                                customEdgeSmoothnessSlider.value,
                                customEdgeThicknessSlider.value,
                                customEdgeRedSlider.value,
                                customEdgeGreenSlider.value,
                                customEdgeBlueSlider.value)
                }
            }

            Text { text: "Edge Color 'G': " + customEdgeGreenSlider.value }
            Slider {
                id: customEdgeGreenSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 255
                stepSize: 5
                value: 0
                onValueChanged: {
                    plugin.customEdge(
                                customEdgeSmoothnessSlider.value,
                                customEdgeThicknessSlider.value,
                                customEdgeRedSlider.value,
                                customEdgeGreenSlider.value,
                                customEdgeBlueSlider.value)
                }
            }

            Text { text: "Edge Color 'B': " + customEdgeBlueSlider.value }
            Slider {
                id: customEdgeBlueSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 0; to: 255
                stepSize: 5
                value: 0
                onValueChanged: {
                    plugin.customEdge(
                                customEdgeSmoothnessSlider.value,
                                customEdgeThicknessSlider.value,
                                customEdgeRedSlider.value,
                                customEdgeGreenSlider.value,
                                customEdgeBlueSlider.value)
                }
            }
        }

        // === Toon Shading Mode ===
        Column {
            spacing: 6
            visible: modeBox.currentText === "Toon Shading"
            width: parent.width

            Text { text: "Color Levels: " + toonShadingLevelSlider.value }
            Slider {
                id: toonShadingLevelSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 1; to: 10
                stepSize: 1
                value: 1
                onValueChanged: {
                    plugin.toonShading(toonShadingLevelSlider.value, toonShadingEdgeSlider.value)
                }
            }

            Text { text: "Edge Strength: " + toonShadingEdgeSlider.value }
            Slider {
                id: toonShadingEdgeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 1; to: 10
                stepSize: 1
                value: 1
                onValueChanged: {
                    plugin.toonShading(toonShadingLevelSlider.value, toonShadingEdgeSlider.value)
                }
            }
        }

        // === Comic Book Mode ===
        Column {
            spacing: 6
            visible: modeBox.currentText === "Comic Book"
            width: parent.width

            Text { text: "Color Levels: " + comicBookLevelSlider.value }
            Slider {
                id: comicBookLevelSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 1; to: 10
                stepSize: 1
                value: 1
                onValueChanged: {
                    plugin.comicBook(comicBookThresholdSlider.value, comicBookEdgeSlider.value, comicBookLevelSlider.value)
                }
            }

            Text { text: "Edge Thickness: " + comicBookEdgeSlider.value }
            Slider {
                id: comicBookEdgeSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 1; to: 10
                stepSize: 1
                value: 1
                onValueChanged: {
                    plugin.comicBook(comicBookThresholdSlider.value, comicBookEdgeSlider.value, comicBookLevelSlider.value)
                }
            }

            Text { text: "Edge Threshold: " + comicBookThresholdSlider.value }
            Slider {
                id: comicBookThresholdSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.98
                from: 1; to: 10
                stepSize: 1
                value: 1
                onValueChanged: {
                    plugin.comicBook(comicBookThresholdSlider.value, comicBookEdgeSlider.value, comicBookLevelSlider.value)
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
                    classicSmoothnessSlider.value = 1;
                    classicEdgeSlider.value = 1;
                    customEdgeSmoothnessSlider.value = 1;
                    customEdgeThicknessSlider.value = 1;
                    customEdgeRedSlider.value = 0;
                    customEdgeGreenSlider.value = 0;
                    customEdgeBlueSlider.value = 0;
                    toonShadingLevelSlider.value = 1;
                    toonShadingEdgeSlider.value = 1;
                    comicBookLevelSlider.value = 1;
                    comicBookEdgeSlider.value = 1;
                    comicBookThresholdSlider.value = 1;

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
