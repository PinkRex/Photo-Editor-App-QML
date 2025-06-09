#include <QPluginLoader>
#include <QCoreApplication>
#include <QSettings>
#include <QFileInfo>
#include "PluginController.h"
#include "HistoryController.h"
#include "globals/AppState.h"
#include "globals/Helper.h"
#include "controllers/ActionLogController.h"

PluginController::PluginController(QObject* parent) : QObject(parent), m_pluginModel(new PluginModel(this)) {}

QString PluginController::iniPath() const {
    return QCoreApplication::applicationDirPath() + "/plugins.ini";
}

void PluginController::saveLoadedPluginPaths() {
    QSettings settings(iniPath(), QSettings::IniFormat);
    settings.beginGroup("Plugins");
    settings.setValue("LoadedPaths", QStringList(m_loadedPaths.values()));
    settings.endGroup();

    for (const PluginInfo &plugin : std::as_const(m_pluginModel->pluginList())) {
        QString group = QString("PluginState:%1").arg(plugin.filePath);
        settings.beginGroup(group);
        settings.setValue("enabled", plugin.enabled);
        settings.endGroup();
    }
}

static bool areMatsEqual(const cv::Mat& mat1, const cv::Mat& mat2) {
    if (mat1.size() != mat2.size() || mat1.type() != mat2.type()) {
        return false;
    }

    return cv::norm(mat1, mat2, cv::NORM_INF) == 0;
}

void PluginController::performPlugin(int index) {
    if (index < 0 || index >= m_pluginModel->pluginList().size()) return;

    PluginInfo info = m_pluginModel->pluginList().at(index);
    if (!info.enabled) {
        qDebug() << "Plugin is disabled:" << info.name;
        emit showDisabledDialog();
        return;
    }

    cv::Mat input = Helper::QPixmapToCvMat(AppState::instance()->currentImage());
    HistoryController::instance()->push(input);
    if (input.empty()) {
        qWarning() << "Image not found!";
        return;
    }

    cv::Mat output;
    if (!info.hasEditor) {
        info.instance->edit(input, output);
    } else {
        info.instance->showEditor(input, [this, info](cv::Mat result, cv::Mat prePlugin) {
            if (!result.empty() && !prePlugin.empty()) {
                cv::Mat outputMat = result;
                cv::Mat currentImage = Helper::QPixmapToCvMat(AppState::instance()->currentImage());
                if (areMatsEqual(prePlugin, currentImage)) {
                    AppState::instance()->setCurrentImage(Helper::CvMatToQPixmap(outputMat));
                    AppState::instance()->setEdittingBaseImage(Helper::CvMatToQPixmap(outputMat));
                    ActionLogController::instance()->pushAction(info.name);
                } else {
                    AppState::instance()->setPluginImage(Helper::CvMatToQPixmap(outputMat));
                    AppState::instance()->setPluginName(info.name);
                    emit showConfirmDialog();
                }
            } else {
                qDebug() << "Edit canceled";
            }
        });
        return;
    }

    AppState::instance()->setCurrentImage(Helper::CvMatToQPixmap(output));
    AppState::instance()->setEdittingBaseImage(Helper::CvMatToQPixmap(output));
    ActionLogController::instance()->pushAction(info.name);
}

void PluginController::autoLoadPlugins() {
    QSettings settings(iniPath(), QSettings::IniFormat);
    settings.beginGroup("Plugins");
    QStringList savedPaths = settings.value("LoadedPaths").toStringList();
    settings.endGroup();
    loadPluginsManually(savedPaths);
}

void PluginController::loadPluginsManually(const QStringList &filePaths) {
    QSettings settings(iniPath(), QSettings::IniFormat);

    for (const QString &filePath : filePaths) {
        if (m_loadedPaths.contains(filePath)) {
            qWarning() << "Duplicate plugin ignored:" << filePath;
            continue;
        }

        QPluginLoader *loader = new QPluginLoader(filePath);
        QObject *plugin = loader->instance();

        if (!plugin) {
            qWarning() << "Failed to load plugin:" << filePath;
            qWarning() << "Error:" << loader->errorString();
            delete loader;
            continue;
        }

        PluginInterface *loadedPlugin = qobject_cast<PluginInterface*>(plugin);
        if (loadedPlugin) {
            QString group = QString("PluginState:%1").arg(filePath);
            settings.beginGroup(group);
            bool enabled = settings.value("enabled", true).toBool();
            settings.endGroup();

            PluginInfo info = {
                loadedPlugin->name(),
                filePath,
                loadedPlugin,
                loader,
                enabled,
                loadedPlugin->hasEditor()
            };
            m_pluginModel->addPlugin(info);
            m_loadedPaths.insert(filePath);
        } else {
            loader->unload();
            delete loader;
        }
    }
    saveLoadedPluginPaths();
}

void PluginController::unloadPlugins(const QList<int>& indexes) {
    QList<PluginInfo> removed = m_pluginModel->removePluginsAtIndexes(indexes);

    for (const PluginInfo &info : std::as_const(removed)) {
        m_loadedPaths.remove(info.filePath);
    }

    saveLoadedPluginPaths();
}

void PluginController::disablePlugins(const QVariantList &indexesVariant) {
    QList<int> indexes;
    for (const QVariant &v : indexesVariant)
        indexes.append(v.toInt());
    m_pluginModel->disablePluginsAtIndexes(indexes);
    saveLoadedPluginPaths();
}

void PluginController::enablePlugins(const QVariantList &indexesVariant) {
    QList<int> indexes;
    for (const QVariant &v : indexesVariant)
        indexes.append(v.toInt());
    m_pluginModel->enablePluginsAtIndexes(indexes);
    saveLoadedPluginPaths();
}
