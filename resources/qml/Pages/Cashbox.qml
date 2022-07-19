import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Presets

Item
{
    id: root

    anchors.fill: parent
    property var configs: CONFIGS.data

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
            Layout.fillWidth: true
            height: Presets.defaultHeight
            color: Presets.accentColor

            RowLayout
            {
                anchors.fill: parent
                anchors.margins: Presets.defaultMargin

                Text
                {
                    text: "Настройки кассы"
                    color: Presets.highlited
                    font.pointSize: Presets.h2
                    font.bold: true
                }

                Rectangle
                {
                    Layout.fillWidth: true
                }
            }
        }

        ColumnLayout
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: Presets.defaultMargin

            TextField
            {
                id: cashboxIDField
                Layout.fillWidth: true
                height: Presets.defaultHeight
                text: configs["cashboxID"] !== "1000" ? configs["cashboxID"] : ""
                placeholderText: "CashboxID (1000)"
            }

            TextField
            {
                id: serverIDField
                Layout.fillWidth: true
                height: Presets.defaultHeight
                text: configs["serverID"] !== "1001" ? configs["serverID"] : ""
                placeholderText: "ServerID (1001)"
            }

            CheckBox
            {
                id: autostart
                Layout.fillWidth: true
                height: Presets.defaultHeight
                text: "Запускать автоматически"
                checked: configs["autostart"] ? true : false
            }

            Rectangle
            {
                Layout.fillHeight: true
            }

            RowLayout
            {
                Layout.fillWidth: true
                height: Presets.defaultHeight

                Rectangle
                {
                    Layout.fillWidth: true
                }

                Button
                {
                    id: saveButton
                    text: "Сохранить"

                    width: 200
                    height: Presets.defaultHeight
                    highlighted: true

                    onClicked:
                    {
                        defaultAnimations.verifyAnimation()
                        changeData()
                    }
                }
            }
        }

    }

    function changeData()
    {
        let serverID = serverIDField.text.trim() !== "" ? serverIDField.text.trim() : configs["serverID"]
        let cashboxID = cashboxIDField.text.trim() !== "" ? cashboxIDField.text.trim() : configs["cashboxID"]

        configs["serverID"] = serverID
        configs["cashboxID"] = cashboxID
        configs["autostart"] = autostart.checked

        CONFIGS.switchAutostart(configs)
        CONFIGS.data = configs
    }
}
