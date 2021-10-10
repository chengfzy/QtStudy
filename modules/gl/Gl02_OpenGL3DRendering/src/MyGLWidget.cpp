#include "MyGLWidget.h"
#include <QDebug>
#include "Vertex.h"

// Front Verticies
#define VERTEX_FTR Vertex(QVector3D(0.5f, 0.5f, 0.5f), QVector3D(1.0f, 0.0f, 0.0f))
#define VERTEX_FTL Vertex(QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(0.0f, 1.0f, 0.0f))
#define VERTEX_FBL Vertex(QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(0.0f, 0.0f, 1.0f))
#define VERTEX_FBR Vertex(QVector3D(0.5f, -0.5f, 0.5f), QVector3D(0.0f, 0.0f, 0.0f))

// Back Verticies
#define VERTEX_BTR Vertex(QVector3D(0.5f, 0.5f, -0.5f), QVector3D(1.0f, 1.0f, 0.0f))
#define VERTEX_BTL Vertex(QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(0.0f, 1.0f, 1.0f))
#define VERTEX_BBL Vertex(QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(1.0f, 0.0f, 1.0f))
#define VERTEX_BBR Vertex(QVector3D(0.5f, -0.5f, -0.5f), QVector3D(1.0f, 1.0f, 1.0f))

// Create a colored cube
static const Vertex kVertexes[] = {
    // Face 1 (Front)
    VERTEX_FTR, VERTEX_FTL, VERTEX_FBL, VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
    // Face 2 (Back)
    VERTEX_BBR, VERTEX_BTL, VERTEX_BTR, VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
    // Face 3 (Top)
    VERTEX_FTR, VERTEX_BTR, VERTEX_BTL, VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
    // Face 4 (Bottom)
    VERTEX_FBR, VERTEX_FBL, VERTEX_BBL, VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
    // Face 5 (Left)
    VERTEX_FBL, VERTEX_FTL, VERTEX_BTL, VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
    // Face 6 (Right)
    VERTEX_FTR, VERTEX_FBR, VERTEX_BBR, VERTEX_BBR, VERTEX_BTR, VERTEX_FTR};

#undef VERTEX_BBR
#undef VERTEX_BBL
#undef VERTEX_BTL
#undef VERTEX_BTR

#undef VERTEX_FBR
#undef VERTEX_FBL
#undef VERTEX_FTL
#undef VERTEX_FTR

// constructor
MyGLWidget::MyGLWidget(QWidget* parent) : QOpenGLWidget(parent) { transform_.translate(QVector3D(0.0f, 0.0f, -5.0f)); }

void MyGLWidget::initializeGL() {
    // initialize OpenGL backend
    initializeOpenGLFunctions();
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &MyGLWidget::teardownGL);
    connect(this, &MyGLWidget::frameSwapped, this, &MyGLWidget::update);
    printContextInformation();

    // set global information
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // create shader (do not release until Vertex Array Object is created)
    program_ = new QOpenGLShaderProgram();
    program_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
    program_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
    program_->link();
    program_->bind();

    // cache uniform locations
    modelToWorld_ = program_->uniformLocation("modelToWorld");
    worldToView_ = program_->uniformLocation("worldToView");

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

void MyGLWidget::resizeGL(int w, int h) {
    projection_.setToIdentity();
    projection_.perspective(45.0f, static_cast<float>(w) / h, 0.0f, 1000.0f);
}

void MyGLWidget::paintGL() {
    // clear
    glClear(GL_COLOR_BUFFER_BIT);

    // render using our shader
    program_->bind();
    program_->setUniformValue(worldToView_, projection_);
    object_.bind();
    program_->setUniformValue(modelToWorld_,transform_.toMatrix());
    glDrawArrays(GL_TRIANGLES, 0, sizeof(kVertexes) / sizeof(kVertexes[0]));
    object_.release();
    program_->release();
}

void MyGLWidget::update() {
    // update instance information
    transform_.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));

    // schedule a redraw
    QOpenGLWidget::update();
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
