#include "ImageCropper.h"
#include "globals/Helper.h"
#include "globals/AppState.h"

ImageCropper::ImageCropper(QObject* parent) : QObject(parent) {}

QPixmap ImageCropper::crop(int cropX, int cropY, int cropWidth, int cropHeight, int startX, int startY, int viewWidth, int viewHeight) {
    cv::Mat preCropImage = Helper::QPixmapToCvMat(AppState::instance()->currentImage());
    int preWidth = preCropImage.cols;
    int preHeight = preCropImage.rows;

    double widthRatio = static_cast<double>(preWidth) / viewWidth;
    double heightRatio = static_cast<double>(preHeight) / viewHeight;

    int ratioX = static_cast<int>((cropX - startX) * widthRatio);
    int ratioY = static_cast<int>((cropY - startY) * heightRatio);
    int ratioWidth = static_cast<int>(cropWidth * widthRatio);
    int ratioHeight = static_cast<int>(cropHeight * heightRatio);

    cv::Rect croppedArea(ratioX, ratioY, ratioWidth, ratioHeight);
    cv::Mat croppedImage = preCropImage(croppedArea).clone();

    return Helper::CvMatToQPixmap(croppedImage);
}
