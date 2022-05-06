#pragma once
#include <QGraphicsView>

class ImageGraphicsView : public QGraphicsView {
    Q_OBJECT
  public:
    using QGraphicsView::QGraphicsView;

    ~ImageGraphicsView() = default;

  public slots:
    void zoomIn();
    void zoomOut();

  protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

  private:
    bool isMousePressed_ = false;  // indict mouse is pressed
    QPointF centerPos_;            // center pos
    QPoint prePos_;                // previouse mouse pos
};