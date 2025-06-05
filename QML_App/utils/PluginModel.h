#ifndef PLUGINMODEL_H
#define PLUGINMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "PluginInfo.h"

class PluginModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        InstanceRole,
        EnabledRole,
        HasEditorRole
    };

    explicit PluginModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    void addPlugin(const PluginInfo &info);
    QList<PluginInfo>& pluginList();

    Q_INVOKABLE QList<PluginInfo> removePluginsAtIndexes(const QList<int>& indexes);
    Q_INVOKABLE void disablePluginsAtIndexes(const QList<int>& indexes);
    Q_INVOKABLE void enablePluginsAtIndexes(const QList<int>& indexes);
    Q_INVOKABLE QStringList pluginNames() const;
    Q_INVOKABLE QStringList pluginStates() const;

private:
    QList<PluginInfo> m_plugins;
};

#endif // PLUGINMODEL_H
