#include "MyGLWidget.h"
#include "QDebug"
#include "Vertex.h"

// create a colored  triangle
static const Vertex kVertexes[] = {Vertex(QVector3D(0.00f, 0.75f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)),
                                   Vertex(QVector3D(0.75f, -0.75f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)),
                                   Vertex(QVector3D(-0.75f, -0.75f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f))};

// constructor
MyGLWidget::MyGLWidget(QWidget* parent) : QOpenGLWidget(parent) {}

void MyGLWidget::initializeGL() {
    // initialize OpenGL backend
    initializeOpenGLFunctions();
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &MyGLWidget::teardownGL);
    printContextInformation();

    // set global information
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // create shader (do not release until Vertex Array Object is created)
    program_ = new QOpenGLShaderProgram();
    program_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
    program_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
    program_->link();
    program_->bind();

    // create buffer(do not release until Vertex Array Object is created)
    vertex_.create();
    vertex_.bind();
    vertex_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertex_.allocate(kVertexes, sizeof(kVertexes));

    // create vertex array object
    object_.create();
    object_.bind();
    program_->enableAttributeArray(0);
    program_->enableAttributeArray(1);
    program_->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
    program_->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

    // release all
    object_.release();
    vertex_.release();
    program_->release();
}

void MyGLWidget::resizeGL(int w, int h) {}

void MyGLWidget::paintGL() {
    // clear
    glClear(GL_COLOR_BUFFER_BIT);

    // render using our shader
    program_->bind();
    object_.bind();
    glDrawArrays(GL_TRIANGLES, 0, sizeof(kVertexes) / sizeof(kVertexes[0]));
    object_.release();
    program_->release();

    // glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MyGLWidget::teardownGL() {
    object_.destroy();
    vertex_.destroy();
    delete program_;
}

void MyGLWidget::printContextInformation() {
    // get version information
    QString glType = context()->isOpenGLES() ? "OpenGL ES" : "OpenGL";
    QString glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    // get profile information
    QString glProfile;
    switch (format().profile()) {
        case QSurfaceFormat::NoProfile:
            glProfile = "NoProfile";
            break;
        case QSurfaceFormat::CoreProfile:
            glProfile = "CoreProfile";
            break;
        case QSurfaceFormat::CompatibilityProfile:
            glProfile = "CompatibilityProfile";
            break;
    }

    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
