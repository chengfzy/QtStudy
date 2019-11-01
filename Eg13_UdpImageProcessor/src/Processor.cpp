#include "Processor.h"
#include <iostream>

using namespace std;

Processor::Processor(QWidget* parent) : QWidget(parent) {
    setupUI();

    // process thread
    processThread_ = new ProcessThread(this);
    connect(processThread_, &ProcessThread::newImage, this,
            [&](const QImage& image) { imageViewer_->setImage(QPixmap::fromImage(image)); });
    connect(processThread_, &ProcessThread::newResultInfo, this,
            [&](const QString& info) { resultLabel_->setText(info); });
    connect(processThread_, &ProcessThread::error, this, [&](int socketError, const QString& message) {
        QMessageBox::warning(this, "Error", tr("Error: %1, %2").arg(socketError).arg(message));
    });
    connect(processThread_, &ProcessThread::finished, controlButton_, &QPushButton::click);
}

// setup UI
void Processor::setupUI() {
    // image viewer
    imageViewer_ = new ImageViewer(this);

    // start/stop button
    controlButton_ = new QPushButton("Start");
    connect(controlButton_, &QPushButton::clicked, this, &Processor::controlCapture);

    // result label
    resultLabel_ = new QLabel;

    // layout
    auto mainLayout = new QGridLayout;
    mainLayout->addWidget(imageViewer_, 0, 0, 1, 2);
    mainLayout->addWidget(controlButton_, 1, 0);
    mainLayout->addWidget(resultLabel_, 1, 1);
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 3);
    setLayout(mainLayout);

    // title and size
    setWindowTitle("Image Processor");
    setMinimumSize(400, 300);
}

// control(start/stop) capture
void Processor::controlCapture() {
    if (controlButton_->text() == "Start") {
        processThread_->start();
        controlButton_->setText("Stop");
    } else {
        processThread_->stop();
        controlButton_->setText("Start");
    }
}
