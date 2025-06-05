#include "SharpenPlugin.h"
#include "ImageProvider.h"
#include "Helper.h"
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QImage>
#include <QBuffer>
#include <QByteArray>
#include <opencv2/imgproc.hpp>

SharpenPlugin::SharpenPlugin() : m_view(nullptr) {}

SharpenPlugin::~SharpenPlugin()
{
    if (m_view) {
        m_view->close();
        delete m_view;
    }
}

QString SharpenPlugin::name()
{
    return "Sharpen";
}

bool SharpenPlugin::hasEditor() { return true; }

void SharpenPlugin::edit(const cv::Mat &input, cv::Mat &output) {}

void SharpenPlugin::setCurrentImage(cv::Mat img) {
    m_currentImage = img;
}

void SharpenPlugin::setOriginalImage(cv::Mat img) {
    m_originalImage = img;
}

void SharpenPlugin::onOkClicked(const QUrl &source)
{
    if(m_callback)
        m_callback(m_currentImage, m_originalImage);

    if(m_view){
        m_view->close();
    }
}

void SharpenPlugin::onCancelClicked()
{
    if(m_callback)
        m_callback(cv::Mat(), cv::Mat());

    if(m_view){
        m_view->close();
    }
}

void SharpenPlugin::onViewClosed()
{
    if (m_view) {
        m_view->deleteLater();
        m_view = nullptr;
    }
    m_imageProvider = nullptr;
}

void SharpenPlugin::onResetClicked() {
    if (m_imageProvider) {
        m_imageProvider->setImage(Helper::CvMatToQPixmap(m_originalImage).toImage());
        setCurrentImage(m_originalImage);
    }

    if (m_view) {
        QObject* root = m_view->rootObject();
        if (root) QMetaObject::invokeMethod(root, "refreshPreview");
    }
}

void SharpenPlugin::showEditor(const cv::Mat& input, ResultCallback callback)
{
    m_callback = callback;
    m_input = input.clone();
    setOriginalImage(m_input);
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

    m_view->setSource(QUrl(QStringLiteral("qrc:/SharpenDialog.qml")));
    m_view->setTitle("Sharpen Adjust");

    QObject::connect(m_view, &QQuickWindow::closing, this, [this]() {
        qDebug() << "Window is closing";
        this->onViewClosed();
    });

    m_view->show();
}

void SharpenPlugin::updatePreview(int radiusValue, double amountValue, int thresholdValue)
{
    cv::Mat blurred, lowContrastMask, sharpened, absDiff;

    cv::GaussianBlur(m_input, blurred, cv::Size(radiusValue * 2 + 1, radiusValue * 2 + 1), 0);
    cv::absdiff(m_input, blurred, absDiff);
    std::vector<cv::Mat> channels;
    cv::split(absDiff, channels);
    cv::Mat maxDiff = channels[0];
    for (size_t i = 1; i < channels.size(); ++i) {
        cv::max(maxDiff, channels[i], maxDiff);
    }
    cv::threshold(maxDiff, lowContrastMask, thresholdValue, 255, cv::THRESH_BINARY_INV);
    lowContrastMask.convertTo(lowContrastMask, CV_8U);

    cv::addWeighted(m_input, 1 + amountValue, blurred, -amountValue, 0, sharpened);
    m_input.copyTo(sharpened, lowContrastMask);

    QImage img = Helper::CvMatToQPixmap(sharpened).toImage();

    if (m_imageProvider) {
        m_imageProvider->setImage(img);
        setCurrentImage(sharpened);
    }

    if (m_view) {
        QObject* root = m_view->rootObject();
        if (root) QMetaObject::invokeMethod(root, "refreshPreview");
    }
}
