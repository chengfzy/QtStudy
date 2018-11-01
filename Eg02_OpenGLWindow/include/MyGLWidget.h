#pragma once
#include "QOpenGLBuffer"
#include "QOpenGLFunctions"
#include "QOpenGLShaderProgram"
#include "QOpenGLVertexArrayObject"
#include "QOpenGLWidget"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
   public:
    explicit MyGLWidget(QWidget* parent = nullptr);
    ~MyGLWidget() = default;

   protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL();

   protected slots:
    void teardownGL();

   private:
    void printContextInformation();

   private:
    // OpenGL state information
    QOpenGLBuffer vertex_;
    QOpenGLVertexArrayObject object_;
    QOpenGLShaderProgram* program_;
};
