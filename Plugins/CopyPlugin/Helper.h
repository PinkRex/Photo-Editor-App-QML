#ifndef HELPER_H
#define HELPER_H

#include <QPixmap>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

class Helper {
public:
    static QPixmap CvMatToQPixmap(cv::Mat matImage);
    static cv::Mat QImageToCvMat(const QImage &image);
    static cv::Mat QPixmapToCvMat(const QPixmap &pixmap);
};

#endif // HELPER_H
