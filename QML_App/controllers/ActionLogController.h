#ifndef ACTIONLOGCONTROLLER_H
#define ACTIONLOGCONTROLLER_H

#include <QObject>
#include <QClipboard>
#include <QGuiApplication>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include "models/ActionLogModel.h"

class ActionLogController : public QObject {
    Q_OBJECT
    Q_PROPERTY(ActionLogModel* model READ model CONSTANT)
public:
    static ActionLogController* instance();

    explicit ActionLogController(QObject *parent = nullptr);

    Q_INVOKABLE void pushAction(const QString &action);
    Q_INVOKABLE void clearLogs();
    Q_INVOKABLE void copyLogs();
    Q_INVOKABLE void exportLogs();
    ActionLogModel* model();

signals:
    void showCopySuccessDialog();
    void showExportSuccessDialog(const QString &path);
    void showListEmptyDialog();

private:
    static ActionLogController* s_instance;
    ActionLogModel m_model;
};

#endif // ACTIONLOGCONTROLLER_H
