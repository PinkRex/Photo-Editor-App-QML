import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts

Dialog {
    property var pluginNames: []
    property var pluginStates: []
    property var selectedIndexes: []

    id: dialog
    title: "Plugins Manager"
    modal: true
    font.pixelSize: 14
    height: 180
    anchors.centerIn: parent
    standardButtons: Dialog.NoButton

    Overlay.modal: Rectangle {
        color: "#9B9B9BA0"
    }

    background: Rectangle {
        anchors.fill: parent
        color: "white"
        border.color: "#ccc"
        border.width: 1
        layer.enabled: true
    }

    contentItem: ColumnLayout {
        spacing: 20
        width: parent.width

        GridLayout {
            id: grid
            columns: 5
            columnSpacing: 30
            rowSpacing: 15

            Repeater {
                id: repeater
                model: pluginNames

                CheckBox {
                    id: cb
                    text: modelData
                    font.pixelSize: 14
                    Layout.preferredWidth: 100

                    contentItem: Text {
                        leftPadding: 20
                        text: modelData
                        bottomPadding: 2
                        color: pluginStates[index] === "0" ? "gray" : "black"
                    }

                    onCheckedChanged: {
                        if (checked && !dialog.selectedIndexes.includes(index)) {
                            dialog.selectedIndexes.push(index);
                        } else if (!checked && dialog.selectedIndexes.includes(index)) {
                            dialog.selectedIndexes.splice(dialog.selectedIndexes.indexOf(index), 1);
                        }
                    }
                }
            }
        }

        RowLayout {
            spacing: 20
            Layout.alignment: Qt.AlignHCenter
            height: 60

            Button {
                text: "Select All"
                onClicked: {
                    dialog.selectedIndexes = []
                    let i;
                    for (i = 0; i < pluginNames.length; ++i) {
                        dialog.selectedIndexes.push(i)
                    }

                    let j;
                    for (j = 0; j < repeater.count; ++j) {
                        const checkbox = repeater.itemAt(j)
                        if (checkbox) checkbox.checked = true
                    }
                }
            }

            Button {
                text: "Deselect All"
                onClicked: {
                    dialog.selectedIndexes = []
                    let i;
                    for (i = 0; i < pluginNames.length; ++i) {
                        dialog.selectedIndexes.push(i)
                    }

                    let j;
                    for (j = 0; j < repeater.count; ++j) {
                        const checkbox = repeater.itemAt(j)
                        if (checkbox) checkbox.checked = false
                    }
                }
            }

            Button {
                text: "Enable"
                onClicked: {
                    dialog.close()
                    pluginController.enablePlugins(dialog.selectedIndexes)
                }
            }

            Button {
                text: "Disable"
                onClicked: {
                    dialog.close()
                    pluginController.disablePlugins(dialog.selectedIndexes)
                }
            }

            Button {
                text: "Unload"
                onClicked: {
                    dialog.close()
                    pluginController.unloadPlugins(dialog.selectedIndexes)
                }
            }

            Button {
                text: "Close"
                onClicked: dialog.close()
            }
        }
    }

    function selectedPluginNames() {
        return selectedIndexes.map(i => pluginNames[i])
    }

    onVisibleChanged: {
        if (visible) {
            selectedIndexes = []

            for (let i = 0; i < repeater.count; ++i) {
                const checkbox = repeater.itemAt(i)
                if (checkbox) checkbox.checked = false
            }
        }
    }
}
