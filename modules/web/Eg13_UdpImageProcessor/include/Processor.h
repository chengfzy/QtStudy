#pragma once
#include <QtWidgets>
#include "ImageViewer.h"
#include "ProcessThread.h"

/**
 * @brief Image Processor. Receive image from Image Capture, then process image and send result to Image Capture
 */
class Processor : public QWidget {
    Q_OBJECT
  public:
    explicit Processor(QWidget* parent = nullptr);

  private:
    // setup UI
    void setupUI();

    // control(start/stop) capture
    void controlCapture();

  private:
    ImageViewer* imageViewer_ = nullptr;    // image viewer
    QPushButton* controlButton_ = nullptr;  // control button, start/stop capture
    QLabel* resultLabel_ = nullptr;         // result label

    ProcessThread* processThread_ = nullptr;  // process thread
};
