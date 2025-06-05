#include "ContrastPlugin.h"
#include "ImageProvider.h"
#include "Helper.h"
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QImage>
#include <QBuffer>
#include <QByteArray>
#include <opencv2/imgproc.hpp>

ContrastPlugin::ContrastPlugin() : m_view(nullptr) {}

ContrastPlugin::~ContrastPlugin()
{
    if (m_view) {
        m_view->close();
        delete m_view;
    }
}

QString ContrastPlugin::name()
{
    return "Contrast";
}

bool ContrastPlugin::hasEditor() { return true; }

void ContrastPlugin::edit(const cv::Mat &input, cv::Mat &output) {}

void ContrastPlugin::setCurrentImage(cv::Mat img) {
    m_currentImage = img;
}

void ContrastPlugin::setOriginalImage(cv::Mat img) {
    m_originalImage = img;
}

void ContrastPlugin::setMode(const QString &mode) {
    m_mode = mode;
}

void ContrastPlugin::setClipLimit(double limit)
{
    m_clipLimit = limit;
}

void ContrastPlugin::setTileGridSize(int size)
{
    m_tileGridSize = size;
}

void ContrastPlugin::onOkClicked(const QUrl &source)
{
    if(m_callback)
        m_callback(m_currentImage, m_originalImage);

    if(m_view){
        m_view->close();
    }
}

void ContrastPlugin::onCancelClicked()
{
    if(m_callback)
        m_callback(cv::Mat(), cv::Mat());

    if(m_view){
        m_view->close();
    }
}

void ContrastPlugin::onViewClosed()
{
    if (m_view) {
        m_view->deleteLater();
        m_view = nullptr;
    }
    m_imageProvider = nullptr;
}

void ContrastPlugin::showEditor(const cv::Mat& input, ResultCallback callback)
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

    m_view->setSource(QUrl(QStringLiteral("qrc:/PluginDialog.qml")));
    m_view->setTitle("Contrast Adjust");

    QObject::connect(m_view, &QQuickWindow::closing, this, [this]() {
        qDebug() << "Window is closing";
        this->onViewClosed();
    });

    m_view->show();
}

void ContrastPlugin::updatePreview(double contrast, double brightness)
{
    m_lastContrast = contrast / 100.0;
    m_lastBrightness = brightness;

    cv::Mat preview;

    if (m_mode == "CLAHE") {
        cv::Mat lab;
        cv::cvtColor(m_input, lab, cv::COLOR_BGR2Lab);

        std::vector<cv::Mat> channels;
        cv::split(lab, channels);

        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(m_clipLimit, cv::Size(m_tileGridSize, m_tileGridSize));
        clahe->apply(channels[0], channels[0]);

        cv::merge(channels, lab);
        cv::cvtColor(lab, preview, cv::COLOR_Lab2BGR);
    } else {
        double alpha = contrast / 100;
        double beta = brightness - 100.0;
        m_input.convertTo(preview, -1, alpha, beta);
    }

    QImage img = Helper::CvMatToQPixmap(preview).toImage();

    if (m_imageProvider) {
        m_imageProvider->setImage(img);
        setCurrentImage(preview);
    }

    if (m_view) {
        QObject* root = m_view->rootObject();
        if (root)
            QMetaObject::invokeMethod(root, "refreshPreview");
    }
}
