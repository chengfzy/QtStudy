#pragma once
#include "QtNetwork"
#include "QtWidgets"

class Server : public QDialog {
  public:
    explicit Server(QWidget* parent = nullptr);

  private slots:
    void sessionOpened();
    void sendFortune();

  private:
    QLabel* statusLabel_ = nullptr;
    QTcpServer* tcpServer_ = nullptr;
    QList<QString> fortunes_;
};
