#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSharedMemory>
#include <QMessageBox>

#include "atol.h"
#include "core/tray.h"
#include "core/reciepts.h"
#include "core/types/configs.h"

int main(int argc, char *argv[])
{
    bool checked = false;

    Configs configs;
    QObject *_this = new QObject;

    QApplication app(argc, argv);
    QSharedMemory *shared = new QSharedMemory("Atol-Server");

    if ( shared->create( 512, QSharedMemory::ReadWrite ) == false )
    {
        qDebug() << "Multiple instances are not allowed!";

        QMessageBox errorMessage;

        errorMessage.addButton(QMessageBox::Ok);
        errorMessage.setWindowTitle(QObject::tr("Ошибка"));

        errorMessage.setText(QObject::tr("Приложение уже запущено"));
        errorMessage.exec();

        exit(0);
    }

    QObject::connect(&configs, &Configs::configsLoaded, _this, [&] (Configuration c)
    {
        if ( checked )
        {
            return;
        }

        qDebug() << "Configs initialized succsessefull";

        QDir::setCurrent(c.installPath);

        QString appPath = app.applicationFilePath();
        appPath.replace('/', '\\');
        c.appExecutable = appPath;

        configs.switchAutostart(c.toJson());
        configs.Update(c.toJson());
        checked = true;
    });

    Atol atol;
    atol.ChangeConfiguration(&configs);

    QQmlApplicationEngine engine;

    Tray *tray = new Tray();
    QQmlContext *context = engine.rootContext();

    QObject::connect(tray, &Tray::closed, _this, [&] ()
    {
        app.quit();
        engine.quit();
        exit(0);
    });

    context->setContextProperty("TRAY", tray);
    context->setContextProperty("CONFIGS", &configs);
    context->setContextProperty("MEWBAS", &atol.api);
    context->setContextProperty("SERVER", &atol.core);
    context->setContextProperty("RECIEPTS", &atol.reciepts);
    qmlRegisterSingletonType(QUrl("qrc:/qml/Presets.qml"), "Presets", 1, 0, "Presets");

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
