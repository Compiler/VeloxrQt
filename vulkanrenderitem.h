#pragma once

#include <QQuickItem>
#include <QTimer>
#include "renderer.h"

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

    void itemChange(ItemChange change, const ItemChangeData &value) override;

private slots:
    void handleAfterRendering();
    void cleanup();

private:
    void initializeIfNeeded();
    void initializeExternalRenderer();
    void* getNativeWindowHandle();

    RendererCore m_renderer;

    QTimer  m_renderTimer;
    void*   m_windowHandle = nullptr;
    bool    m_windowHandleReady = false;
    bool    m_rendererInitialized = false;
};
