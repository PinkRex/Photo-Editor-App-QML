#include "FileController.h"
#include "globals/AppState.h"
#include "controllers/ActionLogController.h"

FileController::FileController(QObject *parent) : QObject(parent) {}

void FileController::setImageController(ImageController *controller) {
    m_imageController = controller;
}

void FileController::openImage(const QString &filePath, bool isReset) {
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        qWarning() << "Invalid file path:" << filePath;
        return;
    }

    if (m_imageController) {
        m_imageController->setImagePath(filePath);
    }

    // Log action
    if (!isReset) {
        ActionLogController::instance()->pushAction(QString("Open file: %1").arg(filePath));
    } else {
        ActionLogController::instance()->pushAction(QString("Reset to file: %1").arg(filePath));
    }
}

void FileController::saveImageAs(const QString &filePath) {
    if (!m_imageController) {
        qWarning() << "ImageController is null.";
        return;
    }

    QString finalPath = filePath;
    if (QFileInfo(finalPath).suffix().isEmpty()) {
        finalPath += ".png";
    }

    QPixmap pixmap = AppState::instance()->currentImage();
    if (pixmap.isNull()) {
        qWarning() << "No image available to save.";
        return;
    }

    QImage image = pixmap.toImage();
    if (!image.save(finalPath)) {
        qWarning() << "Failed to save image to:" << finalPath;
        return;
    }

    emit showSaveSuccessDialog(finalPath);

    // Log action
    ActionLogController::instance()->pushAction(QString("Saved file as: %1").arg(finalPath));
}
