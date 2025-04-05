#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <vulkanrenderitem.h>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QGuiApplication app(argc, argv);

    qmlRegisterType<VulkanRenderItem>("VulkanRenderItem", 1, 0, "VulkanRenderItem");
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("TestRenderer", "Main");

    return app.exec();
}
