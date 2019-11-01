#include "FortuneThread.h"
#include <QtNetwork>

FortuneThread::FortuneThread(QObject* parent) : QThread(parent), quit_(false) {}

FortuneThread::~FortuneThread() {
    mutex_.lock();
    quit_ = true;
    cond_.wakeOne();
    mutex_.unlock();
    wait();
}

void FortuneThread::requestNewFortune(const QString& hostName, const quint16& port) {
    QMutexLocker locker(&mutex_);
    hostName_ = hostName;
    port_ = port;
    if (!isRunning()) {
        start();
    } else {
        cond_.wakeOne();
    }
}

void FortuneThread::run() {
    mutex_.lock();
    QString serverName = hostName_;
    quint16 serverPort = port_;
    mutex_.unlock();

    while (!quit_) {
        const int kTimeout{5 * 1000};

        // connect to server
        QTcpSocket socket;
        socket.connectToHost(serverName, serverPort);
        if (!socket.waitForConnected(kTimeout)) {
            emit error(socket.error(), socket.errorString());
            return;
        }

        QDataStream in(&socket);
        in.setVersion(QDataStream::Qt_5_12);
        QString fortune;

        do {
            if (!socket.waitForReadyRead(kTimeout)) {
                emit error(socket.error(), socket.errorString());
                return;
            }
            in.startTransaction();
            in >> fortune;
        } while (!in.commitTransaction());

        mutex_.lock();
        emit newFortune(fortune);
        cond_.wait(&mutex_);
        serverName = hostName_;
        serverPort = port_;
        mutex_.unlock();
    }
}