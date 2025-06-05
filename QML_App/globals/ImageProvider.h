#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QPixmap>

class ImageProvider : public QQuickImageProvider {
public:
    ImageProvider();

    void setPixmap(const QPixmap &pixmap);
    void setPreviewPixmap(const QPixmap &pixmap);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    QPixmap m_pixmap;
    QPixmap m_preview;
};

#endif // IMAGEPROVIDER_H
