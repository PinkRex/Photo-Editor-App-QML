#include "ActionLogModel.h"

ActionLogModel::ActionLogModel(QObject *parent)
    : QAbstractListModel(parent) {}

int ActionLogModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_entries.size();
}

QVariant ActionLogModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_entries.size()) return {};

    const auto &entry = m_entries[index.row()];
    switch (role) {
    case ActionRole: return entry.action;
    case TimestampRole: return entry.timestamp.toString("HH:mm:ss");
    default: return {};
    }
}

QHash<int, QByteArray> ActionLogModel::roleNames() const {
    return {
        { ActionRole, "action" },
        { TimestampRole, "timestamp" }
    };
}

void ActionLogModel::addAction(const QString &action) {
    beginInsertRows(QModelIndex(), 0, 0);
    m_entries.prepend({action, QDateTime::currentDateTime()});
    endInsertRows();
}

void ActionLogModel::clearActionList()
{
    beginResetModel();
    m_entries.clear();
    endResetModel();
}
