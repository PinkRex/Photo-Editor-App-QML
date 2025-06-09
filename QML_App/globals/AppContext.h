#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QQmlApplicationEngine>
#include <QQuickWindow>

class FileController;
class ImageController;
class SnipController;
class StatusController;
class ViewController;
class EditController;
class PluginController;
class AppState;
class ActionLogController;
class HistoryController;
class ImageProvider;
class ImageCropper;

class AppContext
{
public:
    AppContext(FileController *fileCtrl,
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
               ImageCropper *cropper);

    void setup(QQmlApplicationEngine& engine);
    void setWindow(QQuickWindow *window);

private:
    FileController *fileController;
    ImageController *imageController;
    SnipController *snipController;
    StatusController *statusController;
    ViewController *viewController;
    EditController *editController;
    PluginController *pluginController;

    AppState *appState;
    ActionLogController *actionLog;
    HistoryController *historyController;
    ImageProvider *imageProvider;
    ImageCropper *cropper;
};

#endif // APPCONTEXT_H
