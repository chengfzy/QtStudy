#pragma once
#include "FortuneThread.h"
#include <QtNetwork>
#include <QtWidgets>

class BlockingClient : public QWidget {
  public:
    explicit BlockingClient(QWidget* parent = nullptr);

  private slots:
    void requestNewFortune();
    void showFortune(const QString& nextFortune);
    void displayError(int socketError, const QString& message);
    void enableGetFortuneButton();

  private:
    QLabel* hostLabel_ = nullptr;
    QLabel* portLabel_ = nullptr;
    QLineEdit* hostLineEdit_ = nullptr;
    QLineEdit* portLineEdit_ = nullptr;
    QLabel* statusLabel_ = nullptr;
    QPushButton* getFortuneButton_ = nullptr;
    QPushButton* quitButton_ = nullptr;
    QDialogButtonBox* buttonBox_ = nullptr;

    FortuneThread thread_;
    QString currentFortune_;
};
