#pragma once
#include <QStringList>
#include <QTcpServer>

class Server : public QTcpServer {
    Q_OBJECT

  public:
    explicit Server(QObject* parent = nullptr);

  protected:
    void incomingConnection(qintptr socketDescriptor) override;

  private:
    QStringList fortunes_;
};
