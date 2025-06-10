#ifndef SNIPCONTROLLER_H
#define SNIPCONTROLLER_H

#include <QObject>
#include <QGuiApplication>
#include <QScreen>
#include <QTimer>
#include <QFileInfo>
#include <QWindow>
#include <QStandardPaths>
#include "controllers/ImageController.h"
#include "controllers/StatusController.h"
#include "interfaces/ControllerInterface.h"

class SnipController : public QObject, public ControllerInterface {
    Q_OBJECT
public:
    explicit SnipController(QObject *parent = nullptr);

    void setImageController(ImageController *controller);
    void setStatusController(StatusController *controller);

    Q_INVOKABLE void setWindow(QWindow *window);
    Q_INVOKABLE void startSnip();
    Q_INVOKABLE void finishSnip(int x, int y, int w, int h);

signals:
    void showOverlay();

private:
    QWindow *m_mainWindow = nullptr;
    ImageController *m_imageController = nullptr;
    StatusController *m_statusController = nullptr;
};


#endif // SNIPCONTROLLER_H
