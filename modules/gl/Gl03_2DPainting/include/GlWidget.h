#pragma once
#include <QOpenGLWidget>
#include <memory>

class Helper;

class GlWidget : public QOpenGLWidget {
    Q_OBJECT
  public:
    GlWidget(const std::shared_ptr<Helper>& helper, QWidget* parent);

  public slots:
    void animate();

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    std::shared_ptr<Helper> helper_;  // helper class
    int elapsed_;                     // elapsed time
};