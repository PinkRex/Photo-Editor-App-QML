#ifndef IMAGECROPPER_H
#define IMAGECROPPER_H

#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QRect>

class ImageCropper : public QObject {
    Q_OBJECT
public:
    explicit ImageCropper(QObject* parent = nullptr);
    QPixmap crop(int cropX, int cropY, int cropWidth, int cropHeight, int startX, int startY, int viewWidth, int viewHeight);
};

#endif // IMAGECROPPER_H
