#include "AppContext.h"
#include "controllers/FileController.h"
#include "controllers/ImageController.h"
#include "controllers/SnipController.h"
#include "controllers/StatusController.h"
#include "controllers/ViewController.h"
#include "controllers/EditController.h"
#include "controllers/PluginController.h"
#include "controllers/HistoryController.h"
#include "controllers/ActionLogController.h"
#include "AppState.h"
#include "ImageProvider.h"
#include "utils/ImageCropper.h"
#include <QQmlContext>
#include <QQuickWindow>

AppContext::AppContext(FileController *fileCtrl,
                       ImageController *imageCtrl,
                       SnipController *snipCtrl,
                       StatusController *statusCtrl,
                       ViewController *viewCtrl,
                       EditController *editCtrl,
                       PluginController *pluginCtrl,
                       AppState *appState,
                       ActionLogController *actionLog,
                       HistoryController *historyCtrl,
                       ImageProvider *imageProvider,
                       ImageCropper *cropper)
    : fileController(fileCtrl)
    , imageController(imageCtrl)
    , snipController(snipCtrl)
    , statusController(statusCtrl)
    , viewController(viewCtrl)
    , editController(editCtrl)
    , pluginController(pluginCtrl)
    , appState(appState)
    , actionLog(actionLog)
    , historyController(historyCtrl)
    , imageProvider(imageProvider)
    , cropper(cropper)
{
}

void AppContext::setup(QQmlApplicationEngine& engine)
{
    // Load plugins before setting properties
    pluginController->autoLoadPlugins();

    // Setup AppState
    appState->setImageProvider(imageProvider);

    // Setup controllers dependencies
    editController->setImageCropper(cropper);
    imageController->setStatusController(statusController);
    editController->setStatusController(statusController);
    snipController->setStatusController(statusController);
    viewController->setImageController(imageController);
    snipController->setImageController(imageController);
    fileController->setImageController(imageController);

    // Add image provider
    engine.addImageProvider("app", imageProvider);

    // Expose controllers and app state to QML
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("appState", appState);
    context->setContextProperty("actionLog", actionLog);
    context->setContextProperty("historyController", historyController);
    context->setContextProperty("editController", editController);
    context->setContextProperty("fileController", fileController);
    context->setContextProperty("imageController", imageController);
    context->setContextProperty("snipController", snipController);
    context->setContextProperty("statusController", statusController);
    context->setContextProperty("viewController", viewController);
    context->setContextProperty("pluginController", pluginController);
}

void AppContext::setWindow(QQuickWindow *window)
{
    snipController->setWindow(window);
}
