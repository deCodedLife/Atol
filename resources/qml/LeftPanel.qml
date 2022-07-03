import QtQuick

Item
{
    id: root

    width: 150
    height: parent.height

    property Loader pload: parent.pload

    property var tiles: [
        {
            title: "Операции",
            callback: () => {
                pload.setSource("Reciepts.qml")
            },
            enabled: true
        },
        {
            title: "Касса",
            callback: () => {
                pload.setSource("Cashbox.qml")
            },
            enabled: true
        },
        {
            title: "JWT",
            callback: () => {
                pload.setSource("JWT.qml")
            },
            enabled: true
        },
        {
            title: "API",
            callback: () => {
                pload.setSource("Settings.qml")
            },
            enabled: true
        }

    ]

    Rectangle
    {
        anchors.fill: parent
        color: presets.accentColor

        ListView
        {
            anchors.fill: parent

            model: tiles
            delegate: ListComponent
            {
                width: root.width
                height: presets.defaultHeight

                title: modelData["title"]
                enabled: modelData["enabled"]

                onClicked:
                {
                    tiles[index]["callback"]()
                }
            }
        }
    }

    Presets
    {
        id: presets
    }
}
