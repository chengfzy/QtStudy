#pragma once
#include "QtWidgets"
#include "Server.h"

class MainWidget : public QWidget {
    Q_OBJECT

  public:
    explicit MainWidget(QWidget* parent = nullptr);

  private:
    QLabel* statusLabel_;
    QPushButton* quitButton_;
    Server server_;
};
