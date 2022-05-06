#include "ImageGraphicsView.h"
#include <QWheelEvent>

#include <QDebug>

void ImageGraphicsView::zoomIn() { scale(1.1, 1.1); }

void ImageGraphicsView::zoomOut() { scale(0.9, 0.9); }

void ImageGraphicsView::wheelEvent(QWheelEvent* event) {
    auto preViewPos = event->position().toPoint();
    auto preScenePos = mapToScene(preViewPos);
    if (event->angleDelta().y() > 0) {
        zoomIn();
    } else {
        zoomOut();
    }
    // remove scroll bar
    setSceneRect(mapToScene(rect()).boundingRect());

    auto scenePos = mapToScene(preViewPos);
    auto deltaP = scenePos - preScenePos;
    qDebug() << QString("deltaP = (%1, %2)").arg(deltaP.x()).arg(deltaP.y());
    setSceneRect(sceneRect().x() - deltaP.x(), sceneRect().y() - deltaP.y(), sceneRect().width(), sceneRect().height());
}

void ImageGraphicsView::mousePressEvent(QMouseEvent* event) { prePos_ = event->pos(); }

void ImageGraphicsView::mouseMoveEvent(QMouseEvent* event) {
    auto deltaP = event->pos() - prePos_;
    prePos_ = event->pos();
    setSceneRect(sceneRect().x() - deltaP.x(), sceneRect().y() - deltaP.y(), sceneRect().width(), sceneRect().height());
}