#include "BlurPlugin.h"
#include "ImageProvider.h"
#include "Helper.h"
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QImage>
#include <QBuffer>
#include <QByteArray>
#include <opencv2/imgproc.hpp>

BlurPlugin::BlurPlugin() : m_view(nullptr) {}

BlurPlugin::~BlurPlugin()
{
    if (m_view) {
        m_view->close();
        delete m_view;
    }
}

QString BlurPlugin::name()
{
    return "Blur";
}

bool BlurPlugin::hasEditor() { return true; }

void BlurPlugin::edit(const cv::Mat &input, cv::Mat &output) {}

void BlurPlugin::setCurrentImage(cv::Mat img) {
    m_currentImage = img;
}

void BlurPlugin::setOriginalImage(cv::Mat img) {
    m_originalImage = img;
}

void BlurPlugin::setMode(const QString &mode) {
    m_mode = mode;
}

void BlurPlugin::onOkClicked(const QUrl &source)
{
    if(m_callback)
        m_callback(m_currentImage, m_originalImage);

    if(m_view){
        m_view->close();
    }
}

void BlurPlugin::onCancelClicked()
{
    if(m_callback)
        m_callback(cv::Mat(), cv::Mat());

    if(m_view){
        m_view->close();
    }
}

void BlurPlugin::onViewClosed()
{
    if (m_view) {
        m_view->deleteLater();
        m_view = nullptr;
    }
    m_imageProvider = nullptr;
}

void BlurPlugin::onResetClicked() {
    if (m_imageProvider) {
        m_imageProvider->setImage(Helper::CvMatToQPixmap(m_originalImage).toImage());
        setCurrentImage(m_originalImage);
    }

    if (m_view) {
        QObject* root = m_view->rootObject();
        if (root) QMetaObject::invokeMethod(root, "refreshPreview");
    }
}

void BlurPlugin::showEditor(const cv::Mat& input, ResultCallback callback)
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

    m_view->setSource(QUrl(QStringLiteral("qrc:/BlurDialog.qml")));
    m_view->setTitle("Blur Adjust");

    QObject::connect(m_view, &QQuickWindow::closing, this, [this]() {
        qDebug() << "Window is closing";
        this->onViewClosed();
    });

    m_view->show();
}

void BlurPlugin::updatePreview(int blurValue)
{
    cv::Mat preview;

    int ksize = blurValue * 3 + 1;
    if (ksize % 2 == 0) ++ksize;

    if (m_mode == "Gaussian") {
        cv::GaussianBlur(m_originalImage, preview, cv::Size(ksize, ksize), 0);
    } else {
        cv::blur(m_originalImage, preview, cv::Size(ksize, ksize));
    }

    QImage img = Helper::CvMatToQPixmap(preview).toImage();

    if (m_imageProvider) {
        m_imageProvider->setImage(img);
        setCurrentImage(preview);
    }

    if (m_view) {
        QObject* root = m_view->rootObject();
        if (root) QMetaObject::invokeMethod(root, "refreshPreview");
    }
}
