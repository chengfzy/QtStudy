#include <QApplication>
#include "Sender.h"

int main(int argc, char* argv[]) {
    // find network card support multicast
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    for (auto& v : list) {
        qDebug() << v.name() << " " << v.flags();

        QList<QNetworkAddressEntry> listAddrs = v.addressEntries();
        for (auto& addr : listAddrs) {
            qDebug() << addr.ip().toString();
        }
        qDebug() << "========================";
    }

    QApplication app(argc, argv);
    Sender sender;
    sender.show();

    return app.exec();
}