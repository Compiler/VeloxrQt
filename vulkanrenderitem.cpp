#include "vulkanrenderitem.h"
#include <QQuickWindow>
#include <QTimer>
#include <QDebug>

#ifdef Q_OS_WIN
#  include <windows.h>
#endif

VulkanRenderItem::VulkanRenderItem(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents, true);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        if (window()) {
            window()->update();
        }
    });
    timer->start(16);
}

VulkanRenderItem::~VulkanRenderItem()
{
    cleanup();
}

void VulkanRenderItem::handleWindowChanged(QQuickWindow *win)
{
    if (!win) {
        return;
    }

    //win->setSceneGraphAutoClearEnabled(false);

    connect(win, &QQuickWindow::afterRendering,
            this, &VulkanRenderItem::handleAfterRendering,
            Qt::DirectConnection);
}

void VulkanRenderItem::handleAfterRendering()
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

    if (m_windowHandleReady && m_rendererInitialized) {
        try {
            m_renderer.drawFrame();  // your custom Vulkan draw
        }
        catch (const std::exception& e) {
            qWarning() << "[VulkanRenderItem] Vulkan rendering error:" << e.what();
        }
    }
}

void* VulkanRenderItem::getNativeWindowHandle()
{
    if (!window()) {
        return nullptr;
    }

#ifdef Q_OS_WIN
    HWND hwnd = reinterpret_cast<HWND>(window()->winId());
    if (!hwnd) {
        qWarning() << "[VulkanRenderItem] Failed to get HWND from QQuickWindow::winId()";
        return nullptr;
    }
    return hwnd;

#elif defined(Q_OS_MACOS)
    QWindow* w = window();
    if (!w) return nullptr;

    NSView* nsView = (NSView*) w->winId();
    if (!nsView) {
        qWarning() << "[VulkanRenderItem] NSView is null!";
        return nullptr;
    }

    [nsView setWantsLayer:YES];
    if (![nsView.layer isKindOfClass:[CAMetalLayer class]]) {
        CAMetalLayer* metalLayer = [CAMetalLayer layer];
        [nsView setLayer:metalLayer];
    }
    return (__bridge void*)nsView.layer;

#else
    return nullptr;
#endif
}

void VulkanRenderItem::initializeExternalRenderer()
{
    if (!m_windowHandleReady || !m_windowHandle) {
        qWarning() << "[VulkanRenderItem] Window handle not yet available!";
        return;
    }

    try {
        int width = int(window()->width() * window()->devicePixelRatio());
        int height = int(window()->height() * window()->devicePixelRatio());
        m_renderer.setWindowDimensions(width, height);
        m_renderer.init(m_windowHandle);  // your Vulkan init
        m_rendererInitialized = true;

        qDebug() << "[VulkanRenderItem] Vulkan renderer initialized successfully.";
    }
    catch (const std::exception &ex) {
        qWarning() << "[VulkanRenderItem] Failed to init Vulkan:" << ex.what();
    }
}

void VulkanRenderItem::cleanup()
{
    if (m_rendererInitialized) {
        try {
            m_renderer.destroy();
        }
        catch (const std::exception& ex) {
            qWarning() << "[VulkanRenderItem] Error destroying Vulkan renderer:" << ex.what();
        }
    }
    m_windowHandle = nullptr;
    m_windowHandleReady = false;
    m_rendererInitialized = false;
}

void VulkanRenderItem::releaseResources()
{
    cleanup();
}

QSGNode* VulkanRenderItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
    return oldNode;
}

void* VulkanRenderItem::getWindowHandle()
{
    return m_windowHandle;
}
