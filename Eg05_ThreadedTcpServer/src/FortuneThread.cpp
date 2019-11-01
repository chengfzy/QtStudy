#include "FortuneThread.h"
#include <QtNetwork>

FortuneThread::FortuneThread(int socketDescriptor, const QString& fortune, QObject* parent)
    : QThread(parent), socketDescriptors_(socketDescriptor), text_(fortune) {}

void FortuneThread::run() {
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptors_)) {
        emit error(tcpSocket.error());
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << text_;

    tcpSocket.write(block);
    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
}