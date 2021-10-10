#pragma once
#include <QtNetwork>
#include <QtWidgets>

class Receiver : public QDialog {
    Q_OBJECT
  public:
    explicit Receiver(QWidget* parent = nullptr);

  private slots:
    void processPendingDatagrams();

  private:
    QLabel* statusLabel_ = nullptr;
    QUdpSocket udpSocket4_;
    QUdpSocket udpSocket6_;
    QHostAddress groupAddress4_;
    QHostAddress groupAddress6_;
};
