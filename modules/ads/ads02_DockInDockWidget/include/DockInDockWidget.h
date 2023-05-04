#pragma once
#include <DockAreaWidget.h>

class DockInDockManager;
class PerspectivesManager;

class DockInDockWidget : public QWidget {
    Q_OBJECT

  public:
    DockInDockWidget(QWidget* parent, bool canCreateNewGroups, PerspectivesManager* perspectivesManager);
    ~DockInDockWidget() = default;

  public:
    inline const DockInDockManager* getManager() const { return manager_; }
    inline DockInDockManager* getManager() { return manager_; }
    inline const DockInDockWidget* getTopLevelDockWidget() const { return topLevelDockWidget_; }
    inline DockInDockWidget* getTopLevelDockWidget() { return topLevelDockWidget_; }
    inline bool canCreateNewGroups() const { return canCreateNewGroups_; }
    inline const PerspectivesManager* getPerspectivesManager() const { return perspectivesManager_; }
    inline PerspectivesManager* getPerspectivesManager() { return perspectivesManager_; }
    bool isTopLevel() const;

    ads::CDockAreaWidget* addTabWidget(QWidget* widget, const QString& title, ads::CDockAreaWidget* after);
    ads::CDockAreaWidget* addTabWidget(QWidget* widget, const QString& title, QIcon icon, ads::CDockAreaWidget* after);
    DockInDockWidget* createGroup(const QString& groupName, ads::CDockAreaWidget*& insertedWidget);
    DockInDockWidget* createGroup(const QString& groupName, QIcon icon, ads::CDockAreaWidget*& insertedWidget);

    QString getGroupNameError(const QString& groupName);
    void destroyGroup(DockInDockWidget* widget);

    /** Attach a view menu that will be automatically fill */
    void attachViewMenu(QMenu* menu);
    /** Manually fill a given view menu */
    void setupViewMenu(QMenu* menu);
    void setupMenu(QMenu* menu, const std::vector<DockInDockManager*>& moveTo);

    void setNewPerspectiveDefaultName(const QString& defaultName);
    void dumpStatus(std::ostream& str, std::string tab = "");

  private:
    DockInDockWidget(QWidget* parent, DockInDockWidget* topLevelDockWidget, PerspectivesManager* perspectivesManager);
    void fillPerspectivesMenu(QMenu* menu);
    void autoFillAttachedViewMenu();
    void createPerspective();

  private:
    DockInDockManager* manager_ = nullptr;
    DockInDockWidget* topLevelDockWidget_ = nullptr;

    bool canCreateNewGroups_ = false;

    PerspectivesManager* perspectivesManager_;
    QString perspectiveName_;
};