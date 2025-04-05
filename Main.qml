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


    Window {
        visible: true
        width: 1080
        height: 720

        // Gonna render over everthing
        VulkanRenderItem {
            anchors.fill:parent
        }
        Rectangle {
            width: 50
            height: 50
            color: "red"
        }

    }


}
