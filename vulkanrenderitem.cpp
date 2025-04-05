// vulkanrenderitem.cpp
#include "vulkanrenderitem.h"

#include <QGuiApplication>

#ifdef Q_OS_WIN
#include <windows.h>
#include <QWindow>
#elif defined(Q_OS_MACOS)
#include <Cocoa/Cocoa.h>
#include <QuartzCore/CAMetalLayer.h>
#endif

VulkanRenderItem::VulkanRenderItem(QQuickItem *parent) : QQuickItem(parent) {

    connect(this, &QQuickItem::windowChanged, this, &VulkanRenderItem::handleWindowChanged);
}

VulkanRenderItem::~VulkanRenderItem() {
    cleanup();
}

void VulkanRenderItem::handleWindowChanged(QQuickWindow *win) {
    if (win) {
        connect(win, &QQuickWindow::afterRendering, this, &VulkanRenderItem::handleAfterRendering, Qt::DirectConnection);
    }
}

void VulkanRenderItem::handleAfterRendering() {

    if (!m_windowHandleReady) {
        m_windowHandle = getNativeWindowHandle();

        if (m_windowHandle) {
            m_windowHandleReady = true;
            emit readyChanged();

            // Disconnect so this is only called once
            if (window()) {
                disconnect(window(), &QQuickWindow::afterRendering, this, &VulkanRenderItem::handleAfterRendering);
            }
        }
    }
}

void VulkanRenderItem::cleanup()
{
    // Clean up any resources if needed
    m_windowHandle = nullptr;
    m_windowHandleReady = false;
}

void VulkanRenderItem::releaseResources()
{
    cleanup();
}

void* VulkanRenderItem::getNativeWindowHandle()
{
    if (!window()) {
        return nullptr;
    }

#ifdef Q_OS_WIN
    // Get the native Windows HWND
    HWND hwnd = (HWND)window()->winId();
    return hwnd;

#elif defined(Q_OS_MACOS)
    // For macOS, we need to get the NSView
    NSView* nsView = (NSView*)window()->winId();
    if (!nsView) {
        return nullptr;
    }

    // Depending on what your external library needs:
    // If it needs the NSView directly:
    return (__bridge void*)nsView;

    // If it needs a CAMetalLayer (common for Vulkan/MoltenVK):
    /*
    // Make sure the view is layer-backed
    [nsView setWantsLayer:YES];

    // Create a CAMetalLayer if it doesn't exist
    if (![nsView.layer isKindOfClass:[CAMetalLayer class]]) {
        CAMetalLayer* metalLayer = [CAMetalLayer layer];
        [nsView setLayer:metalLayer];
    }

    return (__bridge void*)nsView.layer;
    */
#else
    return nullptr;
#endif
}

void* VulkanRenderItem::getWindowHandle()
{
    return m_windowHandle;
}

void VulkanRenderItem::initializeExternalRenderer() {
    qDebug() << "Window handle loading.";
    if (!m_windowHandleReady || !m_windowHandle) {
        qWarning() << "Window handle not yet available";
        return;
    }
    qDebug() << "Running: ";

    core.init(m_windowHandle);

}
