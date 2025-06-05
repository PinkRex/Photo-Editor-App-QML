#include "StatusController.h"
#include "globals/Helper.h"
#include "globals/AppState.h"

StatusController::StatusController(QObject *parent) : QObject(parent), m_statusText("The image information will be shown here!") {}

int StatusController::getWidth() const {
    QPixmap pixmap = AppState::instance()->currentImage();
    return pixmap.width();
}

int StatusController::getHeight() const {
    QPixmap pixmap = AppState::instance()->currentImage();
    return pixmap.height();
}

QString StatusController::statusText() const {
    return m_statusText;
}

void StatusController::setStatusText(const QString &path) {
    cv::Mat image = cv::imread(path.toUtf8().constData(), cv::IMREAD_COLOR);
    QPixmap pixmap = Helper::CvMatToQPixmap(image);
    QString status = QString("%1, %2x%3, %4 Bytes").arg(path).arg(pixmap.width()).arg(pixmap.height()).arg(QFile(path).size());

    if (m_statusText != status) {
        m_statusText = status;
        emit statusTextChanged();
    }
}

void StatusController::setEditedStatusText(const QString &text) {
    QPixmap pixmap = AppState::instance()->currentImage();
    QString status = QString("%1, %2x%3").arg(text).arg(pixmap.width()).arg(pixmap.height());
    m_statusText = status;
    emit statusTextChanged();
}

void StatusController::setGuideText(const QString &text) {
    m_statusText = text;
    emit statusTextChanged();
}
