#include "Capture.h"
#include <fmt/format.h>
#include <iostream>

using namespace std;
using namespace fmt;

Capture::Capture(QWidget* parent) : QWidget(parent) {
    setupUI();

    // sender
    sender_ = new QUdpSocket(this);

    // receiver
    receiver_ = new QUdpSocket(this);
    receiver_->bind(receivePort_, QUdpSocket::ShareAddress);
    connect(receiver_, &QUdpSocket::readyRead, this, &Capture::processPendingResultDatagram);

    // timer
    connect(&timer_, &QTimer::timeout, this, &Capture::capture);

    // other connection
    connect(this, &Capture::newImageGot, this, &Capture::processImage);
}

// setup UI
void Capture::setupUI() {
    // image viewer
    imageViewer_ = new ImageViewer(this);

    // start/stop button
    controlButton_ = new QPushButton("Start");
    connect(controlButton_, &QPushButton::clicked, this, &Capture::controlCapture);

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
    setWindowTitle("Image Capture");
    setMinimumSize(300, 200);
}

// control(start/stop) capture
void Capture::controlCapture() {
    if (controlButton_->text() == "Start") {
        controlButton_->setText("Stop");
        timer_.start(1000);
    } else {
        timer_.stop();
        controlButton_->setText("Start");
    }
}

// capture image
void Capture::capture() {
    // to simplify the capture process, I just read an image from folder and draw some text to distinguish it
    QImage image("../../data/img.png");
    QPainter painter(&image);
    QFont font = painter.font();
    font.setPointSize(20);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QPoint(50, 50), tr("Image #%1").arg(imageNo_));
    painter.end();
    ++imageNo_;

    // emit signal
    emit newImageGot(image);
}

// process image, show image in viewer and send to Processor
void Capture::processImage(const QImage& image) {
    // show in viewer
    imageViewer_->setImage(QPixmap::fromImage(image));

    // compress to JPG
    QByteArray imageByte;
    QBuffer imageBuffer(&imageByte);
    image.save(&imageBuffer, "JPG", 75);
    // compress and to base64
    QByteArray base64Byte = qCompress(imageByte).toBase64();

    // add header to send bytes
    QString header = tr("START %1").arg(quint32(base64Byte.size()));
    QByteArray sendBytes;
    QDataStream dataStream(&sendBytes, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_12);
    dataStream << header;
    dataStream << base64Byte;
    cout << format("Send Image [{}]: image size  = {}, send size = {}", imageNo_, base64Byte.size(), sendBytes.size())
         << endl;

    // split it and send using broad cast
    static const int kMaxLen{1024 * 10};  // max size = 10k
    for (int i = 0; i < sendBytes.size(); i += kMaxLen) {
        QByteArray bytes = sendBytes.mid(i, kMaxLen);
        if (-1 == sender_->writeDatagram(bytes.data(), bytes.size(), QHostAddress::Broadcast, sendPort_)) {
            cout << format("Error: {}: {}", sender_->error(), sender_->errorString().toStdString()) << endl;
        }
    }
}

// process pending result datagram
void Capture::processPendingResultDatagram() {
    QByteArray datagram;
    datagram.resize(receiver_->pendingDatagramSize());
    receiver_->readDatagram(datagram.data(), datagram.size());

    // show in label
    resultLabel_->setText(datagram);
}