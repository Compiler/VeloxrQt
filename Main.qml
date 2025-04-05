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

        VulkanRenderItem {
            anchors.fill:parent
        }

    }


}
