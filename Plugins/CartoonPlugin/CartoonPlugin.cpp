#include "CartoonPlugin.h"
#include "ImageProvider.h"
#include "Helper.h"
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QImage>
#include <QBuffer>
#include <QByteArray>
#include <opencv2/imgproc.hpp>

CartoonPlugin::CartoonPlugin() : m_view(nullptr) {}

CartoonPlugin::~CartoonPlugin()
{
    if (m_view) {
        m_view->close();
        delete m_view;
    }
}

QString CartoonPlugin::name()
{
    return "Cartoon";
}

bool CartoonPlugin::hasEditor() { return true; }

void CartoonPlugin::edit(const cv::Mat &input, cv::Mat &output) {}

void CartoonPlugin::setCurrentImage(cv::Mat img) {
    m_currentImage = img;
}

void CartoonPlugin::setOriginalImage(cv::Mat img) {
    m_originalImage = img;
}

void CartoonPlugin::setMode(const QString &mode) {
    m_mode = mode;
}

void CartoonPlugin::onOkClicked(const QUrl &source)
{
    if(m_callback)
        m_callback(m_currentImage, m_originalImage);

    if(m_view){
        m_view->close();
    }
}

void CartoonPlugin::onCancelClicked()
{
    if(m_callback)
        m_callback(cv::Mat(), cv::Mat());

    if(m_view){
        m_view->close();
    }
}

void CartoonPlugin::onViewClosed()
{
    if (m_view) {
        m_view->deleteLater();
        m_view = nullptr;
    }
    m_imageProvider = nullptr;
}

void CartoonPlugin::onResetClicked() {
    if (m_imageProvider) {
        m_imageProvider->setImage(Helper::CvMatToQPixmap(m_originalImage).toImage());
        setCurrentImage(m_originalImage);
    }

    if (m_view) {
        QObject* root = m_view->rootObject();
        if (root) QMetaObject::invokeMethod(root, "refreshPreview");
    }
}

void CartoonPlugin::showEditor(const cv::Mat& input, ResultCallback callback)
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

    m_view->setSource(QUrl(QStringLiteral("qrc:/CartoonDialog.qml")));
    m_view->setTitle("Cartoon Adjust");

    QObject::connect(m_view, &QQuickWindow::closing, this, [this]() {
        qDebug() << "Window is closing";
        this->onViewClosed();
    });

    m_view->show();
}

void CartoonPlugin::updatePreview(cv::Mat result)
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

void CartoonPlugin::classicCartoon(int smoothness, int edgeStrength) {
    cv::Mat bilateral, gray, edges, color, result;

    cv::bilateralFilter(m_input, bilateral, 9 + smoothness, 75, 75);
    cv::cvtColor(m_input, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, gray, 7);
    int blockSize = std::max(3, edgeStrength | 1);
    cv::adaptiveThreshold(gray, edges, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, blockSize, 2);
    cv::cvtColor(edges, edges, cv::COLOR_GRAY2BGR);
    cv::bitwise_and(bilateral, edges, result);

    updatePreview(result);
}

void CartoonPlugin::customEdge(int smoothness, int edgeThickness, int red, int green, int blue) {
    cv::Mat smoothed, gray, edges, dilated, colorEdges, result;

    int blurKernelSize = smoothness * 2 + 1;
    if (smoothness > 0) {
        cv::bilateralFilter(m_input, smoothed, blurKernelSize, 75, 75);
    } else {
        smoothed = m_input.clone();
    }

    cv::cvtColor(smoothed, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, edges, 50, 150);

    if (edgeThickness > 1) {
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(edgeThickness, edgeThickness));
        cv::dilate(edges, dilated, kernel);
    } else {
        dilated = edges.clone();
    }

    cv::cvtColor(dilated, colorEdges, cv::COLOR_GRAY2BGR);
    for (int y = 0; y < colorEdges.rows; ++y) {
        for (int x = 0; x < colorEdges.cols; ++x) {
            if (dilated.at<uchar>(y, x) > 0) {
                colorEdges.at<cv::Vec3b>(y, x) = cv::Vec3b(blue, green, red);
            }
        }
    }

    cv::addWeighted(m_input, 1.0, colorEdges, 1.0, 0.0, result);

    updatePreview(result);
}

void CartoonPlugin::toonShading(int colorLevel, int edgeStrength) {
    cv::Mat imgBlurred, gray, edges, quantized, result;

    cv::bilateralFilter(m_input, imgBlurred, 9, 75, 75);
    cv::cvtColor(m_input, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, gray, 7);
    int blockSize = std::max(3, edgeStrength | 1);
    cv::adaptiveThreshold(gray, edges, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, blockSize, 2);
    cv::cvtColor(edges, edges, cv::COLOR_GRAY2BGR);
    quantized = imgBlurred.clone();
    int div = std::max(1, 256 / colorLevel);
    quantized = quantized / div * div + div / 2;
    cv::bitwise_and(quantized, edges, result);

    updatePreview(result);
}

void CartoonPlugin::comicBook(int threshold, int thickness, int level) {
    cv::Mat gray, edges, dilated, edgeColor, result;

    cv::cvtColor(m_input, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, edges, threshold, threshold * 2);
    if (thickness > 1) {
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,
                                                   cv::Size(thickness, thickness));
        cv::dilate(edges, dilated, kernel);
    } else {
        dilated = edges.clone();
    }

    edgeColor = m_input.clone();
    for (int y = 0; y < edgeColor.rows; ++y) {
        for (int x = 0; x < edgeColor.cols; ++x) {
            if (dilated.at<uchar>(y, x) > 0) {
                edgeColor.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
            }
        }
    }

    result = edgeColor.clone();
    int div = std::max(1, 256 / level);
    result = result / div * div + div / 2;

    updatePreview(result);
}
