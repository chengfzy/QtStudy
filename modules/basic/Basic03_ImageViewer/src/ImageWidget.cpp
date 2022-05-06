#include "ImageWidget.h"

#include <QDebug>

ImageWidget::ImageWidget(QWidget* parent) : QWidget(parent) { setupUI(); }

void ImageWidget::setImage(const QImage& image) {
    image_ = image;
    imageItem_->setPixmap(QPixmap::fromImage(image));
}

void ImageWidget::setupUI() {
    auto toolBar = new QToolBar(this);
    auto zoomInAction = toolBar->addAction("Zoom In");
    connect(zoomInAction, &QAction::triggered, view_, &ImageGraphicsView::zoomIn);
    auto zoomOutAction = toolBar->addAction("Zoom Out");
    connect(zoomOutAction, &QAction::triggered, view_, &ImageGraphicsView::zoomOut);
    auto resetAction = toolBar->addAction("Reset");
    connect(resetAction, &QAction::triggered, this, [&]() { view_->fitInView(imageItem_, Qt::KeepAspectRatio); });

    // graphics scene
    auto graphicsScene = new QGraphicsScene;
    view_ = new ImageGraphicsView(graphicsScene, this);
    view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    imageItem_ = new QGraphicsPixmapItem;
    view_->scene()->addItem(imageItem_);
    view_->fitInView(imageItem_, Qt::KeepAspectRatio);

    // main layout
    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(view_);
    setLayout(mainLayout);

    setMinimumSize(600, 400);
}
