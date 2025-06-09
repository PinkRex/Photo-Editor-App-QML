#ifndef ACTIONLOGMODEL_H
#define ACTIONLOGMODEL_H

#include <QAbstractListModel>
#include "ActionLogInfo.h"

class ActionLogModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum ActionRoles {
        ActionRole = Qt::UserRole + 1,
        TimestampRole
    };

    explicit ActionLogModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addAction(const QString &action);
    void clearActionList();

private:
    QList<ActionLogInfo> m_entries;
};

#endif // ACTIONLOGMODEL_H
