#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();

    void setImage(const QImage& image);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    QImage m_image;
};

#endif // IMAGEPROVIDER_H
