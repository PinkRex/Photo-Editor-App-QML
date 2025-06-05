#include "PluginModel.h"

PluginModel::PluginModel(QObject* parent)
    : QAbstractListModel(parent) {}

int PluginModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_plugins.size();
}

QVariant PluginModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_plugins.size())
        return {};

    const PluginInfo &info = m_plugins.at(index.row());
    switch (role) {
    case NameRole: return info.name;
    case InstanceRole: return QVariant::fromValue<void*>(info.instance);
    case EnabledRole: return info.enabled;
    case HasEditorRole: return info.hasEditor;
    default: return {};
    }
}

QHash<int, QByteArray> PluginModel::roleNames() const {
    return {
        { NameRole, "name" },
        { InstanceRole, "instance" },
        { EnabledRole, "enabled" },
        { HasEditorRole, "hasEditor" }
    };
}

void PluginModel::addPlugin(const PluginInfo &info) {
    beginInsertRows(QModelIndex(), m_plugins.size(), m_plugins.size());
    m_plugins.append(info);
    endInsertRows();
}

QList<PluginInfo>& PluginModel::pluginList() {
    return m_plugins;
}

QStringList PluginModel::pluginNames() const {
    QStringList names;
    for (const PluginInfo& info : m_plugins) {
        names << info.name;
    }
    return names;
}

QStringList PluginModel::pluginStates() const {
    QStringList states;
    for (const PluginInfo& info : m_plugins) {
        states << (info.enabled ? "1" : "0");
    }
    return states;
}

QList<PluginInfo> PluginModel::removePluginsAtIndexes(const QList<int>& indexes) {
    QList<int> sorted = indexes;
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());

    QList<PluginInfo> removedPlugins;

    beginResetModel();
    for (int i : sorted) {
        if (i >= 0 && i < m_plugins.size()) {
            PluginInfo info = m_plugins[i];
            if (!info.loader->unload()) {
                qWarning() << "Failed to unload plugin:" << info.filePath;
            } else {
                qDebug() << "Plugin unloaded:" << info.filePath;
            }

            delete info.loader;
            removedPlugins.append(info);
            m_plugins.removeAt(i);
        }
    }
    endResetModel();

    return removedPlugins;
}

void PluginModel::disablePluginsAtIndexes(const QList<int>& indexes) {
    for (int i : indexes) {
        if (i >= 0 && i < m_plugins.size()) {
            m_plugins[i].enabled = false;
        }
    }
    emit dataChanged(index(0), index(m_plugins.size() - 1));
}

void PluginModel::enablePluginsAtIndexes(const QList<int>& indexes) {
    for (int i : indexes) {
        if (i >= 0 && i < m_plugins.size()) {
            m_plugins[i].enabled = true;
        }
    }
    emit dataChanged(index(0), index(m_plugins.size() - 1));
}

