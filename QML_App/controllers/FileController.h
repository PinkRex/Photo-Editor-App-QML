#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QFileInfo>
#include <QImage>
#include "controllers/ImageController.h"
#include "interfaces/ControllerInterface.h"

class FileController : public QObject, public ControllerInterface {
    Q_OBJECT

public:
    explicit FileController(QObject *parent = nullptr);

    Q_INVOKABLE void openImage(const QString &filePath, bool isReset = false);
    Q_INVOKABLE void saveImageAs(const QString &filePath);

    void setImageController(ImageController *controller);

signals:
    void showSaveSuccessDialog(const QString &path);

private:
    ImageController *m_imageController = nullptr;
};

#endif // FILECONTROLLER_H
