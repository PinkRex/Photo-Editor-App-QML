#include "Helper.h"

QPixmap Helper::CvMatToQPixmap(cv::Mat matImage) {
    QImage image_edited;

    if (matImage.channels() == 4) {
        cv::Mat converted;
        cv::cvtColor(matImage, converted, cv::COLOR_BGRA2RGBA);
        image_edited = QImage(converted.data, converted.cols, converted.rows, converted.step, QImage::Format_RGBA8888).copy();
    } else if (matImage.channels() == 3) {
        cv::Mat converted;
        cv::cvtColor(matImage, converted, cv::COLOR_BGR2RGB);
        image_edited = QImage(converted.data, converted.cols, converted.rows, converted.step, QImage::Format_RGB888).copy();
    } else if (matImage.channels() == 1) {
        image_edited = QImage(matImage.data, matImage.cols, matImage.rows, matImage.step, QImage::Format_Grayscale8).copy();
    } else {
        image_edited = QImage();
    }

    return QPixmap::fromImage(image_edited);
}

cv::Mat Helper::QImageToCvMat(const QImage &image) {
    switch (image.format()) {
    case QImage::Format_RGB888: {
        QImage swapped = image.rgbSwapped();
        return cv::Mat(swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine()).clone();
    }
    case QImage::Format_RGBA8888: {
        return cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
    }
    case QImage::Format_Grayscale8: {
        return cv::Mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
    }
    default:
        return cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
    }
}

cv::Mat Helper::QPixmapToCvMat(const QPixmap &pixmap) {
    return Helper::QImageToCvMat(pixmap.toImage());
}
