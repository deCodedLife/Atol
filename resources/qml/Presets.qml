pragma Singleton

import QtQuick
import QtQuick.Controls.Material

Item
{
    id: root

    // Controls configs
    Material.theme: Material.Light
    Material.accent: Material.Teal

    // Padding
    property int defaultMargin: 5
    property int defaultHeight: 48

    // Colors
    property string accentColor: "#476A6F"
    property string secondColor: "#519E8A"
    property string thirdColor: "#7EB09B"
    property string redColor: "#DE7928"

    // Text
    property string textColor: "black"
    property string highlited: "white"
    property int h5: 10
    property int h4: 12
    property int h3: 14
    property int h2: 16
    property int h1: 18
}
