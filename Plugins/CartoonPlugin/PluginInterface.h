#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject>
#include <QString>
#include <functional>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

class PluginInterface {
public:
    virtual ~PluginInterface() {}
    virtual QString name() = 0;
    virtual bool hasEditor() = 0;
    virtual void edit(const cv::Mat &input, cv::Mat &output) = 0;

    using ResultCallback = std::function<void(cv::Mat, cv::Mat)>;
    virtual void showEditor(const cv::Mat &input, ResultCallback callback) = 0;
};

#define PLUGIN_INTERFACE_IID "com.nguyenminhduc.plugininterface"
Q_DECLARE_INTERFACE(PluginInterface, PLUGIN_INTERFACE_IID)

#endif // PLUGININTERFACE_H
