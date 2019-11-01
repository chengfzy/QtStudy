#pragma once
#include <QtNetwork>
#include <QtWidgets>
#include "ImageViewer.h"

/**
 * @brief Image Processor. Receive image from Image Capture, then process image and send to result Image Capture
 */
class Processor : public QWidget {
    Q_OBJECT

  public:
    explicit Processor(QWidget* parent = nullptr);

  signals:
    // signals if got new image
    void newImageGot(const QImage& image);

  private:
    // setup UI
    void setupUI();

    // control(start/stop) capture
    void controlCapture();

    // read image datagram
    void readImageDatagram();

    // process image, show in viewer, process it and send result to receiver
    void processImage(const QImage& image);

  private:
    ImageViewer* imageViewer_ = nullptr;    // image viewer
    QPushButton* controlButton_ = nullptr;  // control button, start/stop capture
    QLabel* resultLabel_ = nullptr;         // result label

    // QUdpSocket* receiver_ = nullptr;                      // receiver, receive image from Capture
    QUdpSocket* sender_ = nullptr;                        // sender, send process result to Capture
    quint16 receivePort_ = 45454;                         // receive port
    QHostAddress sendAddress_ = QHostAddress::Broadcast;  // sender address, broadcast mode
    quint16 sendPort_ = 45455;                            // sender port
    bool stop_ = false;                                   // stop flag

    int imageNo_ = 1;  // image number
};
