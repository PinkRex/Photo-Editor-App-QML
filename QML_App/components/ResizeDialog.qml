import QtQuick
import QtQuick.Controls.Fusion

Popup {
    property int widthValue: 800
    property int heightValue: 600
    signal accepted(int width, int height)

    id: resizePopup
    anchors.centerIn: parent
    width: 230
    height: 150
    modal: true
    focus: true

    Column {
        anchors.centerIn: parent
        spacing: 10
        Column {
            spacing: 5
            Text { text: "Enter Width:" }
            TextField {
                id: widthInput
                width: 190
                height: 25
                text: resizePopup.widthValue.toString()
                validator: RegularExpressionValidator { regularExpression: /[0-9]+/ }
                leftPadding: 8
                rightPadding: 8
                verticalAlignment: Text.AlignVCenter

                background: Rectangle {
                    color: "white"
                    border.color: "#999"
                    radius: 4
                }
            }
        }

        Column {
            spacing: 5
            Text { text: "Enter Height:" }
            TextField {
                id: heightInput
                width: 190
                height: 25
                text: resizePopup.heightValue.toString()
                validator: RegularExpressionValidator { regularExpression: /[0-9]+/ }
                leftPadding: 8
                rightPadding: 8
                verticalAlignment: Text.AlignVCenter

                background: Rectangle {
                    color: "white"
                    border.color: "#999"
                    radius: 4
                }
            }
        }

        Row {
            spacing: 10
            Button {
                width: 50
                height: 30
                text: "OK"
                onClicked: {
                    resizePopup.widthValue = parseInt(widthInput.text)
                    resizePopup.heightValue = parseInt(heightInput.text)
                    resizePopup.accepted(resizePopup.widthValue, resizePopup.heightValue)
                    resizePopup.close()
                }
            }
            Button {
                width: 70
                height: 30
                text: "Cancel"
                onClicked: resizePopup.close()
            }
        }
    }
}
