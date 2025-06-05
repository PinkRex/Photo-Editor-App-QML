#include "CopyPlugin.h"
#include "Helper.h"
#include <QImage>
#include <QClipboard>
#include <QGuiApplication>
#include <opencv2/imgproc.hpp>

CopyPlugin::CopyPlugin() {}

QString CopyPlugin::name()
{
    return "Copy To Clipboard";
}

bool CopyPlugin::hasEditor() { return false; }

void CopyPlugin::edit(const cv::Mat &input, cv::Mat &output) {
    QClipboard* clipboard = QGuiApplication::clipboard();
    if (clipboard) {
        clipboard->setImage(Helper::CvMatToQPixmap(input).toImage());
    }
    output = input;
}

void CopyPlugin::showEditor(const cv::Mat& input, ResultCallback callback) {}
