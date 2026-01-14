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
                anchors.margins: 16
                spacing: 12

                Label {
                    text: "Primary IP"
                    color: root.mutedTextColor
                    font.pixelSize: 14
                }
                Label {
                    text: "Not Available"
                    color: root.textColor
                    font.pixelSize: 20
                    font.weight: Font.DemiBold
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Repeater {
                model: ["Test LED", "Test LAN", "Test Temperature", "Test Display"]
                delegate: Button {
                    id: testButton
                    text: modelData
                    Layout.fillWidth: true
                    Layout.preferredHeight: 48
                    font.pixelSize: 14
                    font.weight: Font.Medium

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
                    background: Rectangle {
                        color: "#12181f"
                        radius: 8
                        border.color: root.outlineColor
                        border.width: 1
                    }
                    text: "Results will appear here."
                }
            }
        }
    }
}
