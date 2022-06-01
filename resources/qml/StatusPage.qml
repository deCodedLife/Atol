import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import 'custom'

Item
{
    id: root

    property Provider provider: parent.provider
    anchors.fill: parent

    Rectangle
    {
        width: 1
        height: parent.height
        color: "black"
        opacity: .1
        anchors.left: parent.left
        z: 10
    }

    ColumnLayout
    {
        anchors.fill: parent

        Rectangle
        {
            id: topPanel

            Layout.fillWidth: true
            height: presets.defaultHeight
            color: presets.accentColor

            RowLayout
            {
                anchors.fill: parent
                anchors.margins: presets.defaultMargin

                Text
                {
                    text: "Сервер Атол"
                    color: presets.highlited
                    font.pointSize: presets.h2
                    font.bold: true
                }
            }
        }

        ColumnLayout
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Layout.margins: presets.defaultMargin

            RowLayout
            {
                Layout.fillWidth: true
                height: presets.defaultHeight

                Text
                {
                    text: "Касса "
                    color: presets.textColor
                    font.pointSize: presets.h2
                    font.bold: true
                }
            }

            Switch
            {
                id: onlineSwitcher

                state: "offline"
                states: [
                    State
                    {
                        name: "online"
                        PropertyChanges
                        {
                            target: onlineSwitcher
                            text: "Онлайн"
                        }
                    },
                    State
                    {
                        name: "offline"
                        PropertyChanges
                        {
                            target: onlineSwitcher
                            text: "Оффлайн"
                        }
                    }
                ]
                checked: false

                onCheckedChanged:
                {
                    if ( checked )
                    {
                        state = "online"
                        provider.cashboxSetOnline()
                    }
                    else
                    {
                        state = "offline"
                        provider.cashboxSetOffline()
                    }
                }
            }

            RowLayout
            {
                Layout.fillWidth: true

                Button
                {
                    id: closeShift
                    highlighted: true
                    text: "Закрытие смены"

                    onClicked:
                    {
                        enabled = false
                        provider.closeShift()

                        delay.interval = 2000
                        delay.onTriggered.connect(() => { closeShift.enabled = true })
                        delay.start()
                    }
                }

                Button
                {
                    id: xReport

                    highlighted: true
                    text: "X отчет"

                    onClicked:
                    {
                        enabled = false
                        provider.xReport()

                        delay.interval = 2000
                        delay.onTriggered.connect(() => { xReport.enabled = true })
                        delay.start()
                    }
                }

                Button
                {
                    id: resultsReconciliation
                    highlighted: true
                    text: "Сверка итогов"

                    onClicked:
                    {
                        enabled = false
                        provider.resultsReconciliation()

                        delay.interval = 2000
                        delay.onTriggered.connect(() => { resultsReconciliation.enabled = true })
                        delay.start()
                    }
                }
            }

            VerticalSeparator{}

            Rectangle
            {
                Layout.fillHeight: true
            }
        }

    }

    Timer
    {
        id: delay
        interval: 0
    }

    Presets
    {
        id: presets
    }
}
