#include "Sender.h"

Sender::Sender(QWidget* parent) : QWidget(parent) {
    // status label
    statusLabel_ = new QLabel("Ready to broadcast datagrams on port 45454");
    statusLabel_->setWordWrap(true);
    // start button, quit button
    startButton_ = new QPushButton("&Start");
    auto quitButton = new QPushButton("Quit");
    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton_, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    // main layout
    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel_);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    // title
    setWindowTitle("Broadcast Sender");

    // UDP socket
    udpSocket_ = new QUdpSocket(this);
    // connection
    connect(startButton_, &QPushButton::clicked, this, &Sender::startBroadcasting);
    connect(quitButton, &QPushButton::clicked, this, &Sender::close);
    connect(&timer_, &QTimer::timeout, this, &Sender::broadcastDatagram);
}

void Sender::startBroadcasting() {
    startButton_->setEnabled(false);
    timer_.start(1000);
}

void Sender::broadcastDatagram() {
    statusLabel_->setText(tr("Now broadcasting datagram %1").arg(messageNo_));
    QByteArray datagram = "Broadcast message " + QByteArray::number(messageNo_);
    udpSocket_->writeDatagram(datagram, QHostAddress::Broadcast, 45454);
    ++messageNo_;
}