#include "GlWidget.h"
#include <QPainter>
#include <QTimer>
#include "Helper.h"

using namespace std;

GlWidget::GlWidget(const shared_ptr<Helper>& helper, QWidget* parent)
    : QOpenGLWidget(parent), helper_(helper), elapsed_(0) {
    setFixedSize(200, 200);
}

void GlWidget::animate() {
    elapsed_ = (elapsed_ + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}

void GlWidget::paintEvent(QPaintEvent* event) {
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    helper_->paint(&painter, event, elapsed_);
    painter.end();
}
