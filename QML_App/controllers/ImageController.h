#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H

#include <QObject>
#include <QString>
#include "controllers/StatusController.h"

class ImageController : public QObject {
    Q_OBJECT

public:
    explicit ImageController(QObject *parent = nullptr);

    QString imagePath() const;
    void setStatusController(StatusController *controller);

public slots:
    void setImagePath(const QString &path);

private:
    QString m_imagePath;
    StatusController *m_statusController = nullptr;
};


#endif // IMAGECONTROLLER_H
