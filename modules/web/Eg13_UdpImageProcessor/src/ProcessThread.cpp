#include "ProcessThread.h"
#include <fmt/format.h>
#include <iostream>

using namespace std;
using namespace fmt;

ProcessThread::ProcessThread(QObject* parent) : QThread(parent) {}

ProcessThread::~ProcessThread() {
    stop();
    wait();
}

// stop thread
void ProcessThread::stop() {
    QMutexLocker locker(&mutex_);
    stop_ = true;
}

// Override run operation in QThread
void ProcessThread::run() {
    // set stop flag
    {
        QMutexLocker locker(&mutex_);
        stop_ = false;
    }

    // create receiver and sender
    static const int kTimeout{5 * 1000};  // time out: 10s
    QUdpSocket receiver;
    receiver.bind(receivePort_, QUdpSocket::ShareAddress);
    cout << format("Receiver address: {}:{}", receiver.localAddress().toString().toStdString(), receiver.localPort())
         << endl;
    QUdpSocket sender;

    // define some variable
    quint32 imageSize{0};
    QByteArray imageBytes;

    while (!stop_) {
        if (!receiver.waitForReadyRead(kTimeout)) {
            cout << format("Wait Error: {}: {}", receiver.error(), receiver.errorString().toStdString()) << endl;
            emit error(receiver.error(), receiver.errorString());
            emit finished();
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
        if (header.startsWith("ImageHeader")) {
            dataStream >> imageSize;
            imageBytes = bytes.mid(dataStream.device()->pos());
            cout << format("got header, image size = {}", imageSize) << endl;
        } else {
            imageBytes.append(bytes);
            // cout << format("\timage size = {}", imageBytes.size()) << endl;

            // check image is received complete
            if (imageSize != 0 && imageBytes.size() >= imageSize) {
                // decoding
                QImage image;
                image.loadFromData(imageBytes);

                // emit image signals
                emit newImage(image);
                ++imageNo_;

                // emit result info, to simplify the process procedure, here I just calculate the image size and send it
                // to Capture
                QString result = tr("[%1] Image Size = %2 x %3, %4 Mb")
                                     .arg(imageNo_)
                                     .arg(image.width())
                                     .arg(image.height())
                                     .arg(image.sizeInBytes() / 1024. / 1024.);
                emit newResultInfo(result);

                // send result using broad cast
                QByteArray resultByte = result.toUtf8();
                if (-1 == sender.writeDatagram(resultByte.data(), resultByte.size(), sendAddress_, sendPort_)) {
                    cout << format("Write Error: {}: {}", receiver.error(), receiver.errorString().toStdString())
                         << endl;
                    emit error(receiver.error(), receiver.errorString());
                }
            }
        }
    }

    // send empty image after finish
    emit newImage(QImage());
}
