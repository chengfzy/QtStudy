#pragma once
#include "QtNetwork"
#include "QtWidgets"

class Client : public QDialog {
  public:
    explicit Client(QWidget* parent = nullptr);

  private slots:
    void requestNewFortune();
    void readFortune();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetFortuneButton();
    void sessionOpened();

  private:
    QComboBox* hostCombo_ = nullptr;
    QLineEdit* portLineEdit_ = nullptr;
    QLabel* statusLabel_ = nullptr;
    QPushButton* getFortuneButton_ = nullptr;

    QTcpSocket* tcpSocket_ = nullptr;
    QDataStream in_;
    QString currentFortune_;
    QNetworkSession* networkSession_ = nullptr;
};
