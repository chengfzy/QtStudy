#pragma once
#include "QMutex"
#include "QThread"
#include "QWaitCondition"

class FortuneThread : public QThread {
    Q_OBJECT

  public:
    explicit FortuneThread(QObject* parent = nullptr);
    ~FortuneThread();

  public:
    void requestNewFortune(const QString& hostName, const quint16& port);
    void run() override;

  signals:
    void newFortune(const QString& fortune);
    void error(int socketError, const QString& message);

  private:
    QString hostName_;
    quint16 port_;
    QMutex mutex_;
    QWaitCondition cond_;
    bool quit_;
};
