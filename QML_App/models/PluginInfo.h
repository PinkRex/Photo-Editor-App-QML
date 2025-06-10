#ifndef PLUGININFO_H
#define PLUGININFO_H

#include <QString>
#include <QPluginLoader>
#include "interfaces/PluginInterface.h"

struct PluginInfo {
    QString name;
    QString filePath;
    PluginInterface *instance;
    QPluginLoader *loader;
    bool enabled = true;
    bool hasEditor = false;
};

#endif // PLUGININFO_H
