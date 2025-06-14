#ifndef STATUSCONTROLLER_H
#define STATUSCONTROLLER_H

#include <QString>
#include <QObject>
#include <QFileInfo>
#include <QPixmap>
#include "interfaces/ControllerInterface.h"

class StatusController : public QObject, public ControllerInterface {
    Q_OBJECT
    Q_PROPERTY(QString statusText READ statusText  NOTIFY statusTextChanged)

public:
    explicit StatusController(QObject *parent = nullptr);

    QString statusText() const;
    Q_INVOKABLE int getWidth() const;
    Q_INVOKABLE int getHeight() const;
    void setStatusText(const QString &path);
    Q_INVOKABLE void setEditedStatusText(const QString &text);
    Q_INVOKABLE void setGuideText(const QString &text);

signals:
    void statusTextChanged();

private:
    QString m_statusText;
};

#endif // STATUSCONTROLLER_H
