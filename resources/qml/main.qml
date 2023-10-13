import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls

import Presets
import "Custom/"
import "Windows/"

ApplicationWindow
{
    id: application
    title: qsTr("Atol Server")

    width: 640
    height: 480

    visible: false
    Material.theme: Material.Light
    Material.accent: Material.Teal

    Notifier
    {
        id: notifyWindow
        visible: false
    }

    Connections
    {
        target: SERVER
        
        function onNotify( title, message ) 
        {
            notifyWindow.title = title
            notifyWindow.description = message
            notifyWindow.show()
        }
    }

    property var pages: 
    [
        {
            title: "Операции",
            enabled: true,

            callback: () => pageLoader.setSource("Pages/Receipts.qml")
        },
        {
            title: "Касса",
            enabled: true,

            callback: () => pageLoader.setSource("Pages/Cashbox.qml")
        },
        {
            title: "JWT",
            enabled: true,

            callback: () => pageLoader.setSource("Pages/JWT.qml")
        },
        {
            title: "API",
            enabled: true,

            callback: () => pageLoader.setSource("Pages/Settings.qml")
        }
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
            if ( application.visibility == Window.Hidden )
            {
                application.visible = true
                application.show()

                return
            }

            application.visible = false
        }
    }
    

    RowLayout
    {
        anchors.fill: parent
        spacing: 0

        LeftPanel
        {
            id: leftPanel
            pages: application.pages
            Layout.fillHeight: true

            z: 10
        }

        Loader
        {
            id: pageLoader
            z: 0

            Layout.fillWidth: true
            Layout.fillHeight: true

            active: true
            asynchronous: true

            Component.onCompleted: pages[0].callback()
            
            function reset()
            {
                setSource("NotExisting.qml")
            }

            function reload() 
            {
                var lastSourse = source
                reset()

                setSource(lastSourse)
            }
        }
    }


    StatusWindow
    {
        id: status
        visible: false

        Connections
        {
            target: SERVER
            onStatusChanged: status.show()
        }
    }

    onClosing: function(close)
    {
        if (ignoreQuit)
        {
            close.accepted = false
            application.visible = false
            pageLoader.reload()

            return
        }

        Qt.quit()
    }
}
