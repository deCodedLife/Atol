import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import ".."

Item
{
    id: root

    property var task: root.modelData
    
    visible: task["active"]
    enabled: task["active"]

    signal clicked
    // property string textColor: task["status"] == "done" ? Qt.rgba(0.7, 0.7, 0.7, 1) : "white"

    Rectangle
    {
        anchors.fill: parent
        color: task["status"] == "done" ? Qt.rgba(0,0,0,0) : "#80ff5959"

        RowLayout
        {
            anchors.fill: parent
            anchors.margins: 5

            Rectangle
            {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Qt.rgba(0,0,0,0)

                Text
                {
                    anchors.centerIn: parent

                    text: getPaymentSum()
                    color: "black"

                    font.bold: true
                    font.pointSize: presets.h2
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

                    text: task["time"]
                    color: "black"
                    
                    font.bold: true
                    font.pointSize: presets.h5
                }
            }

            Rectangle
            {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Qt.rgba(0,0,0,0)

                Rectangle
                {
                    anchors.fill: parent
                    anchors.right: parent.right
                    color: Qt.rgba(0,0,0,0)

                    Button
                    {
                        anchors.centerIn: parent

                        width: parent.height - 5
                        height: parent.height - 5

                        icon.source: "qrc:/images/close_icon.svg"
                        highlighted: true

                        onClicked: root.clicked()
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
            sum += payment["sum"]
        })

        return sum
    }

    Presets
    {
        id: presets
    }
}
