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
    property string title: "Подтвердите действие"
    property string description: "Данное действие приведет к ОТМЕНЕ ОПЕРАЦИИ. Её проведение не будет совершено, а в ЦРМ она будет помечена ошибкой."

    color: Qt.rgba(0, 0, 0, 0)

    Connections
    {
        target: SERVER
        onTaskEnded: close()
    }

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
                    text: "Закрыть"

                    flat: true
                    highlighted: true
                    font.bold: true

                    onClicked: close()
                }

                Button
                {
                    id: confirmButton
                    text: "Подтвердить"

                    flat: true
                    enabled: false
                    font.bold: true

                    onClicked:
                    {
                        SERVER.qmlCancelOperation()
                        close()
                    }

                    Timer
                    {
                        id: enableConfirmTimer
                        repeat: false
                        interval: 5000
                        onTriggered:
                        {
                            confirmButton.enabled = true
                        }
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
