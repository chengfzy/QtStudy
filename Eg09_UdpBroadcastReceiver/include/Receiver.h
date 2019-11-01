#pragma once
#include <QtNetwork>
#include <QtWidgets>

class Receiver : public QWidget {
    Q_OBJECT

  public:
    explicit Receiver(QWidget* parent = nullptr);

  private slots:
    void processPendingDatagrams();

  private:
    QLabel* statusLabel_ = nullptr;
    QUdpSocket* udpSocket_ = nullptr;
};
