#include "ImageViewer.h"
#include <QPaintEvent>
#include <QPainter>

// Construct with parent widget
ImageViewer::ImageViewer(QWidget* parent) : QWidget(parent) {}

// Set the image
void ImageViewer::setImage(const QPixmap& image) {
    image_ = image;
    update();
}

// Override function to repaint image to the widget
void ImageViewer::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    if (image_.isNull()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QSize pixSize = image_.size();
    pixSize.scale(event->rect().size(), Qt::KeepAspectRatio);

    QPoint topleft;
    topleft.setX((this->width() - pixSize.width()) / 2);
    topleft.setY((this->height() - pixSize.height()) / 2);

    painter.drawPixmap(topleft, image_.scaled(pixSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
