import QtQuick
import QtQuick.Controls.Material

Item
{
    id: root

    // Controls configs
    Material.theme: Material.Light
    Material.accent: Material.Red

    // Padding
    property int defaultMargin: 5
    property int defaultHeight: 48

    // Colors
    property string accentColor: Qt.rgba(.8, 0, 0, 1)
    property string secondColor: Qt.rgba(.6, 0, 0, 1)

    // Text
    property string textColor: "black"
    property string highlited: "white"
    property int h5: 10
    property int h4: 12
    property int h3: 14
    property int h2: 16
    property int h1: 18
}
