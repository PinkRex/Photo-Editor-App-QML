#include "ErodePlugin.h"
#include "ImageProvider.h"
#include "Helper.h"
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QImage>
#include <QBuffer>
#include <QByteArray>
#include <opencv2/imgproc.hpp>

ErodePlugin::ErodePlugin() : m_view(nullptr) {}

ErodePlugin::~ErodePlugin()
{
    if (m_view) {
        m_view->close();
        delete m_view;
    }
}

QString ErodePlugin::name()
{
    return "Erode";
}

bool ErodePlugin::hasEditor() { return true; }

void ErodePlugin::edit(const cv::Mat &input, cv::Mat &output) {}

void ErodePlugin::setCurrentImage(cv::Mat img) {
    m_currentImage = img;
}

void ErodePlugin::setOriginalImage(cv::Mat img) {
    m_originalImage = img;
}

void ErodePlugin::setMode(const QString &mode) {
    m_mode = mode;
}

void ErodePlugin::onOkClicked(const QUrl &source)
{
    if(m_callback)
        m_callback(m_currentImage, m_originalImage);

    if(m_view){
        m_view->close();
    }
}

void ErodePlugin::onCancelClicked()
{
    if(m_callback)
        m_callback(cv::Mat(), cv::Mat());

    if(m_view){
        m_view->close();
    }
}

void ErodePlugin::onViewClosed()
{
    if (m_view) {
        m_view->deleteLater();
        m_view = nullptr;
    }
    m_imageProvider = nullptr;
}

void ErodePlugin::onResetClicked() {
    if (m_imageProvider) {
        m_imageProvider->setImage(Helper::CvMatToQPixmap(m_originalImage).toImage());
        setCurrentImage(m_originalImage);
    }

    if (m_view) {
        QObject* root = m_view->rootObject();
        if (root) QMetaObject::invokeMethod(root, "refreshPreview");
    }
}

void ErodePlugin::showEditor(const cv::Mat& input, ResultCallback callback)
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

    m_view->setSource(QUrl(QStringLiteral("qrc:/ErodeDialog.qml")));
    m_view->setTitle("Erode Adjust");

    QObject::connect(m_view, &QQuickWindow::closing, this, [this]() {
        qDebug() << "Window is closing";
        this->onViewClosed();
    });

    m_view->show();
}

void ErodePlugin::updatePreview(int kernelSize, int iteration)
{
    m_kernelSize = kernelSize;
    m_iteration = iteration;

    cv::Mat result = m_input.clone();

    if (m_kernelSize < 1) m_kernelSize = 1;
    if (m_kernelSize % 2 == 0) m_kernelSize += 1;

    if (m_iteration < 1) m_iteration = 1;

    int morphShape;

    if (m_mode == "Rect") {
        morphShape = cv::MORPH_RECT;
    } else if (m_mode == "Ellipse") {
        morphShape = cv::MORPH_ELLIPSE;
    } else if (m_mode == "Cross") {
        morphShape = cv::MORPH_CROSS;
    } else {
        morphShape = cv::MORPH_RECT;
    }

    cv::Mat element = cv::getStructuringElement(morphShape, cv::Size(m_kernelSize, m_kernelSize));

    cv::erode(result, result, element, cv::Point(-1, -1), m_iteration);

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
