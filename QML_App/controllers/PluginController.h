#ifndef PLUGINCONTROLLER_H
#define PLUGINCONTROLLER_H

#include <QObject>
#include <QDir>
#include <QSet>
#include <QString>
#include <QDebug>
#include "models/PluginModel.h"
#include "interfaces/ControllerInterface.h"

class PluginController : public QObject, public ControllerInterface {
    Q_OBJECT
    Q_PROPERTY(PluginModel* pluginModel READ pluginModel CONSTANT)

public:
    explicit PluginController(QObject* parent = nullptr);

    PluginModel* pluginModel() const { return m_pluginModel; }

    Q_INVOKABLE void unloadPlugins(const QList<int>& indexes);
    Q_INVOKABLE void performPlugin(int index);
    Q_INVOKABLE void autoLoadPlugins();
    Q_INVOKABLE void loadPluginsManually(const QStringList &filePaths);
    Q_INVOKABLE void disablePlugins(const QVariantList &indexes);
    Q_INVOKABLE void enablePlugins(const QVariantList &indexes);

signals:
    void showConfirmDialog();
    void showDisabledDialog();

private:
    PluginModel* m_pluginModel;

    QSet<QString> m_loadedPaths;
    QString iniPath() const;
    void saveLoadedPluginPaths();
};

#endif // PLUGINCONTROLLER_H
