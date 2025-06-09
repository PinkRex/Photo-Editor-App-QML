#include "ViewController.h"
#include "globals/AppState.h"
#include "controllers/ActionLogController.h"

ViewController::ViewController(QObject *parent) : QObject(parent) {}

void ViewController::setImageController(ImageController *controller) {
    m_imageController = controller;
}

void ViewController::previousImage() {
    QFileInfo current(AppState::instance()->currentPath());
    QDir dir = current.absoluteDir();

    QStringList extensionFilters;
    extensionFilters << "*.jpg" << "*png";
    QStringList fileNames = dir.entryList(extensionFilters, QDir::Files, QDir::Name);
    int index = fileNames.indexOf(QRegularExpression(QRegularExpression::escape(current.fileName())));
    if (index > 0) {
        const QString path = dir.absoluteFilePath(fileNames.at(index - 1));
        m_imageController->setImagePath(path);

        // Log action
        ActionLogController::instance()->pushAction(QString("Previous image: %1").arg(path));
    } else {
        emit showFirstImageDialog();
    }
}

void ViewController::nextImage() {
    QFileInfo current(AppState::instance()->currentPath());
    QDir dir = current.absoluteDir();

    QStringList extensionFilters;
    extensionFilters << "*.jpg" << "*png";
    QStringList fileNames = dir.entryList(extensionFilters, QDir::Files, QDir::Name);
    int index = fileNames.indexOf(QRegularExpression(QRegularExpression::escape(current.fileName())));
    if (index < fileNames.length() - 1) {
        const QString path = dir.absoluteFilePath(fileNames.at(index + 1));
        m_imageController->setImagePath(path);

        // Log action
        ActionLogController::instance()->pushAction(QString("Next image: %1").arg(path));
    } else {
        emit showFirstImageDialog();
    }
}
