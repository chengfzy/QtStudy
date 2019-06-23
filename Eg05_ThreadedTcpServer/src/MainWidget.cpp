#include "MainWidget.h"
#include "QtNetwork"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
    // start TCP listen
    if (!server_.listen()) {
        QMessageBox::critical(this, tr("Threaded Server"),
                              tr("Unable to start the server: %1").arg(server_.errorString()));
        close();
        return;
    }

    // find IP address
    QString ipAddress;
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (size_t i = 0; i < ipAddressList.size(); ++i) {
        if (ipAddressList.at(i) != QHostAddress::LocalHost && ipAddressList.at(i).toIPv4Address()) {
            ipAddress = ipAddressList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty()) {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }

    // set up UI
    statusLabel_ = new QLabel;
    statusLabel_->setWordWrap(true);
    statusLabel_->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\nRun the client example now.")
                              .arg(ipAddress)
                              .arg(server_.serverPort()));
    quitButton_ = new QPushButton(tr("Quit"));
    quitButton_->setAutoDefault(false);

    // button layout
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton_);
    buttonLayout->addStretch(1);
    // main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel_);
    mainLayout->addLayout(buttonLayout);

    // set layout and title
    setLayout(mainLayout);
    setWindowTitle(tr("Threaded Server"));

    // connection
    connect(quitButton_, SIGNAL(clicked()), this, SLOT(close()));
}