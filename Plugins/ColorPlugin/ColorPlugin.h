#ifndef COLORPLUGIN_H
#define COLORPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QTimer>
#include <QPixmap>
#include <QObject>
#include <QQuickView>
#include <QQuickItem>
#include <opencv2/opencv.hpp>
#include "PluginInterface.h"

class ImageProvider;

class ColorPlugin: public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_INTERFACE_IID)
    Q_INTERFACES(PluginInterface)
public:
    ColorPlugin();
    ~ColorPlugin();

    QString name() override;
    bool hasEditor() override;
    void edit(const cv::Mat &input, cv::Mat &output) override;
    void showEditor(const cv::Mat& input, ResultCallback callback) override;

    void setCurrentImage(cv::Mat img);
    void setOriginalImage(cv::Mat img);

    Q_INVOKABLE void manualRGBA(int redScale, int greenScale, int blueScale, int alphaScale);
    Q_INVOKABLE void autoWhiteBalance(int strength, int neutralGrayTarget , bool preserveSaturation);

    Q_INVOKABLE void updatePreview(cv::Mat result);
    Q_INVOKABLE void onOkClicked(const QUrl &source);
    Q_INVOKABLE void onCancelClicked();
    Q_INVOKABLE void onViewClosed();
    Q_INVOKABLE void setMode(const QString &mode);
    Q_INVOKABLE void onResetClicked();

private:
    QQuickView* m_view = nullptr;
    cv::Mat m_input;
    cv::Mat m_currentImage;
    cv::Mat m_originalImage;
    ResultCallback m_callback;
    ImageProvider* m_imageProvider = nullptr;

    QString m_mode = "Classic";
};

#endif // COLORPLUGIN_H
