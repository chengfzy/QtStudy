#pragma once
#include <QWidget>
#include <memory>
#include "Helper.h"

class Window : public QWidget {
    Q_OBJECT

  public:
    Window();

  private:
    std::shared_ptr<Helper> helper_;
};