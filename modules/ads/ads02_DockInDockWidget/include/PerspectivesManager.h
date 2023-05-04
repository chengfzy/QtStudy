#pragma once
#include <QAction>
#include <QMap>
#include <QMenu>
#include <QObject>
#include <QString>
#include <memory>

class QMenu;
class QSettings;

class DockInDockWidget;
class PerspectivesManager : public QObject {
    Q_OBJECT
  private:
    // Partially bypass ADS perspective management, store list here
    // and then ADS will only have one perspective loaded
    // this is because all docking widgets must exist when a perspective is loaded
    // we will guarantee that!
    class PerspectiveInfo {
      public:
        std::shared_ptr<QSettings> settings;
        QMap<QString, QStringList> groups;
    };

  public:
    PerspectivesManager(const QString& perspectivesFolder);
    virtual ~PerspectivesManager();

    QStringList perspectiveNames() const;

    void addPerspective(const QString& name, DockInDockWidget& widget);
    void openPerspective(const QString& name, DockInDockWidget& widget);
    void removePerspectives();
    void removePerspective(const QString& name);

    void loadPerspectives();
    void savePerspectives() const;

  signals:
    void perspectivesListChanged();
    void openingPerspective();
    void openedPerspective();

  private:
    QString getSettingsFileName(const QString& perspective, bool temp) const;
    std::shared_ptr<QSettings> getSettingsObject(const QString& filePath) const;

  private:
    QMap<QString, PerspectiveInfo> perspectives_;

    QString perspectivesFolder_;
};

class LoadPerspectiveAction : public QAction {
    Q_OBJECT
  public:
    LoadPerspectiveAction(QMenu* parent, const QString& name, DockInDockWidget& dockManager);

  public:
    void load();

  private:
    QString name_;
    DockInDockWidget& dockManager_;
};

class RemovePerspectiveAction : public QAction {
    Q_OBJECT
  public:
    RemovePerspectiveAction(QMenu* parent, const QString& name, DockInDockWidget& dockManager);

  public:
    void remove();

  private:
    QString name_;
    DockInDockWidget& dockManager_;
};