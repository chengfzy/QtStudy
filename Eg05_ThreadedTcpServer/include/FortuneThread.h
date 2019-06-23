#pragma once
#include "QTcpSocket"
#include "QThread"

class FortuneThread : public QThread {
    Q_OBJECT
  public:
    FortuneThread(int socketDescriptor, const QString& fortune, QObject* parent);

  public:
    void run() override;

  signals:
    void error(QTcpSocket::SocketError socketError);

  private:
    int socketDescriptors_;
    QString text_;
};
