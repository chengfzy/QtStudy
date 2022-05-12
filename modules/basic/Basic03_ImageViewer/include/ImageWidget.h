#pragma once
#include <QtWidgets>
#include "ImageGraphicsView.h"

class ImageWidget : public QWidget {
    Q_OBJECT

  public:
    explicit ImageWidget(QWidget* parent = nullptr);

  signals:
    void newInfo(const QString& msg);

  public:
    void setImage(const QImage& image);

  private:
    void setupUI();

  private:
    ImageGraphicsView* view_ = nullptr;  // graphic view
};