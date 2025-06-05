#ifndef COPYPLUGIN_H
#define COPYPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QPixmap>
#include <QObject>
#include <opencv2/opencv.hpp>
#include "PluginInterface.h"

class ImageProvider;

class CopyPlugin: public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_INTERFACE_IID)
    Q_INTERFACES(PluginInterface)
public:
    CopyPlugin();
    QString name() override;
    bool hasEditor() override;
    void edit(const cv::Mat &input, cv::Mat &output) override;
    void showEditor(const cv::Mat& input, ResultCallback callback) override;

    void setCurrentImage(cv::Mat img);
    void setOriginalImage(cv::Mat img);

private:
    cv::Mat m_input;};

#endif // COPYPLUGIN_H
