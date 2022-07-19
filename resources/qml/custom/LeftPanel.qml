import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Presets

Item
{
    id: root

    width: minimumWidth
    Layout.fillHeight: true

    property Loader pageLoader
    property var pages: [{}]

    property bool isClosed: true
    property bool isStopped: true


    property var show: () => 
    {
        if ( isStopped )
        {
            showAnimation.start()
        }
    }

    property var hide: () => 
    {
        if ( isStopped )
        {
            pageList.visible = false
            invisible.visible = true

            hideAnimation.start()
        }
    }

    property var toggle: () =>
    {
        console.log("nya")

        if ( isStopped == false )
        {
            return
        }

        if ( isClosed ) show()
        else hide()
    }

    readonly property int maximumWidth: 200 
    readonly property int minimumWidth: Presets.defaultHeight 

    states: 
    [
        State
        {
            name: "minimized"

            PropertyChanges
            {
                target: root
                isClosed: true
                isStopped: true
            }

            PropertyChanges
            {
                target: pageList
                visible: false
            }

            PropertyChanges
            {
                target: invisible
                visible: true
            }
        },
        State
        {
            name: "maximized"
            
            PropertyChanges
            {
                target: root
                isClosed: false
                isStopped: true
            }

            PropertyChanges
            {
                target: pageList
                visible: true
            }

            PropertyChanges
            {
                target: invisible
                visible: false
            }
        }
    ]
    state: "minimized"


    Rectangle
    {
        anchors.fill: parent
        color: Presets.secondColor

        ColumnLayout
        {
            anchors.fill: parent
            spacing: 0

            Rectangle
            {
                width: Presets.defaultHeight
                height: Presets.defaultHeight
                color: Qt.rgba(0,0,0,0)

                Button
                {
                    width: Presets.defaultHeight
                    height: Presets.defaultHeight

                    anchors.centerIn: parent

                    icon.source: "qrc:/images/menu_icon.svg"
                    icon.width: Presets.defaultHeight / 2
                    icon.height: Presets.defaultHeight / 2

                    background: Rectangle
                    {
                        width: Presets.defaultHeight
                        height: Presets.defaultHeight
                        color: Qt.rgba(0,0,0,0)
                    }
                    
                    bottomInset: 0
                    topInset: 0
                    leftInset: 0
                    rightInset: 0
                    highlighted: true

                    display: Button.IconOnly

                    horizontalPadding: 0
                    verticalPadding: 0

                    onClicked: 
                    {
                        if ( isStopped == false )
                        {
                            return
                        }

                        if ( isClosed ) show()
                        else hide()
                    }
                }
            }

            ListView
            {
                id: pageList

                Layout.fillWidth: true
                Layout.fillHeight: true

                model: pages
                delegate: ListComponent
                {
                    width: root.width
                    height: Presets.defaultHeight

                    title: modelData["title"]
                    enabled: modelData["enabled"]

                    onClicked:
                    {
                        pages[index]["callback"]()
                        hide()
                    }
                }
            }

            Rectangle { id: invisible; Layout.fillHeight: true }
        }

    }

    NumberAnimation on width
    {
        id: showAnimation
        
        from: minimumWidth
        to: maximumWidth

        duration: 200
        running: false
        easing.type: Easing.InOutCubic

        onStarted: isStopped = false
        onStopped: state = "maximized"
    }

    NumberAnimation on width
    {
        id: hideAnimation

        from: maximumWidth
        to: minimumWidth
        
        duration: 200
        running: false
        easing.type: Easing.InOutCubic

        onStarted: isStopped = false
        onStopped: state = "minimized"
    }

    onWidthChanged: state = "minimized"
}
