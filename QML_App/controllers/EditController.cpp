#include "EditController.h"
#include "globals/AppState.h"
#include "globals/Helper.h"
#include "controllers/HistoryController.h"
#include "controllers/ActionLogController.h"

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

static cv::Mat flippedImage(const cv::Mat &src, bool isVertical) {
    cv::Mat flipped;
    int flipCode = isVertical ? 0 : 1; // 0 = vertical, 1 = horizontal
    cv::flip(src, flipped, flipCode);
    return flipped;
}

void EditController::rotate(double step) {
    cv::Mat currentImage = Helper::QPixmapToCvMat(AppState::instance()->currentImage());
    HistoryController::instance()->push(currentImage);
    cv::Mat rotated = rotatedImage(currentImage, step);
    AppState::instance()->setCurrentImage(Helper::CvMatToQPixmap(rotated));
    m_statusController->setEditedStatusText("(eddited image)");

    // Log action
    if (step < 0) {
        ActionLogController::instance()->pushAction(QString("Rotate right: 90°"));
    } else {
        ActionLogController::instance()->pushAction(QString("Rotate left: 90°"));
    }
}

void EditController::flip(bool isVertical) {
    cv::Mat currentImage = Helper::QPixmapToCvMat(AppState::instance()->currentImage());
    HistoryController::instance()->push(currentImage);
    cv::Mat flipped = flippedImage(currentImage, isVertical);
    AppState::instance()->setCurrentImage(Helper::CvMatToQPixmap(flipped));
    m_statusController->setEditedStatusText("(eddited image)");

    // Log action
    if (isVertical) {
        ActionLogController::instance()->pushAction(QString("Flip vertical"));
    } else {
        ActionLogController::instance()->pushAction(QString("Flip horizontal"));
    }
}

void EditController::resize(int newWidth, int newHeight) {
    cv::Mat currentImage = Helper::QPixmapToCvMat(AppState::instance()->currentImage());

    HistoryController::instance()->push(currentImage);

    cv::Mat resized;
    cv::resize(currentImage, resized, cv::Size(newWidth, newHeight));
    AppState::instance()->setCurrentImage(Helper::CvMatToQPixmap(resized));
    m_statusController->setEditedStatusText("(eddited image)");

    // Log action
    ActionLogController::instance()->pushAction(QString("Resize to: %1x%2").arg(newWidth).arg(newHeight));
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

    // Log action
    QPixmap pixmap = AppState::instance()->currentImage();
    ActionLogController::instance()->pushAction(QString("Crop to: %1x%2 at x: %3, y: %4")
                                                    .arg(pixmap.width()).arg(pixmap.height())
                                                    .arg(startX).arg(startY));
}
