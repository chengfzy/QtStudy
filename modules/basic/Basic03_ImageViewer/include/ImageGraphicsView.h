#pragma once
#include <QGraphicsView>

class QGraphicsView;

class ImageGraphicsView : public QGraphicsView {
    Q_OBJECT
  public:
    explicit ImageGraphicsView(QWidget* parent = nullptr);
    ~ImageGraphicsView() = default;

  public:
    void setImage(const QImage& image);

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    QImage image_;  // image
    QTransform transform_;
    float ratio_ = 1.0;  // ratio

    // QTimer* displayTimer_ = nullptr;  // timer for display info
};