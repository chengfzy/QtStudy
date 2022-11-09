#pragma once
#include <QByteArray>
#include <QDebug>
#include <QOpenGLWidget>
#include <QString>
#include <QThread>
#include <QTimer>
#include <QtCore/QVariant>
#include <QtCore>
#include <iomanip>
#include <sstream>
#include "FontRender.h"

typedef struct {
    QVector3D cameraPos;
    QVector3D cameraFront;
    QVector3D cameraUp;
    float yaw;
    float pitch;
    float lastX;
    float lastY;
    float fov;
    float deltaTime;  // time between current frame and last frame
    float lastFrame;
} Camera;

class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

  public:
    MainWidget(QWidget* parent = 0);
    ~MainWidget();

    QMatrix4x4& getOrthoProjectionMatrix();
    QMatrix4x4& getPerspectiveProjectionMatrix();
    QMatrix4x4& getViewMatrix();
    void setPerspectiveProjectionMatrix();
    void setOrthoProjectionMatrix();
    void setViewMatrix();

    void setText(float x, float y, const QString& text, float scale);

  protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

  private:
    QMatrix4x4 modelMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 orthoProjection;
    QMatrix4x4 perspectiveProjection;
    Camera camSpec;

    int frameCount;
    QElapsedTimer frameTime;
    QVector2D screenDimension;

    float fps;
    FontRender* fontRender_;
    QElapsedTimer frameTimeForFontLoad;
    bool checkFirstFrameTimeElapsed;
    bool finishedLoadingBoldChars;
};