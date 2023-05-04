#include "DockInDockWidget.h"
#include <QtWidgets>
#include <set>
#include "DockInDockManager.h"
#include "PerspectivesManager.h"

using namespace std;
using namespace ads;

DockInDockWidget::DockInDockWidget(QWidget* parent, bool canCreateNewGroups, PerspectivesManager* perspectivesManager)
    : DockInDockWidget(parent, nullptr, perspectivesManager) {
    canCreateNewGroups_ = canCreateNewGroups;
    topLevelDockWidget_ = this;
}

CDockAreaWidget* DockInDockWidget::addTabWidget(QWidget* widget, const QString& title, CDockAreaWidget* after) {
    return addTabWidget(widget, title, QIcon(), after);
}

CDockAreaWidget* DockInDockWidget::addTabWidget(QWidget* widget, const QString& title, QIcon icon,
                                                CDockAreaWidget* after) {
    for (auto existing : getTopLevelDockWidget()->getManager()->allDockWidgets(true, true)) {
        if (existing.second->objectName() == title) {
            QMessageBox::critical(this, "Error", "Name '" + title + "' already in use");
            return nullptr;
        }
    }

    CDockWidget* DockWidget = new CDockWidget(title);
    DockWidget->setWidget(widget);
    DockWidget->setIcon(icon);

    // Add the dock widget to the top dock widget area
    return manager_->addDockWidget(CenterDockWidgetArea, DockWidget, after);
}

DockInDockWidget* DockInDockWidget::createGroup(const QString& groupName, CDockAreaWidget*& insertedWidget) {
    return createGroup(groupName, QIcon(), insertedWidget);
}

DockInDockWidget* DockInDockWidget::createGroup(const QString& groupName, QIcon icon,
                                                CDockAreaWidget*& insertedWidget) {
    QString error = getGroupNameError(groupName);
    if (!error.isEmpty()) {
        QMessageBox::critical(nullptr, "Error", error);
        return nullptr;
    }

    DockInDockWidget* child = new DockInDockWidget(this, topLevelDockWidget_, perspectivesManager_);
    child->setObjectName(groupName);

    CDockWidget* DockWidget = new CDockWidget(groupName);
    DockWidget->setWidget(child);
    DockWidget->setIcon(icon);

    insertedWidget = manager_->addDockWidget(CenterDockWidgetArea, DockWidget, insertedWidget);

    return child;
}

bool DockInDockWidget::isTopLevel() const { return objectName().isEmpty(); }

QString DockInDockWidget::getGroupNameError(const QString& groupName) {
    if (groupName.isEmpty()) {
        return "Group must have a non-empty title";
    }

    vector<DockInDockManager*> dockManagers = manager_->allManagers(true, true);
    for (auto mgr : dockManagers) {
        if (mgr->getGroupName() == groupName) {
            return "Group title '" + groupName + "' already used";
        }
    }

    return "";
}

void DockInDockWidget::destroyGroup(DockInDockWidget* widgetToRemove) {
    auto topLevelWidget = widgetToRemove->getTopLevelDockWidget();

    if (topLevelWidget && topLevelWidget != widgetToRemove) {
        // re-affect all child docks to top-level, don't use allDockWidgets to preserve sub-groups
        for (auto dockWidget : widgetToRemove->getManager()->getWidgetsInGUIOrder()) {
            MoveDockWidgetAction::move(dockWidget, topLevelWidget->getManager());
        }
        assert(widgetToRemove->getManager()->allDockWidgets(true, true).empty());

        // find widget's parent:
        for (auto dockWidget : topLevelWidget->getManager()->allDockWidgets(true, true)) {
            if (dockWidget.second->widget() == widgetToRemove) {
                dockWidget.first->removeDockWidget(dockWidget.second);
                delete dockWidget.second;
                // delete widgetToRemove; automatically deleted when dockWidget is deleted
                widgetToRemove = nullptr;
                break;
            }
        }
    } else {
        assert(false);
    }
}

void DockInDockWidget::attachViewMenu(QMenu* menu) {
    connect(menu, &QMenu::aboutToShow, this, &DockInDockWidget::autoFillAttachedViewMenu);
}

void DockInDockWidget::setupViewMenu(QMenu* menu) {
    vector<DockInDockManager*> dockManagers = manager_->allManagers(true, true);

    bool hasPerspectivesMenu = false;
    if (getTopLevelDockWidget() == this) {
        hasPerspectivesMenu = perspectivesManager_ != nullptr;
    } else {
        assert(false);
    }

    QMenu* organize = menu;
    if (hasPerspectivesMenu) {
        organize = menu->addMenu("Organize");
    }
    setupMenu(organize, dockManagers);

    if (hasPerspectivesMenu) {
        QMenu* perspectives = menu->addMenu("Perspectives");
        fillPerspectivesMenu(perspectives);
    }
}

void DockInDockWidget::setupMenu(QMenu* menu, const vector<DockInDockManager*>& moveTo) {
    manager_->fillViewMenu(menu, moveTo);
    menu->addSeparator();
    auto moveMenu = menu->addMenu("Move");
    manager_->fillMoveMenu(moveMenu, moveTo);
}
void DockInDockWidget::setNewPerspectiveDefaultName(const QString& defaultName) { perspectiveName_ = defaultName; }

static void dumpStatus(ostream& str, CDockWidget* widget, const string& tab, string suffix) {
    DockInDockManager* manager = DockInDockManager::dockInAManager(widget);
    if (manager) {
        manager->parent().dumpStatus(str, tab);
    } else {
        str << tab << widget->objectName().toStdString() << suffix << endl;
    }
}

void DockInDockWidget::dumpStatus(ostream& str, string tab) {
    str << tab << "Group: " << getManager()->getGroupName().toStdString() << endl;
    tab += "   ";
    set<CDockWidget*> visibleWidgets;
    for (auto widget : getManager()->getWidgetsInGUIOrder()) {
        visibleWidgets.insert(widget);
        ::dumpStatus(str, widget, tab, "");
    }

    for (auto closed : getManager()->dockWidgetsMap()) {
        if (visibleWidgets.find(closed) == visibleWidgets.end()) {
            ::dumpStatus(str, closed, tab, " (closed)");
        }
    }
}

DockInDockWidget::DockInDockWidget(QWidget* parent, DockInDockWidget* topLevelDockWidget,
                                   PerspectivesManager* perspectivesManager)
    : QWidget(parent),
      topLevelDockWidget_(topLevelDockWidget),
      canCreateNewGroups_((topLevelDockWidget) ? topLevelDockWidget->canCreateNewGroups_ : false),
      perspectivesManager_(perspectivesManager) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(manager_ = new DockInDockManager(*this));
}

void DockInDockWidget::fillPerspectivesMenu(QMenu* menu) {
    menu->addAction("Create perspective...", this, &DockInDockWidget::createPerspective);

    QStringList perspectiveNames;
    if (perspectivesManager_) perspectiveNames = perspectivesManager_->perspectiveNames();

    if (!perspectiveNames.isEmpty()) {
        QMenu* load = menu->addMenu("Load perspective");
        for (const auto& title : perspectiveNames) {
            load->addAction(new LoadPerspectiveAction(load, title, *this));
        }
        QMenu* remove = menu->addMenu("Remove perspective");
        for (const auto& title : perspectiveNames) {
            remove->addAction(new RemovePerspectiveAction(remove, title, *this));
        }
    }
}

void DockInDockWidget::autoFillAttachedViewMenu() {
    QMenu* menu = dynamic_cast<QMenu*>(QObject::sender());

    if (menu) {
        menu->clear();
        setupViewMenu(menu);
    } else {
        assert(false);
    }
}

void DockInDockWidget::createPerspective() {
    if (!perspectivesManager_) return;

    QString title = perspectiveName_;
    if (!perspectiveName_.isEmpty()) {
        int index = 2;
        while (perspectivesManager_->perspectiveNames().contains(title)) {
            title = perspectiveName_ + " (" + QString::number(index) + ")";
            ++index;
        }
    }

    while (true) {
        bool ok = false;
        title = QInputDialog::getText(nullptr, "Create perspective", "Enter perspective title", QLineEdit::Normal,
                                      title, &ok);
        if (ok) {
            if (title.isEmpty()) {
                QMessageBox::critical(nullptr, "Error", "Perspective title cannot be empty");
                continue;
            } else if (perspectivesManager_->perspectiveNames().contains(title)) {
                if (QMessageBox::critical(nullptr, "Error", "Perspective '" + title + "' already exists, overwrite it?",
                                          QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
                    continue;
            }

            perspectivesManager_->addPerspective(title, *this);
            break;
        } else {
            break;
        }
    }
}