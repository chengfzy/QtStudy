#include "MainWidget.h"

//#define FLIP_VIEW

constexpr unsigned int SCR_WIDTH_OFFSET = 125;
constexpr unsigned int SCR_HEIGHT_OFFSET = 230;
constexpr unsigned int SCR_WIDTH = 1140;
constexpr unsigned int SCR_HEIGHT = 912;

MainWidget::MainWidget(QWidget* parent) : QOpenGLWidget(parent) {
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(SCR_WIDTH, SCR_HEIGHT);

    camSpec.cameraPos = QVector3D(0.0f, 0.0f, 1.0f);
    camSpec.cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    camSpec.cameraUp = QVector3D(0.0f, 1.0f, 0.0f);
    camSpec.yaw = -90.0f;
    camSpec.pitch = 0.0f;
    camSpec.lastX = SCR_WIDTH / 2.0;
    camSpec.lastY = SCR_HEIGHT / 2.0;
    camSpec.fov = 45.0f;

    screenDimension.setX(SCR_WIDTH);
    screenDimension.setY(SCR_HEIGHT);

    checkFirstFrameTimeElapsed = false;
    finishedLoadingBoldChars = false;
}

MainWidget::~MainWidget() {}

QMatrix4x4& MainWidget::getOrthoProjectionMatrix() { return orthoProjection; }

void MainWidget::setOrthoProjectionMatrix() {
    orthoProjection.setToIdentity();

#ifdef FLIP_VIEW
    orthoProjection.ortho(static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
#else
    orthoProjection.ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT), -1.0f, 1.0f);
#endif  // FLIP_VIEW
}

QMatrix4x4& MainWidget::getPerspectiveProjectionMatrix() { return perspectiveProjection; }

void MainWidget::setPerspectiveProjectionMatrix() {
    perspectiveProjection.setToIdentity();

#ifdef FLIP_VIEW
    perspectiveProjection.perspective(camSpec.fov, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    perspectiveProjection.scale(-1.0f, -1.0f, 1.0f);
#else
    perspectiveProjection.perspective(camSpec.fov, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
#endif  // FLIP_VIEW
}

QMatrix4x4& MainWidget::getViewMatrix() { return viewMatrix; }

void MainWidget::setViewMatrix() {
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(camSpec.cameraPos, camSpec.cameraPos + camSpec.cameraFront, camSpec.cameraUp);
}

void MainWidget::setText(float x, float y, const QString& text, float scale) {
    fontRender_->setText(x, y, text, scale);
}

void MainWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 0.5f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setOrthoProjectionMatrix();
    setPerspectiveProjectionMatrix();
    setViewMatrix();
    frameTime.start();
    frameCount = 0;
    frameTimeForFontLoad.start();

    // doneCurrent();
    fontRender_ = new FontRender();
    fontRender_->init();

    QString stringToDisplay = QString::fromUtf8("Qt Font Load App With Unicode Ex: 成都，你好!");
    fontRender_->setText(10, 10, stringToDisplay, 0.75);
}

void MainWidget::resizeGL(int w, int h) {}

void MainWidget::paintGL() {
#ifdef FLIP_VIEW
    glViewport(-SCR_WIDTH_OFFSET, -SCR_HEIGHT_OFFSET, (GLsizei)SCR_WIDTH, (GLsizei)SCR_HEIGHT);
#else
    glViewport(SCR_WIDTH_OFFSET, SCR_HEIGHT_OFFSET, (GLsizei)SCR_WIDTH, (GLsizei)SCR_HEIGHT);
#endif
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!checkFirstFrameTimeElapsed) {
        int loadTime = frameTimeForFontLoad.elapsed();
        checkFirstFrameTimeElapsed = true;
        qDebug() << "Total load Time for First painGL call" << loadTime << "\n";
    }

    QVector4D fontColor(1.0f, 1.0f, 0.0f, 1.0f);
    modelMatrix.setToIdentity();
    QMatrix4x4 pvmMat = getOrthoProjectionMatrix() * modelMatrix;
    // fontRender_->drawFontGeometry(10.0f, 10.0f, stringToDisplay, 0.75f, pvmMat, fontColor);
    fontRender_->render(pvmMat, fontColor);

    ++frameCount;
    if (frameTime.elapsed() >= 1000) {
        fps = frameCount / ((double)frameTime.elapsed() / 1000.0f);
        qDebug() << "FPS: " << fps;
    }
    update();
}
