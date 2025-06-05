#include "ColorPlugin.h"
#include "ImageProvider.h"
#include "Helper.h"
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QImage>
#include <QBuffer>
#include <QByteArray>
#include <opencv2/imgproc.hpp>

ColorPlugin::ColorPlugin() : m_view(nullptr) {}

ColorPlugin::~ColorPlugin()
{
    if (m_view) {
        m_view->close();
        delete m_view;
    }
}

QString ColorPlugin::name()
{
    return "Color";
}

bool ColorPlugin::hasEditor() { return true; }

void ColorPlugin::edit(const cv::Mat &input, cv::Mat &output) {}

void ColorPlugin::setCurrentImage(cv::Mat img) {
    m_currentImage = img;
}

void ColorPlugin::setOriginalImage(cv::Mat img) {
    m_originalImage = img;
}

void ColorPlugin::setMode(const QString &mode) {
    m_mode = mode;
}

void ColorPlugin::onOkClicked(const QUrl &source)
{
    if(m_callback)
        m_callback(m_currentImage, m_originalImage);

    if(m_view){
        m_view->close();
    }
}

void ColorPlugin::onCancelClicked()
{
    if(m_callback)
        m_callback(cv::Mat(), cv::Mat());

    if(m_view){
        m_view->close();
    }
}

void ColorPlugin::onViewClosed()
{
    if (m_view) {
        m_view->deleteLater();
        m_view = nullptr;
    }
    m_imageProvider = nullptr;
}

void ColorPlugin::onResetClicked() {
    if (m_imageProvider) {
        m_imageProvider->setImage(Helper::CvMatToQPixmap(m_originalImage).toImage());
        setCurrentImage(m_originalImage);
    }

    if (m_view) {
        QObject* root = m_view->rootObject();
        if (root) QMetaObject::invokeMethod(root, "refreshPreview");
    }
}

void ColorPlugin::showEditor(const cv::Mat& input, ResultCallback callback)
{
    m_callback = callback;
    m_input = input.clone();
    setOriginalImage(m_input);
    cv::Mat inputBGR;
    if (m_input.channels() == 4) {
        cv::cvtColor(m_input, inputBGR, cv::COLOR_BGRA2BGR);
    } else {
        inputBGR = m_input;
    }
    m_input = inputBGR;
    m_view = new QQuickView();
    m_view->setResizeMode(QQuickView::SizeRootObjectToView);
    m_view->rootContext()->setContextProperty("plugin", this);

    QImage img = Helper::CvMatToQPixmap(input).toImage();

    if (!m_imageProvider) {
        m_imageProvider = new ImageProvider();
        m_view->engine()->addImageProvider("pluginprovider", m_imageProvider);
    } else {
        m_imageProvider->setImage(QImage());
    }

    m_imageProvider->setImage(img);
    setCurrentImage(m_input);

    m_view->setSource(QUrl(QStringLiteral("qrc:/ColorDialog.qml")));
    m_view->setTitle("Color Adjust");

    QObject::connect(m_view, &QQuickWindow::closing, this, [this]() {
        qDebug() << "Window is closing";
        this->onViewClosed();
    });

    m_view->show();
}

void ColorPlugin::updatePreview(cv::Mat result)
{
    QImage img = Helper::CvMatToQPixmap(result).toImage();

    if (m_imageProvider) {
        m_imageProvider->setImage(img);
        setCurrentImage(result);
    }

    if (m_view) {
        QObject* root = m_view->rootObject();
        if (root) QMetaObject::invokeMethod(root, "refreshPreview");
    }
}

void ColorPlugin::manualRGBA(int redScale, int greenScale, int blueScale, int alphaScale) {
    cv::Mat result;

    CV_Assert(!m_input.empty());
    CV_Assert(m_input.channels() == 3 || m_input.channels() == 4);

    cv::Mat temp = m_input.clone();
    std::vector<cv::Mat> channels;
    cv::split(temp, channels);

    double r = redScale / 100.0;
    double g = greenScale / 100.0;
    double b = blueScale / 100.0;
    double a = alphaScale / 100.0;

    if (channels.size() >= 3) {
        channels[2] *= r; // R
        channels[1] *= g; // G
        channels[0] *= b; // B
    }

    if (channels.size() == 4) {
        channels[3] *= a; // A
    }

    cv::merge(channels, result);

    updatePreview(result);
}

void ColorPlugin::autoWhiteBalance(int strength, int neutralGrayTarget, bool preserveSaturation) {
    cv::Mat result;

    CV_Assert(!m_input.empty());
    CV_Assert(m_input.channels() == 3 || m_input.channels() == 4);

    cv::Mat bgr;
    if (m_input.channels() == 4) {
        cv::cvtColor(m_input, bgr, cv::COLOR_BGRA2BGR);
    } else {
        bgr = m_input.clone();
    }

    std::vector<cv::Mat> channels;
    cv::split(bgr, channels);
    double avgB = cv::mean(channels[0])[0];
    double avgG = cv::mean(channels[1])[0];
    double avgR = cv::mean(channels[2])[0];

    double target = static_cast<double>(neutralGrayTarget);
    double scaleB = target / avgB;
    double scaleG = target / avgG;
    double scaleR = target / avgR;

    channels[0] *= scaleB;
    channels[1] *= scaleG;
    channels[2] *= scaleR;

    cv::merge(channels, bgr);

    if (preserveSaturation) {
        cv::Mat hsv;
        cv::cvtColor(bgr, hsv, cv::COLOR_BGR2HSV);
        std::vector<cv::Mat> hsvChannels;
        cv::split(hsv, hsvChannels);
        hsvChannels[1] = cv::min(hsvChannels[1], 255);
        cv::merge(hsvChannels, hsv);
        cv::cvtColor(hsv, bgr, cv::COLOR_HSV2BGR);
    }

    double alpha = strength / 100.0;
    if (m_input.channels() == 4) {
        cv::Mat merged;
        std::vector<cv::Mat> inputChannels;
        cv::split(m_input, inputChannels);
        inputChannels[0] = inputChannels[0] * (1.0 - alpha) + channels[0] * alpha;
        inputChannels[1] = inputChannels[1] * (1.0 - alpha) + channels[1] * alpha;
        inputChannels[2] = inputChannels[2] * (1.0 - alpha) + channels[2] * alpha;
        cv::merge(inputChannels, result);
    } else {
        result = m_input * (1.0 - alpha) + bgr * alpha;
    }

    updatePreview(result);
}
