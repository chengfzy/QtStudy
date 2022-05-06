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
    auto delta = scenePos - preScenePos;
    setSceneRect(sceneRect().x() - delta.x(), sceneRect().y() - delta.y(), sceneRect().width(), sceneRect().height());
}

void ImageGraphicsView::mousePressEvent(QMouseEvent* event) {
    QGraphicsView::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        isMousePressed_ = true;
        centerPos_ = mapToScene(event->pos()) - event->pos() + QPointF(width() / 2, height() / 2);
        prePos_ = event->pos();
    }
}

void ImageGraphicsView::mouseMoveEvent(QMouseEvent* event) {
    QGraphicsView::mouseMoveEvent(event);

    if (isMousePressed_) {
        // prePos_ = event->pos();
        auto deltaP = (event->pos() - prePos_) / transform().m11();
        prePos_ = event->pos();
        setSceneRect(sceneRect().x() - deltaP.x(), sceneRect().y() - deltaP.y(), sceneRect().width(),
                     sceneRect().height());

#if 0
        auto delta = mapToScene(event->pos()) - mapToScene(prePos_);
        qDebug() << tr("delta = (%1, %2), m11 = %3").arg(delta.x()).arg(delta.y()).arg(transform().m11());
        delta *= transform().m11();
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        centerOn(mapToScene(
            QPoint(viewport()->rect().width() / 2 - delta.x(), viewport()->rect().height() / 2 - delta.y())));
        setTransformationAnchor(QGraphicsView::AnchorViewCenter);
        // setSceneRect(sceneRect().x() - deltaP.x(), sceneRect().y() - deltaP.y(), sceneRect().width(),
        // sceneRect().height());

        prePos_ = event->pos();
#endif

#if 0
        // auto delta = event->pos() - prePos_;
        // qDebug() << tr("delta = (%1, %2), m11 = %3").arg(delta.x()).arg(delta.y()).arg(transform().m11());
        // setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        // centerOn(centerPos_ - delta);
        // setTransformationAnchor(QGraphicsView::AnchorViewCenter);
#endif

#if 0
        auto delta = mapToScene(event->pos()) - mapToScene(prePos_);
        qDebug() << tr("delta = (%1, %2), m11 = %3").arg(delta.x()).arg(delta.y()).arg(transform().m11());
        // delta *= transform().m11();
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        // centerOn(mapToScene(
        //     QPoint(viewport()->rect().width() / 2 - delta.x(), viewport()->rect().height() / 2 - delta.y())));
        centerOn(-delta.x(), -delta.y());
        setTransformationAnchor(QGraphicsView::AnchorViewCenter);
        // setSceneRect(sceneRect().x() - deltaP.x(), sceneRect().y() - deltaP.y(), sceneRect().width(),
        // sceneRect().height());
#endif
    }
}

void ImageGraphicsView::mouseReleaseEvent(QMouseEvent* event) {
    QGraphicsView::mouseReleaseEvent(event);
    isMousePressed_ = false;
}