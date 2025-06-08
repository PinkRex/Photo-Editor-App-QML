#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include "globals/AppState.h"
#include "globals/ImageProvider.h"
#include "controllers/FileController.h"
#include "controllers/ImageController.h"
#include "controllers/SnipController.h"
#include "controllers/StatusController.h"
#include "controllers/ViewController.h"
#include "controllers/EditController.h"
#include "controllers/HistoryController.h"
#include "controllers/PluginController.h"
#include "utils/ImageCropper.h"
#include "globals/AppContext.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/appIcon.png"));
    QQmlApplicationEngine engine;

    FileController fileController;
    ImageController imageController;
    SnipController snipController;
    StatusController statusController;
    ViewController viewController;
    EditController editController;
    PluginController pluginController;
    AppState* appState = AppState::instance();
    HistoryController* historyController = HistoryController::instance();
    ImageProvider* imageProvider = new ImageProvider();
    ImageCropper* cropper = new ImageCropper();

    AppContext appContext(
        &fileController,
        &imageController,
        &snipController,
        &statusController,
        &viewController,
        &editController,
        &pluginController,
        appState,
        historyController,
        imageProvider,
        cropper);

    appContext.setup(engine);

    engine.loadFromModule("QML_App", "Main");

    QObject *rootObject = engine.rootObjects().constFirst();
    QQuickWindow *mainWindow = qobject_cast<QQuickWindow *>(rootObject);
    appContext.setWindow(mainWindow);

    return app.exec();
}
