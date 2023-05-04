#pragma once
#include <DockManager.h>
#include <QAction>
#include <QMap>

class DockInDockWidget;
class DockInDockManager : public ads::CDockManager {
    Q_OBJECT

  public:
    DockInDockManager(DockInDockWidget& parent);
    ~DockInDockManager() = default;

  public:
    inline DockInDockWidget& parent() { return parent_; }
    QString getGroupName();
    QString getPersistGroupName();
    static QString getGroupNameFromPersistGroupName(QString persistGroupName);

    void fillViewMenu(QMenu* menu, const std::vector<DockInDockManager*>& moveTo);
    void fillMoveMenu(QMenu* menu, const std::vector<DockInDockManager*>& moveTo);

    void addPerspectiveRec(const QString& name);
    void openPerspectiveRec(const QString& name);
    void removePerspectivesRec();
    void loadPerspectivesRec(QSettings& Settings);
    void savePerspectivesRec(QSettings& Settings) const;

    static DockInDockManager* dockInAManager(ads::CDockWidget* widget);

    void childManagers(std::vector<DockInDockManager*>& managers, bool recursive) const;
    std::vector<DockInDockManager*> allManagers(bool includeThis, bool recursive) const;
    std::vector<std::pair<DockInDockManager*, ads::CDockWidget*>> allDockWidgets(bool includeThis,
                                                                                 bool recursive) const;

    QMap<QString, QStringList> getGroupContents();

    ads::CDockAreaWidget* getInsertDefaultPos();

    std::vector<ads::CDockWidget*> getWidgetsInGUIOrder() const;

  private:
    DockInDockWidget& parent_;
};

class CreateChildDockAction : public QAction {
    Q_OBJECT
  public:
    CreateChildDockAction(DockInDockWidget& dockInDock, QMenu* menu);

  public slots:
    void createGroup();

  private:
    DockInDockWidget& m_dockInDock;
};

class DestroyGroupAction : public QAction {
    Q_OBJECT
  public:
    DestroyGroupAction(DockInDockWidget* widget, QMenu* menu);

  public slots:
    void destroyGroup();

  private:
    DockInDockWidget* m_widget;
};

class MoveDockWidgetAction : public QAction {
    Q_OBJECT
  public:
    MoveDockWidgetAction(ads::CDockWidget* widget, DockInDockManager* moveTo, QMenu* menu);

    static void move(ads::CDockWidget* widget, DockInDockManager* moveTo);

  public slots:
    void move();

  private:
    ads::CDockWidget* m_widget;
    DockInDockManager* m_moveTo;
};
