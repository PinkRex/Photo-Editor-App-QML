#include "AppState.h"

AppState* AppState::instance() {
    static AppState instance;
    return &instance;
}

AppState::AppState(QObject *parent) : QObject(parent) {}

QString AppState::currentPath() const {
    return m_currentPath;
}

void AppState::setCurrentPath(const QString &path) {
    if (m_currentPath != path) {
        m_currentPath = path;
        emit currentPathChanged();
    }
}

QPixmap AppState::currentImage() const {
    return m_currentImage;
}

void AppState::setCurrentImage(const QPixmap &pixmap) {
    m_currentImage = pixmap;

    if (m_imageProvider) m_imageProvider->setPixmap(pixmap);
    m_version++;
    emit imageChanged();
}

QPixmap AppState::pluginImage() const {
    return m_imageFromPlugin;
}

void AppState::setPluginImage(const QPixmap &pixmap) {
    m_imageFromPlugin = pixmap;
}

void AppState::setImageProvider(ImageProvider *provider) {
    m_imageProvider = provider;
}

// void AppState::setCurrentAngle(double angle) {
//     m_currentAngle = angle;
// }

// double AppState::getCurrentAngle() const {
//     return m_currentAngle;
// }

int AppState::getWidth() const {
    QPixmap pixmap = currentImage();
    return pixmap.width();
}

int AppState::getHeight() const {
    QPixmap pixmap = currentImage();
    return pixmap.height();
}
