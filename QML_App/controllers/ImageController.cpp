#include "ImageController.h"
#include "globals/AppState.h"
#include "globals/Helper.h"

ImageController::ImageController(QObject *parent) : QObject(parent) {}

void ImageController::setStatusController(StatusController *controller) {
    m_statusController = controller;
}

QString ImageController::imagePath() const {
    return m_imagePath;
}

void ImageController::setImagePath(const QString &path) {
    if (m_imagePath != path) {
        m_imagePath = "file:///" + path;

        cv::Mat image = cv::imread(path.toUtf8().constData(), cv::IMREAD_COLOR);
        QPixmap pixmap = Helper::CvMatToQPixmap(image);

        m_statusController->setStatusText(path);
        AppState::instance()->setCurrentPath(path);
        AppState::instance()->setCurrentImage(pixmap);
        AppState::instance()->setEdittingBaseImage(pixmap);
    }
}
