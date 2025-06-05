#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QString>
#include <QObject>
#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>
#include <QStringList>
#include "controllers/ImageController.h"
#include "controllers/StatusController.h"

class ViewController : public QObject {
    Q_OBJECT

public:
    explicit ViewController(QObject *parent = nullptr);
    void setImageController(ImageController *controller);
    Q_INVOKABLE void previousImage();
    Q_INVOKABLE void nextImage();

signals:
    void showLastImageDialog();
    void showFirstImageDialog();

private:
    ImageController *m_imageController = nullptr;
    StatusController *m_statusController = nullptr;
};

#endif // VIEWCONTROLLER_H
