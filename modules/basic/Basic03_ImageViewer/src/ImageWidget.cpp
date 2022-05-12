#include "ImageWidget.h"

#include <QDebug>

ImageWidget::ImageWidget(QWidget* parent) : QWidget(parent) { setupUI(); }

void ImageWidget::setImage(const QImage& image) { view_->setImage(image); }

void ImageWidget::setupUI() {
    // graphics view
    auto graphicsScene = new QGraphicsScene;
    view_ = new ImageGraphicsView(graphicsScene, this);
    view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto toolBar = new QToolBar(this);
    auto zoomInAction = toolBar->addAction("Zoom In");
    connect(zoomInAction, &QAction::triggered, view_, &ImageGraphicsView::zoomIn);
    auto zoomOutAction = toolBar->addAction("Zoom Out");
    connect(zoomOutAction, &QAction::triggered, view_, &ImageGraphicsView::zoomOut);
    auto resetAction = toolBar->addAction("Reset");
    connect(resetAction, &QAction::triggered, view_, &ImageGraphicsView::fitToWindow);
    auto moveAction = toolBar->addAction("Move");
    connect(moveAction, &QAction::triggered, view_, &ImageGraphicsView::move);

    // main layout
    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(view_);
    setLayout(mainLayout);

    // connection
    connect(view_, &ImageGraphicsView::newInfo, this, [&](const QString& msg) { emit newInfo(msg); });
}
