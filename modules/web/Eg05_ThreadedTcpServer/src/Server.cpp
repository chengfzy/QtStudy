#include "Server.h"
#include "FortuneThread.h"
#include <QRandomGenerator>

Server::Server(QObject* parent) : QTcpServer(parent) {
    fortunes_ << tr("You've been leading a dog's life. Stay off the furniture.")
              << tr("You've got to think about tomorrow.") << tr("You will be surprised by a loud noise")
              << tr("You will feel hungry again in another hour.") << tr("You might have mail.")
              << tr("You cannot kill time without injuring eternity.")
              << tr("Computer are not intelligent. They only think they are.");
}

void Server::incomingConnection(qintptr socketDescriptor) {
    QString fortune = fortunes_.at(QRandomGenerator::global()->bounded(fortunes_.size()));
    FortuneThread* thread = new FortuneThread(socketDescriptor, fortune, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}