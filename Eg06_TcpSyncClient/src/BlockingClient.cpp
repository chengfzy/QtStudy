#include "BlockingClient.h"

BlockingClient::BlockingClient(QWidget* parent) : QWidget(parent) {
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // find out which IP to connect to
    QString ipAddress;
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
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

    // host and port
    hostLineEdit_ = new QLineEdit(ipAddress);
    portLineEdit_ = new QLineEdit;
    portLineEdit_->setValidator(new QIntValidator(1, 65535, this));
    hostLabel_ = new QLabel("&Server name:");
    hostLabel_->setBuddy(hostLineEdit_);
    portLabel_ = new QLabel("S&ever port:");
    portLabel_->setBuddy(portLineEdit_);
    // status label
    statusLabel_ = new QLabel("This examples requires that you run the TcpServer as well");
    statusLabel_->setWordWrap(true);
    // get fortune buton
    getFortuneButton_ = new QPushButton("Get Fortune");
    getFortuneButton_->setDefault(true);
    getFortuneButton_->setEnabled(false);
    // quit button
    quitButton_ = new QPushButton("Quit");
    buttonBox_ = new QDialogButtonBox;
    buttonBox_->addButton(getFortuneButton_, QDialogButtonBox::ActionRole);
    buttonBox_->addButton(quitButton_, QDialogButtonBox::RejectRole);

    // main layout
    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->addWidget(hostLabel_, 0, 0);
    mainLayout->addWidget(hostLineEdit_, 0, 1);
    mainLayout->addWidget(portLabel_, 1, 0);
    mainLayout->addWidget(portLineEdit_, 1, 1);
    mainLayout->addWidget(statusLabel_, 2, 0, 1, 2);
    mainLayout->addWidget(buttonBox_, 3, 0, 1, 2);

    portLineEdit_->setFocus();

    // connect
    connect(hostLineEdit_, &QLineEdit::textChanged, this, &BlockingClient::enableGetFortuneButton);
    connect(portLineEdit_, &QLineEdit::textChanged, this, &BlockingClient::enableGetFortuneButton);
    connect(getFortuneButton_, &QAbstractButton::clicked, this, &BlockingClient::requestNewFortune);
    connect(quitButton_, &QAbstractButton::clicked, this, &QWidget::close);
    connect(&thread_, &FortuneThread::newFortune, this, &BlockingClient::showFortune);
    connect(&thread_, &FortuneThread::error, this, &BlockingClient::displayError);
}

void BlockingClient::requestNewFortune() {
    getFortuneButton_->setEnabled(false);
    thread_.requestNewFortune(hostLineEdit_->text(), static_cast<quint16>(portLineEdit_->text().toInt()));
}

void BlockingClient::showFortune(const QString& nextFortune) {
    if (nextFortune == currentFortune_) {
        requestNewFortune();
        return;
    }

    currentFortune_ = nextFortune;
    statusLabel_->setText(currentFortune_);
    getFortuneButton_->setEnabled(true);
}

void BlockingClient::displayError(int socketError, const QString& message) {
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, "TCP BlockingClient",
                                     "The host was not found, please check the host name and port settings");
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, "TCP BlockingClient",
                                     "The connection was refused by the peer, make sure the fortune server is running, "
                                     "and check that the host name and port settings are connect");
            break;
        default:
            QMessageBox::information(this, "TCP BlockingClient", tr("The following error occurred: %1").arg(message));
    }

    getFortuneButton_->setEnabled(true);
}

void BlockingClient::enableGetFortuneButton() {
    getFortuneButton_->setEnabled(!hostLineEdit_->text().isEmpty() && !portLineEdit_->text().isEmpty());
}