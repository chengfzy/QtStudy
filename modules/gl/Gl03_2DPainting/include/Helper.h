#pragma once
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>

class Helper {
  public:
    Helper();

  public:
    void paint(QPainter* painter, QPaintEvent* event, int elapsed);

  private:
    QBrush background_;
    QBrush circleBrush_;
    QFont textFont_;
    QPen circlePen_;
    QPen textPen_;
};