#include "EditController.h"
#include "globals/AppState.h"
#include "globals/Helper.h"
#include "controllers/HistoryController.h"

EditController::EditController(QObject *parent) : QObject(parent) {}

void EditController::setImageController(ImageController *controller) {
    m_imageController = controller;
}

void EditController::setStatusController(StatusController *controller) {
    m_statusController = controller;
}

void EditController::setImageCropper(ImageCropper *cropper) {
    m_imageCropper = cropper;
}

static double calculateNextAngle(double current, double step = 90.0) {
    current += step;
    if (current >= 360.0) {
        current -= 360.0;
    }
    return current;
}

static cv::Mat rotatedImage(const cv::Mat &src, double angle) {
    double scale = 1.0;
    cv::Point2f center = cv::Point(src.cols / 2.0, src.rows / 2.0);
    cv::Mat rotateMatrix = cv::getRotationMatrix2D(center, angle, scale);

    cv::Rect2f boundingBox = cv::RotatedRect(cv::Point2f(), src.size(), angle).boundingRect2f();
    rotateMatrix.at<double>(0, 2) += boundingBox.width / 2.0 - src.cols / 2.0;
    rotateMatrix.at<double>(1, 2) += boundingBox.height / 2.0 - src.rows / 2.0;

    cv::Mat rotated;
    cv::warpAffine(src, rotated, rotateMatrix, boundingBox.size(), cv::INTER_CUBIC, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
    return rotated;
}

void EditController::rotate(double step) {
    auto currentAngle = AppState::instance()->getCurrentAngle();
    currentAngle = calculateNextAngle(currentAngle, step);
    AppState::instance()->setCurrentAngle(currentAngle);

    cv::Mat currentImage = Helper::QPixmapToCvMat(AppState::instance()->currentImage());

    HistoryController::instance()->push(currentImage);

    cv::Mat rotated = rotatedImage(currentImage, step);

    AppState::instance()->setCurrentImage(Helper::CvMatToQPixmap(rotated));
    m_statusController->setEditedStatusText("(eddited image)");
}

void EditController::resize(int newWidth, int newHeight) {
    cv::Mat currentImage = Helper::QPixmapToCvMat(AppState::instance()->currentImage());

    HistoryController::instance()->push(currentImage);

    cv::Mat resized;
    cv::resize(currentImage, resized, cv::Size(newWidth, newHeight));
    AppState::instance()->setCurrentImage(Helper::CvMatToQPixmap(resized));
    m_statusController->setEditedStatusText("(eddited image)");
}

void EditController::crop(int cropX, int cropY, int cropWidth, int cropHeight, int startX, int startY, int viewWidth, int viewHeight) {
    if (!m_imageCropper) return;
    cv::Mat currentImage = Helper::QPixmapToCvMat(AppState::instance()->currentImage());
    HistoryController::instance()->push(currentImage);

    QPixmap cropped = m_imageCropper->crop(cropX, cropY, cropWidth, cropHeight, startX, startY, viewWidth, viewHeight);
    if (!cropped.isNull()) {
        AppState::instance()->setCurrentImage(cropped);
        m_statusController->setEditedStatusText("(eddited image)");
    }
}
