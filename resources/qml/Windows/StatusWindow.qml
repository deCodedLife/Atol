import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Material

import Presets
import "../Custom"

ApplicationWindow
{
    id: root

    x: Screen.width / 2 - width / 2
    y: 0

    width: 500
    height: 100

    minimumWidth: width
    minimumHeight: height

    maximumWidth: width
    maximumHeight: height

    flags: Qt.CustomizeWindowHint | Qt.WindowStaysOnTopHint | Qt.WA_TranslucentBackground | Qt.FramelessWindowHint
    visible: false
    color: Qt.rgba(0,0,0,0)

    Material.theme: Material.Light
    Material.accent: Material.Teal

    Connections
    {
        target: TRAY
        function onClosed() { close() }
    }

    Connections
    {
        target: MEWBAS

        function onNewTask(task)
        {
            status.text = "Получена задача"
            // TODO: Введите код на терминале
        }
    }

    ConfirmWindow
    {
        id: confirmWindow
        visible: false
    }

    Connections
    {
        target: SERVER

        function onStatusChanged()
        {
            status.text = SERVER.status

            if ( SERVER.status != SERVER.cashboxStatus && SERVER.status != SERVER.terminalStatus)
            {
                textStatus.visible = false
            }
            else
            {
                textStatus.visible = true
            }

            if ( SERVER.status == SERVER.cashboxStatus )
            {
                cancel.visible = false
                progress.arcEnd = 10
                currentOperation.duration = 1000 * 10
            }
            else
            {
                cancel.visible = true
                progress.arcEnd = 60 * 2
                currentOperation.duration = 1000 * 60 * 2
            }

            currentOperation.restart()
            cancelDuration.start()
        }

        function onTaskEnded() { close() }
    }

    Rectangle
    {
        anchors.fill: parent
        radius: 20
        color: "white"

        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: 10

            RowLayout
            {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Text
                {
                    id: textStatus
                    text: "Статус: "
                    font.pointSize: 14
                    font.bold: true
                }

                Text
                {
                    id: status
                    Layout.fillWidth: true
                    text: "Ожидание оплаты терминалом"
                    font.pointSize: 14
                }

                CircleProgress
                {
                    id: progress
                    size: 40
                    colorCircle: Presets.accentColor
                    colorBackground: "#E6E6E6"
                    showBackground: true
                    arcBegin: 0
                    arcEnd: 60 * 2
                    isPie: true

                    NumberAnimation on arcEnd
                    {
                        id: currentOperation
                        duration: 1000 * 60 * 2
                        from: 360
                        to: 0
                        running: true
                    }
                }
            }

            Button
            {
                id: cancel
                visible: false
                Layout.fillHeight: true

                highlighted: true
                enabled: true
                flat: true

                text: "Отменить операцию"
                font.pointSize: 12
                font.bold: true

                Timer
                {
                    id: cancelDuration
                    interval: 5000
                    repeat: false
                    onTriggered: cancel.enabled = true
                }

                onClicked:
                {
                    confirmWindow.show()
                    confirmWindow.reset()
                }
            }
        }
    }

}
