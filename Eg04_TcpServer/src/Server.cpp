#include "Server.h"

Server::Server(QWidget* parent) : QDialog(parent), statusLabel_(new QLabel()) {
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    statusLabel_->setTextInteractionFlags(Qt::TextBrowserInteraction);

    sessionOpened();

    fortunes_ = {"You've been leading a dog's life. Stay off the furniture.",
                 "You've got to think about tomorrow.",
                 "You will be surprised by a loud noise",
                 "You will feel hungry again in another hour.",
                 "You might have mail.",
                 "You cannot kill time without injuring eternity.",
                 "Computer are not intelligent. They only think they are."};

    auto quitButton = new QPushButton("Quit");
    quitButton->setAutoDefault(false);
    connect(quitButton, &QPushButton::clicked, this, &QWidget::close);
    connect(tcpServer_, &QTcpServer::newConnection, this, &Server::sendFortune);

    auto buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(statusLabel_);
    mainLayout->addLayout(buttonLayout);

    setWindowTitle(QGuiApplication::applicationDisplayName());
}

void Server::sessionOpened() {
    tcpServer_ = new QTcpServer(this);
    if (!tcpServer_->listen()) {
        QMessageBox::critical(this, "Fortune Server",
                              tr("Unable to start the server: %1.").arg(tcpServer_->errorString()));
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (const auto& ip : ipAddressList) {
        if (ip != QHostAddress::LocalHost && ip.toIPv4Address()) {
            ipAddress = ip.toString();
            break;
        }
    }
    // if we didn't find one, use IPv4 localhost
    if (ipAddress.isEmpty()) {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }
    statusLabel_->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\nRun the fortune BlockingClient example now.")
                              .arg(ipAddress)
                              .arg(tcpServer_->serverPort()));
}

void Server::sendFortune() {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    out << fortunes_[QRandomGenerator::global()->bounded(fortunes_.size())];

    QTcpSocket* clientConnection = tcpServer_->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);
    clientConnection->write(block);
    clientConnection->disconnectFromHost();
}
