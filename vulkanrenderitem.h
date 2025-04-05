// vulkanrenderitem.h
#ifndef VULKANRENDERITEM_H
#define VULKANRENDERITEM_H

#include <QQuickItem>
#include <QQuickWindow>
#include <renderer.h>

class VulkanRenderItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)
private:
    RendererCore core;

public:
    VulkanRenderItem(QQuickItem *parent = nullptr);
    ~VulkanRenderItem();

    bool isReady() const { return m_windowHandleReady; }

public slots:
    void initializeExternalRenderer();
    void cleanup();
    void* getWindowHandle();

signals:
    void readyChanged();

private slots:
    void handleWindowChanged(QQuickWindow *win);
    void handleAfterRendering();
    void render();

private:
    void releaseResources() override;
    void* getNativeWindowHandle();

    bool m_windowHandleReady = false;
    void* m_windowHandle = nullptr;
};

#endif // VULKANRENDERITEM_H
