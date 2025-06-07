#ifndef APPSTATE_H
#define APPSTATE_H

#include <QObject>
#include <QString>
#include <QPixmap>
#include <QVector>
#include <QTransform>
#include "ImageProvider.h"

class AppState : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString currentPath READ currentPath WRITE setCurrentPath NOTIFY currentPathChanged)
    Q_PROPERTY(QString imageUrl READ imageUrl NOTIFY imageChanged)

public:
    static AppState* instance();

    void setCurrentPath(const QString &path);
    void setPluginImage(const QPixmap &pixmap);
    void setImageProvider(ImageProvider *provider);
    // void setCurrentAngle(double angle);
    // double getCurrentAngle() const;
    QString imageUrl() const { return QString("image://app/main?version=%1").arg(m_version); }

    Q_INVOKABLE QString currentPath() const;
    Q_INVOKABLE QPixmap currentImage() const;
    Q_INVOKABLE QPixmap pluginImage() const;
    Q_INVOKABLE QString getDefaultUrl() const { return "image://app/main?version=0"; }
    Q_INVOKABLE int getWidth() const;
    Q_INVOKABLE int getHeight() const;
    Q_INVOKABLE void setCurrentImage(const QPixmap &pixmap);

signals:
    void currentPathChanged();
    void imageChanged();

private:
    explicit AppState(QObject *parent = nullptr);

    QString m_currentPath;
    QPixmap m_currentImage;
    QPixmap m_imageFromPlugin;

    ImageProvider *m_imageProvider = nullptr;

    int m_version = 0;
    // double m_currentAngle = 0.0;
};

#endif // APPSTATE_H
