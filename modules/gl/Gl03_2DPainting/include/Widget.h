#pragma once
#include <QWidget>
#include <memory>

class Helper;

class Widget : public QWidget {
    Q_OBJECT
  public:
    Widget(const std::shared_ptr<Helper>& helper, QWidget* parent);

  public slots:
    void animate();

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    std::shared_ptr<Helper> helper_;  // helper class
    int elapsed_;                     // elapsed time
};