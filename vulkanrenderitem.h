#pragma once

#include <QQuickItem>
#include <QTimer>
#include "renderer.h" // Include your RendererCore class

// Platform-specific includes
#ifdef Q_OS_WIN
#include <windows.h>
#elif defined(Q_OS_MACOS)
#include <Cocoa/Cocoa.h>
#include <QuartzCore/CAMetalLayer.h>
#endif

class VulkanRenderItem : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)

public:
    VulkanRenderItem(QQuickItem *parent = nullptr);
    ~VulkanRenderItem();

    bool isReady() const { return m_windowHandleReady; }
    void* getWindowHandle();

signals:
    void readyChanged();

private slots:
    void handleWindowChanged(QQuickWindow *win);
    void handleAfterRendering();
    void render();

private:
    void cleanup();
    void releaseResources() override;
    void* getNativeWindowHandle();
    void initializeExternalRenderer();

    void* m_windowHandle = nullptr;
    bool m_windowHandleReady = false;
    bool m_rendererInitialized = false;
    RendererCore core;
};
