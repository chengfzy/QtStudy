#pragma once
#include <QGraphicsView>

class ImageGraphicsView : public QGraphicsView {
    Q_OBJECT
  public:
    explicit ImageGraphicsView(QGraphicsScene* scene, QWidget* parent = nullptr);

    ~ImageGraphicsView() = default;

  public:
    void setImage(const QImage& image);

  signals:
    void newInfo(const QString& msg);

  public slots:
    void fitToWindow();
    void zoomIn();
    void zoomOut();

    void move();

  protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

  private:
    QImage image_;                 // image
    QTransform transform_;         // transform
    bool isMousePressed_ = false;  // indict mouse is pressed
    QPoint prePos_;                // previous position when mouse pressed
    QPoint currentPos_;            // current pos
};