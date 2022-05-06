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
    QImage image_;                              // image
    ImageGraphicsView* view_ = nullptr;         // graphic view
    QGraphicsPixmapItem* imageItem_ = nullptr;  // image graphics item
    double scale_ = 1.0;
};