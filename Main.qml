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
    Window {
        id: dork
        visible: false
        width: 1080
        height: 720

        // Gonna render over everthing, kinda
        VulkanRenderItem {
            id: vki
            anchors.fill:parent
            Slider {
                anchors.top: parent.top
                onValueChanged: {
                    vki.zoom = value
                }
            }

            MouseArea {
                anchors.fill: parent
                onPositionChanged: {
                    vki.panX += (mouseX - previousX) * 0.1
                    vki.panY += (mouseY - previousY) * 0.1
                }
            }
        }
    }
}
