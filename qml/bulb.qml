import QtQuick 2.7
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.12

Item {
    width: 250
    height: 270
    property color bulbColor
    Connections {
        target: custom
    }
    Button {
        id:control
        background: Rectangle {
               implicitWidth: 250
               implicitHeight: 270
               color:"#f4f4f4"
               opacity: enabled ? 1 : 0.3
               border.color: control.down ? "#FA8072" : "#696969"
               border.width: 2
               radius: 6

           }

        onClicked: custom.on_pushButton_clicked()
        anchors.fill: parent
        //color: "white"
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        flat:true
        //highlighted: true
    }
//    Slider {
//        id: radslider
//        x: 42
//        y: 0
//        width: 158
//        height: 40
//        from: 1
//        value: 1
//        to: 100
//        stepSize: 1
//    }
//    Text {
//        id: radText
//        width: 36
//        height: 32
//        text: radslider.value
//        font.pointSize: 20
//    }
//    Slider {
//        id: sampslider
//        x: 42
//        y: 54
//        stepSize: 1
//        width: 158
//        height: 40
//        from: 1
//        value: 1
//        to: 100
//    }
//    Text {
//        id: sampText
//        x: 0
//        y: 59
//        width: 36
//        height: 30
//        text: sampslider.value
//        font.pointSize: 20
//    }


    Image {
        id: image
        x: 0
        y: 0
        width: 200
        height: 200
        fillMode: Image.PreserveAspectFit
        source: "LightControl_1.png"
        anchors.fill: parent
        //fillMode: Image.Tile
    }


    Glow {
        anchors.fill: image
        radius: 10
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        samples: 4
        color: bulbColor
        source: image
    }
}
