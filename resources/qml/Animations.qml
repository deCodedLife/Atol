import QtQuick 2.0
//import QtGraphicalEffects 1.0

Item
{
    id: standartAnimations
    anchors.fill: parent

    Rectangle
    {
        id: rootObj
        opacity: 0
        color: Qt.rgba(0, 0, 0, 1)
        anchors.fill: parent

        Image
        {
            id: centerImage
            height: 64
            width: 64
            opacity: parent.opacity
            anchors.centerIn: parent
            sourceSize: Qt.size(centerImage.height, centerImage.width)

            /*
            ColorOverlay {
                id: colorOverlay
                anchors.fill: centerImage
                source: centerImage
                color:  "green"
                antialiasing: true
            }
              */
        }

        NumberAnimation on opacity
        {
            property bool isDone: false
            id: showByOpacity
            from: isDone ? 0 : 1
            to: isDone ? 1 : 0
            duration: 200
            onStopped: isDone = !isDone
        }

        ColorAnimation on color
        {
            property bool isDone: false
            id: showByColor
            duration: 200
            from: isDone ? Qt.rgba(0, 0, 0, 1) : Qt.rgba(0, 0, 0, .2)
            to: isDone ? Qt.rgba(0, 0, 0, .2) : Qt.rgba(0, 0, 0, 1)
            onStopped: isDone = !isDone
        }
    }

    MouseArea
    {
        id: clickArea
        anchors.fill: parent
        visible: false
    }

    function verifyAnimation()
    {
        clickArea.visible = true
        centerImage.source = "qrc:/done_icon.svg"

        showByOpacity.start()
        showByColor.start()

        delay.interval = 500
        delay.triggered.connect( function () {
            showByOpacity.start()
            showByColor.start()
            clickArea.visible = false
        })
        delay.start()
    }

    Timer
    {
        id: delay
        repeat: false
    }
}
