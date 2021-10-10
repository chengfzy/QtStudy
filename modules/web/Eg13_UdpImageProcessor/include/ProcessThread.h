#pragma once
#include <QtNetwork>
#include <QtWidgets>

/**
 * @brief Process thread, receive image from Image Capture, then process image and send result to Capture
 */
class ProcessThread : public QThread {
    Q_OBJECT
  public:
    explicit ProcessThread(QObject* parent = nullptr);

    ~ProcessThread() override;

  signals:
    // signals if got new image
    void newImage(const QImage& image);

    // signals if got new result info
    void newResultInfo(const QString& info);

    // new socket error occurred
    void error(int socketError, const QString& message);

    // process finish with some error
    void finished();

  public:
    // stop thread
    void stop();

  protected:
    // Override run operation in QThread
    void run() override;

  private:
    quint16 receivePort_ = 45454;                         // receive port
    QHostAddress sendAddress_ = QHostAddress::Broadcast;  // sender address, broadcast mode
    quint16 sendPort_ = 45455;                            // sender port
    QMutex mutex_;                                        // mutex
    bool stop_ = false;                                   // stop flag
    int imageNo_ = 1;                                     // image number
};
