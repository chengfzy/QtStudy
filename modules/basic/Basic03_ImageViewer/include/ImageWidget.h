#pragma once
#include <QtWidgets>
#include "ImageGraphicsView.h"

class ImageWidget : public QWidget {
    Q_OBJECT

  public:
    explicit ImageWidget(QWidget* parent = nullptr);

  public:
    void setImage(const QImage& image);

  protected:
  private:
    void setupUI();

  private:
    ImageGraphicsView* view_ = nullptr;  // graphic view
};