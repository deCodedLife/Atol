import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

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
                    text: "Настройки cервера"
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
                id: mewbasAddrField
                Layout.fillWidth: true
                height: Presets.defaultHeight
                text: configs["mewbasAddr"] !== "https://api.mewbas.com" ? configs["mewbasAddr"] : ""
                placeholderText: "Api URL (https://api.mewbas.com)"
            }

            TextField
            {
                id: serverAddrField
                Layout.fillWidth: true
                height: Presets.defaultHeight
                text: configs["serverAddr"] !== "http://localhost:16732" ? configs["serverAddr"] : ""
                placeholderText: "Server Url (http://localhost:16732)"
            }

            TextField
            {
                id: terminalDirField
                Layout.fillWidth: true
                height: Presets.defaultHeight
                text: configs["terminalDir"] !== "C:/sc552" ? configs["terminalDir"] : ""
                placeholderText: "Terminal dir (C:/sc552)"
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
        let mewbasAddr = mewbasAddrField.text.trim()
        let serverAddr = serverAddrField.text
        let terminalDir = terminalDirField.text

        mewbasAddr = mewbasAddr !== "" ? mewbasAddr : configs["mewbasAddr"]
        serverAddr = serverAddr !== "" ? serverAddr : configs["serverAddr"]
        terminalDir = terminalDir !== "" ? terminalDir : configs["terminalDir"]

        configs["mewbasAddr"] = mewbasAddr
        configs["serverAddr"] = serverAddr
        configs["terminalDir"] = terminalDir

        CONFIGS.data = configs
    }
}
