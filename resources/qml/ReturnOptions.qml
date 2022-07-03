import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

ApplicationWindow
{
    id: root
    visible: false

    width: 250
    height: 72

    minimumWidth: width
    minimumHeight: height

    maximumWidth: width
    maximumHeight: height

    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2

    flags: Qt.Window | Qt.CustomizeWindowHint | Qt.NoDropShadowWindowHint | Qt.WindowStaysOnTopHint
    color: Qt.rgba(0, 0, 0, 0)

    Material.theme: Material.Light
    Material.accent: Material.Red

    property var task: [{}]
    property double ePaySum: getEpaySum(task)

    Rectangle
    {
        anchors.fill: parent

        color: "white"

        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: 5

            RowLayout
            {
                Layout.fillWidth: true

                Button
                {
                    Layout.fillWidth: true
                    text: "Без чека"

                    highlighted: true
                    enabled: ePaySum !== 0 ? true : false

                    onClicked:
                    {
                        SERVER.qmlReturnEpay(ePaySum)
                        RECIEPTS.qmlReturnEpay(task)
                        close()
                    }
                }

                Button
                {
                    Layout.fillWidth: true
                    
                    text: "С чеком"
                    highlighted: true
                    
                    onClicked:
                    {
                        RECIEPTS.remove(task)
                        close()
                    }
                }
            }
        }
    }

    function getEpaySum(task)
    {
        let sum = 0
        let payments = task["request"]["payments"]

        payments.forEach((payment) => {
            if (payment["type"] == "1")
            {
                sum += payment["sum"]
            }
        })

        return sum
    }

    Presets
    {
        id: presets
    }
}
