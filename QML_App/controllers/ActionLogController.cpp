#include "ActionLogController.h"

ActionLogController* ActionLogController::s_instance = nullptr;

ActionLogController* ActionLogController::instance() {
    if (!s_instance)
        s_instance = new ActionLogController();
    return s_instance;
}

ActionLogController::ActionLogController(QObject *parent)
    : QObject(parent) {}

void ActionLogController::pushAction(const QString &action) {
    m_model.addAction(action);
}

void ActionLogController::clearLogs() {
    m_model.clearActionList();
}

void ActionLogController::copyLogs()
{
    if (m_model.rowCount() == 0) {
        qWarning("Export failed: log is empty.");
        emit showListEmptyDialog();
        return;
    }

    QString allLogs;
    for (int i = 0; i < m_model.rowCount(); ++i) {
        QModelIndex idx = m_model.index(i, 0);
        QString action = m_model.data(idx, ActionLogModel::ActionRole).toString();
        QString timestamp = m_model.data(idx, ActionLogModel::TimestampRole).toString();
        allLogs += QString("[%1] %2\n").arg(timestamp, action);
    }

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(allLogs);

    emit showCopySuccessDialog();
}

void ActionLogController::exportLogs()
{
    if (m_model.rowCount() == 0) {
        qWarning("Export failed: log is empty.");
        emit showListEmptyDialog();
        return;
    }

    QString appDir = QCoreApplication::applicationDirPath();

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    QString fileName = QString("Logfile_%1.txt").arg(timestamp);
    QString filePath = QDir(appDir).filePath(fileName);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Cannot open file for writing: %s", qPrintable(file.errorString()));
        return;
    }

    QTextStream out(&file);
    out << "Log Export Time: " << timestamp << "\n";
    out << "===========================\n";
    for (int i = 0; i < m_model.rowCount(); ++i) {
        QModelIndex idx = m_model.index(i, 0);
        QString action = m_model.data(idx, ActionLogModel::ActionRole).toString();
        QString actionTime = m_model.data(idx, ActionLogModel::TimestampRole).toString();
        out << QString("[%1] %2\n").arg(actionTime, action);
    }

    file.close();

    emit showExportSuccessDialog(filePath);
    return;
}

ActionLogModel* ActionLogController::model() {
    return &m_model;
}
