#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Qrimageprovider.hpp"



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.addImageProvider(QLatin1String("qrcodeblack"), new QRImageProvider("#10141c",1));

    engine.addImportPath("qrc:/esterVtech.com/imports");
    engine.addImportPath("_deps/mydesigns-build/");
    engine.addImportPath("_deps/account-build/");
    engine.addImportPath("_deps/nodeconection-build/");

    const QUrl url(u"qrc:/esterVtech.com/imports/BuyMeACoffe/qml/window.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
