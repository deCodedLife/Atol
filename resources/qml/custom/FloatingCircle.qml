import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import "../"

RoundButton
{
    property string iconSrc: "qrc:/images/settings.svg"
    property Loader loader: parent.pload

    property var pages: 
    [
        {
            "title": "Операции",
            callback: () => {
                loader.setSource("../Receipts.qml")
                subWindow.hide()
            },
            "enabled": true
        },
    	{
    		"title": "Касса",
    		callback: () => {
                loader.setSource("../Cashbox.qml")
                subWindow.hide()
    		},
    		"enabled": true
    	},
    	{
    		"title": "JWT",
    		callback: () => {
                loader.setSource("../JWT.qml")
                subWindow.hide()
    		},
    		"enabled": true
    	},
    	{
    		"title": "API",
    		callback: () => {
                loader.setSource("../Settings.qml")
                subWindow.hide()
    		},
    		"enabled": true
    	}
    ]

    Rectangle
    {
    	id: subWindow

    	width: 200
    	height: 0

    	x: -200 + 64
    	y: -250 + 64

    	color: presets.accentColor
    	radius: 5
        clip: true

    	ListView
    	{
            id: optionsList

    		width: parent.width
    		height: parent.height - 64
            clip: true

    		model: pages
    		delegate: ListComponent
    		{
                width: subWindow.width
                height: presets.defaultHeight

                title: modelData["title"]
                enabled: modelData["enabled"]

                onClicked:
                {
                    pages[index]["callback"]()
                }
    		}
    	}

        state: "closed"

    	states: [
    		State
    		{
    			name: "closed"
    		},
    		State
    		{
    			name: "opened"
    		}
    	]

    	NumberAnimation on height
    	{
    		id: showAnimation
    		from: 0
    		to: 250
    		running: false
    		duration: 50
            onStopped: optionsList.height = subWindow.height - 64
    	}

    	NumberAnimation on height
    	{
    		id: hideAnimation
    		from: 250
    		to: 0
    		running: false
    		duration: 50
            onStopped: optionsList.height = 0
    	}

        function hide()
        {
            hideAnimation.start()
            subWindow.state = "closed"
        }
    }

	anchors.right: parent.right
	anchors.bottom: parent.bottom
	anchors.margins: 10
	
    icon.source: iconSrc
	highlighted: true

    onClicked: 
    {
    	if ( subWindow.state == "closed" )
    	{
    		showAnimation.start()
    		subWindow.state = "opened"
    		return
    	}

    	if ( subWindow.state == "opened" )
    	{
    		hideAnimation.start()
    		subWindow.state = "closed"
    	}
    }

	Presets
	{
		id: presets
	}
}
