import QtQuick

Rectangle
{
    id: root

    clip: true
    color: presets.accentColor

    property string title: ""
    property bool enabled: true

    signal clicked();

    Item
    {
        id: button
        anchors.fill: parent
        clip: true

        Rectangle
        {
            id: circle

            height: 0
            width: 0
            color: "white"
            opacity: .2

            transform: Translate
            {
                x: -circle.width / 2
                y: -circle.height / 2
            }

            PropertyAnimation
            {
                id: circleAnimation

                from: 0
                to: button.width * 3
                duration: 300
                easing.type: Easing.OutInQuad

                target: circle
                properties: "width,height,radius"

                onStopped: {
                    circle.width = 0
                    circle.height = 0
                }
            }

        }

        Text
        {
            id: textItem

            text: title
            anchors.centerIn: parent

            color: root.enabled ? presets.highlited : "grey"
            font.bold: true
            font.pointSize: presets.h4
        }

        MouseArea
        {
            enabled: root.enabled
            anchors.fill: parent
            hoverEnabled: true
            onHoveredChanged:
            {
                if (containsMouse && root.color != presets.secondColor)
                {
                    root.color = presets.secondColor
                }
                else {
                    root.color = presets.accentColor
                }
            }
            onClicked:
            {
                circleAnimation.stop()
                root.clicked()
            }
            onPressed:
            {
                circle.x = mouseX
                circle.y = mouseY
                circleAnimation.start()
            }
        }

    }

    Presets
    {
        id: presets
    }
}
