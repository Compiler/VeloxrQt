#include "vulkanrenderitem.h"
#include <QQuickWindow>
#include <QDebug>

#ifdef Q_OS_WIN
#  include <windows.h>
#endif

VulkanRenderItem::VulkanRenderItem(QQuickItem *parent)
    : QQuickItem(parent)
{
    qDebug() << "VulkanRenderItem()" << this;
    setFlag(QQuickItem::ItemHasContents, true);
    connect(&m_renderTimer, &QTimer::timeout, this, [this]() {
        if (window()) {
            window()->update();
        }
    });
    // sue me
    m_renderTimer.start(16); // ~60 fps
}

VulkanRenderItem::~VulkanRenderItem()
{
    qDebug() << "VulkanRenderItem dtor" << this;
    cleanup();
}

void VulkanRenderItem::itemChange(ItemChange change, const ItemChangeData &value)
{
    QQuickItem::itemChange(change, value);

    if (change == QQuickItem::ItemSceneChange) {
        if (value.window) {
            qDebug() << "VulkanRenderItem::itemChange => now in window" << value.window;

            connect(value.window, &QQuickWindow::afterRendering,
                    this, &VulkanRenderItem::handleAfterRendering,
                    Qt::DirectConnection);

            // uhhh this made it work
            value.window->setColor(Qt::transparent);

            initializeIfNeeded();

        } else {
            qDebug() << "VulkanRenderItem::itemChange => removed from window";
            cleanup();
        }
    }
}

void VulkanRenderItem::initializeIfNeeded()
{
    if (!m_windowHandleReady) {
        m_windowHandle = getNativeWindowHandle();
        if (m_windowHandle) {
            m_windowHandleReady = true;
            qDebug() << "[VulkanRenderItem] Got native window handle:" << m_windowHandle;
            emit readyChanged();
            initializeExternalRenderer();
        }
    }
}

void VulkanRenderItem::handleAfterRendering() {
    if (!m_windowHandleReady) {
        initializeIfNeeded();
    }

    if (m_windowHandleReady && m_rendererInitialized) {
        try {
            m_renderer.drawFrame();
            // qDebug() << "[VulkanRenderItem] handleAfterRendering -> drawFrame done.";
        }
        catch (const std::exception &ex) {
            qWarning() << "[VulkanRenderItem] Vulkan rendering error:" << ex.what();
        }
    }
}

void VulkanRenderItem::initializeExternalRenderer()
{
    if (!m_windowHandle || !m_windowHandleReady) {
        qWarning() << "[VulkanRenderItem] Window handle not available, cannot init!";
        return;
    }

    try {
        int width  = int(window()->width()  * window()->devicePixelRatio());
        int height = int(window()->height() * window()->devicePixelRatio());

        m_renderer.setWindowDimensions(width, height);
        m_renderer.init(m_windowHandle);
        m_rendererInitialized = true;

        qDebug() << "[VulkanRenderItem] Vulkan renderer initialized successfully.";
    }
    catch (const std::exception &ex) {
        qWarning() << "[VulkanRenderItem] Failed to init Vulkan:" << ex.what();
    }
}

void* VulkanRenderItem::getNativeWindowHandle()
{
    if (!window())
        return nullptr;

#ifdef Q_OS_WIN
    HWND hwnd = reinterpret_cast<HWND>(window()->winId());
    if (!hwnd) {
        qWarning() << "[VulkanRenderItem] Failed to get HWND from QQuickWindow::winId()";
        return nullptr;
    }
    return hwnd;

#elif defined(Q_OS_MACOS)
    // On macOS, QQuickWindow::winId() is an NSView* but we need CAMetalLayer.
    QWindow *w = window();
    if (!w) return nullptr;

    NSView *nsView = (NSView*)w->winId();
    if (!nsView) {
        qWarning() << "[VulkanRenderItem] NSView is null!";
        return nullptr;
    }
    [nsView setWantsLayer:YES];
    if (![nsView.layer isKindOfClass:[CAMetalLayer class]]) {
        CAMetalLayer *metalLayer = [CAMetalLayer layer];
        [nsView setLayer:metalLayer];
    }
    return (__bridge void*)nsView.layer;

#else
    // On Linux, it depends on the backend (XCB vs Wayland).
    //   xcb_window_t wid = (xcb_window_t) window()->winId();
    // ... but mixing scene graph & raw vk on Linux is typically not supported.
    return nullptr;
#endif
}

void VulkanRenderItem::cleanup()
{
    if (m_rendererInitialized) {
        try {
            m_renderer.destroy();
        }
        catch (const std::exception &ex) {
            qWarning() << "[VulkanRenderItem] Error destroying Vulkan:" << ex.what();
        }
    }
    m_windowHandle = nullptr;
    m_windowHandleReady = false;
    m_rendererInitialized = false;
}

QSGNode* VulkanRenderItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    return oldNode;
}

void VulkanRenderItem::releaseResources()
{
    qDebug() << "VulkanRenderItem::releaseResources";
    cleanup();
}

void* VulkanRenderItem::getWindowHandle()
{
    return m_windowHandle;
}
