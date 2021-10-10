#include "Receiver.h"

Receiver::Receiver(QWidget* parent)
    : QDialog(parent),
      groupAddress4_(QStringLiteral("239.255.43.21")),
      groupAddress6_(QStringLiteral("ff12::2115"))

{
    statusLabel_ = new QLabel("Listening for multicast message on both IPv and IPv6");

    // quite button and layout
    auto quitButton = new QPushButton("&Quit");
    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    // main layout
    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel_);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    // window title
    setWindowTitle("Multicast Receiver");

    // UDP setting
    udpSocket4_.bind(QHostAddress::AnyIPv4, 45454, QUdpSocket::ShareAddress);
    udpSocket4_.joinMulticastGroup(groupAddress4_);
    if (!udpSocket6_.bind(QHostAddress::AnyIPv6, 45454, QUdpSocket::ShareAddress) ||
        !udpSocket6_.joinMulticastGroup(groupAddress6_)) {
        statusLabel_->setText("Listening for multicast message on IPv4 only");
    }

    // connection
    connect(&udpSocket4_, &QUdpSocket::readyRead, this, &Receiver::processPendingDatagrams);
    connect(&udpSocket6_, &QUdpSocket::readyRead, this, &Receiver::processPendingDatagrams);
    connect(quitButton, &QPushButton::clicked, this, &Receiver::close);
}

void Receiver::processPendingDatagrams() {
    QByteArray datagram;
    QString message;

    // using QUdpSocket::readDatagram since Qt4
    while (udpSocket4_.hasPendingDatagrams()) {
        datagram.resize(udpSocket4_.pendingDatagramSize());
        udpSocket4_.readDatagram(datagram.data(), datagram.size());
        message = tr("Received IPv4 datagram:\n\"%1\"").arg(datagram.constData());
    }

    // using QUdpSocket::receiveDatagram since Qt5
    while (udpSocket6_.hasPendingDatagrams()) {
        QNetworkDatagram networkDatagram = udpSocket6_.receiveDatagram();
        message += tr("\n\nReceived IPv6 datagram from [%2]:%3:\n\"%1\"")
                       .arg(datagram.constData())
                       .arg(networkDatagram.senderAddress().toString())
                       .arg(networkDatagram.senderPort());
    }

    statusLabel_->setText(message);
}