#include "PerspectivesManager.h"
#include <QDir>
#include <QFile>
#include <QSettings>
#include "DockInDockManager.h"
#include "DockInDockWidget.h"

#define GROUP_PREFIX QString("Group")

PerspectivesManager::PerspectivesManager(const QString& perspectivesFolder) : perspectivesFolder_(perspectivesFolder) {}

PerspectivesManager::~PerspectivesManager() {
    // remove temp files:
    for (auto perspective : perspectives_) {
        QString fileName = perspective.settings->fileName();
        perspective.settings.reset();
        QFile::remove(fileName);
    }
}

QStringList PerspectivesManager::perspectiveNames() const { return perspectives_.keys(); }

void PerspectivesManager::addPerspective(const QString& name, DockInDockWidget& widget) {
    if (!perspectivesFolder_.isEmpty()) {
        perspectives_[name].settings = getSettingsObject(getSettingsFileName(name, true));
        perspectives_[name].groups = widget.getManager()->getGroupContents();

        // save perspective internally
        widget.getManager()->addPerspectiveRec(name);
        // store it in QSettings object
        widget.getManager()->savePerspectivesRec(*(perspectives_[name].settings));
        // remove internal perspectives
        widget.getManager()->removePerspectives(widget.getManager()->perspectiveNames());
    } else {
        assert(false);
    }

    emit perspectivesListChanged();
}

ads::CDockWidget* findWidget(QString name, const std::vector<DockInDockManager*>& managers) {
    for (auto mgr : managers) {
        auto widget = mgr->findDockWidget(name);
        if (widget) return widget;
    }
    return NULL;
}

void PerspectivesManager::openPerspective(const QString& name, DockInDockWidget& widget) {
    assert(widget.getTopLevelDockWidget() == &widget);

    if (!perspectivesFolder_.isEmpty()) {
        if (perspectives_.contains(name)) {
            emit openingPerspective();

            if (widget.canCreateNewGroups()) {
                auto curGroups = widget.getManager()->allManagers(true, true);
                for (auto group : perspectives_[name].groups.keys()) {
                    bool found = false;
                    for (auto curgroup : curGroups) {
                        if (curgroup->getPersistGroupName() == group) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        group = DockInDockManager::getGroupNameFromPersistGroupName(group);

                        // restore group in file but not in GUI yet
                        ads::CDockAreaWidget* insertPos = NULL;
                        widget.createGroup(group, insertPos);
                    }
                }

                curGroups = widget.getManager()->allManagers(false, true);
                for (auto curgroup : curGroups) {
                    if (!perspectives_[name].groups.keys().contains(curgroup->getPersistGroupName())) {
                        widget.destroyGroup(&curgroup->parent());
                    }
                }
            }

            auto managers = widget.getManager()->allManagers(true, true);
            for (auto group : perspectives_[name].groups.keys()) {
                for (auto mgr : managers) {
                    if (mgr->getPersistGroupName() == group) {
                        for (QString widgetName : perspectives_[name].groups[group]) {
                            ads::CDockWidget* widget = findWidget(widgetName, {mgr});
                            if (widget) {
                                // OK, widget is already in the good manager!
                            } else {
                                widget = findWidget(widgetName, managers);
                                if (widget) {
                                    // move dock widget in the same group as it used to be when perspective was saved
                                    // this guarantee load/open perspectives will work smartly
                                    MoveDockWidgetAction::move(widget, mgr);
                                }
                            }
                        }
                    }
                }
            }

            // internally load perspectives from QSettings
            widget.getManager()->loadPerspectivesRec(*(perspectives_[name].settings));
            // load perspective (update GUI)
            widget.getManager()->openPerspectiveRec(name);
            // remove internal perspectives
            widget.getManager()->removePerspectives(widget.getManager()->perspectiveNames());

            emit openedPerspective();
        }
    } else {
        assert(false);
    }
}

void PerspectivesManager::removePerspectives() {
    perspectives_.clear();
    emit perspectivesListChanged();
}

void PerspectivesManager::removePerspective(const QString& name) {
    perspectives_.remove(name);
    emit perspectivesListChanged();
}

QString PerspectivesManager::getSettingsFileName(const QString& perspective, bool temp) const {
    auto name = (perspective.isEmpty()) ? "perspectives.ini" : "perspective_" + perspective + (temp ? ".tmp" : ".ini");

    return perspectivesFolder_ + "/" + name;
}

std::shared_ptr<QSettings> PerspectivesManager::getSettingsObject(const QString& filePath) const {
    return std::make_shared<QSettings>(filePath, QSettings::IniFormat);
}

void PerspectivesManager::loadPerspectives() {
    if (!perspectivesFolder_.isEmpty()) {
        QDir().mkpath(perspectivesFolder_);

        perspectives_.clear();

        auto mainSettings = getSettingsObject(getSettingsFileName("", false));
        std::string debug = mainSettings->fileName().toStdString();

        int Size = mainSettings->beginReadArray("Perspectives");

        for (int i = 0; i < Size; ++i) {
            mainSettings->setArrayIndex(i);
            QString perspective = mainSettings->value("Name").toString();

            if (!perspective.isEmpty()) {
                // load perspective file:
                auto toLoad = getSettingsFileName(perspective, false);
                auto loaded = getSettingsFileName(perspective, true);

#ifdef _DEBUG
                std::string debug1 = loaded.toStdString();
                std::string debug2 = toLoad.toStdString();
#endif

                QFile::remove(loaded);
                if (!QFile::copy(toLoad, loaded)) assert(false);

                perspectives_[perspective].settings = getSettingsObject(loaded);

                // load group info:
                mainSettings->beginGroup(GROUP_PREFIX);
                for (const auto& key : mainSettings->allKeys()) {
                    perspectives_[perspective].groups[key] = mainSettings->value(key).toStringList();
                }
                mainSettings->endGroup();
            } else {
                assert(false);
            }
        }

        mainSettings->endArray();
    }

    emit perspectivesListChanged();
}

void PerspectivesManager::savePerspectives() const {
    if (!perspectivesFolder_.isEmpty()) {
        auto mainSettings = getSettingsObject(getSettingsFileName("", false));

        // Save list of perspective and group organization
        mainSettings->beginWriteArray("Perspectives", perspectives_.size());
        int i = 0;
        for (auto perspective : perspectives_.keys()) {
            mainSettings->setArrayIndex(i);
            mainSettings->setValue("Name", perspective);
            mainSettings->beginGroup(GROUP_PREFIX);
            for (auto group : perspectives_[perspective].groups.keys()) {
                mainSettings->setValue(group, perspectives_[perspective].groups[group]);
            }
            mainSettings->endGroup();
            ++i;
        }
        mainSettings->endArray();

        // Save perspectives themselves
        for (auto perspectiveName : perspectives_.keys()) {
            auto toSave = getSettingsFileName(perspectiveName, false);
            QSettings& settings = *(perspectives_[perspectiveName].settings);
            settings.sync();

#ifdef _DEBUG
            std::string debug1 = settings.fileName().toStdString();
            std::string debug2 = toSave.toStdString();
#endif

            QFile::remove(toSave);
            if (!QFile::copy(settings.fileName(), toSave)) assert(false);
        }
    }
}

//////////////////////////////
// LoadPerspectiveAction
//////////////////////////////
LoadPerspectiveAction::LoadPerspectiveAction(QMenu* parent, const QString& name, DockInDockWidget& dockManager)
    : QAction(name, parent), name_(name), dockManager_(dockManager) {
    connect(this, SIGNAL(triggered()), this, SLOT(load()));
}

void LoadPerspectiveAction::load() { dockManager_.getPerspectivesManager()->openPerspective(name_, dockManager_); }

//////////////////////////////
// RemovePerspectiveAction
//////////////////////////////
RemovePerspectiveAction::RemovePerspectiveAction(QMenu* parent, const QString& name, DockInDockWidget& dockManager)
    : QAction(name, parent), name_(name), dockManager_(dockManager) {
    connect(this, SIGNAL(triggered()), this, SLOT(remove()));
}

void RemovePerspectiveAction::remove() { dockManager_.getPerspectivesManager()->removePerspective(name_); }
