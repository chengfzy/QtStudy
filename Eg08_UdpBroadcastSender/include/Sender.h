#pragma once
#include <QtNetwork>
#include <QtWidgets>

class Sender : public QWidget {
    Q_OBJECT

  public:
    explicit Sender(QWidget* parent = nullptr);

  private slots:
    void startBroadcasting();
    void broadcastDatagram();

  private:
    QLabel* statusLabel_ = nullptr;
    QPushButton* startButton_ = nullptr;
    QUdpSocket* udpSocket_ = nullptr;
    QTimer timer_;
    int messageNo_ = 1;
};
