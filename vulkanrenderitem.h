#pragma once

#include <QQuickItem>
#include <QPointer>
#include "renderer.h"  // Your "RendererCore" class header

class QQuickWindow;

class VulkanRenderItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)

public:
    explicit VulkanRenderItem(QQuickItem *parent = nullptr);
    ~VulkanRenderItem();

    bool isReady() const { return m_windowHandleReady; }
    void* getWindowHandle();

signals:
    void readyChanged();

protected:
    void releaseResources() override;
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) override;

private slots:
    void handleWindowChanged(QQuickWindow *win);
    void handleAfterRendering();
    void cleanup();

private:
    void* getNativeWindowHandle();
    void initializeExternalRenderer();

    RendererCore m_renderer;
    void* m_windowHandle = nullptr;
    bool  m_windowHandleReady = false;
    bool  m_rendererInitialized = false;
};
