#pragma once
#include <qcustomplot.h>
#include <QtWidgets>

class MainWindow : public QMainWindow {
  public:
    explicit MainWindow(QWidget* parent = nullptr);

  private:
    void setupPlot();

  private:
    QCustomPlot* plot_;
};
