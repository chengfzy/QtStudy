#pragma once
#include <qcustomplot.h>
#include <QtWidgets>

class MainWindow : public QMainWindow {
  public:
    explicit MainWindow(QWidget* parent = nullptr);

  public slots:
    void setupPlot();

  private:
    QCustomPlot* plot_;
};
