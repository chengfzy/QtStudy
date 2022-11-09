#include "MainWidget.h"

//#define FLIP_VIEW

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

MainWidget::~MainWidget() {
    if (fpObj->isRunning()) {
        fpObj->quit();
        delete fpObj;
    }
}

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

void MainWidget::initShaders() {
    // Font shaders
    if (!programFont.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/text.vert")) close();
    if (!programFont.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/text.frag")) close();
    if (!programFont.link()) close();
}

void MainWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 0.5f);
    setOrthoProjectionMatrix();
    setPerspectiveProjectionMatrix();
    setViewMatrix();
    initShaders();
    frameTime.start();
    frameCount = 0;
    frameTimeForFontLoad.start();

    QOpenGLContext* current = context();
    doneCurrent();
    fpObj = new FontProvider();

    // the background thread's context is shared from current
    QOpenGLContext* shared = fpObj->context;
    shared->setFormat(current->format());
    shared->setShareContext(current);
    shared->create();

    // must move the shared context to the background thread
    shared->moveToThread(fpObj);

    // setup the background thread's surface
    // must be created here in the main thread
    QOffscreenSurface* surface = fpObj->surface;
    surface->setFormat(shared->format());
    surface->create();

    // worker signal
    connect(fpObj, SIGNAL(started()), fpObj, SLOT(initializeFontProvider()));
    connect(fpObj, SIGNAL(finished()), this, SLOT(OnFinishedChildThread()));

    // must move the thread to itself
    fpObj->moveToThread(fpObj);

    // the worker can finally start
    fpObj->start();

    glEnable(GL_DEPTH_TEST);
}

void MainWidget::resizeGL(int w, int h) {}

void MainWidget::OnFinishedChildThread() { finishedLoadingBoldChars = true; }

void MainWidget::paintGLHelperForFontRendering() {
    QVector4D fontColor(1.0f, 1.0f, 0.0f, 1.0f);
    programFont.bind();
    modelMatrix.setToIdentity();
    programFont.setUniformValue("mvp_matrix", getOrthoProjectionMatrix() * modelMatrix);
    programFont.setUniformValue("textColor", fontColor);
    programFont.setUniformValue("text", 0);
    QString stringToDisplay = QString::fromUtf8("Qt Font Load App With Unicode Ex: 成都，你好!");
    fpObj->drawFontGeometry(&programFont, 10.0f, 10.0f, stringToDisplay, 0.75f);
    programFont.release();
}

//#if 0
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

    if (finishedLoadingBoldChars) paintGLHelperForFontRendering();

    ++frameCount;
    if (frameTime.elapsed() >= 1000) {
        fps = frameCount / ((double)frameTime.elapsed() / 1000.0f);
        qDebug() << "FPS: " << fps;
    }
    update();
}
//#endif
