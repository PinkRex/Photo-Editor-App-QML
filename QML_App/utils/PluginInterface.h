#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject>
#include <QString>
#include <functional>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

// Plugin Interface nay chi dung de giao tiep ben trong backend

class PluginInterface {
public:
    virtual ~PluginInterface() {}

    // Tra ve ten cua plugin
    virtual QString name() = 0;

    // Xem plugin nay co UI rieng hay khong
    virtual bool hasEditor() = 0;

    // Ham xu ly khi khong co UI rieng
    virtual void edit(const cv::Mat &input, cv::Mat &output) = 0;

    // Tra ve 2 bien: output va bien original
    // bien output de gan cho currentImage => hien thi len giao dien app chinh
    // bien original de xac dinh anh khi gui len plugin va anh hien tai ben main app co giong nhau hay khong
    using ResultCallback = std::function<void(cv::Mat, cv::Mat)>;

    // Ham xu ly khi co UI rieng
    virtual void showEditor(const cv::Mat &input, ResultCallback callback) = 0;
};

#define PLUGIN_INTERFACE_IID "com.nguyenminhduc.plugininterface"
Q_DECLARE_INTERFACE(PluginInterface, PLUGIN_INTERFACE_IID)

#endif // PLUGININTERFACE_H
