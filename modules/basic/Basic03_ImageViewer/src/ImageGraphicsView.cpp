#include "ImageGraphicsView.h"
#include <QWheelEvent>

#include <QDebug>

using namespace std;

ImageGraphicsView::ImageGraphicsView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {
    setMouseTracking(true);
}

void ImageGraphicsView::setImage(const QImage& image) {
    image_ = image;

    // set ratio
    qDebug() << QString("image size: %1x%2").arg(image_.width()).arg(image_.height());
    qDebug() << QString("view size: %1x%2").arg(width()).arg(height());
    qDebug() << QString("view port size: %1x%2").arg(viewport()->width()).arg(viewport()->height());

    fitToWindow();
}

void ImageGraphicsView::fitToWindow() {
    auto ratioW = static_cast<double>(image_.width()) / viewport()->width();
    auto ratioH = static_cast<double>(image_.height()) / viewport()->height();
    transform_.reset();
    if (ratioW > 1.0 || ratioH > 1.0) {
        // scale
        auto ratio = 1. / max(ratioW, ratioH);
        transform_.scale(ratio, ratio);

        qDebug() << tr("fit to window: scale = %1, transform = ").arg(ratio) << transform_;
    }
    qDebug() << "fit to window, transform = " << transform_;

    // horizontal center
    auto scale = transform_.m11();
    auto dx = (viewport()->width() / scale - image_.width()) / 2;
    transform_.translate(dx, 0);

    viewport()->update();
}

void ImageGraphicsView::zoomIn() {
    transform_.scale(1.1, 1.1);
    qDebug() << "zoom in, transform = " << transform_;
    viewport()->update();
}

void ImageGraphicsView::zoomOut() {
    transform_.scale(0.9, 0.9);
    qDebug() << "zoom out, transform = " << transform_;
    viewport()->update();
}

void ImageGraphicsView::move() {
    transform_.translate(100, 200);
    qDebug() << "move, transform = " << transform_;
    viewport()->update();
}

void ImageGraphicsView::paintEvent(QPaintEvent* event) {
    QGraphicsView::paintEvent(event);
    qDebug() << "paint event";

    QPainter painter(viewport());
    painter.setWorldTransform(transform_);
    painter.drawImage(QRect(0, 0, image_.width(), image_.height()), image_,
                      QRect(0, 0, image_.width(), image_.height()));
    painter.setWorldMatrixEnabled(false);

    // show current position and RGB
    if (currentPos_.x() >= 0 && currentPos_.y() >= 0 && currentPos_.x() < image_.width() &&
        currentPos_.y() < image_.height()) {
        auto color = image_.pixel(currentPos_);
        QString msg = tr("<font color='black'>(x=%1, y=%2) </font>").arg(currentPos_.x()).arg(currentPos_.y());
        if (image_.isGrayscale()) {
            msg += tr("<font color='gray'>L:%1</font>").arg(qRed(color));
        } else {
            msg += tr("<font color='red'>R:%1 </font><font color='green'>G:%2 </font><font color='blue'>B:%3</font>")
                       .arg(qRed(color))
                       .arg(qGreen(color))
                       .arg(qBlue(color));
        }
        emit newInfo(msg);
    }
}

void ImageGraphicsView::resizeEvent(QResizeEvent* event) {
    QGraphicsView::resizeEvent(event);
    fitToWindow();
}

void ImageGraphicsView::wheelEvent(QWheelEvent* event) {
    QGraphicsView::wheelEvent(event);

    auto preViewPos = event->position();
    auto preScenePos = transform_.inverted().map(preViewPos);
    if (event->angleDelta().y() > 0) {
        zoomIn();
    } else {
        zoomOut();
    }

    auto scenePos = transform_.inverted().map(preViewPos);
    auto delta = scenePos - preScenePos;
    transform_.translate(delta.x(), delta.y());
    qDebug() << "wheel event, transform = " << transform_;
}

void ImageGraphicsView::mousePressEvent(QMouseEvent* event) {
    QGraphicsView::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        isMousePressed_ = true;
        prePos_ = event->pos();
    }
}

void ImageGraphicsView::mouseMoveEvent(QMouseEvent* event) {
    QGraphicsView::mouseMoveEvent(event);

    if (isMousePressed_) {
        auto deltaP = (event->pos() - prePos_) / transform().m11();
        prePos_ = event->pos();
        transform_.translate(deltaP.x(), deltaP.y());
        qDebug() << "mouse move, transform = " << transform_;
    }

    currentPos_ = transform_.inverted().map(event->pos());
    viewport()->update();
}

void ImageGraphicsView::mouseReleaseEvent(QMouseEvent* event) {
    QGraphicsView::mouseReleaseEvent(event);
    isMousePressed_ = false;
}
