#include "SnipController.h"
#include "globals/AppState.h"
#include "controllers/ActionLogController.h"
#include "globals/Helper.h"

SnipController::SnipController(QObject *parent) : QObject(parent) {}

void SnipController::setImageController(ImageController *controller) {
    m_imageController = controller;
}

void SnipController::setStatusController(StatusController *controller) {
    m_statusController = controller;
}

void SnipController::setWindow(QWindow *w) {
    m_mainWindow = w;
}

void SnipController::startSnip() {
    QTimer::singleShot(50, this, [this]() {
        emit showOverlay();
    });
}

void SnipController::finishSnip(int x, int y, int w, int h) {
    qDebug() << "Snip finished at:" << x << y << w << h;

    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) return;

    QPixmap pix = screen->grabWindow(0);
    QPixmap cropped = pix.copy(x, y, w, h);

    m_statusController->setEditedStatusText("Snapped image");
    AppState::instance()->setCurrentImage(cropped);
    AppState::instance()->setEdittingBaseImage(cropped);

    // Log action
    QString path = AppState::instance()->imageUrl();
    ActionLogController::instance()->pushAction(QString("Screenshot saved to RAM: %1").arg(path));
}
