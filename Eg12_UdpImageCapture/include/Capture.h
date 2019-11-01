#pragma once
#include <QtNetwork>
#include <QtWidgets>
#include "ImageViewer.h"

/**
 * @brief Image Capture. Capture image and send to Image Processor, and also receive the processed result
 */
class Capture : public QWidget {
    Q_OBJECT

  public:
    explicit Capture(QWidget* parent = nullptr);

  signals:
    // signals if got new image
    void newImageGot(const QImage& image);

  private:
    // setup UI
    void setupUI();

    // control(start/stop) capture
    void controlCapture();

    // capture image
    void capture();

    // process image, show image in viewer and send to Processor
    void processImage(const QImage& image);

    // process pending result datagram
    void processPendingResultDatagram();

  private:
    ImageViewer* imageViewer_ = nullptr;    // image viewer
    QPushButton* controlButton_ = nullptr;  // control button, start/stop capture
    QLabel* resultLabel_ = nullptr;         // result label

    QUdpSocket* sender_ = nullptr;                        // sender, send image to Processor
    QUdpSocket* receiver_ = nullptr;                      // receiver, receive result from Processor
    QHostAddress sendAddress_ = QHostAddress::Broadcast;  // sender address, broadcast mode
    quint16 sendPort_ = 45454;                            // sender port
    quint16 receivePort_ = 45455;                         // receive port

    QTimer timer_;     // timer
    int imageNo_ = 1;  // image number
};
