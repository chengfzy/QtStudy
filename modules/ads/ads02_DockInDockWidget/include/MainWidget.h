#pragma once
#include <DockManager.h>
#include <QtWidgets>
#include <memory>
#include "DockInDockWidget.h"
#include "PerspectivesManager.h"

class MainWidget : public QMainWindow {
  public:
    explicit MainWidget(QWidget* parent = nullptr);

  private:
    void setupUI();

  private:
    DockInDockWidget* dockWidget_;
    std::unique_ptr<PerspectivesManager> perspectiveManager_;

    // ads::CDockManager* dockManager_ = nullptr;  // docker manager
};
