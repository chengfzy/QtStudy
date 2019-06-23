#pragma once
#include "QtNetwork"
#include "QtWidgets"

class BlockingClient : public QDialog {
    Q_OBJECT

  public:
    explicit BlockingClient(QWidget* parent = nullptr);

  private slots:
    void requestNewFortune();
    void readFortune();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetFortuneButton();

  private:
    QComboBox* hostCombo_ = nullptr;
    QLineEdit* portLineEdit_ = nullptr;
    QLabel* statusLabel_ = nullptr;
    QPushButton* getFortuneButton_ = nullptr;

    QTcpSocket* tcpSocket_ = nullptr;
    QDataStream in_;
    QString currentFortune_;
};
