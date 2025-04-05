#pragma once

#include <QQuickItem>
#include <QTimer>
#include "renderer.h"  // Your RendererCore (Vulkan) header

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
    // Called when QML's scene graph is releasing resources (e.g., window closing)
    void releaseResources() override;

    // Called each time the scene graph wants to update the item
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) override;

    // Called when certain item properties change (including being added/removed from a window)
    void itemChange(ItemChange change, const ItemChangeData &value) override;

private slots:
    void handleAfterRendering();  // Called after QML's scene graph
    void cleanup();               // Destroys Vulkan resources

private:
    void initializeIfNeeded();
    void initializeExternalRenderer();
    void* getNativeWindowHandle();

    RendererCore m_renderer;  // Your Vulkan engine

    QTimer  m_renderTimer;    // For continuous updates
    void*   m_windowHandle = nullptr;
    bool    m_windowHandleReady = false;
    bool    m_rendererInitialized = false;
};
