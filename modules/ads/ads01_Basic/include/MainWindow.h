#pragma once
#include <DockManager.h>
#include <QtWidgets>

class MainWindow : public QMainWindow {
  public:
    explicit MainWindow(QWidget* parent = nullptr);

  private:
    void setupUI();

  private:
    ads::CDockManager* dockManager_ = nullptr;  // docker manager
};
