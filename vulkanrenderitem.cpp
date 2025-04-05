#include "vulkanrenderitem.h"
#include <QDebug>
#include <QQuickWindow>

VulkanRenderItem::VulkanRenderItem(QQuickItem *parent) : QQuickItem(parent) {
    // Set up necessary flags for rendering
    setFlag(QQuickItem::ItemHasContents, true);

    // Connect to window changed signal to handle window initialization
    connect(this, &QQuickItem::windowChanged, this, &VulkanRenderItem::handleWindowChanged);
}

VulkanRenderItem::~VulkanRenderItem() {
    cleanup();
}

void VulkanRenderItem::handleWindowChanged(QQuickWindow *win) {
    if (win) {
        connect(win, &QQuickWindow::afterRendering, this, &VulkanRenderItem::handleAfterRendering, Qt::DirectConnection);
        connect(win, &QQuickWindow::beforeRendering, this, &VulkanRenderItem::render, Qt::DirectConnection);

        //win->setClearBeforeRendering(false);
    }
}

void VulkanRenderItem::handleAfterRendering() {
    if (!m_windowHandleReady) {
        m_windowHandle = getNativeWindowHandle();
        if (m_windowHandle) {
            m_windowHandleReady = true;
            qDebug() << "Window handle is ready";
            emit readyChanged();

            // Initialize the Vulkan renderer with the window handle
            initializeExternalRenderer();

            // Disconnect so initialization is only called once
            if (window()) {
                disconnect(window(), &QQuickWindow::afterRendering, this, &VulkanRenderItem::handleAfterRendering);
            }
        }
    }
}

void VulkanRenderItem::render() {
    // Only render if the renderer is initialized
    if (m_windowHandleReady && m_rendererInitialized) {
        try {
            // Draw a frame using the Vulkan renderer
            core.drawFrame();

            // Request another frame to keep the rendering loop going
            if (window()) {
                window()->update();
            }
        } catch (const std::exception& e) {
            qWarning() << "Error during rendering:" << e.what();
        }
    }
}

void VulkanRenderItem::cleanup() {
    if (m_rendererInitialized) {
        try {
            core.destroy();
        } catch (const std::exception& e) {
            qWarning() << "Error during cleanup:" << e.what();
        }
    }
    m_windowHandle = nullptr;
    m_windowHandleReady = false;
    m_rendererInitialized = false;
}

void VulkanRenderItem::releaseResources() {
    // This is called when the item is about to be destroyed or its window is changing
    cleanup();
}

void* VulkanRenderItem::getNativeWindowHandle() {
    if (!window()) {
        return nullptr;
    }

#ifdef Q_OS_WIN
    // Get the native Windows HWND
    HWND hwnd = (HWND)window()->winId();
    return hwnd;
#elif defined(Q_OS_MACOS)
    // For macOS, we need to get the NSView and set up a CAMetalLayer
    NSView* nsView = (NSView*)window()->winId();
    if (!nsView) {
        return nullptr;
    }

    // Make sure the view is layer-backed
    [nsView setWantsLayer:YES];

    // Create a CAMetalLayer if it doesn't exist
    if (![nsView.layer isKindOfClass:[CAMetalLayer class]]) {
        CAMetalLayer* metalLayer = [CAMetalLayer layer];
        [nsView setLayer:metalLayer];
    }

    // Return the CAMetalLayer for Vulkan/MoltenVK
    return (__bridge void*)nsView.layer;
#else
    // For other platforms
    return nullptr;
#endif
}

void* VulkanRenderItem::getWindowHandle() {
    return m_windowHandle;
}

void VulkanRenderItem::initializeExternalRenderer() {
    qDebug() << "Initializing external Vulkan renderer";
    if (!m_windowHandleReady || !m_windowHandle) {
        qWarning() << "Window handle not yet available";
        return;
    }

    try {
        int width = window()->width() * window()->devicePixelRatio();
        int height = window()->height() * window()->devicePixelRatio();
        core.setWindowDimensions(width, height);

        core.init(m_windowHandle);
        m_rendererInitialized = true;

        if (window()) {
            window()->update();
        }

        qDebug() << "Vulkan renderer initialized successfully";
    } catch (const std::exception& e) {
        qWarning() << "Failed to initialize Vulkan renderer:" << e.what();
    }
}
