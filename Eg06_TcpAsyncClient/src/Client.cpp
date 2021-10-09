#include "Client.h"

BlockingClient::BlockingClient(QWidget* parent)
    : QDialog(parent),
      hostCombo_(new QComboBox),
      portLineEdit_(new QLineEdit),
      getFortuneButton_(new QPushButton("Get Fortune")),
      tcpSocket_(new QTcpSocket(this)) {
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    hostCombo_->setEditable(true);

    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        hostCombo_->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty()) {
            hostCombo_->addItem(name + QChar('.') + domain);
        }
    }

    // find out IP address of this machine
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (const auto& ip : ipAddressList) {
        if (!ip.isLoopback()) {
            hostCombo_->addItem(ip.toString());
        }
    }
    // add localhost addresses
    for (const auto& ip : ipAddressList) {
        if (ip.isLoopback()) {
            hostCombo_->addItem(ip.toString());
        }
    }

    // port
    portLineEdit_->setValidator(new QIntValidator(1, 65535, this));
    // host and port label
    auto hostLabel = new QLabel("&Server name:");
    hostLabel->setBuddy(hostCombo_);
    auto portLabel = new QLabel("S&ever port:");
    portLabel->setBuddy(portLineEdit_);
    // status label
    statusLabel_ = new QLabel("This examples requires that you run the TcpServer as well");
    getFortuneButton_->setDefault(true);
    getFortuneButton_->setEnabled(false);
    // quit button
    auto quitButton = new QPushButton("Quit");
    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(getFortuneButton_, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    // main layout
    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostCombo_, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit_, 1, 1);
    mainLayout->addWidget(statusLabel_, 2, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);

    // input data stream
    in_.setDevice(tcpSocket_);
    in_.setVersion(QDataStream::Qt_5_12);

    // connect
    connect(hostCombo_, &QComboBox::editTextChanged, this, &BlockingClient::enableGetFortuneButton);
    connect(portLineEdit_, &QLineEdit::textChanged, this, &BlockingClient::enableGetFortuneButton);
    connect(getFortuneButton_, &QAbstractButton::clicked, this, &BlockingClient::requestNewFortune);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(tcpSocket_, &QIODevice::readyRead, this, &BlockingClient::readFortune);
    connect(tcpSocket_, &QAbstractSocket::errorOccurred, this, &BlockingClient::displayError);
}

void BlockingClient::requestNewFortune() {
    getFortuneButton_->setEnabled(false);
    // because we allow the user to click Get Fortune before the previous connection finished closing, we start off by
    // aborting the previous connection by calling QTcpSocket::abort(). On an unconnected socket, this function does
    // nothing
    tcpSocket_->abort();
    tcpSocket_->connectToHost(hostCombo_->currentText(), portLineEdit_->text().toInt());
}

void BlockingClient::readFortune() {
    in_.startTransaction();

    QString nextFortune;
    in_ >> nextFortune;

    if (!in_.commitTransaction()) {
        return;
    }

    if (nextFortune == currentFortune_) {
        QTimer::singleShot(0, this, &BlockingClient::requestNewFortune);
        return;
    }

    currentFortune_ = nextFortune;
    statusLabel_->setText(currentFortune_);
    getFortuneButton_->setEnabled(true);
}

void BlockingClient::displayError(QAbstractSocket::SocketError socketError) {
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, "TCP Client",
                                     "The host was not found, please check the host name and port settings");
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, "TCP Client",
                                     "The connection was refused by the peer, make sure the fortune server is running, "
                                     "and check that the host name and port settings are connect");
            break;
        default:
            QMessageBox::information(this, "TCP Client",
                                     tr("The following error occurred: %1").arg(tcpSocket_->errorString()));
    }

    getFortuneButton_->setEnabled(true);
}

void BlockingClient::enableGetFortuneButton() {
    getFortuneButton_->setEnabled(!hostCombo_->currentText().isEmpty() && !portLineEdit_->text().isEmpty());
}