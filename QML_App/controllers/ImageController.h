#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H

#include <QObject>
#include <QString>
#include "controllers/StatusController.h"
#include "interfaces/ControllerInterface.h"

class ImageController : public QObject, public ControllerInterface {
    Q_OBJECT

public:
    explicit ImageController(QObject *parent = nullptr);

    QString imagePath() const;
    void setStatusController(StatusController *controller);
    void setImagePath(const QString &path);

private:
    QString m_imagePath;
    StatusController *m_statusController = nullptr;
};

#endif // IMAGECONTROLLER_H
