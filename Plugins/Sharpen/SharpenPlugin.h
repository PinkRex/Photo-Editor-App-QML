#ifndef SHARPENPLUGIN_H
#define SHARPENPLUGIN_H

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

class SharpenPlugin: public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_INTERFACE_IID)
    Q_INTERFACES(PluginInterface)
public:
    SharpenPlugin();
    ~SharpenPlugin();

    QString name() override;
    bool hasEditor() override;
    void edit(const cv::Mat &input, cv::Mat &output) override;
    void showEditor(const cv::Mat& input, ResultCallback callback) override;

    void setCurrentImage(cv::Mat img);
    void setOriginalImage(cv::Mat img);

    Q_INVOKABLE void updatePreview(int radiusValue, double amountValue, int thresholdValue);
    Q_INVOKABLE void onOkClicked(const QUrl &source);
    Q_INVOKABLE void onCancelClicked();
    Q_INVOKABLE void onViewClosed();
    Q_INVOKABLE void onResetClicked();

private:
    QQuickView* m_view = nullptr;
    cv::Mat m_input;
    cv::Mat m_currentImage;
    cv::Mat m_originalImage;
    ResultCallback m_callback;
    ImageProvider* m_imageProvider = nullptr;

    int m_radiusValue = 0;
    double m_amountValue = 0.0;
    int m_thresholdValue = 0;
};

#endif // SHARPENPLUGIN_H
