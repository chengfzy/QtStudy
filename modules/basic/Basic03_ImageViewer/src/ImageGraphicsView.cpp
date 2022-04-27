#include "ImageGraphicsView.h"
#include <QTimer>

#include <QDebug>

using namespace std;

ImageGraphicsView::ImageGraphicsView(QWidget* parent) : QGraphicsView(parent) {
    setContentsMargins(0, 0, 0, 0);
    setMinimumSize(400, 300);
    setAlignment(Qt::AlignHCenter);

    // // display timer
    //  displayTimer_ = new QTimer(this);
    //  timer

    // setInteractive(false);
    // setMouseTracking(true);
}

void ImageGraphicsView::setImage(const QImage& image) {
    image_ = image;

    // set ratio
    qDebug() << QString("image size: %1x%2").arg(image_.width()).arg(image_.height());
    qDebug() << QString("view size: %1x%2").arg(width()).arg(height());
    // ratio_ = min(static_cast<float>(width()) / image_.width(), static_cast<float>(height()) / image_.height());
    ratio_ = static_cast<float>(width()) / image_.width();
    resize(width(), image_.height() * ratio_);
    auto h = image_.height() * ratio_;
    qDebug() << QString("view size: %1x%2, ratio = %3, h = %4").arg(width()).arg(height()).arg(ratio_).arg(h);

    updateGeometry();
    viewport()->update();
}

void ImageGraphicsView::paintEvent(QPaintEvent* event) {
    QPainter painter(viewport());

    painter.setWorldTransform(transform_);
    painter.drawImage(QRect(0, 0, viewport()->width(), viewport()->height()), image_,
                      QRect(0, 0, image_.width(), image_.height()));
    painter.setWorldMatrixEnabled(false);
}