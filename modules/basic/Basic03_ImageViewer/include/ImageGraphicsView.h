#pragma once
#include <QGraphicsView>

class ImageGraphicsView : public QGraphicsView {
    Q_OBJECT
  public:
    using QGraphicsView::QGraphicsView;

    ~ImageGraphicsView() = default;

  public:
    void zoomIn();
    void zoomOut();

  protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

  private:
    QPointF prePos_;  // previouse mouse pos
};