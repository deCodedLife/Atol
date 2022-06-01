import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.5

import "StatusWindow"

ApplicationWindow {
    id: application
    title: qsTr("Atol Server")

    width: 640
    height: 480

    visible: false

    property var pages: [
        "JWT.qml",
        "Settings.qml",
        "Service.qml"
    ]
    property bool ignoreQuit: true

    Connections
    {
        target: TRAY

        function onShowed()
        {
            application.visible = true
            application.show()
        }

        function onClosed()
        {
            ignoreQuit = false
            close()
        }

        function onIconShowed()
        {
            if (application.visibility == Window.Hidden)
            {
                application.visible = true
                application.show()

                return
            }

            application.visible = false
        }
    }

    Connections
    {
        target: SERVER

        onStatusChanged:
        {
            status.show()
        }
    }

    StatusWindow
    {
        id: status
        visible: false
    }

    LeftPanel
    {
        pload: pageLoader

        id: leftPanel
        anchors.left: parent.left
    }

    Loader
    {
        id: pageLoader
        property Animations standartAnimations: animations

        width: parent.width - leftPanel.width
        height: parent.height
        anchors.right: parent.right

        active: true
        asynchronous: true
        source: "Cashbox.qml"
    }

    Animations
    {
        id: animations
        anchors.fill: parent
    }

    onClosing:
    {
        if (ignoreQuit)
        {
            close.accepted = false
            application.visible = false

            return
        }

        Qt.quit()
    }
}
