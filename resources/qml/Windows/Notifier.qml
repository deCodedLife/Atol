import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Presets
import "../Custom"

ApplicationWindow
{
    id: root
    visible: false

    width: 500
    height: 280

    minimumWidth: width
    minimumHeight: height

    maximumWidth: width
    maximumHeight: height

    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2

    Material.theme: Material.Light
    Material.accent: Material.Teal

    flags: Qt.Window | Qt.CustomizeWindowHint | Qt.NoDropShadowWindowHint | Qt.WindowStaysOnTopHint

    property var action
    property string title: ""
    property string description: ""

    color: Qt.rgba(0, 0, 0, 0)

    Rectangle
    {
        anchors.fill: parent
        color: "white"

        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: 10

            Text
            {
                Layout.fillWidth: true

                id: titleObject
                text: title

                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.pointSize: 16
                font.bold: true
            }

            VerticalSeparator{}

            Text
            {
                Layout.fillWidth: true

                id: descriptionObject
                text: description

                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.pointSize: 14
            }

            Rectangle
            {
                Layout.fillHeight: true
            }

            RowLayout
            {
                Layout.fillWidth: true

                Rectangle
                {
                    Layout.fillWidth: true
                }

                Button
                {
                    id: confirmButton
                    text: "Возобновить работу"

                    flat: true
                    enabled: true
                    font.bold: true

                    onClicked:
                    {
                        SERVER.qmlRestoreServer()
                        close()
                    }
                }
            }

        }

    }

    function reset()
    {
        confirmButton.enabled = false
        enableConfirmTimer.start()
    }
}
