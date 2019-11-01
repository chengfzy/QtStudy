#include "Processor.h"
#include <fmt/format.h>
#include <iostream>

using namespace std;
using namespace fmt;

Processor::Processor(QWidget* parent) : QWidget(parent) {
    setupUI();

    // sender
    sender_ = new QUdpSocket(this);

    // other connection
    connect(this, &Processor::newImageGot, this, &Processor::processImage);
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
    setMinimumSize(300, 200);
}

// control(start/stop) capture
void Processor::controlCapture() {
    if (controlButton_->text() == "Start") {
        controlButton_->setText("Stop");
        readImageDatagram();
    } else {
        controlButton_->setText("Start");
        stop_ = true;
    }
}

// process pending result datagram
void Processor::readImageDatagram() {
    // set stop flag
    stop_ = false;

    // connect
    static const int kTimeout{5 * 1000};  // time out: 3s
    QUdpSocket receiver;
    receiver.bind(receivePort_, QUdpSocket::ShareAddress);
    cout << format("Receiver address: {}:{}", receiver.localAddress().toString().toStdString(), receiver.localPort())
         << endl;

    // define some variable
    quint32 imageSize{0};
    QByteArray imageBytes;

    while (!stop_) {
        if (!receiver.waitForReadyRead(kTimeout)) {
            cout << format("Error: {}: {}", receiver.error(), receiver.errorString().toStdString()) << endl;
            break;
        }

        QByteArray bytes;
        bytes.resize(receiver.pendingDatagramSize());
        receiver.readDatagram(bytes.data(), bytes.size());
        QDataStream dataStream(&bytes, QIODevice::ReadOnly);
        dataStream.setVersion(QDataStream::Qt_5_12);
        QString header;
        dataStream >> header;
        // check is a header
        if (header.startsWith("START")) {
            dataStream >> imageSize;
            imageBytes = bytes.mid(dataStream.device()->pos());
            cout << format("got header, image size = {}", imageSize) << endl;
        } else {
            imageBytes.append(bytes);

            // check image is received complete
            if (imageSize != 0 && imageBytes.size() >= imageSize) {
                // uncompress
                QByteArray decryptedByte = QByteArray::fromBase64(imageBytes.data());
                QByteArray uncompressByte = qUncompress(decryptedByte);
                QImage image;
                image.loadFromData(uncompressByte);
                emit newImageGot(image);
                ++imageNo_;
                break;
            }
        }
    }
}

// process image, show in viewer, process it and send result to receiver
void Processor::processImage(const QImage& image) {
    // show in viewer
    imageViewer_->setImage(QPixmap::fromImage(image));

    // to simplify the process procedure, here I just calculate the image size and send it to Capture
    QString result = tr("[%1] Image Size = %2 x %3, %4 Mb")
                         .arg(imageNo_)
                         .arg(image.width())
                         .arg(image.height())
                         .arg(image.sizeInBytes() / 1024. / 1024.);
    resultLabel_->setText(result);

    // send result to Capture
    QByteArray resultByte = result.toUtf8();
    // send using broad cast
    sender_->writeDatagram(resultByte.data(), resultByte.size(), sendAddress_, sendPort_);
}