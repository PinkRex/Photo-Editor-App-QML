#include "ImageProvider.h"

ImageProvider::ImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}

void ImageProvider::setPixmap(const QPixmap &pixmap) {
    m_pixmap = pixmap;
}

void ImageProvider::setPreviewPixmap(const QPixmap &pixmap) {
    m_preview = pixmap;
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {
    QPixmap source;
    QString basedId = id.split('?').constFirst();

    if (basedId == "main") {
        source = m_pixmap;
    } else {
        source = m_preview;
    }

    if (source.isNull()) {
        QPixmap empty(1, 1);
        empty.fill(Qt::transparent);
        if (size)
            *size = empty.size();
        return empty;
    }

    if (size)
        *size = source.size();

    if (!requestedSize.isEmpty())
        return source.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    return source;
}
