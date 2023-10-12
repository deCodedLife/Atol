import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs

import Presets

Item
{
    id: root

    anchors.fill: parent
    property var configs

    Component.onCompleted: configs = CONFIGS.data

    MessageDialog {
        id: messageDialog
        title: "AtolServer 2.3"
        text: "Данные успешно изменены"
        buttons: MessageDialog.Ok
        onAccepted: {}
        visible: false
    }

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
                    text: "Настройки JWT"
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
                id: loginTextField
                Layout.fillWidth: true
                height: Presets.defaultHeight

                text: configs["jwtUsername"] !== "test@mail.ru" ? configs["jwtUsername"] : ""
                placeholderText: "Enter login"
            }

            TextField
            {
                id: passwordTextField
                Layout.fillWidth: true
                height: Presets.defaultHeight
                echoMode: TextField.Password

                text: configs["jwtPassword"] !== "1234" ? configs["jwtPassword"] : ""
                placeholderText: "Enter password"
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
                        changeData()
                        messageDialog.visible = true
                    }
                }

            }
        }

    }

    function changeData()
    {
        let login = loginTextField.text.trim()
        let password = passwordTextField.text.trim()

        login = login !== "" ? login: configs["jwtUsername"]
        password = password !== "" ? password : configs["jwtPassword"]

        configs["jwtUsername"] = login
        configs["jwtPassword"] = password

        CONFIGS.data = configs
    }
}
