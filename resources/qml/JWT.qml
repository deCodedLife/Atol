import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Item
{
    id: root

    anchors.fill: parent

    property Animations defaultAnimations: parent.standartAnimations
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
            height: presets.defaultHeight
            color: presets.accentColor

            RowLayout
            {
                anchors.fill: parent
                anchors.margins: presets.defaultMargin

                Text
                {
                    text: "Настройки JWT"
                    color: presets.highlited
                    font.pointSize: presets.h2
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
            Layout.margins: presets.defaultMargin

            TextField
            {
                id: loginTextField
                Layout.fillWidth: true
                height: presets.defaultHeight

                text: configs["jwtUsername"] !== "test@mail.ru" ? configs["jwtUsername"] : ""
                placeholderText: "Enter login"
            }

            TextField
            {
                id: passwordTextField
                Layout.fillWidth: true
                height: presets.defaultHeight
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
                height: presets.defaultHeight

                Rectangle
                {
                    Layout.fillWidth: true
                }

                Button
                {
                    id: saveButton
                    text: "Сохранить"

                    width: 200
                    height: presets.defaultHeight
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
        let login = loginTextField.text.trim()
        let password = passwordTextField.text.trim()

        login = login !== "" ? login: configs["jwtUsername"]
        password = password !== "" ? password : configs["jwtPassword"]

        configs["jwtUsername"] = login
        configs["jwtPassword"] = password

        CONFIGS.data = configs
    }

    Presets
    {
        id: presets
    }
}
