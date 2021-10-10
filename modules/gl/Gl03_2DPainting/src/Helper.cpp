#include "Helper.h"
#include <QPaintEvent>
#include <QPainter>

Helper::Helper() {
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, QColor(0xa6, 0xce, 0x39));

    background_ = QBrush(QColor(64, 32, 64));
    circleBrush_ = QBrush(gradient);
    circlePen_ = QPen(Qt::black);
    circlePen_.setWidth(1);
    textPen_ = QPen(Qt::white);
    textFont_.setPixelSize(50);
}

void Helper::paint(QPainter* painter, QPaintEvent* event, int elapsed) {
    painter->fillRect(event->rect(), background_);
    painter->translate(100, 100);

    // draw circle
    painter->save();
    painter->setBrush(circleBrush_);
    painter->setPen(circlePen_);
    painter->rotate(elapsed * 0.030);
    qreal r = elapsed / 1000.;
    int n{30};
    for (int i = 0; i < n; ++i) {
        painter->rotate(30);
        qreal factor = (i + r) / n;
        qreal radius = 120.0 * factor;
        qreal circleRadius = 1 + factor * 20;
        painter->drawEllipse(QRectF(radius, -circleRadius, circleRadius * 2, circleRadius * 2));
    }
    painter->restore();

    // draw text
    painter->setPen(textPen_);
    painter->setFont(textFont_);
    painter->drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, "Qt");
}
