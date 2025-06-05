#ifndef CONTRASTPLUGIN_H
#define CONTRASTPLUGIN_H

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

class ContrastPlugin: public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_INTERFACE_IID)
    Q_INTERFACES(PluginInterface)
public:
    ContrastPlugin();
    ~ContrastPlugin();

    QString name() override;
    bool hasEditor() override;
    void edit(const cv::Mat &input, cv::Mat &output) override;
    void showEditor(const cv::Mat& input, ResultCallback callback) override;

    void setCurrentImage(cv::Mat img);
    void setOriginalImage(cv::Mat img);

    Q_INVOKABLE void updatePreview(double contrast, double brightness);
    Q_INVOKABLE void onOkClicked(const QUrl &source);
    Q_INVOKABLE void onCancelClicked();
    Q_INVOKABLE void onViewClosed();
    Q_INVOKABLE void setMode(const QString &mode);
    Q_INVOKABLE void setClipLimit(double limit);
    Q_INVOKABLE void setTileGridSize(int size);

private:
    QQuickView* m_view = nullptr;
    cv::Mat m_input;
    cv::Mat m_currentImage;
    cv::Mat m_originalImage;
    ResultCallback m_callback;
    ImageProvider* m_imageProvider = nullptr;

    QString m_mode = "Linear";
    double m_clipLimit = 4.0;
    int m_tileGridSize = 8;

    double m_lastContrast = 1.0;
    double m_lastBrightness = 100.0;
};

#endif // CONTRASTPLUGIN_H
