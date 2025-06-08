#include "ImageProvider.h"

ImageProvider::ImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}

void ImageProvider::setPixmap(const QPixmap &pixmap) {
    m_pixmap = pixmap;
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {
    if (m_pixmap.isNull()) {
        QPixmap empty(1, 1);
        empty.fill(Qt::transparent);
        if (size)
            *size = empty.size();
        return empty;
    }

    if (size)
        *size = m_pixmap.size();

    if (!requestedSize.isEmpty())
        return m_pixmap.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    return m_pixmap;
}
