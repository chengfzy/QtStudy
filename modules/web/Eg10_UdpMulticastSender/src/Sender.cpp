#include "Sender.h"

Sender::Sender(QWidget* parent)
    : QDialog(parent), groupAddress4_(QStringLiteral("239.255.43.21")), groupAddress6_(QStringLiteral("ff12::2115")) {
    // force binding to their respective families
    udpSocket4_.bind(QHostAddress(QHostAddress::AnyIPv4), 0);
    udpSocket6_.bind(QHostAddress(QHostAddress::AnyIPv6), udpSocket4_.localPort());

    // status label and message
    QString msg = tr("Ready to multicast datagrams to group %1 and [%2] on port 45454").arg(groupAddress4_.toString());
    if (udpSocket6_.state() != QAbstractSocket::BoundState) {
        msg = tr("IPv6 failed. Ready to multicast datagrams to group %1 on port 45454").arg(groupAddress4_.toString());
    } else {
        msg = msg.arg(groupAddress6_.toString());
    }
    statusLabel_ = new QLabel(msg);

    // ttl label and layout
    auto ttlLabel = new QLabel("TTL for IPv4 multicast datagrams");
    auto ttlSpinBox = new QSpinBox;
    ttlSpinBox->setRange(0, 255);
    ttlSpinBox->setValue(1);
    auto ttlLayout = new QHBoxLayout;
    ttlLayout->addWidget(ttlLabel);
    ttlLayout->addWidget(ttlSpinBox);

    // start and quit button
    startButton_ = new QPushButton("&Start");
    auto quitButton = new QPushButton("&Quit");
    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton_, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    // main layout
    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel_);
    mainLayout->addLayout(ttlLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    // title
    setWindowTitle("Multicast Sender");

    // connection
    connect(ttlSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &Sender::ttlChanged);
    connect(startButton_, &QPushButton::clicked, this, &Sender::startSending);
    connect(quitButton, &QPushButton::clicked, this, &Sender::close);
    connect(&timer_, &QTimer::timeout, this, &Sender::sendDatagram);
}

void Sender::ttlChanged(int newTtl) {
    // we only set the TTL on the IPv4 socket, as that changes the multicast scope
    udpSocket4_.setSocketOption(QAbstractSocket ::MulticastTtlOption, newTtl);
}

void Sender::startSending() {
    startButton_->setEnabled(false);
    timer_.start(1000);
}

void Sender::sendDatagram() {
    statusLabel_->setText(tr("Now sending datagram %1").arg(messgeNo_));
    QByteArray datagram = "Multicast message " + QByteArray::number(messgeNo_);
    udpSocket4_.writeDatagram(datagram, groupAddress4_, 45454);
    if (udpSocket6_.state() == QAbstractSocket::BoundState) {
        udpSocket6_.writeDatagram(datagram, groupAddress6_, 45454);
    }

    ++messgeNo_;
}