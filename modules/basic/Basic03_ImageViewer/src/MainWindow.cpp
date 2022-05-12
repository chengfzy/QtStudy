#include "MainWindow.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) { setupUI(); }

void MainWindow::setImage(const QImage& image) { imageWidget_->setImage(image); }

void MainWindow::setupUI() {
    imageWidget_ = new ImageWidget(this);
    setCentralWidget(imageWidget_);

    // status bar
    infoLabel_ = new QLabel;
    infoLabel_->setAlignment(Qt::AlignHCenter);
    statusBar()->addWidget(infoLabel_);

    connect(imageWidget_, &ImageWidget::newInfo, this, [&](const QString& msg) { infoLabel_->setText(msg); });
}