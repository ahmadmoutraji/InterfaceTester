import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 900
    height: 600
    visible: true
    title: "Interface Tester"
    color: "#0f141a"

    property color panelColor: "#171f26"
    property color accentColor: "#2ec4b6"
    property color textColor: "#e6edf3"
    property color mutedTextColor: "#9aa7b2"
    property color outlineColor: "#26313b"

    function startTest(action) {
        if (action === "led") {
            testManager.startLedTest()
        } else if (action === "lan") {
            testManager.startLanTest()
        } else if (action === "temp") {
            testManager.startTemperatureTest()
        } else if (action === "display") {
            testManager.startDisplayTest()
        }
    }

    property bool displayPromptVisible: false
    property bool displayQuestionVisible: false
    property string displayColorHex: "#ff0000"
    property string displayColorName: "Red"
    property bool ledPromptVisible: false

    Timer {
        id: displayPromptTimer
        interval: 1000
        repeat: false
        onTriggered: displayQuestionVisible = true
    }

    Connections {
        target: testManager
        function onDisplayPromptRequested(colorName, colorHex) {
            displayPromptVisible = true
            displayQuestionVisible = false
            displayColorName = colorName
            displayColorHex = colorHex
            displayPromptTimer.restart()
        }
        function onLedPromptRequested() {
            ledPromptVisible = true
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 96
            color: root.panelColor
            radius: 12
            border.color: root.outlineColor
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 5
                spacing: 12

                Item {
                    Layout.fillWidth: true
                }

                Label {
                    text: "Primary IP: "
                    color: root.mutedTextColor
                    font.pixelSize: 22
                    horizontalAlignment: Text.AlignHCenter
                }
                Label {
                    text: networkInfo.primaryIp
                    color: root.textColor
                    font.pixelSize: 20
                    font.weight: Font.DemiBold
                    horizontalAlignment: Text.AlignHCenter
                }
                Item {
                    Layout.fillWidth: true
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Repeater {
                model: [
                    { label: "Test LED", action: "led" },
                    { label: "Test LAN", action: "lan" },
                    { label: "Test Temperature", action: "temp" },
                    { label: "Test Display", action: "display" }
                ]
                delegate: Button {
                    id: testButton
                    text: modelData.label
                    Layout.fillWidth: true
                    Layout.preferredHeight: 48
                    font.pixelSize: 14
                    font.weight: Font.Medium
                    onClicked: root.startTest(modelData.action)

                    background: Rectangle {
                        color: testButton.down ? "#1e2a33" : root.panelColor
                        radius: 10
                        border.color: testButton.hovered ? root.accentColor : root.outlineColor
                        border.width: 1
                    }

                    contentItem: Text {
                        text: testButton.text
                        color: root.textColor
                        font: testButton.font
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: root.panelColor
            radius: 12
            border.color: root.outlineColor
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 8

                Label {
                    text: "Results"
                    color: root.mutedTextColor
                    font.pixelSize: 14
                }

                TextArea {
                    id: resultsArea
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    readOnly: true
                    wrapMode: TextArea.Wrap
                    color: root.textColor
                    text: testManager.resultsText
                    background: Rectangle {
                        color: "#12181f"
                        radius: 8
                        border.color: root.outlineColor
                        border.width: 1
                    }
                }
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: displayColorHex
        visible: displayPromptVisible
        z: 10

        Rectangle {
            width: parent.width * 0.7
            height: 200
            radius: 12
            color: "#10161c"
            anchors.centerIn: parent
            visible: displayQuestionVisible

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 24
                spacing: 16

                Label {
                    text: "Do you see a solid " + displayColorName.toLowerCase() + " screen?"
                    color: root.textColor
                    font.pixelSize: 20
                    font.weight: Font.DemiBold
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                    wrapMode: Text.Wrap
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 16

                    Button {
                        text: "Yes"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 44
                        onClicked: {
                            displayPromptVisible = false
                            displayQuestionVisible = false
                            displayPromptTimer.stop()
                            testManager.submitDisplayResponse(true)
                        }
                    }
                    Button {
                        text: "No"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 44
                        onClicked: {
                            displayPromptVisible = false
                            displayQuestionVisible = false
                            displayPromptTimer.stop()
                            testManager.submitDisplayResponse(false)
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        visible: ledPromptVisible
        color: "#10161c"
        z: 11

        Rectangle {
            width: parent.width * 0.7
            height: 200
            radius: 12
            color: "#0f141a"
            border.color: root.outlineColor
            border.width: 1
            anchors.centerIn: parent

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 24
                spacing: 16

                Label {
                    text: "Did the LED turn on?"
                    color: root.textColor
                    font.pixelSize: 20
                    font.weight: Font.DemiBold
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                    wrapMode: Text.Wrap
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 16

                    Button {
                        text: "Yes"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 44
                        onClicked: {
                            ledPromptVisible = false
                            testManager.submitLedResponse(true)
                        }
                    }
                    Button {
                        text: "No"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 44
                        onClicked: {
                            ledPromptVisible = false
                            testManager.submitLedResponse(false)
                        }
                    }
                }
            }
        }
    }
}
