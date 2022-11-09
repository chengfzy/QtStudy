#pragma once
#include <QByteArray>
#include <QDebug>
#include <QOpenGLWidget>
#include <QString>
#include <QThread>
#include <QTimer>
#include <QtCore/QVariant>
#include <QtCore>
// #include <QtMultimedia>
#include <iomanip>
#include <sstream>
#include "FontProvider.h"

#define NO_FLIP_VIEW
//#define FLIP_VIEW

const unsigned int SCR_WIDTH_OFFSET = 125;
const unsigned int SCR_HEIGHT_OFFSET = 230;

const unsigned int SCR_WIDTH = 1140;
const unsigned int SCR_HEIGHT = 912;

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

  public slots:
    void OnFinishedChildThread();

  public:
    MainWidget(QWidget* parent = 0);
    ~MainWidget();

    QMatrix4x4& getOrthoProjectionMatrix();
    QMatrix4x4& getPerspectiveProjectionMatrix();
    QMatrix4x4& getViewMatrix();
    void setPerspectiveProjectionMatrix();
    void setOrthoProjectionMatrix();
    void setViewMatrix();
    void initShaders();
    void paintGLHelperForFontRendering();
    // void			drawFontGeometry(GLfloat pos_x, GLfloat pos_y, const QString& text, GLfloat
    // scale_factor, bool boldAndItalic);

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

    QOpenGLShaderProgram programFont;

    float fps;

    QThread threadCons;

    FontProvider* fpObj;
    QElapsedTimer frameTimeForFontLoad;
    bool checkFirstFrameTimeElapsed;
    bool finishedLoadingBoldChars;
};