#pragma once
#include <DockManager.h>
#include <QtWidgets>

class MainWidget : public QWidget {
  public:
    explicit MainWidget(QWidget* parent = nullptr);

  private:
    void setupUI();

  private:
    ads::CDockManager* dockManager_ = nullptr;  // docker manager
};
