import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Presets
import "../Custom"
import "../Windows"

Item
{
    id: root

    anchors.fill: parent
    property var configs: CONFIGS.data
    property var allReciepts: RECIEPTS.reciepts
    property var visibleReciepts: [{}]

    ReturnOptions
    {
        id: returnOptions
        visible: false
    }

    Connections
    {
        target: RECIEPTS

        function onRecieptsChanged(reciepts)
        {
            allReciepts = reciepts
            visibleReciepts = reciepts
        }
    }

    Component.onCompleted: 
    {
        allReciepts = RECIEPTS.reciepts
        visibleReciepts = allReciepts
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
        spacing: 0

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
                    Layout.fillWidth: true
                    text: "Последние операции"
                    color: Presets.highlited

                    font.pointSize: Presets.h2
                    font.bold: true
                }

                Rectangle
                {
                    Layout.fillWidth: true
                }

                TextField
                {
                    id: search
                    Layout.fillHeight: true

                    placeholderText: focus ? "" : "Сумма или время"
                    maximumLength: 32

                    leftPadding: 42
                    bottomPadding: 8

                    width: Presets.defaultHeight * 6
                    implicitWidth: Presets.defaultHeight * 6

                    font.pointSize: Presets.h4
                    background: Rectangle
                    {
                        radius: 5
                        color: "white"

                        border.width: 1
                        border.color: "white"

                        Button
                        {
                            icon.width: parent.height * 2
                            icon.height: parent.height * 2
                            icon.source: "qrc:/images/search_icon.svg"

                            width: parent.height
                            height: parent.height

                            flat: true
                            anchors.left: parent.left
                            anchors.leftMargin: 2
                            anchors.verticalCenter: parent.verticalCenter

                            onClicked: {}
                        }
                    }

                    onTextChanged:
                    {
                        if (text === "")
                        {
                            visibleReciepts = allReciepts
                            return
                        }

                        var myReciepts = []

                        allReciepts.forEach((reciept) =>
                        {
                            if ( reciept["time"].split(text).length > 1 )
                            {
                                myReciepts.push(reciept)
                                return
                            }

                            if( getPaymentSum(reciept).toString() == text )
                            {
                                myReciepts.push(reciept)
                            }
                        })

                        visibleReciepts = myReciepts
                    }
                }
            }
        }

        ListView
        {
            id: recieptsList
            clip: true

            Layout.fillWidth: true
            Layout.fillHeight: true

            model: visibleReciepts
            delegate: Receipt
            {
                Layout.fillWidth: true
                height: Presets.defaultHeight + 10
                task: modelData
                isEven: index % 2 == 0 ? true : false

                onClicked:
                {
                    returnOptions.task = modelData
                    returnOptions.show()
                }
            }
        }
    }

    function getPaymentSum(payment)
    {
        let sum = 0
        let payments = payment["request"]["payments"]

        payments.forEach((payment) => {
            sum += payment["sum"]
        })

        return sum
    }
}
