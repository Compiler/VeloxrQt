import QtQuick
import QtQuick.VirtualKeyboard
import QtQuick.Controls
import VulkanRenderItem

ApplicationWindow {
    id: window
    width: 1920
    height: 1080
    visible: true
    title: qsTr("Hello World")

    WindowContainer {
        id: windowContainer
        window: dork

        anchors.centerIn: parent

        width: Math.max(parent.width * 0.5, 200)
        height: Math.max(parent.height * 0.5, 200)
    }

    Slider {
        anchors.top: parent.top
        onValueChanged: {
        }
    }
    Window {
        id: dork
        visible: false
        width: 1080
        height: 720

        VulkanRenderItem {
            id: vki
            anchors.fill:parent

            MouseArea {
                anchors.fill: parent
                onPositionChanged: {
                }
            }
        }
    }
}
