#pragma once
#include <QtNetwork>
#include <QtWidgets>

class Sender : public QDialog {
    Q_OBJECT

  public:
    explicit Sender(QWidget* parent = nullptr);

  private slots:
    void ttlChanged(int newTtl);
    void startSending();
    void sendDatagram();

  private:
    QLabel* statusLabel_ = nullptr;
    QPushButton* startButton_ = nullptr;
    QUdpSocket udpSocket4_;
    QUdpSocket udpSocket6_;
    QTimer timer_;
    QHostAddress groupAddress4_;
    QHostAddress groupAddress6_;
    int messgeNo_ = 1;
};
