import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Presets

Item
{
    id: root

    property var task: root.modelData
    
    visible: task["active"]
    enabled: task["active"]

    property bool isEven: false
    property bool hasCash: false
    property bool hasEpay: false
    property string textColor: getColor() 

    signal returnOperation
    signal printReceipt
    // property string textColor: task["status"] == "done" ? Qt.rgba(0.7, 0.7, 0.7, 1) : "white"
    Component.onCompleted: {
        console.log( "test lol" )
        console.log( root.width, root.height )
    }

    function backgroundColor()
    {
        if ( task["status"] != "done" )
        {
            return Presets.redColor
        }

        if ( isEven )
        {
            return Presets.thirdColor
        }

        return Qt.rgba(0,0,0,0)
    }

    function getColor() 
    {
        if ( task["status"] != "done" )
        {
            return "white"
        }

        if ( isEven )
        {
            return "white"
        }

        return "black"
    }

    Rectangle
    {
        anchors.fill: parent
        color: backgroundColor()

        RowLayout
        {
            anchors.fill: parent
            anchors.margins: 10

            Rectangle
            {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Qt.rgba(0,0,0,0)

                RowLayout
                {
                    anchors.fill: parent
                    anchors.centerIn: parent
                    spacing: 0

                    Text
                    {
                        Layout.alignment: Qt.AlignLeft

                        color: textColor
                        text: "Сумма: "

                        font.bold: false
                        font.pointSize: Presets.h4
                    }

                    Text
                    {
                        Layout.alignment: Qt.AlignLeft

                        text: getPaymentSum()
                        color: textColor

                        font.bold: true
                        font.pointSize: Presets.h4
                    }

                    Rectangle { Layout.fillWidth: true }
                }
            }

            Rectangle
            {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Qt.rgba(0,0,0,0)

                Text
                {
                    anchors.centerIn: parent

                    text: payments()
                    color: textColor
                    
                    font.bold: false
                    font.pointSize: Presets.h5

                    function payments()
                    {
                        var types = ""

                        if ( hasCash )
                        {
                            types += "Наличные"
                        }

                        if ( hasCash && hasEpay )
                        {
                            types += "+"
                        }

                        if ( hasEpay )
                        {
                            types += "Безнал"
                        }

                        return types
                    }
                }
            }

            Rectangle
            {
                id: buttonParent
                
                color: Qt.rgba(0,0,0,0)
                clip: true
                
                Layout.fillWidth: true
                Layout.fillHeight: true

                RowLayout
                {
                    anchors.fill: parent
                    spacing: 20
                    Rectangle { Layout.fillWidth: true }

                    Text
                    {
                        text: task["time"]
                        color: textColor

                        font.bold: true
                        font.pointSize: Presets.h5
                    }

                    Image {
                        source: "qrc:/images/receipt_icon.svg"
                        Layout.preferredWidth: Presets.defaultHeight - 16
                        Layout.preferredHeight: Presets.defaultHeight - 16

                        MouseArea {
                            anchors.fill: parent
                            onClicked: root.printReceipt()
                        }
                    }

                    Image {
                        source: "qrc:/images/close_icon.svg"
                        Layout.preferredWidth: Presets.defaultHeight - 16
                        Layout.preferredHeight: Presets.defaultHeight - 16

                        MouseArea {
                            anchors.fill: parent
                            onClicked: root.returnOperation()
                        }
                    }
                }
            }
        }
    }

    function getPaymentSum()
    {
        let sum = 0
        let payments = task["request"]["payments"]

        payments.forEach((payment) => {

            if ( payment["type"] == "1" )
            {
                hasEpay = true
            }

            if ( payment["type"] == "cash" )
            {
                hasCash = true
            }

            sum += payment["sum"]
        })

        return sum
    }
}
