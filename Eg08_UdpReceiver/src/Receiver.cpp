#include "Receiver.h"

Receiver::Receiver(QWidget* parent) : QWidget(parent) {
    // status label
    statusLabel_ = new QLabel("Listening for broadcasted messages");
    statusLabel_->setWordWrap(true);
    // quit button
    auto quitButton = new QPushButton("Quit");
    // button layout
    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    // main layout
    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel_);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    // title
    setWindowTitle("Broadcast Receiver");

    // UDP socket
    udpSocket_ = new QUdpSocket(this);
    udpSocket_->bind(45454, QUdpSocket::ShareAddress);
    // connection
    connect(udpSocket_, &QUdpSocket::readyRead, this, &Receiver::processPendingDatagrams);
    connect(quitButton, &QPushButton::clicked, this, &Receiver::close);
}

void Receiver::processPendingDatagrams() {
    QByteArray datagram;
    while (udpSocket_->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket_->pendingDatagramSize()));
        udpSocket_->readDatagram(datagram.data(), datagram.size());
        statusLabel_->setText(tr("Received datagram: \"%1\"").arg(datagram.constData()));
    }
}