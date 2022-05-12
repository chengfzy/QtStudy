#pragma once
#include <QMainWindow>
#include "ImageWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow(QWidget* parent = nullptr);

  public:
    void setImage(const QImage& image);

  private:
    void setupUI();

  private:
    ImageWidget* imageWidget_ = nullptr;
    QLabel* infoLabel_ = nullptr;
};