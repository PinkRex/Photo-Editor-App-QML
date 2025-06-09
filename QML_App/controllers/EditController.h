#ifndef EDITCONTROLLER_H
#define EDITCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QFileInfo>
#include <QImage>
#include <opencv2/core/core.hpp>
#include "controllers/ImageController.h"
#include "controllers/StatusController.h"
#include "utils/ImageCropper.h"

class EditController : public QObject {
    Q_OBJECT

public:
    explicit EditController(QObject *parent = nullptr);

    Q_INVOKABLE void rotate(double step);
    Q_INVOKABLE void flip(bool isVertical);
    Q_INVOKABLE void resize(int newWidth, int newHeight);
    Q_INVOKABLE void crop(int cropX, int cropY, int cropWidth, int cropHeight, int startX, int startY, int viewWidth, int viewHeight);
    Q_INVOKABLE void contrast(double constrastValue);

    void setImageController(ImageController *controller);
    void setStatusController(StatusController *controller);
    void setImageCropper(ImageCropper *cropper);

private:
    ImageController *m_imageController = nullptr;
    StatusController *m_statusController = nullptr;
    ImageCropper *m_imageCropper = nullptr;
};


#endif // EDITCONTROLLER_H
